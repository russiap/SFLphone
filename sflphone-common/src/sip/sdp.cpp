/*
 *  Copyright (C) 2009 Savoir-Faire Linux inc.
 *
 *  Author: Emmanuel Milou <emmanuel.milou@savoirfairelinux.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "sdp.h"
#include "global.h"
#include "manager.h"
#define ZRTP_VERSION "1.10"

static const pj_str_t STR_AUDIO = { (char*) "audio", 5};
static const pj_str_t STR_VIDEO = { (char*) "video", 5};
static const pj_str_t STR_IN = { (char*) "IN", 2 };
static const pj_str_t STR_IP4 = { (char*) "IP4", 3};
static const pj_str_t STR_IP6 = { (char*) "IP6", 3};
static const pj_str_t STR_RTP_AVP = { (char*) "RTP/AVP", 7 };
static const pj_str_t STR_SDP_NAME = { (char*) "sflphone", 8 };
static const pj_str_t STR_SENDRECV = { (char*) "sendrecv", 8 };
static const pj_str_t STR_RTPMAP = { (char*) "rtpmap", 6 };

Sdp::Sdp (pj_pool_t *pool)
        : _local_media_cap()
        , _session_media (0)
        , _negociator (NULL)
        , _ip_addr ("")
        , _local_offer (NULL)
        , _negociated_offer (NULL)
        , _pool (NULL)
        , _local_extern_audio_port (0)
{
    _pool = pool;
}

Sdp::~Sdp()
{

    //unsigned int k;

    /*
    for( k=0; k<_session_media.size(); k++ ){
        delete _session_media[k];
        _session_media[k] = 0;
    }*/

    //for( k=0; k<_local_media_cap.size(); k++ ){
    //  delete _local_media_cap[k];
    //_local_media_cap[k] = 0;
    //}
}

void Sdp::set_media_descriptor_line (sdpMedia *media, pjmedia_sdp_media** p_med)
{

    pjmedia_sdp_media* med;
    pjmedia_sdp_rtpmap rtpmap;
    pjmedia_sdp_attr *attr;
    AudioCodec *codec;
    int count, i;
    std::string tmp;

    med = PJ_POOL_ZALLOC_T (_pool, pjmedia_sdp_media);

    // Get the right media format
    pj_strdup (_pool, &med->desc.media,
               (media->get_media_type() == MIME_TYPE_AUDIO) ? &STR_AUDIO : &STR_VIDEO);
    med->desc.port_count = 1;
    med->desc.port = media->get_port();
    pj_strdup (_pool, &med->desc.transport, &STR_RTP_AVP);

    // Media format ( RTP payload )
    count = media->get_media_codec_list().size();
    med->desc.fmt_count = count;

    // add the payload list

    for (i=0; i<count; i++) {
        codec = media->get_media_codec_list() [i];
        tmp = this->convert_int_to_string (codec->getPayload ());
        _debug ("%s\n", tmp.c_str());
        pj_strdup2 (_pool, &med->desc.fmt[i], tmp.c_str());

        // Add a rtpmap field for each codec
        // We could add one only for dynamic payloads because the codecs with static RTP payloads
        // are entirely defined in the RFC 3351, but if we want to add other attributes like an asymmetric
        // connection, the rtpmap attribute will be useful to specify for which codec it is applicable
        rtpmap.pt = med->desc.fmt[i];
        rtpmap.enc_name = pj_str ( (char*) codec->getCodecName().c_str());
        rtpmap.clock_rate = codec->getClockRate();
        // Add the channel number only if different from 1

        if (codec->getChannel() > 1)
            rtpmap.param = pj_str ( (char*) codec->getChannel());
        else
            rtpmap.param.slen = 0;

        pjmedia_sdp_rtpmap_to_attr (_pool, &rtpmap, &attr);

        med->attr[med->attr_count++] = attr;
    }

    // Add the direction stream
    attr = (pjmedia_sdp_attr*) pj_pool_zalloc (_pool, sizeof (pjmedia_sdp_attr));
    pj_strdup2 (_pool, &attr->name, media->get_stream_direction_str().c_str());
    med->attr[ med->attr_count++] = attr;
    
    if(!_zrtp_hello_hash.empty()) {
        try {
            sdp_add_zrtp_attribute(med,_zrtp_hello_hash);
        } catch (...) {
            throw;
        }
    } else { 
        _debug("No hash specified\n"); 
    }

    *p_med = med;
}

int Sdp::create_local_offer ()
{
    pj_status_t status;

    _debug ("Create local offer\n");
    // Build local media capabilities
    set_local_media_capabilities ();

    // Reference: RFC 4566 [5]

    /* Create and initialize basic SDP session */
    this->_local_offer = PJ_POOL_ZALLOC_T (_pool, pjmedia_sdp_session);
    this->_local_offer->conn = PJ_POOL_ZALLOC_T (_pool, pjmedia_sdp_conn);

    /* Initialize the fields of the struct */
    sdp_add_protocol();
    sdp_add_origin();
    sdp_add_session_name();
    sdp_add_connection_info();
    sdp_add_timing();
    //sdp_addAttributes( _pool );
    sdp_add_media_description();

    //toString ();

    // Validate the sdp session
    status = pjmedia_sdp_validate (this->_local_offer);

    if (status != PJ_SUCCESS)
        return status;

    return PJ_SUCCESS;
}

int Sdp::create_initial_offer()
{
    pj_status_t status;
    pjmedia_sdp_neg_state state;

    _debug ("Create initial offer\n");
    // Build the SDP session descriptor
    status = create_local_offer();

    if (status != PJ_SUCCESS) {
        return status;
    }

    // Create the SDP negociator instance with local offer
    status = pjmedia_sdp_neg_create_w_local_offer (_pool, get_local_sdp_session(), &_negociator);

    state = pjmedia_sdp_neg_get_state (_negociator);

    PJ_ASSERT_RETURN (status == PJ_SUCCESS, 1);

    return PJ_SUCCESS;
}

int Sdp::receiving_initial_offer (pjmedia_sdp_session* remote)
{
    // Create the SDP negociator instance by calling
    // pjmedia_sdp_neg_create_w_remote_offer with the remote offer, and by providing the local offer ( optional )

    pj_status_t status;

    _debug ("Receiving initial offer\n");

    // Create the SDP negociator instance by calling
    // pjmedia_sdp_neg_create_w_remote_offer with the remote offer, and by providing the local offer ( optional )

    // Build the local offer to respond
    status = create_local_offer();

    if (status != PJ_SUCCESS) {
        return status;
    }

    // Retrieve some useful remote information
    this->set_media_transport_info_from_remote_sdp (remote);

    status = pjmedia_sdp_neg_create_w_remote_offer (_pool,
             get_local_sdp_session(), remote, &_negociator);

    PJ_ASSERT_RETURN (status == PJ_SUCCESS, 1);

    return PJ_SUCCESS;
}

pj_status_t Sdp::check_sdp_answer (pjsip_inv_session *inv, pjsip_rx_data *rdata)
{
    static const pj_str_t str_application = { (char*) "application", 11 };
    static const pj_str_t str_sdp = { (char*) "sdp", 3 };
    pj_status_t status;
    pjsip_msg * message = NULL;
    pjmedia_sdp_session * remote_sdp = NULL;

    if (pjmedia_sdp_neg_get_state (inv->neg) == PJMEDIA_SDP_NEG_STATE_LOCAL_OFFER) {

        message = rdata->msg_info.msg;

        if (message == NULL) {
            _debug ("No message");
            return PJMEDIA_SDP_EINSDP;
        }

        if (message->body == NULL) {
            _debug ("Empty message body\n");
            return PJMEDIA_SDP_EINSDP;
        }

        if (pj_stricmp (&message->body->content_type.type, &str_application) || pj_stricmp (&message->body->content_type.subtype, &str_sdp)) {
            _debug ("Incoming Message does not contain SDP\n");
            return PJMEDIA_SDP_EINSDP;
        }

        // Parse the SDP body.
        status = pjmedia_sdp_parse (rdata->tp_info.pool, (char*) message->body->data, message->body->len, &remote_sdp);

        if (status == PJ_SUCCESS) {
            status = pjmedia_sdp_validate (remote_sdp);
        }

        if (status != PJ_SUCCESS) {
            _debug ("SDP cannot be validated\n");
            return PJMEDIA_SDP_EINSDP;
        }

        // This is an answer
        _debug ("Got SDP answer %s\n", pjsip_rx_data_get_info (rdata));

        status = pjmedia_sdp_neg_set_remote_answer (inv->pool, inv->neg, remote_sdp);

        if (status != PJ_SUCCESS) {
            _debug ("An error occured while processing remote answer %s\n", pjsip_rx_data_get_info (rdata));
            return PJMEDIA_SDP_EINSDP;
        }

        // Prefer our codecs to remote when possible
        pjmedia_sdp_neg_set_prefer_remote_codec_order (inv->neg, 0);

        status = pjmedia_sdp_neg_negotiate (inv->pool, inv->neg, 0);

        _debug ("Negotiation returned with status %d PJ_SUCCESS being %d\n", status, PJ_SUCCESS);
    }

    return status;
}

void Sdp::sdp_add_protocol (void)
{
    this->_local_offer->origin.version = 0;
}

void Sdp::sdp_add_origin (void)
{
    pj_time_val tv;
    pj_gettimeofday (&tv);

    this->_local_offer->origin.user = pj_str (pj_gethostname()->ptr);
    // Use Network Time Protocol format timestamp to ensure uniqueness.
    this->_local_offer->origin.id = tv.sec + 2208988800UL;
    // The type of network ( IN for INternet )
    this->_local_offer->origin.net_type = STR_IN;
    // The type of address
    this->_local_offer->origin.addr_type = STR_IP4;
    // The address of the machine from which the session was created
    this->_local_offer->origin.addr = pj_str ( (char*) _ip_addr.c_str());
}

void Sdp::sdp_add_session_name (void)
{
    this->_local_offer->name = STR_SDP_NAME;
}


void Sdp::sdp_add_connection_info (void)
{
    this->_local_offer->conn->net_type = _local_offer->origin.net_type;
    this->_local_offer->conn->addr_type = _local_offer->origin.addr_type;
    this->_local_offer->conn->addr = _local_offer->origin.addr;
}


void Sdp::sdp_add_timing (void)
{
    // RFC 3264: An offer/answer model session description protocol
    // As the session is created and destroyed through an external signaling mean (SIP), the line
    // should have a value of "0 0".

    this->_local_offer->time.start = this->_local_offer->time.stop = 0;
}

void Sdp::sdp_add_attributes()
{
    pjmedia_sdp_attr *a;
    this->_local_offer->attr_count = 1;
    a =  PJ_POOL_ZALLOC_T (_pool, pjmedia_sdp_attr);
    a->name=STR_SENDRECV;
    _local_offer->attr[0] = a;
}


void Sdp::sdp_add_media_description()
{
    pjmedia_sdp_media* med;
    int nb_media, i;

    med = PJ_POOL_ZALLOC_T (_pool, pjmedia_sdp_media);
    nb_media = get_local_media_cap().size();
    this->_local_offer->media_count = nb_media;

    for (i=0; i<nb_media; i++) {
        set_media_descriptor_line (get_local_media_cap() [i], &med);
        this->_local_offer->media[i] = med;
    }
}

void Sdp::sdp_add_zrtp_attribute(pjmedia_sdp_media* media, std::string hash) 
{
    pjmedia_sdp_attr *attribute;    
    char tempbuf[256];
    int len;
        
    attribute = (pjmedia_sdp_attr*)pj_pool_zalloc( _pool, sizeof(pjmedia_sdp_attr) );

    attribute->name = pj_strdup3(_pool, "zrtp-hash");
    
    /* Format: ":version value" */
    len = pj_ansi_snprintf(tempbuf, sizeof(tempbuf),
                            "%.*s %.*s",
                            4,
                            ZRTP_VERSION,
                            hash.size(),
                            hash.c_str());

    attribute->value.slen = len;
    attribute->value.ptr = (char*) pj_pool_alloc(_pool, attribute->value.slen+1);
    pj_memcpy(attribute->value.ptr, tempbuf, attribute->value.slen+1);
    
    if(pjmedia_sdp_media_add_attr(media, attribute) != PJ_SUCCESS) {
        throw sdpException();
    }
}

std::string Sdp::media_to_string (void)
{
    int size, i;
    std::ostringstream res;

    size = _local_media_cap.size();

    for (i = 0; i < size ; i++) {
        res << _local_media_cap[i]->to_string();
    }

    res << std::endl;

    return res.str();
}

void Sdp::clean_session_media()
{
    _session_media.clear();
}

void Sdp::set_negotiated_sdp (const pjmedia_sdp_session *sdp)
{

    int nb_media, nb_codecs;
    int i,j, port;
    pjmedia_sdp_media *current;
    sdpMedia *media;
    std::string type, dir;
    CodecsMap codecs_list;
    CodecsMap::iterator iter;
    pjmedia_sdp_attr *attribute;
    pjmedia_sdp_rtpmap *rtpmap;

    _negociated_offer = (pjmedia_sdp_session*) sdp;

    codecs_list = Manager::instance().getCodecDescriptorMap().getCodecsMap();

    // retrieve the media information
    nb_media = _negociated_offer->media_count;

    for (i=0; i<nb_media ; i++) {
        // Retrieve the media
        current = _negociated_offer->media[i];
        type = current->desc.media.ptr;
        port = current->desc.port;
        media = new sdpMedia (type, port);
        // Retrieve the payload
        nb_codecs = current->desc.fmt_count;  // Must be one

        for (j=0 ; j<nb_codecs ; j++) {
            attribute = pjmedia_sdp_media_find_attr (current, &STR_RTPMAP, NULL);
            // pj_strtoul(attribute->pt)
            pjmedia_sdp_attr_to_rtpmap (_pool, attribute, &rtpmap);

            // _debug("================== set_negociated_offer ===================== %i\n", pj_strtoul(&rtpmap->pt));
            // _debug("================== set_negociated_offer ===================== %s\n", current->desc.fmt[j].ptr);
            // _debug("================== set_negociated_offer ===================== %i\n", atoi(current->desc.fmt[j].ptr));
            iter = codecs_list.find ( (AudioCodecType) pj_strtoul (&rtpmap->pt));

            if (iter==codecs_list.end())
                return;

            media->add_codec (iter->second);
        }

        _session_media.push_back (media);
    }
}

AudioCodec* Sdp::get_session_media (void)
{

    int nb_media;
    int nb_codec;
    AudioCodec *codec = NULL;
    std::vector<sdpMedia*> media_list;

    _debug ("sdp line %d - get_session_media ()\n", __LINE__);

    media_list = get_session_media_list ();
    nb_media = media_list.size();

    if (nb_media > 0) {
        nb_codec = media_list[0]->get_media_codec_list().size();

        if (nb_codec > 0) {
            codec = media_list[0]->get_media_codec_list() [0];
        }
    }

    return codec;
}



void Sdp::toString (void)
{

    std::ostringstream sdp;
    int count, i;

    sdp <<  "origin= " <<  _local_offer->origin.user.ptr << "\n";
    sdp << "origin.id= " << _local_offer->origin.id << "\n";
    sdp << "origin.version= " << _local_offer->origin.version<< "\n";
    sdp << "origin.net_type= " << _local_offer->origin.net_type.ptr<< "\n";
    sdp << "origin.addr_type= " << _local_offer->origin.addr_type.ptr<< "\n";

    sdp << "name=" << _local_offer->name.ptr<< "\n";

    sdp << "conn.net_type=" << _local_offer->conn->net_type.ptr<< "\n";
    sdp << "conn.addr_type=" << _local_offer->conn->addr_type.ptr<< "\n";
    sdp << "conn.addr=" << _local_offer->conn->addr.ptr<< "\n";

    sdp << "start=" <<_local_offer->time.start<< "\n";
    sdp << "stop=" <<_local_offer->time.stop<< "\n";

    sdp << "attr_count=" << _local_offer->attr_count << "\n";
    sdp << "media_count=" << _local_offer->media_count << "\n";
    sdp << "m=" << _local_offer->media[0]->desc.media.ptr << " ";
    sdp << _local_offer->media[0]->desc.port << " ";
    sdp << _local_offer->media[0]->desc.transport.ptr << " ";
    count = _local_offer->media[0]->desc.fmt_count;

    for (i=0; i<count; i++) {
        sdp << _local_offer->media[0]->desc.fmt[i].ptr << " ";
    }

    sdp << "\n";

    _debug ("LOCAL SDP: \n%s\n", sdp.str().c_str());

}

void Sdp::set_local_media_capabilities ()
{

    CodecOrder selected_codecs;
    unsigned int i;
    sdpMedia *audio;
    CodecsMap codecs_list;
    CodecsMap::iterator iter;

    // Clean it first
    _local_media_cap.clear();

    _debug ("Fetch local media capabilities. Local extern audio port: %i\n" , get_local_extern_audio_port());

    /* Only one audio media used right now */
    audio = new sdpMedia (MIME_TYPE_AUDIO);
    audio->set_port (get_local_extern_audio_port());

    /* We retrieve the codecs selected by the user */
    selected_codecs = Manager::instance().getCodecDescriptorMap().getActiveCodecs();
    codecs_list = Manager::instance().getCodecDescriptorMap().getCodecsMap();

    for (i=0; i<selected_codecs.size(); i++) {
        iter=codecs_list.find (selected_codecs[i]);

        if (iter!=codecs_list.end()) {
            audio->add_codec (iter->second);
        }
    }

    _local_media_cap.push_back (audio);
}

void Sdp::attribute_port_to_all_media (int port)
{

    std::vector<sdpMedia*> medias;
    int i, size;

    set_local_extern_audio_port (port);

    medias = get_local_media_cap ();
    size = medias.size();

    for (i=0; i<size; i++) {
        medias[i]->set_port (port);
    }
}

std::string Sdp::convert_int_to_string (int value)
{
    std::ostringstream result;
    result << value;
    return result.str();
}

void Sdp::set_remote_ip_from_sdp (const pjmedia_sdp_session *r_sdp)
{

    std::string remote_ip (r_sdp->conn->addr.ptr, r_sdp->conn->addr.slen);
    _debug ("            Remote IP from fetching SDP: %s\n", remote_ip.c_str());
    this->set_remote_ip (remote_ip);
}

void Sdp::set_remote_audio_port_from_sdp (pjmedia_sdp_media *r_media)
{

    int remote_port;

    remote_port = r_media->desc.port;
    _debug ("            Remote Audio Port from fetching SDP: %d\n", remote_port);
    this->set_remote_audio_port (remote_port);
}

void Sdp::set_media_transport_info_from_remote_sdp (const pjmedia_sdp_session *remote_sdp)
{

    _debug ("Fetching media from sdp\n");

    pjmedia_sdp_media *r_media;

    this->get_remote_sdp_media_from_offer (remote_sdp, &r_media);

    if (r_media==NULL) {
        _debug ("SDP Failure: no remote sdp media found in the remote offer\n");
        return;
    }

    this->set_remote_audio_port_from_sdp (r_media);

    this->set_remote_ip_from_sdp (remote_sdp);
}

void Sdp::get_remote_sdp_media_from_offer (const pjmedia_sdp_session* remote_sdp, pjmedia_sdp_media** r_media)
{
    int count, i;

    count = remote_sdp->media_count;
    *r_media =  NULL;

    for (i = 0; i < count; ++i) {
        if (pj_stricmp2 (&remote_sdp->media[i]->desc.media, "audio") == 0) {
            *r_media = remote_sdp->media[i];
            return;
        }
    }
}

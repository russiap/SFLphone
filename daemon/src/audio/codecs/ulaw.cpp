/*
 *  Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010, 2011 Savoir-Faire Linux Inc.
 * Author:  Yan Morin <yan.morin@savoirfairelinux.com>
 * Author:  Laurielle Lea <laurielle.lea@savoirfairelinux.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Additional permission under GNU GPL version 3 section 7:
 *
 *  If you modify this program, or any covered work, by linking or
 *  combining it with the OpenSSL project's OpenSSL library (or a
 *  modified version of that library), containing parts covered by the
 *  terms of the OpenSSL or SSLeay licenses, Savoir-Faire Linux Inc.
 *  grants you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of OpenSSL used as well
 *  as that of the covered work.
 */


#include "global.h"
#include "../common.h"
#include "audiocodec.h"
#include <cassert>

class Ulaw : public sfl::AudioCodec
{
    public:
        // 0 PCMU A 8000 1 [RFC3551]
        Ulaw (int payload=0)
            : sfl::AudioCodec (payload, "PCMU") {
            _clockRate = 8000;
            _frameSize = 160; // samples, 20 ms at 8kHz
            _channel   = 1;
            _bitrate =  64;
            _hasDynamicPayload = false;
        }

        virtual int decode (short *dst, unsigned char *src, size_t buf_size) {
        	assert(buf_size == _frameSize / 2 /* compression factor = 2:1 */ * sizeof(SFLDataFormat));
            unsigned char* end = src+buf_size;

            while (src<end)
                *dst++ = ULawDecode (*src++);

            return _frameSize;
        }

        virtual int encode (unsigned char *dst, short *src, size_t buf_size) {
        	assert(buf_size >= _frameSize / 2 /* compression factor = 2:1 */ * sizeof(SFLDataFormat));
            uint8* end = dst+_frameSize;

            while (dst<end)
                *dst++ = ULawEncode (*src++);

            return _frameSize / 2 /* compression factor = 2:1 */ * sizeof(SFLDataFormat);;
        }

        int ULawDecode (uint8 ulaw) {
            ulaw ^= 0xff;  // u-law has all bits inverted for transmission
            int linear = ulaw&0x0f;
            linear <<= 3;
            linear |= 0x84;  // Set MSB (0x80) and a 'half' bit (0x04) to place PCM value in middle of range

            uint shift = ulaw>>4;
            shift &= 7;
            linear <<= shift;
            linear -= 0x84; // Subract uLaw bias

            if (ulaw&0x80)
                return -linear;
            else
                return linear;
        }

        uint8 ULawEncode (int16 pcm16) {
            int p = pcm16;
            uint u;  // u-law value we are forming

            if (p<0) {
                p = ~p;
                u = 0x80^0x10^0xff;  // Sign bit = 1 (^0x10 because this will get inverted later) ^0xff ^0xff to invert final u-Law code
            } else {
                u = 0x00^0x10^0xff;  // Sign bit = 0 (-0x10 because this amount extra will get added later) ^0xff to invert final u-Law code
            }

            p += 0x84; // Add uLaw bias

            if (p>0x7f00)
                p = 0x7f00;  // Clip to 15 bits

            // Calculate segment and interval numbers
            p >>= 3;        // Shift down to 13bit

            if (p>=0x100) {
                p >>= 4;
                u ^= 0x40;
            }

            if (p>=0x40) {
                p >>= 2;
                u ^= 0x20;
            }

            if (p>=0x20) {
                p >>= 1;
                u ^= 0x10;
            }

            u ^= p; // u now equal to encoded u-law value (with all bits inverted)

            return u;
        }
};

// the class factories
extern "C" sfl::Codec* create()
{
    return new Ulaw (0);
}

extern "C" void destroy (sfl::Codec* a)
{
    delete a;
}

#include "H264EncoderTest.h"

#include "sip/sdp/RtpMap.h"
#include "sip/sdp/Fmtp.h"

#include "video/VideoFormat.h"
#include "video/codec/GstCodecH264.h"
#include "video/rtp/VideoRtpSessionSimple.h"
#include "video/source/VideoInputSourceGst.h"

#include "util/Dimension.h"

#include <ccrtp/rtp.h>

void H264EncoderTest::setUp() {
	std::cout << "Setting up..." << std::endl;

	if (system("./client.sh >> /dev/null &") < 0) {
		CPPUNIT_FAIL("Failed to start client in video RTP test.");
	}
}

void H264EncoderTest::tearDown() {
	std::cout << "Tearing down..." << std::endl;

	if (system("killall gst-launch-0.10") < 0) {
		CPPUNIT_FAIL("Failed to stop client in video RTP test.");
	}
}

void H264EncoderTest::testSend()
{
	std::cout << "Testing encoding and network transmission ... " << std::endl;

	// Create a video source from which we will encode
	sfl::VideoInputSourceGst source;
	source.setDevice("/dev/video0", sfl::Dimension(960, 720), sfl::FrameRate(30,1));

	// Create a video session
	ost::InetHostAddress address("127.0.0.1");
	sfl::VideoRtpSessionSimple* session = new sfl::VideoRtpSessionSimple(address, (ost::tpport_t) 5055);

	// Set the video source for this RTP session
	session->setVideoSource(source);

	// Add a destination for the packets
	session->addDestination(address, (ost::tpport_t) 5000);

	// Register supported codecs for this session.
	sfl::GstCodecH264* codec = new sfl::GstCodecH264();
	session->registerCodec(codec);

	// Simulate the arrival of an SDP offer
	// At that point, the correct codec should be loaded and activated.
	// Video frames would flow in the encoder and get sent to the remote peer.
	sfl::RtpMap rtpmap("96", "H264", 9000, "");
	sfl::Fmtp fmtp("96", "profile-level-id=42A01E; packetization-mode=0; sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==");
	session->addSessionCodec(rtpmap, fmtp);

	// Receive data
	session->start();

	// Let the frames flow into the encoder
	source.open();

	sleep(30);

	delete session;
}
/*
 *  Copyright (C) 2011 Savoir-Faire Linux Inc.
 *  Author: Tristan Matthews <tristan.matthews@savoirfairelinux.com>
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

#include <iostream>
#include <cassert>
#include <unistd.h> // for sleep
#include <map>
#include <string>
#include "video_rtp_session.h"

int main (int argc, char* argv[])
{
    std::map<std::string, std::string> txArgs, rxArgs;
    txArgs["input"]       = "/dev/video0";
    txArgs["codec"]       = "mpeg4";
    txArgs["bitrate"]     = "1000000";
    txArgs["destination"] = "rtp://127.0.0.1:5000";
    txArgs["format"]      = "rgb24";
    txArgs["width"]       = "640";
    txArgs["height"]      = "480";

    rxArgs["input"]       = "test.sdp";
    rxArgs["codec"]       = "mpeg4";
    rxArgs["bitrate"]     = "1000000";
    rxArgs["format"]      = "rgb24";
    rxArgs["width"]       = "640";
    rxArgs["height"]      = "480";

    sfl_video::VideoRtpSession session(txArgs, rxArgs);
    //sfl_video::VideoRtpSession videorecv("rtp://127.0.0.1:5000", "libx264", 1000000, "rtp://127.0.0.1:5000", "rgb24");
    session.start();
    sleep(10);
    session.stop();

    return 0;
}
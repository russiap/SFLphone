/*
 *  Copyright (C) 2009 Savoir-Faire Linux inc.
 *  Author: Alexandre Savard <alexandre.savard@savoirfairelinux.com>
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

// Cppunit import
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

#include <assert.h>

#include <stdio.h>
#include <sstream>
#include <ccrtp/rtp.h>

#include <vector>

// pjsip import
#include <pjsip.h>
#include <pjlib.h>
#include <pjsip_ua.h>
#include <pjlib-util.h>
#include <pjnath/stun_config.h>

// Application import
#include "sip/SdesNegotiator.h"
#include "sip/Pattern.h"


/*
 * @file sdesnegotiationTest.cpp  
 * @brief       Regroups unitary tests related to the plugin manager.
 */

#ifndef _SDESNEGOTIATOR_TEST_
#define _SDESNEGOTIATOR_TEST_



class SdesNegotiatorTest : public CppUnit::TestCase {

    /*
     * Use cppunit library macros to add unit test the factory
     */
    CPPUNIT_TEST_SUITE( SdesNegotiatorTest );
    CPPUNIT_TEST( testTagPattern );
    CPPUNIT_TEST( testCryptoSuitePattern );
    CPPUNIT_TEST( testKeyParamsPattern );
    CPPUNIT_TEST( testKeyParamsPatternWithoutMKI );
    CPPUNIT_TEST( testNegotiation );
    CPPUNIT_TEST( testMostSimpleCase );
    CPPUNIT_TEST_SUITE_END();

    public:

        /*
         * Code factoring - Common resources can be released here.
         * This method is called by unitcpp after each test
         */
		void testTagPattern();

		void testCryptoSuitePattern();

		void testKeyParamsPattern();
		
        void testKeyParamsPatternCiscoStyle();

		void testKeyParamsPatternWithoutMKI();

		void testNegotiation();
		
		void testComponent();

		void testMostSimpleCase();

    private:

		sfl::Pattern *pattern;

		sfl::SdesNegotiator *sdesnego;

		std::vector<std::string> *remoteOffer;

		std::vector<sfl::CryptoSuiteDefinition> *localCapabilities;
};

/* Register our test module */
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SdesNegotiatorTest, "SdesNegotiatorTest");
CPPUNIT_TEST_SUITE_REGISTRATION( SdesNegotiatorTest );

#endif
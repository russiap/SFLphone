/* $Id: rand.c 2394 2008-12-23 17:27:53Z bennylp $ */
/* 
 * Copyright (C) 2008-2009 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 *
 *  Additional permission under GNU GPL version 3 section 7:
 *
 *  If you modify this program, or any covered work, by linking or
 *  combining it with the OpenSSL project's OpenSSL library (or a
 *  modified version of that library), containing parts covered by the
 *  terms of the OpenSSL or SSLeay licenses, Teluu Inc. (http://www.teluu.com)
 *  grants you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of OpenSSL used as well
 *  as that of the covered work.
 */
#include <pj/rand.h>
#include <pj/log.h>
#include "test.h"

#if INCLUDE_RAND_TEST

#define COUNT  1024
static int values[COUNT];

/*
 * rand_test(), simply generates COUNT number of random number and
 * check that there's no duplicate numbers.
 */
int rand_test(void)
{
    int i;

    for (i=0; i<COUNT; ++i) {
	int j;

	values[i] = pj_rand();
	for (j=0; j<i; ++j) {
	    if (values[i] == values[j]) {
		PJ_LOG(3,("test", "error: duplicate value %d at %d-th index",
			 values[i], i));
		return -10;
	    }
	}
    }

    return 0;
}

#endif	/* INCLUDE_RAND_TEST */


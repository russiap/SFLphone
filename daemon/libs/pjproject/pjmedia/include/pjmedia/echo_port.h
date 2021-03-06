/* $Id: echo_port.h 2394 2008-12-23 17:27:53Z bennylp $ */
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
#ifndef __PJMEDIA_AEC_PORT_H__
#define __PJMEDIA_AEC_PORT_H__

/**
 * @file echo_port.h
 * @brief AEC (Accoustic Echo Cancellation) media port.
 */
#include <pjmedia/port.h>



/**
 * @defgroup PJMEDIA_ECHO_PORT Echo Cancellation Port
 * @ingroup PJMEDIA_PORT
 * @brief Echo Cancellation
 * @{
 *
 * Wrapper to \ref PJMEDIA_Echo_Cancel into media port interface.
 */


PJ_BEGIN_DECL


/**
 * Create echo canceller port. 
 *
 * @param pool		Pool to allocate memory.
 * @param dn_port	Downstream port.
 * @param tail_ms	Tail length in miliseconds.
 * @param latency_ms	Total lacency introduced by playback and 
 *			recording device. Set to zero if the latency
 *			is not known.
 * @param options	Options, as in #pjmedia_echo_create().
 * @param p_port	Pointer to receive the port instance.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_echo_port_create(pj_pool_t *pool,
					      pjmedia_port *dn_port,
					      unsigned tail_ms,
					      unsigned latency_ms,
					      unsigned options,
					      pjmedia_port **p_port );



PJ_END_DECL

/**
 * @}
 */


#endif	/* __PJMEDIA_AEC_PORT_H__ */

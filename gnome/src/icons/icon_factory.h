/*
 *  Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010, 2011 Savoir-Faire Linux Inc.
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

#ifndef ICON_FACTORY_H
#define ICON_FACTORY_H

#include <gtk/gtkiconfactory.h>
#include "icons/pixmap_data.h"
#include "sflphone_const.h"

G_BEGIN_DECLS

#define GTK_STOCK_PICKUP				"gnome-stock-pickup"
#define GTK_STOCK_HANGUP				"gnome-stock-hangup"
#define GTK_STOCK_ONHOLD				"gnome-stock-onhold"
#define GTK_STOCK_OFFHOLD				"gnome-stock-offhold"
#define GTK_STOCK_IM					"gnome-stock-im"
#define GTK_STOCK_TRANSFER				"gnome-stock-transfer"
#define GTK_STOCK_DIAL					"gnome-stock-dial"
#define GTK_STOCK_CALL_CURRENT			"gnome-stock-call-current"
#define GTK_STOCK_ADDRESSBOOK			"gnome-stock-addressbook"
#define GTK_STOCK_CALLS					"gnome-stock-calls"
#define GTK_STOCK_SFLPHONE				"gnome-stock-sflphone"
#define GTK_STOCK_FAIL					"gnome-stock-fail"
#define GTK_STOCK_USER					"gnome-stock-user"

void init_icon_factory (void);

GtkIconSet* lookup_sflphone_factory (const gchar *stock_id);

G_END_DECLS

#endif

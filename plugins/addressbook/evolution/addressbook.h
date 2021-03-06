/*
 *  Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010, 2011 Savoir-Faire Linux Inc.
 *  Author: Julien Bonjean <julien.bonjean@savoirfairelinux.com>
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

/**
 * This file contains functions specific for addressbook.
 * It is used as a "mapping" between real search implementation
 * and search bar.
 */

#ifndef __ADDRESSBOOK_H__
#define __ADDRESSBOOK_H__

#include <gtk/gtk.h>
// #include <addressbook/eds.h>


#define EMPTY_ENTRY     "empty"

typedef enum {ABOOK_QUERY_IS, ABOOK_QUERY_BEGINS_WITH, ABOOK_QUERY_CONTAINS} AddrbookSearchType;

/**
 * Represent a contact entry
 */
typedef struct _Hit {
    gchar *name;
    GdkPixbuf *photo;
    gchar *phone_business;
    gchar *phone_home;
    gchar *phone_mobile;
} Hit;

/**
 * Book structure for "outside world"
 */
typedef struct {
    gchar *uid;
    gchar *uri;
    gchar *name;
    gboolean active;
    gboolean isdefault;
} book_data_t;

typedef struct _AddressBook_Config {
    // gint64: a signed integer guaranteed to be 64 bits on all platforms
    // To print or scan values of this type, use G_GINT64_MODIFIER and/or G_GINT64_FORMAT
    gint enable;
    gint max_results;
    gint display_contact_photo;
    gint search_phone_home;
    gint search_phone_business;
    gint search_phone_mobile;
} AddressBook_Config;

typedef struct AddrBookHandle AddrBookHandle;

struct AddrBookHandle {
    void (*init) (gchar **);
    gboolean (*is_ready) (void);
    gboolean (*is_enabled) (void);
    gboolean (*is_active) (void);
    void (*search) (AddrBookHandle *, GtkEntry *, AddressBook_Config *);
    GSList *(*get_books_data)(gchar **);
    book_data_t *(*get_book_data_by_uid)(gchar *);
    void (*set_current_book)(gchar *); 
    void (*set_search_type)(AddrbookSearchType);
    void (*search_cb)(GList *, gpointer); 
};

/**
 * Initialize addressbook
 */
void addressbook_init(gchar **book_list);

/**
 * Return addressbook state
 */
gboolean addressbook_is_ready();

/**
 * Return addressbook state
 */
gboolean addressbook_is_enabled();

/**
 * Return if at least one addressbook is active
 */
gboolean addressbook_is_active();


/**
 * Perform a search in addressbook
 */
void addressbook_search (AddrBookHandle *, GtkEntry *, AddressBook_Config *);

/**
 * Get a list of addressbook book
 */
GSList *addressbook_get_books_data(gchar **book_list);

book_data_t *addressbook_get_book_data_by_uid(gchar *);

void addressbook_set_current_book(gchar *);

void addressbook_set_search_type(AddrbookSearchType);

#endif

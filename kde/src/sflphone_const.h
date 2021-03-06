/*
 *  Copyright (C) 2008 Savoir-Faire Linux inc.
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

#ifndef __SFLPHONE_CONST_H
#define __SFLPHONE_CONST_H

#include <QtCore/QString>

/* @file sflphone_const.h
 * @brief Contains the global variables for the client code
 */
 
#define APP_NAME                          "SFLphone KDE Client"

#define SIP                               0
#define IAX                               1

#define TOOLBAR_SIZE                      22

#define CONTACT_ITEM_HEIGHT               40

#define CONFIG_FILE_PATH                  "/.sflphone/sflphonedrc"

#define ACTION_LABEL_CALL                 i18n("New call")
#define ACTION_LABEL_HANG_UP              i18n("Hang up")
#define ACTION_LABEL_HOLD                 i18n("Hold on")
#define ACTION_LABEL_TRANSFER             i18n("Transfer")
#define ACTION_LABEL_RECORD               i18n("Record")
#define ACTION_LABEL_ACCEPT               i18n("Pick up")
#define ACTION_LABEL_REFUSE               i18n("Hang up")
#define ACTION_LABEL_UNHOLD               i18n("Hold off")
#define ACTION_LABEL_GIVE_UP_TRANSF       i18n("Give up transfer")
#define ACTION_LABEL_CALL_BACK            i18n("Call back")
#define ACTION_LABEL_MAILBOX              i18n("Voicemail")

#define SCREEN_MAIN                       0
#define SCREEN_HISTORY                    1
#define SCREEN_ADDRESS                    2

#define ICON_INCOMING                     ":/images/icons/ring.svg"
#define ICON_RINGING                      ":/images/icons/ring.svg"
#define ICON_CURRENT                      ":/images/icons/current.svg"
#define ICON_CURRENT_REC                  ":/images/icons/rec_call.svg"
#define ICON_DIALING                      ":/images/icons/dial.svg"
#define ICON_HOLD                         ":/images/icons/hold.svg"
#define ICON_FAILURE                      ":/images/icons/fail.svg"
#define ICON_BUSY                         ":/images/icons/busy.svg"
#define ICON_TRANSFER                     ":/images/icons/transfert.svg"
#define ICON_TRANSF_HOLD                  ":/images/icons/transfert.svg"
#define ICON_CONFERENCE                   ":/images/icons/user-group-properties.svgz"

#define ICON_CALL                         ":/images/icons/call.svg"
#define ICON_HANGUP                       ":/images/icons/hang_up.svg"
#define ICON_UNHOLD                       ":/images/icons/unhold.svg"
#define ICON_ACCEPT                       ":/images/icons/accept.svg"
#define ICON_REFUSE                       ":/images/icons/refuse.svg"
#define ICON_EXEC_TRANSF                  ":/images/icons/call.svg"
#define ICON_REC_DEL_OFF                  ":/images/icons/record_disabled.svg"
#define ICON_REC_DEL_ON                   ":/images/icons/record.svg"
#define ICON_MAILBOX                      ":/images/icons/mailbox.svg"

#define ICON_REC_VOL_0                    ":/images/icons/mic.svg"
#define ICON_REC_VOL_1                    ":/images/icons/mic_25.svg"
#define ICON_REC_VOL_2                    ":/images/icons/mic_50.svg"
#define ICON_REC_VOL_3                    ":/images/icons/mic_75.svg"

#define ICON_SND_VOL_0                    ":/images/icons/speaker.svg"
#define ICON_SND_VOL_1                    ":/images/icons/speaker_25.svg"
#define ICON_SND_VOL_2                    ":/images/icons/speaker_50.svg"
#define ICON_SND_VOL_3                    ":/images/icons/speaker_75.svg"

#define ICON_SCREEN_MAIN                  ":/images/icons/sflphone.svg"
#define ICON_SCREEN_HISTORY               ":/images/icons/history2.svg"
#define ICON_SCREEN_ADDRESS               ":/images/icons/x-office-address-book.png"

#define ICON_DISPLAY_VOLUME_CONSTROLS     ":/images/icons/icon_volume_off.svg"
#define ICON_DISPLAY_DIALPAD              ":/images/icons/icon_dialpad.svg"

#define ICON_HISTORY_INCOMING             ":/images/icons/incoming.svg"
#define ICON_HISTORY_OUTGOING             ":/images/icons/outgoing.svg"
#define ICON_HISTORY_MISSED               ":/images/icons/missed.svg"

#define ICON_ACCOUNT_LED_RED              ":/images/icons/led-red.svg"
#define ICON_ACCOUNT_LED_GREEN            ":/images/icons/led-green.svg"
#define ICON_ACCOUNT_LED_GRAY             ":/images/icons/led-gray.svg"

#define ICON_QUIT                         ":/images/icons/application-exit.png"

#define ICON_SFLPHONE                     ":/images/icons/sflphone.svg"
#define ICON_TRAY_NOTIF                   ":/images/icons/sflphone_notif.svg"

#define RECORD_DEVICE                     "mic"
#define SOUND_DEVICE                      "speaker"


/** Account details */
#define ACCOUNT_TYPE                     "Account.type"
#define ACCOUNT_ALIAS		         "Account.alias"
#define ACCOUNT_ENABLED		         "Account.enable"
#define ACCOUNT_MAILBOX		         "Account.mailbox"
#define ACCOUNT_RESOLVE_ONCE             "Account.resolveOnce"
#define ACCOUNT_REGISTRATION_EXPIRE      "Account.expire"
#define ACCOUNT_SIP_STUN_SERVER	         "STUN.server"
#define ACCOUNT_SIP_STUN_ENABLED         "STUN.enable"
#define ACCOUNT_HOSTNAME                 "hostname"
#define ACCOUNT_USERNAME                 "username"
#define ACCOUNT_PASSWORD                 "password"
#define ACCOUNT_AUTHENTICATION_USERNAME  "authenticationUsername"
#define ACCOUNT_REALM                    "realm"
#define ACCOUNT_KEY_EXCHANGE             "SRTP.keyExchange"
#define ACCOUNT_SRTP_ENABLED             "SRTP.enable"
#define ACCOUNT_ZRTP_DISPLAY_SAS         "ZRTP.displaySAS"
#define ACCOUNT_ZRTP_NOT_SUPP_WARNING    "ZRTP.notSuppWarning"
#define ACCOUNT_ZRTP_HELLO_HASH          "ZRTP.helloHashEnable"
#define ACCOUNT_DISPLAY_SAS_ONCE         "ZRTP.displaySasOnce"
#define KEY_EXCHANGE_NONE                "0"
#define ZRTP                             "1"
#define SDES                             "2"

/** TLS */
#define TLS_LISTENER_PORT                   "TLS.listenerPort"
#define TLS_ENABLE                       "TLS.enable"
#define TLS_PORT                         "TLS.port"
#define TLS_CA_LIST_FILE                 "TLS.certificateListFile"
#define TLS_CERTIFICATE_FILE             "TLS.certificateFile"
#define TLS_PRIVATE_KEY_FILE             "TLS.privateKeyFile"
#define TLS_PASSWORD                     "TLS.password"
#define TLS_METHOD                       "TLS.method"
#define TLS_CIPHERS                      "TLS.ciphers"
#define TLS_SERVER_NAME                  "TLS.serverName"
#define TLS_VERIFY_SERVER                "TLS.verifyServer"
#define TLS_VERIFY_CLIENT                "TLS.verifyClient"
#define TLS_REQUIRE_CLIENT_CERTIFICATE   "TLS.requireClientCertificate"  
#define TLS_NEGOTIATION_TIMEOUT_SEC      "TLS.negotiationTimeoutSec"
#define TLS_NEGOTIATION_TIMEOUT_MSEC     "TLS.negotiationTimemoutMsec"

#define LOCAL_INTERFACE                  "Account.localInterface"
#define PUBLISHED_SAMEAS_LOCAL           "Account.publishedSameAsLocal"
#define LOCAL_PORT                       "Account.localPort"
#define PUBLISHED_PORT                   "Account.publishedPort"
#define PUBLISHED_ADDRESS                "Account.publishedAddress"

#define REGISTRATION_STATUS              "Status"
#define REGISTRATION_STATE_CODE          "Registration.code" 
#define REGISTRATION_STATE_DESCRIPTION   "Registration.description"


/** Maybe to remove **/
#define ACCOUNT_EXPIRE                   "Account.expire"
#define ACCOUNT_STATUS                   "Status"
#define ACCOUNT_EXPIRE_DEFAULT            600
#define ACCOUNT_ENABLED_TRUE              "true"
#define ACCOUNT_ENABLED_FALSE             "false"
#define ACCOUNT_TYPE_SIP                  "SIP"
#define ACCOUNT_TYPE_IAX                  "IAX"
#define ACCOUNT_TYPES_TAB                 {QString(ACCOUNT_TYPE_SIP), QString(ACCOUNT_TYPE_IAX)}
/*********************/

/** Constant variables */
#define ACCOUNT_MAILBOX_DEFAULT_VALUE     "888"

/** Account States */
#define ACCOUNT_STATE_REGISTERED          "REGISTERED"
#define ACCOUNT_STATE_UNREGISTERED        "UNREGISTERED"
#define ACCOUNT_STATE_TRYING              "TRYING"
#define ACCOUNT_STATE_ERROR               "ERROR"
#define ACCOUNT_STATE_ERROR_AUTH          "ERROR_AUTH"
#define ACCOUNT_STATE_ERROR_NETWORK       "ERROR_NETWORK"
#define ACCOUNT_STATE_ERROR_HOST          "ERROR_HOST"
#define ACCOUNT_STATE_ERROR_CONF_STUN     "ERROR_CONF_STUN"
#define ACCOUNT_STATE_ERROR_EXIST_STUN    "ERROR_EXIST_STUN"

/** Calls details */
#define CALL_PEER_NAME                    "DISPLAY_NAME"
//#define CALL_PEER_NAME                    "PEER_NAME"
#define CALL_PEER_NUMBER                  "PEER_NUMBER"
#define CALL_ACCOUNTID                    "ACCOUNTID"
#define CALL_STATE                        "CALL_STATE"
#define CALL_TYPE                         "CALL_TYPE"

/** Call States */
#define CALL_STATE_CHANGE_HUNG_UP         "HUNGUP"
#define CALL_STATE_CHANGE_RINGING         "RINGING"
#define CALL_STATE_CHANGE_CURRENT         "CURRENT"
#define CALL_STATE_CHANGE_HOLD            "HOLD"
#define CALL_STATE_CHANGE_BUSY            "BUSY"
#define CALL_STATE_CHANGE_FAILURE         "FAILURE"
#define CALL_STATE_CHANGE_UNHOLD_CURRENT  "UNHOLD_CURRENT"
#define CALL_STATE_CHANGE_UNHOLD_RECORD   "UNHOLD_RECORD"
#define CALL_STATE_CHANGE_UNKNOWN         "UNKNOWN"

#define DAEMON_CALL_STATE_INIT_CURRENT    "CURRENT"
#define DAEMON_CALL_STATE_INIT_HOLD       "HOLD"
#define DAEMON_CALL_STATE_INIT_BUSY       "BUSY"
#define DAEMON_CALL_STATE_INIT_INCOMING       "INCOMING"
#define DAEMON_CALL_STATE_INIT_RINGING       "RINGING"
#define DAEMON_CALL_STATE_INIT_INACTIVE   "INACTIVE"

#define DAEMON_CALL_TYPE_INCOMING         "0"
#define DAEMON_CALL_TYPE_OUTGOING         "1"

#define DAEMON_HISTORY_TYPE_MISSED        "0"
#define DAEMON_HISTORY_TYPE_OUTGOING      "1"
#define DAEMON_HISTORY_TYPE_INCOMING      "2"

/** Address Book Settings */
#define ADDRESSBOOK_MAX_RESULTS           "ADDRESSBOOK_MAX_RESULTS"
#define ADDRESSBOOK_DISPLAY_CONTACT_PHOTO "ADDRESSBOOK_DISPLAY_CONTACT_PHOTO"
#define ADDRESSBOOK_DISPLAY_BUSINESS      "ADDRESSBOOK_DISPLAY_PHONE_BUSINESS"
#define ADDRESSBOOK_DISPLAY_HOME          "ADDRESSBOOK_DISPLAY_PHONE_HOME"
#define ADDRESSBOOK_DISPLAY_MOBILE        "ADDRESSBOOK_DISPLAY_PHONE_MOBILE"
#define ADDRESSBOOK_ENABLE                "ADDRESSBOOK_ENABLE"

/** Hooks settings */
#define HOOKS_ADD_PREFIX                  "PHONE_NUMBER_HOOK_ADD_PREFIX"
#define HOOKS_ENABLED                     "PHONE_NUMBER_HOOK_ENABLED"
#define HOOKS_COMMAND                     "URLHOOK_COMMAND"
#define HOOKS_IAX2_ENABLED                "URLHOOK_IAX2_ENABLED"
#define HOOKS_SIP_ENABLED                 "URLHOOK_SIP_ENABLED"
#define HOOKS_SIP_FIELD                   "URLHOOK_SIP_FIELD"

/** Constant variables */
#define MAX_HISTORY_CAPACITY              60

/** Codecs details */
#define CODEC_NAME                        0
#define CODEC_SAMPLE_RATE                 1
#define CODEC_BIT_RATE                    2
#define CODEC_BANDWIDTH                   3

/** Audio Managers */
#define CONST_ALSA                        0
#define CONST_PULSEAUDIO                  1

/** TLS */
#define TLS_LISTENER_PORT                   "TLS.listenerPort"
#define TLS_ENABLE                          "TLS.enable"
#define TLS_PORT                            "TLS.port"
#define TLS_CA_LIST_FILE                    "TLS.certificateListFile"
#define TLS_CERTIFICATE_FILE                "TLS.certificateFile"
#define TLS_PRIVATE_KEY_FILE                "TLS.privateKeyFile"
#define TLS_PASSWORD                        "TLS.password"
#define TLS_METHOD                          "TLS.method"
#define TLS_CIPHERS                         "TLS.ciphers"
#define TLS_SERVER_NAME                     "TLS.serverName"
#define TLS_VERIFY_SERVER                   "TLS.verifyServer"
#define TLS_VERIFY_CLIENT                   "TLS.verifyClient"
#define TLS_REQUIRE_CLIENT_CERTIFICATE      "TLS.requireClientCertificate"  
#define TLS_NEGOTIATION_TIMEOUT_SEC         "TLS.negotiationTimeoutSec"
#define TLS_NEGOTIATION_TIMEOUT_MSEC        "TLS.negotiationTimemoutMsec"

#define ACCOUNT_ID                         "Account.id"
#define ACCOUNT_PASSWORD                   "password"
#define ACCOUNT_AUTHENTICATION_USERNAME    "authenticationUsername"
#define ACCOUNT_REALM                      "realm"
#define ACCOUNT_KEY_EXCHANGE               "SRTP.keyExchange"
#define ACCOUNT_SRTP_ENABLED               "SRTP.enable"
#define ACCOUNT_SRTP_RTP_FALLBACK          "SRTP.rtpFallback"
#define ACCOUNT_ZRTP_DISPLAY_SAS           "ZRTP.displaySAS"
#define ACCOUNT_ZRTP_NOT_SUPP_WARNING      "ZRTP.notSuppWarning"
#define ACCOUNT_ZRTP_HELLO_HASH            "ZRTP.helloHashEnable"
#define ACCOUNT_DISPLAY_SAS_ONCE           "ZRTP.displaySasOnce"
#define KEY_EXCHANGE_NONE                  "0"
#define ZRTP                               "1"
#define SDES                               "2"

/** MIME API */
#define MIME_CALLID "text/sflphone.call.id"
#define MIME_PLAIN_TEXT "text/plain"
#endif

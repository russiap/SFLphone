/*
 *  Copyright (C) 2004, 2005, 2006, 2009, 2008, 2009, 2010 Savoir-Faire Linux Inc.
 *  Author: Pierre-Luc Bacon <pierre-luc.bacon@savoirfairelinux.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
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

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "SFLVideoCairoShm.h"
#include "util/video_endpoint.h"
#include "sflphone_const.h"

#define FOURCC(a,b,c,d) ( (uint32_t) ((((((d) << 8) | (c)) << 8) | (b)) << 8) | (a) )

G_DEFINE_TYPE (SFLVideoCairoShm, sfl_video_cairo_shm, GTK_TYPE_DRAWING_AREA)

#define GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SFL_TYPE_VIDEO_CAIRO_SHM, SFLVideoCairoShmPrivate))

typedef struct _SFLVideoCairoShmPrivate SFLVideoCairoShmPrivate;
struct _SFLVideoCairoShmPrivate
{
  cairo_surface_t* surface;
  unsigned char* image_data;
  int image_stride;

  gchar* shm;

  guint width;
  guint height;
  guint fourcc;

  sflphone_video_endpoint_t* endpt;
};

/**
 * Properties
 */
enum
{
  PROP_SHM_PATH = 1, LAST_PROPERTY
} SFLVideoCairoShmProperties;

/**
 * Signals
 */
enum
{
  PLAYING, LAST_SIGNAL
};

static guint sfl_video_cairo_shm_signals[LAST_SIGNAL] =
  { 0 };

static cairo_format_t
fourcc_to_video_cairo (uint32_t fourcc)
{
  switch (fourcc)
    {
  case FOURCC('R', 'G', 'B', 'A'):
    return CAIRO_FORMAT_ARGB32;
  case FOURCC('R','G','B','3'):
    return CAIRO_FORMAT_RGB24;
  default:
    ERROR("No cairo format for fourcc value %d", fourcc);
    break;
    }

  return -1;
}

static char*
get_timestamp ()
{
  time_t rawtime;
  time (&rawtime);

  struct tm* timeinfo = localtime (&rawtime);

  static char output[80];
  strftime (output, 80, "%d-%m-%Y-%H-%M-%S", timeinfo);

  return output;
}

static void
cairo_dump_buffer (guchar* pucPixelBuffer, gint width, gint height,
    cairo_format_t format)
{
  g_assert (pucPixelBuffer != NULL);
  g_assert (height != 0);

  // Create surface
  cairo_surface_t* surface = cairo_image_surface_create_for_data (
      pucPixelBuffer, format, width, height, cairo_format_stride_for_width (
          format, width));

  cairo_status_t status = cairo_surface_status (surface);
  if (status != CAIRO_STATUS_SUCCESS)
    {
      ERROR("While creating cairo surface for dumping image to png : (%s)", cairo_status_to_string (status));
      return;
    }

  // Create file name based on current date
  gchar* filename =
      g_strconcat ("buffer-dump-", get_timestamp (), ".png", NULL);

  // Write image file
  DEBUG("Writing filename : %s", filename);
  cairo_surface_write_to_png (surface, filename);

  g_free (filename);
  cairo_surface_destroy (surface);
}

static void
on_new_frame_cb (uint8_t* frame, void* widget)
{
  SFLVideoCairoShm* self = SFL_VIDEO_CAIRO_SHM(widget);
  SFLVideoCairoShmPrivate* priv = GET_PRIVATE(self);

  // Return if the widget is not yet realized
  if (gtk_widget_get_realized (GTK_WIDGET (self)) != TRUE)
    {
      return;
    }

  // Copy the frame into the image surface
  memcpy (priv->image_data, frame, priv->width * priv->height * 4); // FIXME. Hardcoded value 4

  gtk_widget_queue_draw (GTK_WIDGET(self));
}

static void
sfl_video_cairo_shm_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SFLVideoCairoShm *self = SFL_VIDEO_CAIRO_SHM(object);
  SFLVideoCairoShmPrivate *priv = GET_PRIVATE(self);

  switch (property_id)
    {
  case PROP_SHM_PATH:
    priv->shm = g_value_dup_string (value);
    DEBUG("Setting property shm path to %s", priv->shm);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    return;
    }
}

static void
sfl_video_cairo_shm_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SFLVideoCairoShm *self = SFL_VIDEO_CAIRO_SHM(object);
  SFLVideoCairoShmPrivate *priv = GET_PRIVATE(self);

  switch (property_id)
    {
  case PROP_SHM_PATH:
    g_value_set_string (value, priv->shm);
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
sfl_video_cairo_shm_finalize (GObject *object)
{
  SFLVideoCairoShm *self = SFL_VIDEO_CAIRO_SHM(object);
  SFLVideoCairoShmPrivate *priv = GET_PRIVATE(self);

  free (priv->image_data);
  cairo_surface_destroy (priv->surface);
  g_free (priv->shm);

  G_OBJECT_CLASS (sfl_video_cairo_shm_parent_class)->finalize (object);
}

static gboolean
sfl_video_cairo_shm_expose (GtkWidget* cairo_video, GdkEventExpose* event)
{
  SFLVideoCairoShmPrivate* priv = GET_PRIVATE (cairo_video);

  // Create the cairo context
  cairo_t* cr = gdk_cairo_create (cairo_video->window);

  // Deal with resizing
  GtkAllocation allocation;
  gtk_widget_get_allocation (cairo_video, &allocation);

  DEBUG("Allocation %d %d image width %d %d\n", allocation.width, allocation.height, priv->width, priv->height);

  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);

  cairo_set_source_surface (cr, priv->surface, 0, 0);
  cairo_paint (cr);

  // Get rid of the cairo context
  cairo_destroy (cr);

  return FALSE;
}

static void
sfl_video_cairo_shm_size_request (GtkWidget* widget,
    GtkRequisition* requisition)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(SFL_IS_VIDEO_CAIRO_SHM(widget));

  DEBUG("Size request");

  requisition->width = 640;
  requisition->height = 480;
}

static void
sfl_video_cairo_shm_class_init (SFLVideoCairoShmClass *class)
{
  GObjectClass* obj_class = G_OBJECT_CLASS (class);
  obj_class->get_property = sfl_video_cairo_shm_get_property;
  obj_class->set_property = sfl_video_cairo_shm_set_property;
  obj_class->finalize = sfl_video_cairo_shm_finalize;

  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (class);
  widget_class->expose_event = sfl_video_cairo_shm_expose;
  widget_class->size_request = sfl_video_cairo_shm_size_request;

  // Install properties
  g_object_class_install_property (obj_class, PROP_SHM_PATH,
      g_param_spec_string ("shm", "shm",
          "The shared memory segment to read data from.", NULL,
          G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB
              | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

  // Install signals
  sfl_video_cairo_shm_signals[PLAYING] = g_signal_new ("playing",
      G_TYPE_FROM_CLASS(class), (GSignalFlags) (G_SIGNAL_RUN_FIRST
          | G_SIGNAL_ACTION), 0, NULL, NULL, g_cclosure_marshal_VOID__VOID,
      G_TYPE_NONE, 0);

  // Install private structure
  g_type_class_add_private (obj_class, sizeof(SFLVideoCairoShmPrivate));
}

static void
sfl_video_cairo_shm_init (SFLVideoCairoShm *self)
{
  SFLVideoCairoShmPrivate *priv = GET_PRIVATE(self);

  DEBUG("Initializing video cairo shm widget ...");

  // Create a new video endpoint for reading frames in the SHM
  priv->endpt = sflphone_video_init ();
}

int
sfl_video_cairo_shm_start (SFLVideoCairoShm *self)
{
  SFLVideoCairoShmPrivate* priv = GET_PRIVATE (self);

  // Get the SHM info from Dbus
  video_shm_info* info = dbus_get_video_shm_info (priv->shm);
  if (info == NULL)
    {
      ERROR("Failed to retrieve SHM info for segment %s", priv->shm);
      return;
    }

  priv->width = info->width;
  priv->height = info->height;
  priv->fourcc = info->fourcc;
  DEBUG("Shared memory segment has format %d %d and color space %d", priv->width, priv->height, priv->fourcc);
  g_free (info);

  // Allocate the cairo surface
  unsigned int size = priv->width * priv->height * 4;

  priv->image_data = malloc (size); // FIXME
  memset (priv->image_data, 0x000000ff, size);

  cairo_format_t format = fourcc_to_video_cairo (priv->fourcc);
  if (format == -1)
    {
      ERROR("A valid cairo format cannot be found for FOURCC value %d", priv->fourcc);
      return -1;
    }
  priv->image_stride = cairo_format_stride_for_width (format, priv->width);

  // Create the cairo surface for data
  priv->surface = cairo_image_surface_create_for_data (priv->image_data,
      format, priv->width, priv->height, priv->image_stride);

  // Create a new endpoint
  priv->endpt = sflphone_video_init ();

  // Register as an observer
  if (sflphone_video_add_observer (priv->endpt, &on_new_frame_cb, self) < 0)
    {
      ERROR("Failed to register as an observer and start video %s:%d", __FILE__, __LINE__);
      return -1;
    }
  DEBUG("Registered as an observer on \"%s\".", priv->shm);

  // Open the video segment
  if (sflphone_video_open (priv->endpt, priv->shm) < 0)
    {
      ERROR("Failed to open and start video %s:%d", __FILE__, __LINE__);
      return -1;
    }

  // Start capturing data
  if (sflphone_video_start_async (priv->endpt) < 0)
    {
      ERROR("Failed to start video %s:%d", __FILE__, __LINE__);
      return -1;
    }

  // Emit the "playing" signal on the first frame received
  g_signal_emit (self, sfl_video_cairo_shm_signals[PLAYING], 0);

  DEBUG("Playing signal emitted");

  return 0;
}

SFLVideoCairoShm*
sfl_video_cairo_shm_new (const gchar* shm)
{
  return g_object_new (SFL_TYPE_VIDEO_CAIRO_SHM, "shm", shm, NULL);
}
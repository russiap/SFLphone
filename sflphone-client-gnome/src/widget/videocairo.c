#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "videocairo.h"
#include "util/video_endpoint.h"
#include "sflphone_const.h"

#define VIDEO_CAIRO_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIDEO_TYPE_CAIRO, VideoCairoPrivate))

typedef struct _VideoCairoPrivate VideoCairoPrivate;
struct _VideoCairoPrivate
{
  cairo_surface_t* surface;
  unsigned char* image_data;
  int image_stride;
  gchar* source;
  gchar* fps;
  gint width;
  gint height;
  gboolean capturing;
  sflphone_video_endpoint_t* endpt;
};

static gpointer video_cairo_parent_class = NULL;

enum
{
  PROP_SOURCE = 1, PROP_WIDTH, PROP_HEIGHT, PROP_FPS, LAST_PROPERTY
} VideoCairoProperties;

static const int DEFAULT_NO_DEVICE_WIDTH = 320;
static const int DEFAULT_NO_DEVICE_HEIGHT = 240;
static const int DEFAULT_BPP = 4;
static const gchar* DEFAULT_FPS = "30/1";

static void
reallocate_buffer (VideoCairo* self)
{
  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE((VideoCairo*) self);

  DEBUG("Reallocating buffers");

  free (priv->image_data);

  priv->image_data = malloc (priv->width * priv->height * DEFAULT_BPP);

  memset (priv->image_data, 0x000000ff, priv->width * priv->height
      * DEFAULT_BPP);

  priv->image_stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32,
      priv->width);

  priv->surface = cairo_image_surface_create_for_data (priv->image_data,
      CAIRO_FORMAT_ARGB32, priv->width, priv->height, priv->image_stride);
}

static void
video_cairo_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  VideoCairo *self = VIDEO_CAIRO(object);
  VideoCairoPrivate *priv = VIDEO_CAIRO_GET_PRIVATE(self);

  DEBUG("Setting property.");

  switch (property_id)
    {
  case PROP_SOURCE:
    DEBUG("Setting source %s", g_value_get_string (value))
    ;
    priv->source = g_strdup (g_value_get_string (value));
    sflphone_video_set_device (priv->endpt, priv->source);
    break;
  case PROP_WIDTH:
    DEBUG("Setting width %d", g_value_get_int(value))
    ;
    priv->width = g_value_get_int (value);
    reallocate_buffer (self);
    sflphone_video_set_width (priv->endpt, priv->width);
    break;
  case PROP_HEIGHT:
    DEBUG("Setting height %d", g_value_get_int(value))
    ;
    priv->height = g_value_get_int (value);
    reallocate_buffer (self);
    sflphone_video_set_height (priv->endpt, priv->height);
    break;
  case PROP_FPS:
    DEBUG("Setting fps %s", g_value_get_string(value))
    ;
    priv->fps = g_strdup (g_value_get_string (value));
    sflphone_video_set_framerate (priv->endpt, priv->fps);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    return;
    }
}

static void
video_cairo_get_property (GObject *object, guint property_id, GValue *value,
    GParamSpec *pspec)
{
  VideoCairo *self = VIDEO_CAIRO(object);
  VideoCairoPrivate *priv = VIDEO_CAIRO_GET_PRIVATE(self);

  switch (property_id)
    {
  case PROP_SOURCE:
    g_value_set_string (value, priv->source);
    break;
  case PROP_WIDTH:
    g_value_set_int (value, priv->width);
    break;
  case PROP_HEIGHT:
    g_value_set_int (value, priv->height);
    break;
  case PROP_FPS:
    g_value_set_string (value, priv->fps);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

void
video_cairo_set_source (VideoCairo* video_cairo, gchar* source)
{
  DEBUG("Setting source (%s)", source);
  g_object_set (G_OBJECT(video_cairo), "source", source, NULL);
}

void
video_cairo_set_capture_width (VideoCairo* video_cairo, gint width)
{
  DEBUG("Setting width (%d)", width);
  g_object_set (G_OBJECT(video_cairo), "width", width, NULL);
}

void
video_cairo_set_capture_height (VideoCairo* video_cairo, gint height)
{
  DEBUG("Setting height (%d)", height);
  g_object_set (G_OBJECT(video_cairo), "height", height, NULL);
}

void
video_cairo_set_capture_framerate (VideoCairo* video_cairo, gchar* fps)
{
  DEBUG("Setting frame rate (%s)", fps);
  g_object_set (G_OBJECT(video_cairo), "fps", fps, NULL);
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
cairo_dump_buffer (guchar* pucPixelBuffer, /* pointer to image-data */
gint iWidth, /* width of image        */
gint iHeight /* height of image       */)
{
  cairo_surface_t* pSurface = NULL;
  cairo_status_t status = CAIRO_STATUS_SUCCESS;

  /* sanity check */
  g_assert (pucPixelBuffer != NULL);
  g_assert (iHeight != 0);

  /* create surface from supplied texture-data */
  pSurface = cairo_image_surface_create_for_data (pucPixelBuffer,
      CAIRO_FORMAT_RGB24, iWidth, iHeight, iWidth);
  /* check for errors */
  status = cairo_surface_status (pSurface);
  if (status != CAIRO_STATUS_SUCCESS)
    {
      ERROR("While creating cairo surface for dumping image to png : (%s)", cairo_status_to_string (status));
      return;
    }

  gchar* filename =
      g_strconcat ("buffer-dump-", get_timestamp (), ".png", NULL);
  DEBUG("Writing filename : %s", filename);
  cairo_surface_write_to_png (pSurface, filename);
  g_free (filename);

  cairo_surface_destroy (pSurface);
}

static void
video_cairo_redraw_canvas (VideoCairo* self)
{
  GtkWidget *widget;
  GdkRegion *region;

  widget = GTK_WIDGET (self);
  if (!widget->window)
    return;

  region = gdk_drawable_get_clip_region (widget->window);

  /* redraw the cairo canvas completely by exposing it */
  gdk_window_invalidate_region (widget->window, region, TRUE);
  //gdk_window_process_updates (widget->window, TRUE);

  gdk_region_destroy (region);
}

static void
on_new_frame_cb (uint8_t* frame, void* widget)
{
  // DEBUG("Got frame");

  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE((VideoCairo*) widget);

  // Copy the frame into the image surface
  memcpy (priv->image_data, frame, priv->width * priv->height * DEFAULT_BPP);

  gtk_widget_queue_draw (GTK_WIDGET(widget));
  //video_cairo_redraw_canvas((VideoCairo*) widget);
}

static void
video_cairo_init (VideoCairo *self)
{
  VideoCairoPrivate *priv = VIDEO_CAIRO_GET_PRIVATE(self);
  DEBUG("Initializing cairo");

  priv->endpt = sflphone_video_init ();

  priv->image_data = malloc (DEFAULT_NO_DEVICE_WIDTH * DEFAULT_NO_DEVICE_HEIGHT
      * DEFAULT_BPP);
  memset (priv->image_data, 0x000000ff, DEFAULT_NO_DEVICE_WIDTH
      * DEFAULT_NO_DEVICE_HEIGHT * DEFAULT_BPP);
  priv->image_stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32,
      DEFAULT_NO_DEVICE_WIDTH);

  priv->surface = cairo_image_surface_create_for_data (priv->image_data,
      CAIRO_FORMAT_ARGB32, DEFAULT_NO_DEVICE_WIDTH, DEFAULT_NO_DEVICE_HEIGHT,
      priv->image_stride);

  priv->capturing = FALSE;
}

static void
video_cairo_finalize (GObject *object)
{
  VideoCairo *self = VIDEO_CAIRO(object);
  VideoCairoPrivate *priv = VIDEO_CAIRO_GET_PRIVATE(self);

  free (priv->image_data);
  cairo_surface_destroy (priv->surface);
  g_free (priv->source);

  G_OBJECT_CLASS (video_cairo_parent_class)->finalize (object);
}

static gboolean
video_cairo_expose (GtkWidget* cairo_video, GdkEventExpose* event)
{
  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE (cairo_video);

  // Redraw on every expose event.
  cairo_t* cairo_context = gdk_cairo_create (cairo_video->window);

  DEBUG("Expose event for video cairo widget invalidate %d %d", event->area.x, event->area.y);

  cairo_set_source_surface (cairo_context, priv->surface, event->area.x,
      event->area.y);
  cairo_paint (cairo_context);
  cairo_destroy (cairo_context);

  return FALSE;
}

static void
video_cairo_class_init (VideoCairoClass *class)
{
  GObjectClass *obj_class;
  GtkWidgetClass *widget_class;

  obj_class = G_OBJECT_CLASS (class);
  obj_class->get_property = video_cairo_get_property;
  obj_class->set_property = video_cairo_set_property;
  obj_class->finalize = video_cairo_finalize;

  widget_class = GTK_WIDGET_CLASS (class);
  widget_class->expose_event = video_cairo_expose;

  g_object_class_install_property (obj_class, PROP_SOURCE, g_param_spec_string (
      "source", "source", "String specifying the source SHM for the video",
      NULL, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB
          | G_PARAM_READWRITE));

  g_object_class_install_property (obj_class, PROP_WIDTH, g_param_spec_int (
      "width", "width", "The frame width", G_MININT, G_MAXINT, 0,
      G_PARAM_READWRITE));

  g_object_class_install_property (obj_class, PROP_HEIGHT, g_param_spec_int (
      "height", "height", "The frame height", G_MININT, G_MAXINT, 0,
      G_PARAM_READWRITE));

  g_object_class_install_property (obj_class, PROP_FPS, g_param_spec_string (
      "fps", "fps",
      "String specifying the frame rate of the form numerator/denominator",
      NULL, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB
          | G_PARAM_READWRITE));

  g_type_class_add_private (obj_class, sizeof(VideoCairoPrivate));
}

static void
video_cairo_class_intern_init (gpointer klass)
{
  video_cairo_parent_class = g_type_class_peek_parent (klass);
  video_cairo_class_init ((VideoCairoClass*) klass);
}

GType
video_cairo_get_type (void)
{
  static GType g_define_type_id = 0;
  if (g_define_type_id == 0)
    {
      static const GTypeInfo g_define_type_info =
        { sizeof(VideoCairoClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) video_cairo_class_intern_init,
            (GClassFinalizeFunc) NULL, NULL, /* class_data */
            sizeof(VideoCairo), 0, /* n_preallocs */
            (GInstanceInitFunc) video_cairo_init, NULL, };
      g_define_type_id = g_type_register_static (GTK_TYPE_DRAWING_AREA,
          "VideoCairo", &g_define_type_info, 0);
    }
  return g_define_type_id;
}

VideoCairo *
video_cairo_new_with_source (const gchar *source)
{
  DEBUG("Creating new VideoCairo.");
  return g_object_new (VIDEO_TYPE_CAIRO, "source", source, "width",
      DEFAULT_NO_DEVICE_WIDTH, "height", DEFAULT_NO_DEVICE_HEIGHT, "fps",
      DEFAULT_FPS, NULL);
}

VideoCairo*
video_cairo_new ()
{
  return video_cairo_new_with_source ("");
}

int
video_cairo_start (VideoCairo* self)
{
  DEBUG("Starting video cairo capture");

  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE(self);

  if (sflphone_video_add_observer (priv->endpt, &on_new_frame_cb, self) < 0) {
    ERROR("Failed to register as an observer and start video %s:%d", __FILE__, __LINE__);
    return -1;
  }

  if (sflphone_video_open (priv->endpt) < 0)
    {
      ERROR("Failed to open and start video %s:%d", __FILE__, __LINE__);
      return -1;
    }

  if (sflphone_video_start_async (priv->endpt) < 0)
    {
      ERROR("Failed to start video %s:%d", __FILE__, __LINE__);
      return -1;
    }

  DEBUG("Registered as an observer");

  priv->capturing = TRUE;
}

int
video_cairo_stop (VideoCairo* self)
{
  DEBUG("Stopping video cairo capture");

  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE(self);

  sflphone_video_remove_observer(priv->endpt, &on_new_frame_cb);

  sflphone_video_stop_async (priv->endpt);
  sflphone_video_close (priv->endpt);

  priv->capturing = FALSE;
}

gboolean
video_cairo_is_capturing(VideoCairo* video_cairo)
{
  VideoCairoPrivate* priv = VIDEO_CAIRO_GET_PRIVATE(video_cairo);
  return priv->capturing;
}
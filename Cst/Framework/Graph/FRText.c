#include <Framework/Graph/FRText.h>

/**
 * this file is only for font usage example.
 **/

struct _FRTextPrivate {
  SysChar *text;
  cairo_text_extents_t ed;
  SysInt font_size;
  SysInt width;
  SysInt height;
  PangoFontDescription *font_desc;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRText, fr_text, SYS_TYPE_OBJECT);


static void draw_text (cairo_t *cr, const SysChar *text) {
#define RADIUS 150
#define N_WORDS 2
#define FONT "Sans Bold 27"

  PangoLayout *layout;
  PangoFontDescription *desc;
  int i;

  /* Center coordinates on the middle of the region we are drawing
  */
  cairo_translate (cr, RADIUS, RADIUS);

  /* Create a PangoLayout, set the font and text */
  layout = pango_cairo_create_layout (cr);

  pango_layout_set_text (layout, text, -1);
  desc = pango_font_description_from_string (FONT);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);

  /* Draw the layout N_WORDS times in a circle */
  for (i = 0; i < N_WORDS; i++)
  {
    int width, height;
    double angle = (360. * i) / N_WORDS;
    double red;

    cairo_save (cr);

    /* Gradient from red at angle == 60 to blue at angle == 240 */
    red   = (1 + cos ((angle - 60) * G_PI / 180.)) / 2;
    cairo_set_source_rgb (cr, red, 0, 1.0 - red);

    cairo_rotate (cr, angle * G_PI / 180.);

    /* Inform Pango to re-layout the text with the new transformation */
    pango_cairo_update_layout (cr, layout);

    pango_layout_get_size (layout, &width, &height);
    cairo_move_to (cr, - ((double)width / PANGO_SCALE) / 2, - RADIUS);
    pango_cairo_show_layout (cr, layout);

    cairo_restore (cr);
  }

  /* free the layout object */
  g_object_unref (layout);
}

void fr_text_hello(FRContext *cr, const SysChar *text) {
  cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
  cairo_paint (cr);
  draw_text (cr, text);
}

void fr_text_show(FRText *self, FRContext *cr) {
  PangoLayout *layout;
  SysInt width = 0, height = 0;

  FRTextPrivate *priv = self->priv;

  layout = pango_cairo_create_layout (cr);

  pango_layout_set_font_description (layout, priv->font_desc);

  cairo_save (cr);

  pango_cairo_update_layout (cr, layout);
  pango_layout_get_size (layout, &width, &height);

  cairo_move_to (cr, width, height);

  pango_cairo_show_layout (cr, layout);

  cairo_restore (cr);

  g_object_unref (layout);
}

void fr_text_get_size(FRText *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FRTextPrivate *priv = self->priv;

  *width = priv->width;
  *height = priv->height;
}

void fr_text_set_font_size(FRText *self, SysInt font_size) {
  sys_return_if_fail(self != NULL);

  FRTextPrivate *priv = self->priv;

  priv->font_size = font_size;
}

SysInt fr_text_get_font_size(FRText *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FRTextPrivate *priv = self->priv;

  return priv->font_size;
}

void fr_text_set_text(FRText *self, const SysChar * text) {
  sys_return_if_fail(self != NULL);

  FRTextPrivate *priv = self->priv;

  if(priv->text) {
    sys_clear_pointer(&priv->text, sys_free);
  }

  priv->text = sys_strdup(text);
}

const SysChar * fr_text_get_text(FRText *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRTextPrivate *priv = self->priv;

  return priv->text;
}

void fr_text_set_font_desc(FRText *self, const SysChar *desc) {
  sys_return_if_fail(self != NULL);

  FRTextPrivate *priv = self->priv;

  priv->font_desc = pango_font_description_from_string (desc);
}

const SysChar* fr_text_get_family(FRText *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRTextPrivate *priv = self->priv;

  return pango_font_description_get_family(priv->font_desc);
}

FRText *fr_text_clone(FRText *o) {
  return o;
}

/* object api */
static void fr_text_construct(SysObject *o) {
}

FRText* fr_text_new(void) {
  return sys_object_new(FR_TYPE_TEXT, NULL);
}

FRText *fr_text_new_I(void) {
  FRText *o = fr_text_new();

  fr_text_construct(SYS_OBJECT(o));

  return o;
}

static void fr_text_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_text_parent_class)->dispose(o);
}

static void fr_text_class_init(FRTextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_text_construct;
  ocls->dispose = fr_text_dispose;
}

void fr_text_init(FRText *self) {
  FRTextPrivate *priv = self->priv = fr_text_get_private(self);

  priv->text = NULL;
  priv->font_size = 14;
  priv->font_desc = pango_font_description_from_string("serif");
}

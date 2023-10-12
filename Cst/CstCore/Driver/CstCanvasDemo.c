#include <CstCore/Driver/CstCanvasDemo.h>


static CstInt dx = 20;
static CstInt dy = 20;


void cst_canvas_draw_stoke_box(CstCanvasCtx *cr, CstInt x, CstInt y) {
  cairo_set_source_rgb(cr, 0.235294118, 0.235294118, 0.235294118);
  cairo_rectangle(cr, x, y, 300, 40);
  cairo_stroke(cr);
}

void cst_canvas_draw_fill_box(CstCanvasCtx *cr, CstInt x, CstInt y) {
  cairo_set_source_rgb(cr, 0.235294118, 0.235294118, 0.235294118);
  cairo_rectangle(cr, x, y, 300, 40);
  cairo_fill(cr);
}

void cst_canvas_draw_text(CstCanvasCtx *cr, CstInt x, CstInt y) {
  cairo_move_to(cr, x, y);
  cairo_select_font_face(cr, "SongTi", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 20);
  cairo_show_text(cr, "字体排版.");
}

void cst_canvas_draw_paint_alpha(CstCanvasCtx *cr, CstInt x, CstInt y) {
}

void cst_canvas_draw_clip(CstCanvasCtx *cr, CstInt x, CstInt y) {
}

void cst_canvas_draw_region(CstCanvasCtx *cr, CstInt x, CstInt y) {
  cairo_rectangle_int_t r, box;
  CstInt n_boxes, i;
  cairo_region_t *region;

  r.x = x;
  r.y = y;

  r.width = 200;
  r.height = 200;

  cairo_set_source_rgba(cr, 0.3, 0.2, 0.1, 1);

  region = cairo_region_create_rectangle(&r);
  n_boxes = cairo_region_num_rectangles(region);

  for (i = 0; i < n_boxes; i++)
  {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_fill(cr);
  cairo_region_destroy(region);
}

void cst_canvas_draw_demo(CstCanvasCtx *cr, CstInt x, CstInt y) {
  int width = 100;
  int sy = 0;


  cairo_rectangle(cr, 0, sy, width, width);
  cairo_rectangle(cr, 0 + 50, sy + 50, width, width);

  cairo_select_font_face(cr, "SongTi", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_move_to(cr, 50, 60);
  cairo_show_text(cr, "字体");

  cairo_stroke(cr);
}

void cst_canvas_draw_hello(CstCanvas *canvas, CstInt x, CstInt y) {
  CstCanvasCtx *cr = canvas->cairo;

  dx = dx + x;
  dy = dy + y;

  g_debug("(%d,%d)", dx, dy);
  cst_canvas_draw_fill_box(cr, dx, dy);

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cst_error_system(_("status hello error: %s"), cairo_status_to_string(cairo_status(cr)));
    return;
  }
}

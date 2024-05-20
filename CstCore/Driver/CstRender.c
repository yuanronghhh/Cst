#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstAlgorithm.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstSurface.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <Framework/Device/FrIDevice.h>

SYS_DEFINE_TYPE(CstRender, cst_render, SYS_TYPE_OBJECT);

static CstRender *g_render = NULL;
static SysMutex g_render_lock;

void cst_render_setup(void) {
  cst_algorithm_setup();

  g_render = cst_render_new_I(true);
  sys_mutex_init(&g_render_lock);
}

void cst_render_teardown(void) {
  sys_mutex_clear(&g_render_lock);
  sys_assert(g_render != NULL);
  sys_clear_pointer(&g_render, _sys_object_unref);

  cst_algorithm_teardown();
}

CstRender *cst_render_get_g_render(void) {
  sys_assert(g_render && "cst_render_setup need called before get");
  CstRender* lv;

  sys_mutex_lock(&g_render_lock);
  lv = g_render;
  sys_mutex_unlock(&g_render_lock);

  return lv;
}

CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

FrDevice *cst_render_get_default_device(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->device;
}

static FrRegion *idevice_create_region(FrDevice *device) {
  FrRegion *region;
  FrBound bound = { 0 };

  fr_i_device_get_size(FR_I_DEVICE(device), &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  return region;
}

void cst_render_rerender(CstRender* self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

static void render_realize(CstRender *self,
    CstModule *v_module,
    CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderNode* body;

  body = cst_module_realize(v_module, NULL, layout);

  self->body_rnode = body;
}

static CstLayout* render_new_layout(CstRender* self) {
  sys_return_val_if_fail(self  != NULL, NULL);

  FrRegion* region = idevice_create_region(self->device);

  return cst_layout_new_I(self->draw_context, region);
}

void render_render(CstRender* self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(layout != NULL);

  cst_layout_layout_begin(layout);

  cst_layout_layout_root(layout, self->body_rnode);
  cst_layout_paint_root(layout, self->body_rnode);

  cst_layout_layout_end(layout);
}

void cst_render_render(CstRender *self, CstModule *v_module) {
  sys_return_if_fail(self != NULL);
  CstLayout* layout = render_new_layout(self);

  render_realize(self, v_module, layout);

  cst_layout_layout_prepare(layout, self->body_rnode);
  render_render(self, layout);

  sys_clear_pointer(&layout, _sys_object_unref);
}

void cst_render_resize_surface(CstRender *self) {
  sys_return_if_fail(self != NULL);

  CstLayout* layout = render_new_layout(self);
  render_render(self, layout);
  sys_clear_pointer(&layout, _sys_object_unref);
}

void cst_render_create_window(CstRender *self) {
  FrWindow *window;
  FrDisplay* display;
  FrDevice *device;

  FrSurface* paint_surface;
  display = fr_display_new_I();
  window = fr_window_top_new(display);
  device = FR_DEVICE(window);

  self->draw_context = fr_cairo_draw_context_new_I(device);
  self->display = display;
  self->device = device;

  FrSurfaceContext info = {.width = 800, .height = 600};
  paint_surface = cst_surface_create_image_surface(&info);
  fr_draw_context_add_surface(self->draw_context, paint_surface);
}

/* object api */
static void cst_render_construct(CstRender *self, SysBool is_offscreen) {
  if(!is_offscreen) {

    cst_render_create_window(self);
  } 
}

CstRender* cst_render_new_I(SysBool is_offscreen) {
  CstRender* o = cst_render_new();

  cst_render_construct(o, is_offscreen);

  return o;
}

static void cst_render_init(CstRender *self) {
}

static void cst_render_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstRender *self = CST_RENDER(o);

  if (self->device) {

    sys_clear_pointer(&self->device, _sys_object_unref);
    sys_clear_pointer(&self->display, _sys_object_unref);
    sys_clear_pointer(&self->draw_context, _sys_object_unref);
  }

  if(self->body_rnode) {

    sys_clear_pointer(&self->body_rnode, cst_render_node_unlink_node_r);
  }

  SYS_OBJECT_CLASS(cst_render_parent_class)->dispose(o);
}

static void cst_render_class_init(CstRenderClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_dispose;
}

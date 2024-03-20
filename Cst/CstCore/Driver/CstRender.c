#include <CstCore/Driver/CstRender.h>

#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstSurface.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/Flex/CstFlexAlgorithm.h>

SYS_DEFINE_TYPE(CstRender, cst_render, SYS_TYPE_OBJECT);


static CstRender *g_render = NULL;
static SysMutex g_render_lock;


void cst_render_setup(void) {
  g_render = cst_render_new_I(false);
  sys_mutex_init(&g_render_lock);
}

void cst_render_teardown(void) {
  sys_mutex_clear(&g_render_lock);

  sys_clear_pointer(&g_render, _sys_object_unref);
}

static void init_body_layout_info(CstRenderNode* rnode, CstLayout* layout) {
  SysInt width, height;

  cst_layout_get_buffer_size(layout, &width, &height);
  cst_render_node_set_prefer_size(rnode, width, height);
  cst_render_node_set_size(rnode, width, height);
}

CstSurface *cst_render_get_surface(CstRender *self, SysUInt surf_idx) {
  sys_return_val_if_fail(self != NULL, NULL);

  if (surf_idx >= self->surfaces.len) {
    sys_warning_N("surface index not correct: %d", surf_idx);
    return NULL;
  }

  return self->surfaces.pdata[surf_idx];
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

FRWindow *cst_render_get_default_window(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
}

FRRegion *render_create_region(FRWindow *window) {
  FRRegion *region;
  FRRect bound = { 0 };

  fr_window_get_framebuffer_size(window, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  return region;
}

static void render_layout_surfaces(SysHArray *surfs, CstRenderNode *rnode, CstLayout *layout) {
  for (SysUInt i = 0; i < surfs->len; i++) {
    CstSurface *s = surfs->pdata[i];

    cst_surface_layout_r(s, rnode, layout);
  }
}


void cst_render_rerender(CstRender* self, FRRegion* region, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

#if 0
  CstRenderNode *rnode;

  rnode = self->body_rnode;

  cst_layout_begin_layout(layout);
  init_body_layout_info(rnode, layout);
  render_layout_surfaces(&self->surfaces, rnode, layout);
  cst_layout_end_layout(layout);
#endif
}

void cst_render_realize(CstRender *self, CstModule *v_module) {
  sys_return_if_fail(self != NULL);

  CstRenderNode* body;

  body = cst_module_realize(v_module, NULL);

  self->body_rnode = body;
}

void cst_render_render(CstRender *self, CstModule *v_module) {
  sys_return_if_fail(self != NULL);

  cst_render_realize(self, v_module);

#if 0
  FRRegion *region;
  CstLayout* layout;
  CstRenderNode *rnode;
  FRWindow* window;
  FRDraw* draw;

  window = cst_render_get_default_window(self);
  region = render_create_region(window);
  draw = fr_draw_new_I(window);
  layout = cst_layout_new_I(window, draw, region);
  rnode = self->body_rnode;
  init_body_layout_info(rnode, layout);

  fr_region_destroy(region);
  sys_object_unref(layout);
#endif
}

void cst_render_resize_window(CstRender *self) {
  sys_return_if_fail(self != NULL);

  SysInt width = 0;
  SysInt height = 0;

  fr_window_get_size(self->window, &width, &height);

  cst_render_request_resize_window(self, width, height);
}

void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  FRRegion *region;
  FRRect bound = { 0 };
  FRDraw* draw;
  CstLayout* layout;
  FRWindow* window;

  bound.width = width;
  bound.height = height;

  region = fr_region_create_rectangle(&bound);
  draw = fr_draw_new_I(self->window);
  window = cst_render_get_default_window(self);

  layout = cst_layout_new_I(window, draw, region);
  cst_render_rerender(self, region, layout);

  sys_object_unref(layout);
  fr_region_destroy(region);
}

CstSurface* cst_render_get_default_surface(void) {
  CstSurface* surface;

  sys_mutex_lock(&g_render_lock);

  if (g_render->surfaces.len == 0) {
    return NULL;
  }
  surface = g_render->surfaces.pdata[0];

  sys_mutex_unlock(&g_render_lock);

  return surface;
}

/* object api */
static void cst_render_construct(CstRender *self, SysBool is_offscreen) {
  CstSurface* surf;
  SysInt width = 0, height = 0;

  if (is_offscreen) {
    self->window = NULL;
    surf = cst_surface_create_image_surface(800, 600);
  } else {

    self->display = fr_display_new_I();
    self->window = fr_window_top_new(self->display);

    fr_window_get_framebuffer_size(self->window, &width, &height);
    surf = cst_surface_create_by_window(self->window, width, height);
  }

  sys_harray_add(&self->surfaces, surf);
}

CstRender* cst_render_new_I(SysBool is_offscreen) {
  CstRender* o = cst_render_new();

  cst_render_construct(o, is_offscreen);

  return o;
}

static void cst_render_init(CstRender *self) {
  sys_harray_init_with_free_func(&self->surfaces, (SysDestroyFunc)_sys_object_unref);
}

static void cst_render_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstRender *self = CST_RENDER(o);

  if (self->window) {
    sys_object_unref(self->window);
    sys_object_unref(self->display);
  }

  sys_harray_destroy(&self->surfaces);
  sys_clear_pointer(&self->body_rnode, cst_render_node_unlink_node_r);

  SYS_OBJECT_CLASS(cst_render_parent_class)->dispose(o);
}

static void cst_render_class_init(CstRenderClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_dispose;
}

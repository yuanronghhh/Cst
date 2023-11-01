#include <CstCore/Driver/CstApplication.h>

#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstApplication, cst_application, SYS_TYPE_OBJECT);


SysUInt64 last_time = 0;
static CstApplication *g_application = NULL;

void cst_application_stop (CstApplication *self) {
  sys_return_if_fail(self != NULL);

  fr_main_stop(self->main_loop);
}

FR_FUNC_DEFINE_EVENT(cst_application_event_func) {
  sys_return_val_if_fail(e != NULL, -1);
  CstApplication *app = CST_APPLICATION(user_data);

  UNUSED(app);

  sys_debug_N("%s", "ok");

  return 0;
}

FR_FUNC_DEFINE_EVENT(app_window_resize_test) {
  CstApplication *app = CST_APPLICATION(user_data);
  CstRender *render = app->render;

  cst_render_resize_window(render);

  return 0;
}

static void cst_application_active(CstApplication* self) {
  sys_return_if_fail(self != NULL);

  CstModule* v_module = self->main_module;
  CstRender *v_render = self->render;

  FRAWatch *awatch = fr_awatch_any_new_I(FR_TYPE_EVENT, "app_window_resize_test", app_window_resize_test);
  fr_awatch_bind(awatch, (SysPointer)self);
  cst_module_add_awatch(v_module, awatch);

  cst_context_set_v_module(c, v_module);
  cst_render_render(v_render, self->c, v_module);
}

void cst_application_mono_setup(const SysChar *managed_path) {
#if CST_USE_MONO
  const SysChar *argv[2] = { NULL, CST_BINARY_DIR"/Cst/CstMono/Debug/CstMono.exe" };
  argv[0] = sys_exe_path();
  cst_context_mono_setup(managed_path);
#endif
}

SysInt cst_application_run(CstApplication* self, const SysChar *main_path) {
  sys_return_val_if_fail(self != NULL, 1);

  self->render = cst_render_new_I(false);
  self->main_module = cst_module_load_path(self->c, NULL, main_path);

  cst_application_active(self);

  fr_main_run(self->main_loop);

  return self->status;
}

CstApplication *cst_application_acquire(void) {
  sys_return_val_if_fail(g_application != NULL, NULL);

  return g_application;
}

/* object api */
static void cst_application_dispose(SysObject* o) {
  CstApplication *self = CST_APPLICATION(o);

  sys_clear_pointer(&self->main_module, _sys_object_unref);
  sys_clear_pointer(&self->render, _sys_object_unref);
  sys_clear_pointer(&self->main_loop, _sys_object_unref);
  sys_clear_pointer(&self->app_source, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_application_parent_class)->dispose(o);
}

void cst_application_construct(CstApplication* self, CstContext *c, const SysChar *appname) {
  self->main_loop = fr_main_get_main_loop();
  self->app_source = fr_application_new_I(self);
  self->c = c;

  fr_main_attach(self->main_loop, self->app_source);
}

CstApplication *cst_application_new(void) {
  return sys_object_new(CST_TYPE_APPLICATION, NULL);
}

CstApplication* cst_application_new_I(CstContext *c, const SysChar *appname) {
  sys_return_val_if_fail(appname != NULL, NULL);
  sys_return_val_if_fail(c != NULL, NULL);

  CstApplication* o = cst_application_new();

  cst_application_construct(o, c, appname);

  return o;
}

static void cst_application_class_init(CstApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_application_dispose;
}

static void cst_application_init(CstApplication *self) {
}

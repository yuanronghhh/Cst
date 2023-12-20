#include <CstCore/Driver/CstApplication.h>
#include <CstCore/CstCore.h>


SYS_DEFINE_TYPE(CstApplication, cst_application, SYS_TYPE_OBJECT);


SysUInt64 last_time = 0;
static CstApplication *g_application = NULL;

void cst_application_stop (CstApplication *self) {
  sys_return_if_fail(self != NULL);

  fr_main_stop(self->main_loop);
}


FR_FUNC_DEFINE_EVENT(application_window_resize) {
  // CstApplication *app = CST_APPLICATION(user_data);

  CstRender *render = cst_render_get_g_render();
  cst_render_resize_window(render);

  return 0;
}

FR_FUNC_DEFINE_EVENT(application_quit_key) {
  CstApplication* app = user_data;

  cst_application_stop(app);

  return 0;
}

static void application_init_event(CstApplication* self) {
  FRAWatch* awatch;
  CstModule* v_module = self->main_module;

  awatch = fr_awatch_any_new_I(FR_EVENT_TFRAMEBUFFER_RESIZE,
      "application_window_resize",
      application_window_resize);

  fr_awatch_bind(awatch, (SysPointer)self);
  cst_module_add_awatch(v_module, awatch);

  awatch = fr_awatch_key_new_I(FR_KEY_Q, "application_quit_key", application_quit_key);
  fr_awatch_bind(awatch, (SysPointer)self);
  cst_module_add_awatch(v_module, awatch);
}

static void cst_application_active(CstApplication* self) {
  sys_return_if_fail(self != NULL);
  CstRender* render;

  application_init_event(self);

  render = cst_render_get_g_render();
  cst_render_render(render, self->main_module);
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

  self->main_module = cst_module_load_path(NULL, main_path);
  sys_object_ref(self->main_module);

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

  SYS_OBJECT_CLASS(cst_application_parent_class)->dispose(o);
}

void cst_application_construct(CstApplication* self, const SysChar *appname) {
  self->main_loop = fr_main_get_main_loop();
  self->app_source = fr_application_new_I(self);

  // app_source free by loop
  fr_main_attach(self->main_loop, self->app_source);
}

CstApplication *cst_application_new(void) {
  return sys_object_new(CST_TYPE_APPLICATION, NULL);
}

CstApplication* cst_application_new_I(const SysChar *appname) {
  sys_return_val_if_fail(appname != NULL, NULL);

  CstApplication* o = cst_application_new();
  cst_application_construct(o, appname);

  return o;
}

static void cst_application_class_init(CstApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_application_dispose;
}

static void cst_application_init(CstApplication *self) {
  g_application = self;
}

#include <CstCore/Driver/CstApplication.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>


SYS_DEFINE_TYPE(CstApplication, cst_application, SYS_TYPE_OBJECT);


SysUInt64 last_time = 0;
static SysBool env_inited = false;
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
  CstManager *v_manager = self->manager;
  CstRender *v_render = self->render;

  FRAWatch *awatch = fr_awatch_any_new_I(FR_TYPE_EVENT, "app_window_resize_test", app_window_resize_test);
  fr_awatch_bind(awatch, (SysPointer)self);
  cst_module_add_awatch(v_module, awatch);

  cst_render_realize(v_render, v_module);
  cst_render_render(v_render);
}

void cst_application_mono_setup(const SysChar *managed_path) {
#if CST_USE_MONO
  const SysChar *argv[2] = { NULL, CST_BINARY_DIR"/Cst/CstMono/Debug/CstMono.exe" };
  argv[0] = sys_exe_path();
  cst_context_mono_setup(managed_path);
#endif
}

void cst_application_env_setup(void) {
  if (env_inited) {
    return;
  }

  fr_main_setup();
  fr_window_setup();
  fr_canvas_setup();
  cst_css_setup();
  cst_render_node_setup();
  fr_events_setup();

#if CST_USE_MONO
  fr_mono_setup(CST_MONO_HOME);
#endif

  cst_context_setup(2, NULL);

  env_inited = true;
}

void cst_application_env_teardown(void) {
  if (!env_inited) {
    sys_warning_N("%s", "application enviroment teardown must be after setup.");
    return;
  }

  cst_render_node_teardown();
  cst_css_teardown();
  fr_events_teardown();
  fr_canvas_teardown();
  fr_window_teardown();
  fr_main_teardown();
  cst_context_teardown();

  env_inited = false;
}

SysInt cst_application_run(CstApplication* self, const SysChar *main_path) {
  sys_return_val_if_fail(self != NULL, 1);

  CstRender *v_render = cst_render_new_I(false);

  self->render = v_render;
  self->main_module = cst_manager_load_module(self->manager, NULL, main_path);

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
  sys_clear_pointer(&self->manager, _sys_object_unref);
  sys_clear_pointer(&self->render, _sys_object_unref);

  cst_application_env_teardown();

  SYS_OBJECT_CLASS(cst_application_parent_class)->dispose(o);
}

void cst_application_construct(CstApplication* self, const SysChar *appname) {
  self->main_loop = fr_main_get_main_loop();
  self->app_source = fr_application_new_I(self);

  fr_main_attach(self->main_loop, self->app_source);

  self->manager = cst_manager_new_I();
}

CstApplication *cst_application_new(void) {
  return sys_object_new(CST_TYPE_APPLICATION, NULL);
}

CstApplication* cst_application_new_I(const SysChar *appname) {
  sys_return_val_if_fail(appname != NULL, NULL);

  cst_application_env_setup();

  if (g_application != NULL) {
    sys_abort_N("Only one application supported now: %s", appname);
    return NULL;
  }

  CstApplication* o = cst_application_new();

  cst_application_construct(o, appname);

  g_application = o;

  return o;
}

static void cst_application_class_init(CstApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_application_dispose;
}

static void cst_application_init(CstApplication *self) {
}

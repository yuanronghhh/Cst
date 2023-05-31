#include <CstCore/Driver/CstApplication.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>


struct _CstApplicationPrivate  {
  CstManager *manager;
  FRMain* main_loop;
  FRApplication *app_source;
  CstRender *render;
  CstModule *main_module;
  SysInt status;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(CstApplication, cst_application, SYS_TYPE_OBJECT);


SysUInt64 last_time = 0;
static CstApplication *g_application = NULL;

void cst_application_stop (CstApplication *self) {
  sys_return_if_fail(self != NULL);

  CstApplicationPrivate *priv = self->priv;

  fr_main_stop_nolock(priv->main_loop);
}

FR_FUNC_DEFINE_EVENT(cst_application_event_func) {
  sys_return_val_if_fail(e != NULL, -1);
  CstApplication *app = CST_APPLICATION(user_data);

  UNUSED(app);

  sys_debug_N("%s", "ok");

#if 0
  CstApplicationPrivate *priv = app->priv;
  CstRender *render = priv->render;
  SysUInt64 new_time = sys_get_monoic_time();
  SysInt etype = fr_event_get_event_type(self);

  switch (etype) {
    case FR_EVENT_TWINDOW_CLOSE:
      fr_main_stop_nolock(priv->main);
      break;
    case FR_EVENT_TWINDOW_RESIZE:
      sys_debug_N("%ld,%ld", new_time, last_time);
      last_time = new_time;

      cst_manager_render(priv->manager, render);
      break;
    default:
      break;
  }
#endif
  return 0;
}

FR_FUNC_DEFINE_EVENT(app_window_resize_test) {
  CstApplication *app = CST_APPLICATION(user_data);
  CstApplicationPrivate *priv = app->priv;
  CstRender *render = priv->render;

  cst_render_resize_window(render);

  return 0;
}

void cst_application_set_meta(CstApplication *self, const SysChar *name, SysType stype) {
  sys_return_if_fail(self != NULL);

  CstApplicationPrivate *priv = self->priv;
  CstManager *manager = priv->manager;

  cst_manager_lock(manager);
  cst_manager_set_meta(manager, name, stype);
  cst_manager_unlock(manager);
}

static void cst_application_active(CstApplication* self) {
  sys_return_if_fail(self != NULL);

  CstApplicationPrivate *priv = self->priv;

  CstModule* v_module = priv->main_module;
  CstManager *v_manager = priv->manager;
  CstRender *v_render = cst_render_new_I(false);
  FRAWatchProps props = { 0 };

  priv->render = v_render;

  props.key = FR_KEY_R;
  // pres r for debug render check
  cst_module_add_awatch(v_module, (SysPointer)self, "key_press", "app_window_resize_test", app_window_resize_test, &props);

  cst_manager_realize(v_manager, v_module, v_render);
  cst_render_render(v_module, v_render);
}

void cst_application_mono_setup(const SysChar *managed_path) {
#if CST_USE_MONO
  const SysChar *argv[2] = { NULL, CST_BINARY_DIR"/Cst/CstMono/Debug/CstMono.exe" };
  argv[0] = sys_exe_path();
  cst_context_mono_setup(managed_path);
#endif
}

void cst_application_env_setup(void) {
  fr_window_setup();
  fr_canvas_setup();
  cst_css_setup();

  fr_events_setup();

#if CST_USE_MONO
    fr_mono_setup(CST_MONO_HOME);
#endif

  cst_context_setup(2, NULL);
}

void cst_application_env_teardown(void) {
  cst_css_teardown();

  fr_canvas_teardown();
  fr_window_teardown();
  fr_events_teardown();

  cst_context_teardown();
}

SysInt cst_application_run(CstApplication* self, const SysChar *main_path) {
  sys_return_val_if_fail(self != NULL, 1);

  CstApplicationPrivate *priv = self->priv;

  priv->main_module = cst_manager_load_module(priv->manager, NULL, main_path);

  cst_application_active(self);

  fr_main_run(priv->main_loop);
  sys_object_unref(priv->main_loop);

  return priv->status;
}

CstApplication *cst_application_acquire(void) {
  sys_return_val_if_fail(g_application != NULL, NULL);

  return g_application;
}

/* object api */
static void cst_application_dispose(SysObject* o) {
  CstApplication *self = CST_APPLICATION(o);
  CstApplicationPrivate *priv = self->priv;

  sys_object_unref(priv->render);
  sys_object_unref(priv->manager);

  cst_application_env_teardown();

  SYS_OBJECT_CLASS(cst_application_parent_class)->dispose(o);
}

void cst_application_construct(SysObject* o, const SysChar *appname) {
  SYS_OBJECT_CLASS(cst_application_parent_class)->construct(o);

  CstApplication* self = CST_APPLICATION(o);
  CstApplicationPrivate *priv = self->priv;

  priv->main_loop = fr_main_new_I();
  priv->app_source = fr_application_new_I(self);

  fr_main_attach(priv->main_loop, FR_SOURCE(priv->app_source));

  priv->manager = cst_manager_new_I();
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

  cst_application_construct(SYS_OBJECT(o), appname);

  g_application = o;

  return o;
}

static void cst_application_class_init(CstApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_application_construct;
  ocls->dispose = cst_application_dispose;
}

static void cst_application_init(CstApplication *self) {
  self->priv = cst_application_get_private(self);
}

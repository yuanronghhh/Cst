#include <Framework/FRMono.h>

struct _FRMonoContext {
  MonoDomain *g_domain;
  MonoAssembly *g_assembly;
  MonoImage *g_image;
};

void fr_mono_setup(SysChar *mono_home) {
  SysChar *lib_path = sys_strdup_printf("%s/lib", mono_home);
  SysChar *etc_path = sys_strdup_printf("%s/etc", mono_home);

  mono_set_dirs(lib_path, etc_path);

  sys_free_N(lib_path);
  sys_free_N(etc_path);

  mono_set_use_llvm(true);
}

void fr_mono_context_free(FRMonoContext *c) {
  sys_free_N(c);
}

FRMonoContext *fr_mono_context_new (
  const SysChar *domain_name, 
  const SysChar *managed_path) {

  sys_return_val_if_fail(domain_name != NULL, NULL);
  sys_return_val_if_fail(managed_path != NULL, NULL);

  FRMonoContext *c = sys_new0_N(FRMonoContext, 1);

  c->g_domain = mono_jit_init(domain_name);

  if (!sys_path_exists(managed_path)) {
    sys_abort_N("file not exist: %s", managed_path);
  }

  c->g_assembly = mono_domain_assembly_open(c->g_domain, managed_path);
  c->g_image = mono_assembly_get_image(c->g_assembly);

  return c;
}

SysType fr_mono_get_meta_from_name(FRMonoContext *c, const SysChar *name_space, const SysChar *class_name) {
  MonoClass *cls = mono_class_from_name(c->g_image, name_space, class_name);

  return (SysType)mono_class_vtable(c->g_domain, cls);
}

void *fr_mono_object_new_fast(SysType meta) {
  sys_return_val_if_fail(meta != 0, NULL);

  MonoVTable *vt = (MonoVTable *)meta;

  MonoObject *o = mono_object_new_fast(vt);
  mono_runtime_object_init(o);

  return o;
}

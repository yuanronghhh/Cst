#ifndef __FR_MONO_H__
#define __FR_MONO_H__

#include <Framework/FRTypes.h>

SYS_BEGIN_DECLS

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-gc.h>

void fr_mono_setup(SysChar *mono_home);

void fr_mono_context_free(FRMonoContext *c);
FRMonoContext *fr_mono_context_new(
  const SysChar *domain_name,
  const SysChar *managed_path);

#define fr_mono_object_new(meta) fr_mono_object_new_fast(meta)

void *fr_mono_object_new_fast(SysType meta);
SysType fr_mono_get_meta_from_name(FRMonoContext *c, const SysChar *name_space, const SysChar *class_name);

SYS_END_DECLS

#endif

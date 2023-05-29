#ifndef __CST_MACROS_H__
#define __CST_MACROS_H__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>

SYS_BEGIN_DECLS

#define CST_DEFINE_GETTER_H(RType, p_name, SType, prop) \
SYS_API RType p_name##_get_##prop(SType *self);

#define CST_DEFINE_GETTER_C(RType, p_name, SType, prop) \
RType p_name##_get_##prop(SType *self) { \
  SType##Private* priv = self->priv; \
  return priv->prop; \
}

#define CST_DEFINE_SETTER_H(RType, p_name, SType, prop) \
SYS_API void p_name##_set_##prop(SType *self, RType v);

#define CST_DEFINE_SETTER_C(RType, p_name, SType, prop) \
void p_name##_set_##prop(SType *self, RType v) { \
  SType##Private* priv = self->priv; \
  priv->prop = v; \
}

#define CST_DEFINE_SETTER_STR_C(p_name, SType, prop) \
void p_name##_set_##prop(SType *self, SysChar* v) { \
  SType##Private* priv = self->priv; \
  if(priv->prop) { \
    sys_free_N(priv->prop); \
  } \
  priv->prop = sys_strdup(v); \
}

#define M4_VSET(m, v) if(v >= 0) { m = v; }
#define M4_SET(marray, m0,m1,m2,m3)  { M4_VSET(marray[0], m0); M4_VSET(marray[1], m1); M4_VSET(marray[2], m2); M4_VSET(marray[3], m3); } 
#define M4_GET(marray, m0,m1,m2,m3)  { *m0 = marray[0]; *m1 = marray[1]; *m2 = marray[2]; *m3 = marray[3]; }

#define CST_DEFINE_M4_GETTER_H(p_name, SType, prop) \
SYS_API void p_name##_get_##prop(SType *self, SysInt *m0, SysInt *m1, SysInt *m2, SysInt *m3);

#define CST_DEFINE_M4_GETTER_C(p_name, SType, prop) \
void p_name##_get_##prop(SType *self, SysInt *m0, SysInt *m1, SysInt *m2, SysInt *m3) { \
  SType##Private* priv = self->priv; \
  M4_GET(priv->prop, m0, m1, m2, m3); \
}

#define CST_DEFINE_M4_SETTER_H(p_name, SType, prop) \
SYS_API void p_name##_set_##prop(SType *self, SysInt m0, SysInt m1, SysInt m2, SysInt m3);

#define CST_DEFINE_M4_SETTER_C(p_name, SType, prop) \
void p_name##_set_##prop(SType *self, SysInt m0, SysInt m1, SysInt m2, SysInt m3) { \
  SType##Private* priv = self->priv; \
  M4_SET(priv->prop, m0, m1, m2, m3); \
}

SYS_END_DECLS

#endif


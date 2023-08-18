#include <CstCore/Driver/CstLayout.h>


struct _CstLayoutPrivate {
  SysInt state;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_construct(CstLayout* o) {
  CstLayoutPrivate *priv = o->priv;

  priv->state = 0;
}

SysBool cst_layout_state_layout(CstLayout *self) {
  CstLayoutPrivate *priv = self->priv;

  return priv->state & CST_RENDER_STATE_LAYOUT;
}

CstLayout *cst_layout_new_I(void) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o);

  return o;
}

SysInt cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstLayoutPrivate *priv = self->priv;

  return priv->state;
}

/* object api */
static void cst_layout_init(CstLayout *self) {
  self->priv = cst_layout_get_private(self);
}

static void cst_layout_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
  cls->construct = cst_layout_construct;
}

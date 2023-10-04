#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/CstFrontCore.h>


SYS_DEFINE_TYPE(CstCssValue, cst_css_value, SYS_TYPE_OBJECT);

static SysHashTable *gcss_node_ht = NULL;

static void cst_css_value_bind_map(const SysChar *name, SysType type) {
  sys_hash_table_insert(gcss_node_ht, (SysPointer)name, (SysPointer)type);
}

void cst_css_value_setup(void) {
  sys_assert(gcss_node_ht == NULL && "css value should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, NULL);

  cst_css_value_bind_map("x",           CST_CSS_TYPE_X           );
  cst_css_value_bind_map("y",           CST_CSS_TYPE_Y           );
  cst_css_value_bind_map("padding",     CST_CSS_TYPE_PADDING     );
  cst_css_value_bind_map("margin",      CST_CSS_TYPE_MARGIN      );
  cst_css_value_bind_map("width",       CST_CSS_TYPE_W           );
  cst_css_value_bind_map("height",      CST_CSS_TYPE_H           );
  cst_css_value_bind_map("font-family", CST_CSS_TYPE_FONT_FAMILY );
  cst_css_value_bind_map("font-size",   CST_CSS_TYPE_FONT_SIZE   );
  cst_css_value_bind_map("wrap",        CST_CSS_TYPE_WRAP        );
  cst_css_value_bind_map("color",       CST_CSS_TYPE_COLOR       );
}

void cst_css_value_teardown(void) {
  sys_assert(gcss_node_ht != NULL && "css value should init before use.");

  sys_hash_table_unref(gcss_node_ht);
}

/* object api */
static void cst_css_value_dispose(SysObject* o) {
  CstCssValue *self = CST_CSS_VALUE(o);

  SYS_OBJECT_CLASS(cst_css_value_parent_class)->dispose(o);
}

static void cst_css_value_construct(CstCssValue* self) {
}

CstCssValue *cst_css_value_new(void) {
  return sys_object_new(CST_TYPE_CSS_VALUE, NULL);
}

static void cst_css_value_class_init(CstCssValueClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_value_dispose;

  cls->parse = ast_css_value_parse;
}

static void cst_css_value_init(CstCssValue *self) {
}



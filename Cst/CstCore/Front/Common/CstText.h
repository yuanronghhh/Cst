#ifndef __CST_TEXT_H__
#define __CST_TEXT_H__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_TEXT (cst_text_get_type())
#define CST_TEXT(o) ((CstText* )sys_object_cast_check(o, CST_TYPE_TEXT))
#define CST_TEXT_CLASS(o) ((CstTextClass *)sys_class_cast_check(o, CST_TYPE_TEXT))
#define CST_TEXT_GET_CLASS(o) sys_instance_get_class(o, CstTextClass)


struct _CstText {
  CstNode parent;

  /* <private> */
  FRDrawLayout *playout;
  PangoFontDescription *font_desc;
};

struct _CstTextClass {
  CstNodeClass parent;
};

SysType cst_text_get_type(void);
CstNode* cst_text_new(void);
const SysChar* cst_text_get_text(CstText* self);
void cst_text_set_font_desc(CstText *self, const SysChar * font_desc);
void cst_text_set_text(CstText* self, const SysChar *text);
void cst_text_set_font_size(CstText *text, SysInt font_size);
SysInt cst_text_get_font_size(CstText *text);

SYS_END_DECLS

#endif

#ifndef __CST_TEXT_H__
#define __CST_TEXT_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define CST_TYPE_TEXT _CST_TYPE_TEXT
#define CST_TEXT(o) _CST_TEXT(o)
#define CST_TEXT_CLASS(o) _CST_TEXT_CLASS(o)
#define CST_TEXT_GET_CLASS(o) _CST_TEXT_GET_CLASS(o)

SysType cst_text_get_type(void);
CstNode* cst_text_new(void);
void cst_text_set_font_desc(CstText *self, const SysChar * font_desc);
void cst_text_set_text(CstText* self, const SysChar *text);
void cst_text_set_font_size(CstText *text, SysInt font_size);
SysInt cst_text_get_font_size(CstText *text);

SYS_END_DECLS

#endif

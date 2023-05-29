#ifndef __CST_MTEXT_H__
#define __CST_MTEXT_H__

#include <CstCore/Front/Mono/CstMWidget.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_TEXT (cst_text_get_type())
#define _CST_TEXT(o) ((CstText* )o)

SysType cst_text_get_type(void);
CstNode* _cst_text_new(void);

void cst_text_set_text(CstText* self, SysChar *text);
void cst_text_set_font_size(CstText *text, SysInt font_size);
SysInt cst_text_get_font_size(CstText *text);
void cst_text_set_font_face(CstText *text, SysChar * font_face);
SysChar * cst_text_get_font_face(CstText *text);

SYS_END_DECLS

#endif


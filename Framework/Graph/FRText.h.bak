#ifndef __FR_TEXT_H__
#define __FR_TEXT_H__

#include <Framework/FRCommon.h>


SYS_BEGIN_DECLS


#define FR_TYPE_TEXT (fr_text_get_type())
#define FR_TEXT(o) ((FRText* )sys_object_cast_check(o, FR_TYPE_TEXT))
#define FR_TEXT_CLASS(o) ((FRTextClass *)sys_class_cast_check(o, FR_TYPE_TEXT))
#define FR_TEXT_GET_CLASS(o) sys_instance_get_class(o, FRTextClass)


struct _FRTextClass {
  SysObjectClass parent;
};

struct _FRText {
  SysObject parent;

  FRTextPrivate *priv;
};

SYS_API SysType fr_text_get_type(void);
SYS_API FRText *fr_text_new_I(void);

void fr_text_hello(FRContext *cr, const SysChar *text);
void fr_text_show(FRText * self, FRContext * cr);
void fr_text_get_size(FRText * self, SysInt * width, SysInt * height);
void fr_text_set_text(FRText *self, const SysChar * text);
const SysChar * fr_text_get_text(FRText *self);
void fr_text_set_font_desc(FRText * self, const SysChar * desc);
const SysChar * fr_text_get_family(FRText * self);
FRText * fr_text_clone(FRText * o);
void fr_text_set_font_size(FRText *self, SysInt font_size);
SysInt fr_text_get_font_size(FRText * self);

SYS_END_DECLS

#endif


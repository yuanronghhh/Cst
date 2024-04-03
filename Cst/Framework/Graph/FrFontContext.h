#ifndef __FR_FONT_CONTEXT_H__
#define __FR_FONT_CONTEXT_H__

#include <Framework/Graph/FrContext.h>

SYS_BEGIN_DECLS

#define FR_TYPE_FONT_CONTEXT (fr_font_context_get_type())
#define FR_FONT_CONTEXT(o) ((FrFontContext* )sys_object_cast_check(o, FR_TYPE_FONT_CONTEXT))
#define FR_FONT_CONTEXT_CLASS(o) ((FrFontContextClass *)sys_class_cast_check(o, FR_TYPE_FONT_CONTEXT))
#define FR_FONT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrFontContextClass)

struct _FrFontContextClass {
  FrContextClass parent;
};

struct _FrFontContext {
  FrContext parent;

  /* <private> */
  PangoLayout *layout;
};

SYS_API SysType fr_font_context_get_type(void);
SYS_API FrFontContext *fr_font_context_new(void);

SYS_API FrFontContext *fr_font_context_new_I(void);

SYS_END_DECLS

#endif

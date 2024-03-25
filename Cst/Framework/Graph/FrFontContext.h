#ifndef __FR_FONT_CONTEXT_H__
#define __FR_FONT_CONTEXT_H__

#include <Framework/Graph/FrContext.h>

SYS_BEGIN_DECLS

#define FR_TYPE_FONT_CONTEXT (fr_font_context_get_type())
#define FR_FONT_CONTEXT(o) ((FrFontContext* )sys_object_cast_check(o, FR_TYPE_FONT_CONTEXT))
#define FR_FONT_CONTEXT_CLASS(o) ((FrFontContextClass *)sys_class_cast_check(o, FR_TYPE_FONT_CONTEXT)
#define FR_FONT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrFontContextClass)

struct _FrFontContextClass {
  FrContextClass parent;
};

struct _FrFontContext {
  FrContext parent;

  /* <private> */
  FrDrawLayout *layout;
};

SYS_API SysType fr_font_context_get_type(void);
SYS_API FrFontContext *fr_font_context_new(void);

SYS_API FrFontContext *fr_font_context_new_I(void);
SYS_API void fr_font_context_draw_text(FrFontContext* self, FrFontContextLayout* layout, SysInt x, SysInt y);
SYS_API void fr_font_context_show_text(FrFontContext * self, FrFontContextLayout* layout, SysInt x, SysInt y, SysInt m1, SysInt m0);
SYS_API void fr_font_context_set_color(FrFontContext *self, FrColor *color);
SYS_API void fr_font_context_layout_layout(FrFontContext * self, FrFontContextLayout * layout);

SYS_END_DECLS

#endif

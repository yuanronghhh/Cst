#ifndef __FR_GL_RENDER_H__
#define __FR_GL_RENDER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_GL_RENDER (fr_gl_render_get_type())
#define FR_GL_RENDER(o) ((FrGlRender* )sys_object_cast_check(o, FR_TYPE_GL_RENDER))
#define FR_GL_RENDER_CLASS(o) ((FrGlRenderClass *)sys_class_cast_check(o, FR_TYPE_GL_RENDER))
#define FR_GL_RENDER_GET_CLASS(o) sys_instance_get_class(o, FrGlRenderClass)

struct _FrGlRenderClass {
  SysObjectClass parent;
};

struct _FrGlRender  {
  SysObject parent;

  /* <private> */
  GLuint tex_id;
  GLFWwindow *window;
  SysInt width;
  SysInt height;
};

SYS_API SysType fr_gl_render_get_type(void);
SYS_API FrGlRender *fr_gl_render_new(void);

FrGlRender *fr_gl_render_new_I(GLFWwindow *window);

SYS_END_DECLS

#endif

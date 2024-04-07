#ifndef __FR_DRAW_H__
#define __FR_DRAW_H__

#include <Framework/Graph/FrIDraw.h>


SYS_BEGIN_DECLS


#define FR_TYPE_DRAW (fr_draw_get_type())
#define FR_DRAW(o) ((FrDraw* )sys_object_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_CLASS(o) ((FrDrawClass *)sys_class_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_GET_CLASS(o) sys_instance_get_class(o, FrDrawClass)


struct _FrDrawClass {
  SysObjectClass parent;

  void (*construct) (FrDraw *o);
};

struct _FrDraw {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_draw_get_type(void);

SYS_API void fr_draw_setup(const SysChar *name);
SYS_API void fr_draw_teardown(void);
SYS_API FrIDrawInterface* fr_draw_get_iface(void);
SYS_API FrIDraw* fr_draw_get_g_idraw(void);

SYS_END_DECLS

#endif


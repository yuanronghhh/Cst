#ifndef __FR_I_DRAW__
#define __FR_I_DRAW__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DRAW (fr_i_draw_get_type())
#define FR_I_DRAW(o) ((FrIDraw* )sys_object_cast_check(o, FR_TYPE_I_DRAW))
#define FR_I_DRAW_GET_IFACE(o) ((FrIDrawInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DRAW))

struct _FrIDrawInterface {
  SysTypeInterface parent;

  /* surface */
  FrDrawSurface* (*create_image_surface) (SysInt width, SysInt height);
  FrDrawSurface* (*create_surface) (FrIDevice* idevice, SysInt width, SysInt height);
  FrDrawSurface* (*create_surface_N) (FrIDevice* idevice);
  FrDrawSurface* (*create_image_surface_from_surface) (FrDrawSurface *surface, SysInt width, SysInt height);

  /* context */
  FrDrawContext *(*create_cr) (FrDrawSurface *surface);
  FrDrawContext* (*create_cr_default) (FrDraw *self);
  void (*set_source_rgba) (FrDraw *self, FrDrawContext *cr, SysDouble r, SysDouble  g, SysDouble b, SysDouble a);
  void (*stroke_mp) (FrDrawContext* cr, const FrRect *bound, const FrSInt4* m4, const FrSInt4* p4);
  void (*context_fill_background) (FrDrawContext *cr, SysInt width, SysInt height);
};

SysType fr_i_draw_get_type(void);

SYS_API FrDraw * fr_i_draw_get_iface (const SysChar *name);
SYS_API FrDrawContext* fr_i_draw_create_cr (FrDrawSurface *surface);
SYS_API FrDrawContext* fr_i_draw_create_cr_default (FrDraw *self);
SYS_API void fr_i_draw_set_color (FrDraw *self, FrDrawContext *cr, SysDouble r, SysDouble  g, SysDouble b, SysDouble a);
SYS_API FrDrawSurface* fr_i_draw_create_image_surface (FrDraw *self, SysInt width, SysInt height);
SYS_API FrDrawSurface* fr_i_draw_create_surface (FrIDevice* idevice, SysInt width, SysInt height);
SYS_API FrDrawSurface* fr_i_draw_create_image_surface_from_surface (FrDrawSurface *surface, SysInt width, SysInt height);
SYS_API void fr_i_draw_stroke_mp (FrDrawContext* cr, const FrRect *bound, const FrSInt4* m4, const FrSInt4* p4);
SYS_API void fr_i_draw_context_fill_background (FrDrawContext *cr, SysInt width, SysInt height) ;


SYS_END_DECLS

#endif

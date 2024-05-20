#ifndef __FR_I_MEDIA__
#define __FR_I_MEDIA__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_MEDIA (fr_i_media_get_type())
#define FR_I_MEDIA(o) ((FrIMedia* )sys_object_cast_check(o, FR_TYPE_I_MEDIA))
#define FR_I_MEDIA_GET_IFACE(o) ((FrIMediaInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_MEDIA))


struct _FrIMediaInterface {
  SysTypeInterface parent;

  /* <private> */
};


SysType fr_i_media_get_type(void);

SYS_END_DECLS

#endif

#include <Framework/Media/FrIMediaRender.h>

SYS_DEFINE_INTERFACE(FrIMediaRender, fr_i_media_render, SYS_TYPE_OBJECT);

/* object api */
void fr_i_media_render_default_init(FrIMediaRenderInterface* iface) {
}

void fr_i_media_render_render (FrIMediaRender *self, FrMediaFrame *frame, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  FR_I_MEDIA_RENDER_GET_IFACE(self)->render(self, frame, user_data);
}

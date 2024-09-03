#include <CstCore/Front/CstVideo.h>

SYS_DEFINE_TYPE(CstVideo, cst_video, CST_TYPE_RENDER_NODE);

/* object api */
static void cst_video_construct_i(CstRenderNode *self, CstVideoContext *info) {
}

CstRenderNode* cst_video_new(void) {
  return sys_object_new(CST_TYPE_VIDEO, NULL);
}

CstRenderNode *cst_video_new_I(CstVideoContext *info) {
  CstRenderNode *o = cst_video_new();

  cst_video_construct_i(o, info);

  return o;
}

static void cst_video_dispose(SysObject* o) {
  CstVideo *self = CST_VIDEO(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(cst_video_parent_class)->dispose(o);
}

static void cst_video_class_init(CstVideoClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_video_dispose;
}

static void cst_video_init(CstVideo* self) {
}

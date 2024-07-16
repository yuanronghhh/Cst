#ifndef __CST_VIDEO_H__
#define __CST_VIDEO_H__

#include <CstCore/Driver/CstRenderNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_VIDEO (cst_video_get_type())
#define CST_VIDEO(o) ((CstVideo* )sys_object_cast_check(o, CST_TYPE_VIDEO))
#define CST_VIDEO_CLASS(o) ((CstVideoClass *)sys_class_cast_check(o, CST_TYPE_VIDEO))
#define CST_VIDEO_GET_CLASS(o) sys_instance_get_class(o, CstVideoClass)

struct _CstVideoClass {
  CstRenderNodeClass parent;
};

struct _CstVideo {
  CstRenderNode parent;

  /* <private> */
};

struct _CstVideoContext {
  SysChar* filename;
};

SYS_API SysType cst_video_get_type(void);
SYS_API CstRenderNode *cst_video_new(void);

SYS_API CstRenderNode *cst_video_new_I(CstVideoContext *info);

SYS_END_DECLS

#endif

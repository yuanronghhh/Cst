#ifndef __FR_TYPES_H__
#define __FR_TYPES_H__

#include <Framework/FrConfig.h>
#include <System/SysCore.h>

SYS_BEGIN_DECLS

typedef enum _FR_EVENT_ENUM {
  FR_EVENT_T_WINDOW_RESIZE = 1,
  FR_EVENT_T_WINDOW_EXPOSE,
  FR_EVENT_T_WINDOW_CURSOR,
  FR_EVENT_T_WINDOW_CLOSE,
  FR_EVENT_T_WINDOW_REFRESH,
  FR_EVENT_T_FRAMEBUFFER_RESIZE,
  FR_EVENT_T_WINDOW,

  FR_EVENT_T_GAMEPAD,
  FR_EVENT_T_HAT,
  FR_EVENT_T_JOYSTICK,
  FR_EVENT_T_KEY,
  FR_EVENT_T_MOUSE_KEY,
  FR_EVENT_T_CURSOR_MOVE,
  FR_EVENT_T_INPUT,
  FR_EVENT_T_SYSTEM,
  FR_EVENT_T_LAST
} FR_EVENT_ENUM;

typedef enum _FR_ACTION_ENUM {
  FR_ACTION_TUNKNOWN,

  FR_ACTION_TWINDOW_RESIZE,
  FR_ACTION_TWINDOW_EXPOSE,
  FR_ACTION_TWINDOW_REFRESH,
  FR_ACTION_TWINDOW_CLOSE,

  FR_ACTION_TMOUSE_PRESS,
  FR_ACTION_TCORSOR_MOVE,
  FR_ACTION_TMOUSE_RELEASE,
  FR_ACTION_TSYSTEM,

  FR_ACTION_TDRAG_START,
  FR_ACTION_TDRAG_END,
  FR_ACTION_TSCROLL_IN,
  FR_ACTION_TSCROLL_OUT,
  FR_ACTION_TKEY_PRESS,
  FR_ACTION_TKEY_RELEASE,
  FR_ACTION_TKEY_REPEAT,
  FR_ACTION_TMULTI_KEY_PRESS,
  FR_ACTION_TMULTI_DRAG,
  FR_ACTION_TDOUBLE_CLICK,

  FR_ACTION_TLAST
} FR_ACTION_ENUM;

typedef enum _FR_WINDOW_BACKEND_ENUM {
  FR_WINDOW_X11,
  FR_WINDOW_WIN32,
  FR_WINDOW_GLFW
} FR_WINDOW_BACKEND_ENUM;

typedef  struct  _FrCanvasBackend  FrCanvasBackend;

typedef  struct  _FrValue          FrValue;

typedef  struct  _FrDisplay           FrDisplay;
typedef  struct  _FrDisplayClass      FrDisplayClass;

typedef struct _FrDevice FrDevice;
typedef struct _FrDeviceClass FrDeviceClass;

typedef struct _FrCairoDrawContext FrCairoDrawContext;
typedef struct _FrCairoDrawContextClass FrCairoDrawContextClass;

typedef struct _FrIDraw FrIDraw;
typedef struct _FrIDrawInterface FrIDrawInterface;

typedef struct _FrSurfaceContext FrSurfaceContext;
typedef struct _FrSurface FrSurface;
typedef struct _FrSurfaceClass FrSurfaceClass;

typedef struct _FrImageContext FrImageContext;
typedef struct _FrImage FrImage;
typedef struct _FrImageClass FrImageClass;

typedef struct _FrContext FrContext;
typedef struct _FrContextClass FrContextClass;

typedef struct _FrFrontContext FrFrontContext;
typedef struct _FrFrontContextClass FrFontContextClass;

typedef struct _FrDrawContext FrDrawContext;
typedef struct _FrDrawContextClass FrDrawContextClass;

typedef  struct  _FrWindow           FrWindow;
typedef  struct  _FrWindowClass      FrWindowClass;

typedef struct _FrIDevice FrIDevice;
typedef struct _FrIDeviceInterface FrIDeviceInterface;

typedef  struct  _FrAction              FrAction;
typedef  struct  _FrActionClass         FrActionClass;
typedef  struct  _FrAKey                FrAKey;
typedef  struct  _FrAKeyClass           FrAKeyClass;
typedef  struct  _FrAMouseKey           FrAMouseKey;
typedef  struct  _FrAMouseKeyClass      FrAMouseKeyClass;
typedef  struct  _FrACursorMove         FrACursorMove;
typedef  struct  _FrACursorMoveClass    FrACursorMoveClass;

typedef  struct  _FrAWatchBuilder                   FrAWatchBuilder;
typedef  struct  _FrAWatchBuilderClass              FrAWatchBuilderClass;

typedef  struct  _FrAWatch                   FrAWatch;
typedef  struct  _FrAWatchClass              FrAWatchClass;
typedef  struct  _FrAWatchAny                FrAWatchAny;
typedef  struct  _FrAWatchAnyClass           FrAWatchAnyClass;
typedef  struct  _FrAWatchRefresh                FrAWatchRefresh;
typedef  struct  _FrAWatchRefreshClass           FrAWatchRefreshClass;
typedef  struct  _FrAWatchKey                FrAWatchKey;
typedef  struct  _FrAWatchKeyClass           FrAWatchKeyClass;
typedef  struct  _FrAWatchKeyPress                FrAWatchKeyPress;
typedef  struct  _FrAWatchKeyPressClass           FrAWatchKeyPressClass;

typedef  struct  _FrAWatchMousePress         FrAWatchMousePress;
typedef  struct  _FrAWatchMousePressClass    FrAWatchMousePressClass;
typedef  struct  _FrAWatchMouseRelease         FrAWatchMouseRelease;
typedef  struct  _FrAWatchMouseReleaseClass    FrAWatchMouseReleaseClass;
typedef  struct  _FrAWatchCursorMove         FrAWatchCursorMove;
typedef  struct  _FrAWatchCursorMoveClass    FrAWatchCursorMoveClass;


typedef  struct  _FrEvent                   FrEvent;
typedef  struct  _FrEventClass              FrEventClass;

typedef  struct  _FrEventAny                   FrEventAny;
typedef  struct  _FrEventAnyClass              FrEventAnyClass;

typedef  struct  _FrEventRefresh                   FrEventRefresh;
typedef  struct  _FrEventRefreshClass              FrEventRefreshClass;
typedef  struct  _FrEventKey                FrEventKey;
typedef  struct  _FrEventKeyClass           FrEventKeyClass;
typedef  struct  _FrEventMouseKey           FrEventMouseKey;
typedef  struct  _FrEventMouseKeyClass      FrEventMouseKeyClass;
typedef  struct  _FrEventCursorMove         FrEventCursorMove;
typedef  struct  _FrEventCursorMoveClass    FrEventCursorMoveClass;

typedef  struct  _FrEnv           FrEnv;
typedef  struct  _FrEnvClass      FrEnvClass;

typedef  struct  _FrPair           FrPair;
typedef  struct  _FrPairClass      FrPairClass;

typedef  struct  _FrNode           FrNode;
typedef  struct  _FrNodeClass      FrNodeClass;

typedef  struct  _FrMain           FrMain;
typedef  struct  _FrMainClass      FrMainClass;

typedef struct _FrJobTask FrJobTask;
typedef struct _FrJobTaskClass FrJobTaskClass;

typedef struct _FrJob FrJob;
typedef struct _FrJobClass FrJobClass;

typedef  struct  _FrSource           FrSource;
typedef  struct  _FrSourceClass      FrSourceClass;

typedef  struct  _FrApplication           FrApplication;
typedef  struct  _FrApplicationClass      FrApplicationClass;

typedef  struct  _FrWorker           FrWorker;
typedef  struct  _FrWorkerClass      FrWorkerClass;

typedef  struct  _FrMonoContext   FrMonoContext;

typedef SysPointer (*FrJobFunc) (FrJob* job, SysPointer user_data);
typedef SysPointer (*FrJobTaskFunc) (FrJobTask* task, SysPointer user_data);
typedef SysBool (*FrEventCheckFunc) (FrEvent *e, SysPointer user_data);
typedef SysInt (*FrEventFunc) (FrEvent *e, SysPointer user_data);
typedef const FrBound* (*FrGetBoundFunc) (SysPointer user_data);
typedef SysBool (*FrNodeFunc) (FrNode* node, SysPointer user_data);

#include <Framework/Media/FrMediaTypes.h>

SYS_END_DECLS

#endif

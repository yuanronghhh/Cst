#ifndef __FR_TYPES_H__
#define __FR_TYPES_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

typedef enum _FR_EVENT_ENUM {
  FR_EVENT_TWINDOW_RESIZE = 1,
  FR_EVENT_TWINDOW_EXPOSE,
  FR_EVENT_TWINDOW_CURSOR,
  FR_EVENT_TWINDOW_CLOSE,
  FR_EVENT_TWINDOW_REFRESH,
  FR_EVENT_TFRAMEBUFFER_RESIZE,
  FR_EVENT_TWINDOW,

  FR_EVENT_TGAMEPAD,
  FR_EVENT_THAT,
  FR_EVENT_TJOYSTICK,
  FR_EVENT_TKEY,
  FR_EVENT_TMOUSE_KEY,
  FR_EVENT_TCURSOR_MOVE,
  FR_EVENT_TINPUT,
  FR_EVENT_TSYSTEM,
  FR_EVENT_TLAST
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

typedef  struct  _FRCanvasBackend  FRCanvasBackend;

typedef  struct  _FRValue          FRValue;

typedef  struct  _FRDisplay           FRDisplay;
typedef  struct  _FRDisplayClass      FRDisplayClass;
typedef  struct  _FRDisplayPrivate    FRDisplayPrivate;

typedef  struct  _FRText              FRText;
typedef  struct  _FRTextClass         FRTextClass;
typedef  struct  _FRTextPrivate       FRTextPrivate;

typedef  struct  _FRDraw              FRDraw;
typedef  struct  _FRDrawClass         FRDrawClass;
typedef  struct  _FRDrawPrivate       FRDrawPrivate;

typedef  struct  _FRWindow           FRWindow;
typedef  struct  _FRWindowClass      FRWindowClass;
typedef  struct  _FRWindowPrivate    FRWindowPrivate;

typedef  struct  _FRAction              FRAction;
typedef  struct  _FRActionClass         FRActionClass;
typedef  struct  _FRActionPrivate       FRActionPrivate;
typedef  struct  _FRAKey                FRAKey;
typedef  struct  _FRAKeyClass           FRAKeyClass;
typedef  struct  _FRAKeyPrivate         FRAKeyPrivate;
typedef  struct  _FRAMouseKey           FRAMouseKey;
typedef  struct  _FRAMouseKeyClass      FRAMouseKeyClass;
typedef  struct  _FRAMouseKeyPrivate    FRAMouseKeyPrivate;
typedef  struct  _FRACursorMove         FRACursorMove;
typedef  struct  _FRACursorMoveClass    FRACursorMoveClass;
typedef  struct  _FRACursorMovePrivate  FRACursorMovePrivate;

typedef  struct  _FRAWatchProps              FRAWatchProps;
typedef  struct  _FRAWatch                   FRAWatch;
typedef  struct  _FRAWatchClass              FRAWatchClass;
typedef  struct  _FRAWatchPrivate            FRAWatchPrivate;
typedef  struct  _FRAWatchAny                FRAWatchAny;
typedef  struct  _FRAWatchAnyClass           FRAWatchAnyClass;
typedef  struct  _FRAWatchAnyPrivate         FRAWatchAnyPrivate;
typedef  struct  _FRAWatchRefresh                FRAWatchRefresh;
typedef  struct  _FRAWatchRefreshClass           FRAWatchRefreshClass;
typedef  struct  _FRAWatchRefreshPrivate         FRAWatchRefreshPrivate;
typedef  struct  _FRAWatchKey                FRAWatchKey;
typedef  struct  _FRAWatchKeyClass           FRAWatchKeyClass;
typedef  struct  _FRAWatchKeyPrivate         FRAWatchKeyPrivate;
typedef  struct  _FRAWatchKeyPress                FRAWatchKeyPress;
typedef  struct  _FRAWatchKeyPressClass           FRAWatchKeyPressClass;
typedef  struct  _FRAWatchKeyPressPrivate         FRAWatchKeyPressPrivate;

typedef  struct  _FRAWatchMousePress         FRAWatchMousePress;
typedef  struct  _FRAWatchMousePressClass    FRAWatchMousePressClass;
typedef  struct  _FRAWatchMousePressPrivate  FRAWatchMousePressPrivate;
typedef  struct  _FRAWatchMouseRelease         FRAWatchMouseRelease;
typedef  struct  _FRAWatchMouseReleaseClass    FRAWatchMouseReleaseClass;
typedef  struct  _FRAWatchMouseReleasePrivate  FRAWatchMouseReleasePrivate;
typedef  struct  _FRAWatchCursorMove         FRAWatchCursorMove;
typedef  struct  _FRAWatchCursorMoveClass    FRAWatchCursorMoveClass;
typedef  struct  _FRAWatchCursorMovePrivate  FRAWatchCursorMovePrivate;


typedef  struct  _FREvent                   FREvent;
typedef  struct  _FREventClass              FREventClass;
typedef  struct  _FREventPrivate            FREventPrivate;

typedef  struct  _FREventAny                   FREventAny;
typedef  struct  _FREventAnyClass              FREventAnyClass;
typedef  struct  _FREventAnyPrivate            FREventAnyPrivate;

typedef  struct  _FREventRefresh                   FREventRefresh;
typedef  struct  _FREventRefreshClass              FREventRefreshClass;
typedef  struct  _FREventRefreshPrivate            FREventRefreshPrivate;
typedef  struct  _FREventKey                FREventKey;
typedef  struct  _FREventKeyClass           FREventKeyClass;
typedef  struct  _FREventKeyPrivate         FREventKeyPrivate;
typedef  struct  _FREventMouseKey           FREventMouseKey;
typedef  struct  _FREventMouseKeyClass      FREventMouseKeyClass;
typedef  struct  _FREventMouseKeyPrivate    FREventMouseKeyPrivate;
typedef  struct  _FREventCursorMove         FREventCursorMove;
typedef  struct  _FREventCursorMoveClass    FREventCursorMoveClass;
typedef  struct  _FREventCursorMovePrivate  FREventCursorMovePrivate;

typedef  struct  _FREnv           FREnv;
typedef  struct  _FREnvClass      FREnvClass;
typedef  struct  _FREnvPrivate    FREnvPrivate;

typedef  struct  _FRMain           FRMain;
typedef  struct  _FRMainClass      FRMainClass;
typedef  struct  _FRMainPrivate    FRMainPrivate;

typedef  struct  _FRSource           FRSource;
typedef  struct  _FRSourceClass      FRSourceClass;
typedef  struct  _FRSourcePrivate    FRSourcePrivate;

typedef  struct  _FRApplication           FRApplication;
typedef  struct  _FRApplicationClass      FRApplicationClass;
typedef  struct  _FRApplicationPrivate    FRApplicationPrivate;

typedef  struct  _FRWorker           FRWorker;
typedef  struct  _FRWorkerClass      FRWorkerClass;
typedef  struct  _FRWorkerPrivate    FRWorkerPrivate;


typedef  struct  _FRMonoContext   FRMonoContext;

typedef SysBool (*FREventCheckFunc) (FREvent *e, SysPointer user_data);
typedef SysInt (*FREventFunc) (FREvent *e, SysPointer user_data);

SYS_END_DECLS

#endif

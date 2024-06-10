#ifndef __FR_MEDIA_TYPES__
#define __FR_MEDIA_TYPES__

#include <Framework/ThirdParty/FrLibav.h>

SYS_BEGIN_DECLS

typedef enum _FR_MEDIA_ENUM {
  FR_MEDIA_UNKNOWN = AVMEDIA_TYPE_UNKNOWN,
  FR_MEDIA_VIDEO = AVMEDIA_TYPE_VIDEO,
  FR_MEDIA_AUDIO = AVMEDIA_TYPE_AUDIO,
  FR_MEDIA_DATA = AVMEDIA_TYPE_DATA,
  FR_MEDIA_SUBTITLE = AVMEDIA_TYPE_SUBTITLE,
  FR_MEDIA_ATTACHMENT = AVMEDIA_TYPE_ATTACHMENT,
  FR_MEDIA_NB = AVMEDIA_TYPE_NB,
  FR_MEDIA_PACKET,
} FR_MEDIA_ENUM;

typedef enum _FR_MEDIA_ERROR_ENUM {
  FR_MEDIA_ERROR_FILTER_NOT_FOUND = AVERROR_FILTER_NOT_FOUND,
  FR_MEDIA_ERROR_STREAM_NOT_FOUND = AVERROR_STREAM_NOT_FOUND,
  FR_MEDIA_ERROR_EINVAL = AVERROR(EINVAL),
  FR_MEDIA_ERROR_AGAIN = AVERROR(EAGAIN),
  FR_MEDIA_ERROR_EOF = AVERROR_EOF,
  FR_MEDIA_ERROR_EXIT = AVERROR_EXIT,
  FR_MEDIA_ERROR_UNKNOWN = AVERROR_UNKNOWN,
  FR_MEDIA_ERROR_WAIT = -2,
  FR_MEDIA_ERROR_SUCCESS = 0,
} FR_MEDIA_ERROR_ENUM;

typedef enum _FR_MEDIA_STATE_ENUM {
  FR_MEDIA_STATE_PAUSE = 1 << 1,
  FR_MEDIA_STATE_RUNNING = 1 << 2,
  FR_MEDIA_STATE_STOP = 1 << 3,
} FR_MEDIA_STATE_ENUM;

typedef enum _FR_DECODER_CMD_ENUM {
  FR_DECODER_CMD_NOOP,
  FR_DECODER_CMD_INIT,
  FR_DECODER_CMD_PUSH_PACKET,
  FR_DECODER_CMD_SEEK,
  FR_DECODER_CMD_STOP,
  FR_DECODER_CMD_PAUSE,
  FR_DECODER_CMD_READ
} FR_DECODER_CMD_ENUM;

typedef struct AVRational FrRational;

typedef struct _FrPacket FrPacket;
typedef struct _FrPacketClass FrPacketClass;

typedef struct _FrPacketQueue FrPacketQueue;
typedef struct _FrPacketQueueClass FrPacketQueueClass;

typedef struct _FrPacketDecoder FrPacketDecoder;
typedef struct _FrPacketDecoderClass FrPacketDecoderClass;

typedef struct _FrIStream FrIStream;
typedef struct _FrIStreamInterface FrIStreamInterface;

typedef struct _FrPipeline FrPipeline;
typedef struct _FrPipelineClass FrPipelineClass;

typedef struct _FrMediaPipeline FrMediaPipeline;
typedef struct _FrMediaPipelineClass FrMediaPipelineClass;

typedef struct _FrMediaTask FrMediaTask;
typedef struct _FrMediaTaskClass FrMediaTaskClass;

typedef struct _FrDecoder FrDecoder;
typedef struct _FrDecoderClass FrDecoderClass;

typedef struct _FrStream FrStream;
typedef struct _FrStreamClass FrStreamClass;

typedef struct _FrPlayer FrPlayer;
typedef struct _FrPlayerClass FrPlayerClass;

typedef struct _FrIDecoder FrIDecoder;
typedef struct _FrIDecoderInterface FrIDecoderInterface;

typedef struct _FrFrameDecoder FrFrameDecoder;
typedef struct _FrFrameDecoderClass FrFrameDecoderClass;

typedef struct _FrAudioDecoder FrAudioDecoder;
typedef struct _FrAudioDecoderClass FrAudioDecoderClass;

typedef struct _FrMediaPacket FrMediaPacket;
typedef struct _FrMediaPacketClass FrMediaPacketClass;

typedef struct _FrMediaFile FrMediaFile;
typedef struct _FrMediaFileClass FrMediaFileClass;

typedef struct _FrIMedia FrIMedia;
typedef struct _FrIMediaInterface FrIMediaInterface;

typedef struct _FrMediaDecoder FrMediaDecoder;
typedef struct _FrMediaDecoderClass FrMediaDecoderClass;

typedef struct _FrImageSaver FrImageSaver;
typedef struct _FrImageSaverClass FrImageSaverClass;

typedef struct _FrImageScale FrImageScale;
typedef struct _FrImageScaleClass FrImageScaleClass;

typedef struct _FrProportion FrProportion;
typedef struct _FrImageScaleContext FrImageScaleContext;

typedef struct _FrMediaStream FrMediaStream;
typedef struct _FrMediaStreamClass FrMediaStreamClass;

typedef struct _FrMediaPlayer FrMediaPlayer;
typedef struct _FrMediaPlayerClass FrMediaPlayerClass;

typedef struct _FrMediaFrame FrMediaFrame;
typedef struct _FrMediaFrameClass FrMediaFrameClass;

typedef struct _FrMediaFilter FrMediaFilter;
typedef struct _FrMediaFilterClass FrMediaFilterClass;

typedef struct _FrAudioFrame FrAudioFrame;
typedef struct _FrAudioFrameClass FrAudioFrameClass;

typedef struct _FrIMediaRender FrIMediaRender;
typedef struct _FrIMediaRenderInterface FrIMediaRenderInterface;

typedef struct _FrGlRender FrGlRender;
typedef struct _FrGlRenderClass FrGlRenderClass;

typedef struct _FrCairoRender FrCairoRender;
typedef struct _FrCairoRenderClass FrCairoRenderClass;

typedef struct _FrVideoFrame FrVideoFrame;
typedef struct _FrVideoFrameClass FrVideoFrameClass;

typedef struct _FrVideoDecoder FrVideoDecoder;
typedef struct _FrVideoDecoderClass FrVideoDecoderClass;

typedef SysPointer (*FrMediaTaskFunc) (FrMediaTask* task, SysPointer user_data);
typedef SysPointer (*FrMediaPlayerFunc) (FrMediaPlayer *player, SysPointer user_data);

SYS_END_DECLS

#endif

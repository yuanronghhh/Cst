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

typedef enum _FR_AUDIO_FORMAT_ENUM {
  FR_AUDIO_FORMAT_UNKNOWN = AV_SAMPLE_FMT_NONE,
  FR_AUDIO_FORMAT_U8,          ///< unsigned 8 bits
  FR_AUDIO_FORMAT_S16,         ///< signed 16 bits
  FR_AUDIO_FORMAT_S32,         ///< signed 32 bits
  FR_AUDIO_FORMAT_FLT,         ///< float
  FR_AUDIO_FORMAT_DBL,         ///< double
  FR_AUDIO_FORMAT_U8P,         ///< unsigned 8 bits, planar
  FR_AUDIO_FORMAT_S16P,        ///< signed 16 bits, planar
  FR_AUDIO_FORMAT_S32P,        ///< signed 32 bits, planar
  FR_AUDIO_FORMAT_FLTP,        ///< float, planar
  FR_AUDIO_FORMAT_DBLP,        ///< double, planar
  FR_AUDIO_FORMAT_S64,         ///< signed 64 bits
  FR_AUDIO_FORMAT_S64P,        ///< signed 64 bits, planar
} FR_AUDIO_FORMAT_ENUM;

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

typedef enum _FR_JOB_STATE_ENUM {
  FR_JOB_STATE_PAUSE = 1,
  FR_JOB_STATE_RUNNING = 2,
  FR_JOB_STATE_STOP = 3,
} FR_JOB_STATE_ENUM;

typedef FR_JOB_STATE_ENUM FR_JOB_STATE_ENUM;

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

typedef struct _FrDecoderContext FrDecoderContext;
typedef struct _FrDecoder FrDecoder;
typedef struct _FrDecoderClass FrDecoderClass;

typedef struct _FrAvPlayerContext FrAvPlayerContext;
typedef struct _FrAvPlayer FrAvPlayer;
typedef struct _FrAvPlayerClass FrAvPlayerClass;

typedef struct _FrStreamContext FrStreamContext;
typedef struct _FrStream FrStream;
typedef struct _FrStreamClass FrStreamClass;

typedef struct _FrPlayer FrPlayer;
typedef struct _FrPlayerClass FrPlayerClass;

typedef struct _FrIDecoder FrIDecoder;
typedef struct _FrIDecoderInterface FrIDecoderInterface;

typedef struct _FrHwAccelContext FrHwAccelContext;
typedef struct _FrHwAccel FrHwAccel;
typedef struct _FrHwAccelClass FrHwAccelClass;

typedef struct _FrFrameDecoder FrFrameDecoder;
typedef struct _FrFrameDecoderClass FrFrameDecoderClass;

typedef struct _FrAudioDecoderContext FrAudioDecoderContext;
typedef struct _FrAudioDecoder FrAudioDecoder;
typedef struct _FrAudioDecoderClass FrAudioDecoderClass;

typedef struct _FrMediaPacket FrMediaPacket;
typedef struct _FrMediaPacketClass FrMediaPacketClass;

typedef struct _FrMediaFile FrMediaFile;
typedef struct _FrMediaFileClass FrMediaFileClass;

typedef struct _FrIMedia FrIMedia;
typedef struct _FrIMediaInterface FrIMediaInterface;

typedef struct _FrMediaDecoderContext FrMediaDecoderContext;
typedef struct _FrMediaDecoder FrMediaDecoder;
typedef struct _FrMediaDecoderClass FrMediaDecoderClass;

typedef struct _FrImageSaver FrImageSaver;
typedef struct _FrImageSaverClass FrImageSaverClass;

typedef struct _FrImageScale FrImageScale;
typedef struct _FrImageScaleClass FrImageScaleClass;

typedef struct _FrProportion FrProportion;
typedef struct _FrImageScaleContext FrImageScaleContext;

typedef struct _FrMediaStreamContext FrMediaStreamContext;
typedef struct _FrMediaStream FrMediaStream;
typedef struct _FrMediaStreamClass FrMediaStreamClass;

typedef struct _FrAvPlayerContext FrAvPlayerContext;
typedef struct _FrAvPlayer FrAvPlayer;
typedef struct _FrAvPlayerClass FrAvPlayerClass;

typedef struct _FrMediaFrame FrMediaFrame;
typedef struct _FrMediaFrameClass FrMediaFrameClass;

typedef struct _FrMediaFilter FrMediaFilter;
typedef struct _FrMediaFilterClass FrMediaFilterClass;

typedef struct _FrAudioStreamContext FrAudioStreamContext;
typedef struct _FrAudioStream FrAudioStream;
typedef struct _FrAudioStreamClass FrAudioStreamClass;

typedef struct _FrAudioFrame FrAudioFrame;
typedef struct _FrAudioFrameClass FrAudioFrameClass;

typedef struct _FrIMediaRender FrIMediaRender;
typedef struct _FrIMediaRenderInterface FrIMediaRenderInterface;

typedef struct _FrGlRender FrGlRender;
typedef struct _FrGlRenderClass FrGlRenderClass;

typedef struct _FrCairoRender FrCairoRender;
typedef struct _FrCairoRenderClass FrCairoRenderClass;

typedef struct _FrVideoStream FrVideoStream;
typedef struct _FrVideoStreamClass FrVideoStreamClass;

typedef struct _FrVideoFrame FrVideoFrame;
typedef struct _FrVideoFrameClass FrVideoFrameClass;

typedef struct _FrVideoDecoder FrVideoDecoder;
typedef struct _FrVideoDecoderClass FrVideoDecoderClass;

typedef SysPointer (*FrAvPlayerFunc) (FrAvPlayer *player, SysPointer user_data);

SYS_END_DECLS

#endif

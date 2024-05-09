#ifndef __FR_MEDIA_CORE_H__
#define __FR_MEDIA_CORE_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaFilter.h>
#include <Framework/Media/FrMediaPlayer.h>
#include <Framework/Media/FrPipeline.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrStream.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrMediaFrame.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrGlRender.h>
#include <Framework/Media/FrAudioFrame.h>
#include <Framework/Media/FrFrameDecoder.h>
#include <Framework/Media/FrIMediaRender.h>

void ffmpeg_setup(void);
void ffmpeg_teardown(void);

SYS_END_DECLS

#endif

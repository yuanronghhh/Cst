#ifndef __FR_LIBAV_H__
#define __FR_LIBAV_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

#include <libavformat/avformat.h>
#include <libavformat/avio.h>

#include <libavutil/tx.h>
#include <libavutil/fifo.h>
#include <libavutil/avutil.h>
#include <libavutil/rational.h>
#include <libavutil/avstring.h>
#include <libavutil/pixfmt.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <libavutil/bswap.h>
#include <libavutil/hwcontext_drm.h>
#include <libavutil/buffer.h>
#include <libavutil/lfg.h>
#include <libavutil/avstring.h>
#include <libavutil/eval.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/dict.h>
#include <libavutil/parseutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/avassert.h>
#include <libavutil/time.h>
#include <libavutil/mem.h>
#include <libavutil/bprint.h>
#include <libavutil/channel_layout.h>

#include <libavcodec/codec.h>
#include <libavcodec/avfft.h>
#include <libavcodec/avcodec.h>

#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>


SYS_END_DECLS

#endif

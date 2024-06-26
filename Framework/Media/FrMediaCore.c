#include <Framework/Media/FrMediaCore.h>

static SysBool inited = false;

static void sigterm_handler(int sig) {
  sys_debug_N("received signal and exit: %d", sig);
  exit(0);
}

static void ffp_log_callback_brief(void *ptr,
    int level,
    const char *fmt,
    va_list vl) {

  sys_vfprintf(stdout, fmt, vl);
}

void fr_ffmpeg_setup(void) {
  if(inited) { return; }

#if LIBAV_DEVICE
  avdevice_register_all();
#endif

  avformat_network_init();
  av_log_set_callback(ffp_log_callback_brief);

  signal(SIGINT , sigterm_handler); /* Interrupt (ANSI).    */
  signal(SIGTERM, sigterm_handler); /* Termination (ANSI).  */

  inited = true;
}

void fr_ffmpeg_teardown(void) {
  if(!inited) { return; }

  avformat_network_deinit();
  inited = false;
}


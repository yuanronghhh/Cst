#include <Framework/Media/FrMediaCore.h>

static SysBool inited = false;

static void sigterm_handler(int sig) {
  sys_debug_N("received signal and exit: %d", sig);
  exit(0);
}

static void ffp_log_callback_brief(void *ptr,
    int level,
    const char *format,
    va_list vl) {

  if(level >= AV_LOG_WARNING) {
    return;
  }

  sys_vlog(SYS_LOG_ARGS(ffp_log_callback_brief, format) stderr, SYS_LOG_WARNING, format, vl);
}

void fr_ffmpeg_setup(void) {
  if(inited) { return; }

  avdevice_register_all();
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


#include "Demo.hpp"

static const char *level_names[] = {
  "FATAL", "ERROR", "CRITICAL", "WARN",
  "MESSAGE", "INFO", "DEBUG"
};

void setUp(void) {
}

void tearDown(void) {
}

static char* cst_log_get_field(const GLogField* fields, gsize fieldLen, char *name) {
  int i;
  int nLen = g_utf8_strlen(name, 20);

  for (i = 0; i < fieldLen; i++) {
    if (strncmp((fields + i)->key, name, nLen) == 0) {
      return (char *)(fields + i)->value;
    }
  }

  return NULL;
}

static GLogWriterOutput env_logger_callback (
  GLogLevelFlags log_level,
  const GLogField *fields,
  gsize            n_fields,
  gpointer         user_data) {

  GDateTime *tm;
  GTimeZone *tz;
  char *filename, *lineno, *message;
  int idx;

#ifndef G_LOG_USE_STRUCTURED
  return G_LOG_WRITER_UNHANDLED;
#endif

  idx = (int)(log((int)log_level) / log(2));
  tz = g_time_zone_new_local();
  tm = g_date_time_new_now(tz);
  
  message = cst_log_get_field(fields, n_fields, "MESSAGE");

  if (strncmp(fields->key, ("GLIB_OLD_LOG_API"), 16) == 0) {
    printf("[%s]%d-%02d-%02d %02d:%02d:%02d.%03d: :%s\n",
      level_names[idx - 1],
      g_date_time_get_year(tm),
      g_date_time_get_month(tm),
      g_date_time_get_day_of_month(tm),
      g_date_time_get_hour(tm),
      g_date_time_get_minute(tm),
      g_date_time_get_second(tm),
      g_date_time_get_microsecond(tm),
      message);
  }
  else {
    filename = cst_log_get_field(fields, n_fields, "CODE_FILE");
    lineno = cst_log_get_field(fields, n_fields, "CODE_LINE");
    g_return_val_if_fail(message != NULL && filename != NULL && lineno != NULL && "log info cannot access", G_LOG_WRITER_UNHANDLED);

    printf("[%s]%d-%02d-%02d %02d:%02d:%02d.%03d: %s:%s:%s\n",
      level_names[idx - 1],
      g_date_time_get_year(tm),
      g_date_time_get_month(tm),
      g_date_time_get_day_of_month(tm),
      g_date_time_get_hour(tm),
      g_date_time_get_minute(tm),
      g_date_time_get_second(tm),
      g_date_time_get_microsecond(tm),
      filename,
      lineno,
      message);
  }

  g_time_zone_unref(tz);
  g_date_time_unref(tm);

  return G_LOG_WRITER_HANDLED;
}


static void env_init(void) {
#if defined(__CST_WIN32__)
  SMALL_RECT rc = { 0, 0, 300, 80 };
  SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), false, &rc);

  SetConsoleOutputCP(65001);

  CONSOLE_FONT_INFOEX cfi;
  cfi.cbSize = sizeof(cfi);
  cfi.nFont = 0;
  cfi.dwFontSize.X = 0;
  cfi.dwFontSize.Y = 16;
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = FW_NORMAL;
  wcscpy_s(cfi.FaceName, 9, L"Consolas");
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif

  g_log_set_writer_func(env_logger_callback, NULL, NULL);
}

int main(int argc, char *argv[]) {
  env_init();

  g_info("%s", "start1");

  test_vulkan_run();

#ifdef SYS_OS_WIN32
  getchar();
#endif
  return 0;
}

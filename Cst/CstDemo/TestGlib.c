#include <CstDemo/TestGlib.h>

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
#define BSIZE 1024 * 8

#if _MSC_VER
  #define builtin_frame_address(x)  ((void)(x), _AddressOfReturnAddress())
#elif __GNUC__
  #define builtin_frame_address(x)  (__builtin_frame_address(x))
#endif

typedef struct {
  guint8 bytes[16];
} GUuid;

typedef  struct  _ModuleType  ModuleType;
typedef  struct  _FPrototype  FPrototype;
typedef  struct  _FArgs       FArgs;
typedef  struct  _FValue      FValue;
typedef  struct  _TagInfo     TagInfo;

struct _FValue {
  int type;
  union value {
    double v_double;
    int v_int;
    GString * v_string;
  } v;
};

struct _FArgs {
  int type;
  char *var;
  FValue *value;
};

struct _FPrototype {
  char *func_name;
  GPtrArray *args;
};

struct _ModuleType {
  char *name;
  FPrototype *proto;
};

struct _TagInfo {
  char *tag_name;
  char *tag_file;
  char *tag_address;
};

#if defined(MAXPATHLEN)
#define G_PATH_LENGTH MAXPATHLEN
#elif defined(PATH_MAX)
#define G_PATH_LENGTH PATH_MAX
#elif defined(_PC_PATH_MAX)
#define G_PATH_LENGTH sysconf(_PC_PATH_MAX)
#else
#define G_PATH_LENGTH 2048
#endif

static const char *level_names[] = {
  "FATAL", "ERROR", "CRITICAL", "WARN",
  "MESSAGE", "INFO", "DEBUG"
};

typedef void (*WalkFileFunc)(const gchar *dirPath, GPtrArray *dirArray, GPtrArray *fileArray);

static char* cst_log_get_field(const GLogField* fields, gsize field_len, char *name) {
  size_t i;
  int nLen = g_utf8_strlen(name, 20);

  for (i = 0; i < field_len; i++) {
    if (sys_strneq((fields + i)->key, name, nLen) == 0) {
      return (char *)(fields + i)->value;
    }
  }

  return NULL;
}

static GLogWriterOutput env_logger_callback(
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

  idx = (int)(log(log_level) / log(2));
  tz = g_time_zone_new_local();
  tm = g_date_time_new_now(tz);

  message = cst_log_get_field(fields, n_fields, "MESSAGE");

  if (strncmp(fields->key, ("GLIB_OLD_LOG_API"), 16) == 0) {
    printf("[%s]%d-%02d-%02d %02d:%02d:%02d.%03d: %s\n",
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
  g_log_set_writer_func(env_logger_callback, NULL, NULL);
}

void test_random_not_repeat(void) {
  int m = 10000;
  gchar *path = "/home/greyhound/Documents/Cst/tmp.txt";
  GArray *barray;
  int *array;
  int tmp;
  int i;
  int r;
  GError *err = NULL;
  GIOChannel *fio;
  GIOStatus status;
  gint64 end, start;
  char *buff;
  gsize bwriten = 0;

  barray = g_array_sized_new(true, true, sizeof(int), m);
  array = (int *)barray->data;

  for(i = 0; i < m; i++) {
    array[i] = i;
  }

  start = g_get_monotonic_time();
  srand((guint)time(NULL));
  for(i = m - 1; i >= 1; i--) {
    r = rand() % i;

    tmp = array[i];
    array[i] = array[r];
    array[r] = tmp;
  }
  end = g_get_monotonic_time();

  fio = g_io_channel_new_file(path, "w", &err);
  for(i = 0; i < m; i++) {
    buff = g_strdup_printf("%d\n", array[i]);
    status = g_io_channel_write_chars(fio, buff, -1, &bwriten, &err);
    if(status != G_IO_STATUS_NORMAL) {
      printf("[Error] %d\n", status);
      abort();
    }

    g_free(buff);
  }

  g_io_channel_shutdown(fio, true, &err);
  g_io_channel_unref(fio);

  g_print("%ld\n", end - start);
  g_array_free(barray, true);
}

void test_md5(void) {
  char *data = "hello,world";
  GChecksum *checksum;
  gchar *retval;

  checksum = g_checksum_new (G_CHECKSUM_MD5);
  if (!checksum)
    return;

  g_checksum_update (checksum, (const guchar *)data, strlen(data));

  retval = g_strdup (g_checksum_get_string (checksum));
  g_checksum_free (checksum);
  printf("%s\n", retval);
}

char* test_uuid_generate_v4 (void)
{
  int i;
  guint8 *bytes;
  guint32 *ints;
  GUuid suuid;
  GUuid *uuid = &suuid;
  guint version = 4;

  bytes = uuid->bytes;
  ints = (guint32 *) bytes;
  for (i = 0; i < 4; i++)
    ints[i] = g_random_int ();

  bytes = uuid->bytes;

  /*
   * Set the four most significant bits (bits 12 through 15) of the
   * time_hi_and_version field to the 4-bit version number from
   * Section 4.1.3.
   */
  bytes[6] &= 0x0f;
  bytes[6] |= version << 4;
  /*
   * Set the two most significant bits (bits 6 and 7) of the
   * clock_seq_hi_and_reserved to zero and one, respectively.
   */
  bytes[8] &= 0x3f;
  bytes[8] |= 0x80;

  char *result = g_strdup_printf ("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x"
                          "-%02x%02x%02x%02x%02x%02x",
                          bytes[0], bytes[1], bytes[2], bytes[3],
                          bytes[4], bytes[5], bytes[6], bytes[7],
                          bytes[8], bytes[9], bytes[10], bytes[11],
                          bytes[12], bytes[13], bytes[14], bytes[15]);

  return result;
}

void test_uuid(void){
  int i;
  gint64 s,e;

  s = g_get_monotonic_time();
  for(i = 0 ; i < 1000000; i++) {
    test_uuid_generate_v4();
  }
  e = g_get_monotonic_time();
  g_print("%ld\n", e - s);
}

void test_glib_version(void) {
  g_print("[glib version] %d.%d\n", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION);
}

static void os_walk(const gchar *path, WalkFileFunc func) {
  g_return_if_fail(path != NULL);

  GDir *dir = NULL;
  gchar *nName = NULL;
  const gchar *fName = NULL;
  GError *error = NULL;
  gsize i;
  GPtrArray *dirArray;
  GPtrArray *fileArray;

  dir = g_dir_open(path, 0, &error);
  if (dir == NULL) { return; }

  dirArray = g_ptr_array_new_full(0, g_free);
  fileArray = g_ptr_array_new_full(0, g_free);

  while ((fName = g_dir_read_name(dir)) != NULL)  {
    nName = g_canonicalize_filename(fName, path);
    if (nName == NULL) {
      break;
    }

    if(g_file_test(nName, G_FILE_TEST_IS_REGULAR)) {
      g_ptr_array_add(fileArray, (void *)nName);

    } else if(g_file_test(nName, G_FILE_TEST_IS_DIR)) {
      g_ptr_array_add(dirArray, (void *)nName);
    } else {
    }
  }

  if (func != NULL) {
    func(path, dirArray, fileArray);

    for(i = 0; i < dirArray->len; i++) {
      os_walk(nName, func);
    }
  }

  g_ptr_array_free(fileArray, true);
  g_ptr_array_free(dirArray, true);
  g_dir_close(dir);
}

static void walk_handle(const gchar *dirPath, GPtrArray *dirArray, GPtrArray *fileArray) {
  gsize i;
  gchar *name = NULL;

  for(i = 0; i < dirArray->len; i++) {
    name = g_path_get_basename((gchar *)(dirArray->pdata[i]));
    g_free(name);
  }

  for(i = 0; i < fileArray->len; i++) {
    name = g_path_get_basename((gchar *)(fileArray->pdata[i]));
    g_free(name);
  }
}

static void test_dir_walk(void) {
  gchar *path = "/home/greyhound/Git/JSONTestSuite/test_parsing";
  os_walk(path, walk_handle);
}

void dump_hex(const uint8_t *ptr, uint16_t buflen) {
  unsigned char *buf = (unsigned char*)ptr;
  int i, j;

  for (i = 0; i < buflen; i += 16)
  {
    printf("%p: ", (ptr + i));

    for (j = 0; j < 16; j++)
      if (i + j < buflen)
        printf("%02X ", buf[i + j]);
      else
        printf("   ");
    printf(" ");

    for (j = 0; j < 16; j++)
      if (i + j < buflen)
        printf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
    printf("\n");
  }
}

static void test_dump_memory(void) {
  void *buf = builtin_frame_address(0);
  dump_hex(buf, 200);
}

static guint str_hash(void *v) {
  const signed char *p;
  guint32 h = 5381;

  for (p = v; *p != '\0'; p++) {
    h = (h << 5) + h + *p;
  }

  return h;
}

static void test_binary_print(void) {
  gint i;
  guint mask = 0;
  guint nidx = 0;

  for (i = 0; i < 3; i++)
  {
    mask <<= 1;
    mask |= 1;
  }
  double a = 0.1;
  double b = 0.2;
  double c = 0.3;
  double d = a + b;

  char *buf = sys_bin_new(a, sizeof(a));
  buf = sys_bin_tostr(buf, a, sizeof(a));
  buf = sys_bin_tostr(buf, b, sizeof(a));
  buf = sys_bin_tostr(buf, c, sizeof(a));
  buf = sys_bin_tostr(buf, d, sizeof(a));

  g_print("%lf\n", c);
  g_print("%lf\n", d);
  free(buf);

  guint hv = str_hash("hello");
  nidx = hv % 7;
  nidx &= mask;

  g_print("%d\n", mask);
}

jmp_buf jbuff;
static void func(int count)
{
  g_print("a(%d) called\n", count);
  longjmp(jbuff, count + 1); // will return count+1 out of setjmp
}

static void test_set_jump(void) {
  volatile int count = 0;
  if (setjmp(jbuff) != 9) {
    func(++count);
  }
}

#define ENUM_MACRO(e) CUS_ENUM(e)

const char *watch_event[] = {
  "G_FILE_MONITOR_EVENT_CHANGED",
  "G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT",
  "G_FILE_MONITOR_EVENT_DELETED",
  "G_FILE_MONITOR_EVENT_CREATED",
  "G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED",
  "G_FILE_MONITOR_EVENT_PRE_UNMOUNT",
  "G_FILE_MONITOR_EVENT_UNMOUNTED",
  "G_FILE_MONITOR_EVENT_MOVED",
  "G_FILE_MONITOR_EVENT_RENAMED",
  "G_FILE_MONITOR_EVENT_MOVED_IN",
  "G_FILE_MONITOR_EVENT_MOVED_OUT"
};

static GMainLoop *loop = NULL;

static void file_watch_callback(GFileMonitor     *monitor,
  GFile            *file,
  GFile            *other_file,
  GFileMonitorEvent event_type,
  gpointer          user_data) {
  // if (event_type != G_FILE_MONITOR_EVENT_CHANGED) {
  //   return;
  // }
  g_info("[file_watch_callback] %s", watch_event[event_type]);

  g_main_loop_quit(loop);
}

static void test_file_monitor(void) {
  GError *error = NULL;
  char *mpath = "E:/TMP/note.log";
  GFile *path = g_file_new_for_path(mpath);

  GMainContext *ctx = g_main_context_default();
  loop = g_main_loop_new(ctx, true);
  GFileMonitor *m = g_file_monitor_file(path,
    G_FILE_MONITOR_WATCH_MOVES, NULL, &error);

  g_signal_connect(m, "changed", (GCallback)file_watch_callback, NULL);

  g_main_loop_run(loop);
  g_main_loop_unref(loop);
  g_main_context_unref(ctx);
  g_object_unref(m);
  g_object_unref(path);
}

static GHashTable *module_ht = NULL;

static void test_module(void) {
  // char *name = "E:/Codes/REPOSITORY/Cst/build/src/gcoroutine/Debug/gcoroutine.dll";

  // GModule *m = g_module_open(name, G_MODULE_BIND_LOCAL);
  // gpointer *s = NULL;
  // char *sname = "g_coroutine_new";

  // if (!g_module_symbol(m, sname, &s)) {
  // }
}

static char *get_next_ts(char *s, char c) {
  char *tb;

  tb = strchr(s, c); 

  return tb;
}

static TagInfo* get_tag_from_line(char *s, int max) {
  char *ns, *ns2, *ns3;
  TagInfo *t;

  if (*s == '!') {
    return NULL;
  }

  t = sys_new0_N(TagInfo, 1);
  ns = get_next_ts(s, '\t');
  if (!ns || !*(ns + 1)) { goto fail; }

  ns2 = get_next_ts(ns + 1, '\t');
  if (!ns2 || !*(ns2 + 1)) { goto fail; }

  ns3 = get_next_ts(ns2 + 1, '\t');
  if (!ns3) { 
    ns3 = get_next_ts(ns2 + 1, '\n');
    if (!ns3) { goto fail; }
  }

  t->tag_name = sys_strndup(s, ns - s);
  t->tag_file = sys_strndup(ns + 1, ns2 - ns - 1);
  t->tag_address = sys_strndup(ns2 + 1, ns3 - ns2 - 1);

  return t;
fail:
  sys_free_N(t);
  return NULL;
}

void sys_tag_info_free(TagInfo *info) {
  sys_free_N(info->tag_file);
  sys_free_N(info->tag_name);
  sys_free_N(info->tag_address);

  sys_free_N(info);
}

static SysPtrArray* get_tag_from_fp(FILE *tagfp) {
  char *s;
  SysPtrArray *infos;
  TagInfo *info;

  char buffer[BSIZE] = { 0 };
  buffer[BSIZE - 1] = '\0';

  infos = sys_ptr_array_new_with_free_func((SysDestroyFunc)sys_tag_info_free);
  while ((s = fgets(&buffer[0], BSIZE - 1, tagfp)) != NULL) {
    info = get_tag_from_line(s, BSIZE);
    if (!info) {
      continue;
    }

    sys_ptr_array_add(infos, info);
  }

  return infos;
}

static void test_file_count(void) {
  SysPtrArray *infos;
  FILE *tagfp;
  char *filename = "E:/Codes/REPOSITORY/Demo/tags";
  char buffer[BSIZE] = { 0 };
  // char *dirname = sys_strndup(filename, sys_path_rslash(filename));
  // char *dot_filename = sys_strcat(filename, sys_path_rslash(filename), STR_LSTR("ttags.dot"));

  tagfp = sys_fopen(filename, "r");

  //FILE *dotfp = sys_fopen(dot_filename, "w");

  assert(tagfp != NULL && "file not found");

  buffer[BSIZE - 1] = '\0';

  //sys_fwrite_nstr(dotfp, STR_LSTR("digraph G {\n"));
  //sys_fwrite_nstr(dotfp, STR_LSTR("    rankdir=\"LR\";\n    node[shape = \"point\", width = 0.1, height = 0.1];\n    edge[arrowhead = \"none\", style = \"dashed\"]\n"));

  infos = get_tag_from_fp(tagfp);

  //sys_fwrite_nstr(dotfp, STR_LSTR("}\n"));

  //fclose(dotfp);
  sys_fclose(tagfp);

  sys_ptr_array_unref(infos);
}

static void test_fopen_leak(void) {
  char *filename = "E:/Codes/REPOSITORY/Demo/tags";
  FILE *tagfp;

  SYS_LEAK_IGNORE_BEGIN;
    tagfp = sys_fopen(filename, "r");
    SYS_LEAK_IGNORE_END;

  fclose(tagfp);
}

static void test_cond_basic(void) {
  GCond cond;

  g_cond_init(&cond);

}

gchar *test_strpcpy(gchar *dest, const char *src) {
  g_return_val_if_fail(dest != NULL, NULL);
  g_return_val_if_fail(src != NULL, NULL);

  gchar *d = dest;
  const gchar *s = src;


  do {
    *d++ = *s;
  } while (*s++ != '\0');

  return d - 1;
}

static void test_strpcpy_basic(void) {
  char buf[1024] = { 0 };
  char *p = buf;

  p = test_strpcpy(p, "abc");
  p = test_strpcpy(p, "def");
}

static void signal_fault_func(SysInt sgn) {
  SysInt size = 0;
  SysChar **stacks = sys_backtrace_string(&size);

  sys_array_foreach(SysChar *, s, stacks, size) {
    sys_error_N("%s", s);
#if SYS_OS_WIN32
    sys_free_N(s);
#endif
  }
  sys_free_N(stacks);

  signal(sgn, SIG_DFL);
#if SYS_OS_WIN32
  TerminateProcess(GetCurrentProcess(), sgn);
#else
  kill(getpid(), sgn);
#endif
}

static void test_backtrace2(void) {
}

static void test_backtrace(void) {
  SYS_LEAK_IGNORE_BEGIN;
  signal(SIGSEGV, signal_fault_func);
  SYS_LEAK_IGNORE_END;

  SysChar *s = NULL;
  *s = 'b';
}

static void test_array_each(void) {
  sys_array_foreach(const SysChar *, s, level_names, ARRAY_SIZE(level_names)) {
    sys_printf("%s, %d\n", s, i);
  }
}

static gpointer thread1_func(gpointer data) {
  while (1) {
  }

  return NULL;
}

static void test_cpu_thread(void) {
#define NTHREAD 3
  GThread *tharray[NTHREAD] = { 0 };
  GThread *th;

  for (int i = 0; i < NTHREAD; i++) {
    th = g_thread_new("main", thread1_func, NULL);

    tharray[i] = th;
  }

  getchar();
}

static void test_gobject_boxed(void) {
  GString *string = g_string_new("ok");
}

void test_glib_init(int argc, char *argv[]) {
  env_init();

  UNITY_BEGIN();
  {
    // RUN_TEST(test_array_each);
    // RUN_TEST(test_backtrace);
    // RUN_TEST(test_strpcpy_basic);
    // RUN_TEST(test_fopen_leak);
    // RUN_TEST(test_file_count);
    // RUN_TEST(test_module);
    // RUN_TEST(test_file_monitor);
    // RUN_TEST(test_set_jump);
    // RUN_TEST(test_binary_print);
    // RUN_TEST(test_dump_memory);
    RUN_TEST(test_cpu_thread);
  }
  UNITY_END();
}

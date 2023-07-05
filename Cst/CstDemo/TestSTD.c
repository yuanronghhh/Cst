#include <CstDemo/TestSTD.h>

void test_heart(void) {
  float a,x,y;
  SysChar ch;

  for(y = 1.5f; y > -1.5f; y -= 0.16f) {
    for(x = -1.5f; x < 1.5f; x += 0.10f) {
      a = x*x + y*y-1;
      ch = a*a*a-x*x*y*y*y <= 0.0f ? '@' : ' ';
      putchar(ch);
    }

    printf("/n");
  }
}

void test_get_dlopen(void) {
  SysChar *filepath = "D:/GreyHound/PRIVATE/Git/Cst/build/src/system/Debug/system.dll";
  typedef SysChar * (*sys_upper_func)(const SysChar *msg); 

  void* dll = sys_dlopen(filepath);
  if (!dll) {
    return;
  }

  sys_upper_func f = sys_dlsymbol(dll, "sys_str_upper");
  SysChar *cname = f("ok");

  printf("%s\n", cname);

  sys_dlclose(dll);
  getchar();
}

static SysInt64 fibt(int n, SysInt64 ac1, SysInt64 ac2) {
  if (n < 2) { return ac2; }
  return fibt(n - 1, ac2, ac2 + ac1);
}

static void test_gprof2(void) {
  // int rs = fib(30, 0);
  // printf("[prof2] %d\n", rs);
}

static void test_gprof1(void) {
  SysInt64 rs = 0;

  for (int i = 1; i < 93; i++) {
    rs = fibt(i, 0, 1);
    printf("[%d] %lld\n", i, rs);
  }

  getchar();
}

static void test_alignup(void) {
  SysInt base = 8;
  SysInt v1 = 14;
  SysInt v2 = 0;

  char *buff = sys_bin_new(v2, sizeof(v2));

  v2 = (base - 1);
  buff = sys_bin_tostr(buff, v2, sizeof(v2));
  printf("[(base - 1)]\t\t%d\t%s\n", v2, buff);

  sys_printf("[(base - 1)]\t\t%d\t%s\n", v2, buff);

  v2 = ~(base - 1);
  buff = sys_bin_tostr(buff, v2, sizeof(v2));
  sys_printf("[~(base - 1)]\t\t%d\t%s\n", v2, buff);

  v2 = v1 + (base - 1);
  buff = sys_bin_tostr(buff, v2, sizeof(v2));
  sys_printf("[%d + (base - 1)]\t%d\t%s\n", v1, v2, buff);

  v2 = sys_align_up(v1, base);
  buff = sys_bin_tostr(buff, v2, sizeof(v2));
  sys_printf("[v1]\t\t\t%d\t%s\n", v2, buff);

  sys_free_N(buff);
}

void test_basic(void) {
  double x = M_PI / 2;
  double y = sin(x);
  printf("[x,y] %lf,%lf", x, y);
}

static void test_line_macro(void) {
}

static void test_lex_comple(void) {
  const SysChar* cmd[] = { "clang", "--help" };

  SysSubProcessOption option = { 0 };
  SysError *err = NULL;
  SysChar *std0, *std1, *std2;

  std0 = std1 = std2 = NULL;

  option.cmd = cmd;
  option.std0type = SYS_IO_STDOUT;
  option.std1type = SYS_IO_STDOUT;
  option.std2type = SYS_IO_STDOUT;
  option.cmdlen = ARRAY_SIZE(cmd);

  SysSubProcess *sub = sys_subprocess_new_option(&option);
  sys_subprocess_terminate(sub);
}

static void test_strjoin(void) {
  const SysChar* cmd[] = { "clang", "--help", "help2" };
  SysSize len = 0;
  SysChar *s;

  s = sys_strjoin(" ", cmd, ARRAY_SIZE(cmd), &len);
  sys_printf("%s\n", s);
  sys_free_N(s);
}

static void test_queue_leak(void) {
  SysQueue *queue = sys_queue_new();

  sys_queue_push_head(queue, sys_strdup("ok"));
  sys_queue_push_head(queue, sys_strdup("ok2"));

  while (!sys_queue_is_empty(queue)) {
    SysChar *node = sys_queue_pop_head(queue);

    sys_free_N(node);
  }

  sys_free_N(queue);
}

static void mem_alloc() {
  int gb = 10;
  size_t sz = 1024 * 1024 * 1024;

  for (int i = 0; i < gb; i++) {
    void* a = malloc(sz);
    memset(a, 0, sz);
  }
}

void require_get_admin(void) {

  SHELLEXECUTEINFO einfo = { 0 };
  einfo.cbSize = sizeof(SHELLEXECUTEINFO);
  einfo.fMask = SEE_MASK_NOCLOSEPROCESS;
  einfo.hwnd = NULL;
  einfo.lpVerb = "runas";
  einfo.lpFile = sys_exe_path();
  einfo.lpParameters = NULL;
  einfo.nShow = SW_SHOW;
  einfo.hInstApp = NULL;

  SysBool rs = ShellExecuteEx(&einfo);
  CloseHandle(einfo.hProcess);
}

static void ask_get_admin(int argc, char *argv[]) {
  HWND hd = GetConsoleWindow();
  CloseHandle(hd);

  if (argc > 1) {
    require_get_admin();

    printf("%s", "permission ok.\n");
  }
  else {
    printf("%s", "task work ok.\n");
    printf("%s", "task work done.\n");
    getchar();
  }
}

void test_std_init(SysInt argc, SysChar *argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  UNITY_BEGIN();
  {

    RUN_TEST(test_queue_leak);
    // RUN_TEST(test_strjoin);
    // RUN_TEST(test_lex_comple);
    //RUN_TEST(test_line_macro);
    // RUN_TEST(test_lex_comple);
    // RUN_TEST(test_gprof2);
    // RUN_TEST(test_alignup);
    // RUN_TEST(test_get_dlopen);
    // RUN_TEST(test_basic);
  }
  UNITY_END();
}

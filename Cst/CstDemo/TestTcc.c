#define CONFIG_TCC_BCHECK
#include <TestTcc.h>
#include "SysOs.h"
#include "SysString.h"
#include "unity.h"

#define CONFIG_LIBDIR "D:/GreyHound/PRIVATE/Git/lib/win64_vc14"
#define CONFIG_TCCDIR CONFIG_LIBDIR"/tcc"

int add(int a, int b)
{
    return a + b;
}

const char hello[] = "Hello World!";

char my_program[] =
"#include <glib.h>\n" /* include the "Simple libc header for TCC" */
"#include <tcclib.h>\n" /* include the "Simple libc header for TCC" */
"extern int add(int a, int b);\n"
"#ifdef _WIN32\n" /* dynamically linked data needs 'dllimport' */
" __attribute__((dllimport))\n"
"#endif\n"
"extern const char hello[];\n"
"int fib(int n)\n"
"{\n"
"    if (n <= 2)\n"
"        return 1;\n"
"    else\n"
"        return fib(n-1) + fib(n-2);\n"
"}\n"
"\n"
"int foo(int n)\n"
"{\n"
"    int a = g_random_int ();\n"
"    g_print(\"gprint called\\n\");\n"
"    printf(\"%s\\n\", hello);\n"
"    printf(\"fib(%d) = %d\\n\", n, fib(n));\n"
"    printf(\"add(%d, %d) = %d\\n\", n, 2 * n, add(n, 2 * n));\n"
"    return 0;\n"
"}\n";

static char *read_file(char *filename) {
  char *content = NULL;
  gsize len = 0;
  GError *error = NULL;

  if (!g_file_get_contents(filename, &content, &len, &error)) {
    return NULL;
  }

  return content;
}

static void tcc_basic(void) {
  TCCState *s;
  int (*func)(int);

  char *scripts[] = {
    "D:/GreyHound/PRIVATE/Git/Cst/src/cst-demo/Assets/tcc-test.c",
    "D:/GreyHound/PRIVATE/Git/Cst/src/cst-demo/Assets/tcc-test.h"
  };

  s = tcc_new();
  if (!s) {
    fprintf(stderr, "Could not create tcc state\n");
    exit(1);
  }

  // tcc_add_include_path(s, "/usr/include/glib-2.0");
  // tcc_add_include_path(s, "/usr/lib/x86_64-linux-gnu/glib-2.0/include");
  // tcc_add_include_path(s, "/usr/include/x86_64-linux-gnu");
  // tcc_add_library_path(s, "/usr/lib/x86_64-linux-gnu");

  tcc_add_include_path(s, CONFIG_LIBDIR"/glib/include");
  tcc_add_library_path(s, CONFIG_TCCDIR"/glib/lib");
  tcc_add_library_path(s, CONFIG_TCCDIR"/glib/bin");

  tcc_add_include_path(s, CONFIG_TCCDIR"/include");
  tcc_add_library_path(s, CONFIG_TCCDIR"/lib");
  tcc_add_library_path(s, CONFIG_TCCDIR"/libtcc");

  /* MUST BE CALLED before any compilation */
  tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

  for (int i = 0; i < ARRAY_SIZE(scripts); i++) {
    char *sf = scripts[i];

    if (tcc_add_file(s, sf) == -1) {
      return;
    }
  }

  /* as a test, we add symbols that the compiled program can use.
     You may also open a dll with tcc_add_dll() and use symbols from that */
  tcc_add_symbol(s, "add", add);
  tcc_add_symbol(s, "hello", hello);

  /* relocate the code */
  if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
    return;

  /* get entry symbol */
  func = tcc_get_symbol(s, "foo");
  if (!func)
    return;

  /* run the code */
  func(32);

  /* delete the state */
  tcc_delete(s);
}

void test_tcc_init() {
  UNITY_BEGIN();
  {
    RUN_TEST(tcc_basic);
  }
  UNITY_END();
}

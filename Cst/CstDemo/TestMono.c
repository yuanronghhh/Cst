#include <TestMono.h>

#define MONO_HOME PROJECT_DIR"/../../../lib/win64_vc14/mono"
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

MonoDomain *domain;

static void test_sgen_basic(void) {
}

static void test_mcs_basic() {
}

static void test_mono_func_bind(void) {
  char *managed_path = BINARY_DIR"/Cst/MonoDemo/Debug/MonoDemo.exe";

  MonoDomain *domain;
  MonoAssembly* assembly;
  MonoImage *image;
  
  char *domain_name = "MonoDemo";
  char* argv[] = { managed_path, "abc" };

  mono_set_dirs(MONO_HOME"/lib", MONO_HOME"/etc");
  mono_set_use_llvm(true);

  domain = mono_jit_init(domain_name);

  assembly = mono_domain_assembly_open(domain, managed_path);
  if (assembly == NULL) {
    return;
  }

  image = mono_assembly_get_image(assembly);
  if (image == NULL) {
    return;
  }

  MonoClass *main_cls = mono_class_from_name(image, "CstCore.Front", "CstComponent");
  // method = mono_class_get_method_from_name(main_cls, "Test2", 0);
  MonoMethod *method = mono_class_get_method_from_name(main_cls, "Test1", 0);
  // MonoObject *my_class_instance = mono_object_new(domain, component_class);

  mono_jit_cleanup(domain);
}

static void mono_basic(void) {
  // char* managed_path = "D:/GreyHound/PRIVATE/Git/Cst/build/src/CstDemo/Debug/main.exe";
  char *managed_path = "E:/Codes/REPOSITORY/TableDataLib/CryptoTool/bin/Debug/CryptoTool.exe";

  MonoDomain *domain;
  char *domain_name = "MonoDemo";
  int retval;
  MonoAssembly* assembly;
  char* argv[] = { managed_path, "abc" };

  sys_console_setup();
  mono_set_dirs(MONO_HOME"/lib", MONO_HOME"/etc");
  mono_set_use_llvm(true);

  domain = mono_jit_init(domain_name);

  assembly = mono_domain_assembly_open(domain, managed_path);
  retval = mono_jit_exec(domain, assembly, ARRAY_SIZE(argv), argv);

  mono_jit_cleanup(domain);
}

void test_mono_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(mono_basic);
    RUN_TEST(test_mono_func_bind);
    // RUN_TEST(test_sgen_basic);
  }
  UNITY_END();
}

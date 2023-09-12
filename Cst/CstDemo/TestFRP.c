#include <CstDemo/TestFRP.h>

typedef struct _SOption SOption;

struct _SOption {
  int local_port;
  SysChar *remote_host;
  int remote_port;
};

static void object_ref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, SOCKET_TYPE_CONNECTION)) {
    return;
  }

  sys_debug_N("%p\t%d", o, ref_count);
  sys_assert(ref_count > 0);
}

static void object_unref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, SOCKET_TYPE_CONNECTION)) {
    return;
  }

  sys_debug_N("%p\t%d", o, ref_count);
  sys_assert(ref_count > 0);
}

static SysBool parse_args(SOption *option, SysInt argc, const SysChar* argv[]) {
  SysSArg ao;
  int i;

  sys_arg_init(&ao, argc, argv);

  i = sys_arg_index(&ao, "rport", false);
  if (i == -1) { return false; }
  option->remote_port = atoi(ao.argv[i]);

  i = sys_arg_index(&ao, "rhost", false);
  if (i == -1) { return false; }
  option->remote_host = ao.argv[i];

  i = sys_arg_index(&ao, "lport", false);
  if (i == -1) { return false; }
  option->local_port = atoi(ao.argv[i]);

  return true;
}

static void print_help() {
  sys_printf("usage:\n");
  sys_printf("    FRPServer --rport <remote-port> --rhost <remote-host> --lport <local-port>\n");
}

void test_frp_init(SysInt argc, const SysChar *argv[]) {
  SOption options = { 0 };

  sys_object_set_ref_hook(object_ref_debug);
  sys_object_set_unref_hook(object_unref_debug);

  if (!parse_args(&options, argc, argv)) {
    print_help();
    return;
  }
    
  FRPServer* s = frp_server_new_I(options.local_port, options.remote_host, options.remote_port);

  frp_server_run(s);
  sys_object_unref(s);
}

#include <CstDemo/TestFrp.h>

typedef struct _SOption SOption;

struct _SOption {
  int local_port;
  SysChar *remote_host;
  int remote_port;
};

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
  sys_printf("    FRPServer --remote-port <remote-port> --remote-host <remote-host> --local-port <local-port>\n");
}

void test_frp_init(SysInt argc, const SysChar *argv[]) {
  SOption options = { 0 };

  if (!parse_args(&options, argc, argv)) {
    print_help();
    return;
  }

  FRPServer* s = frp_server_new_I(
    options.local_port,
    options.remote_host,
    options.remote_port);

  frp_server_run(s);
  sys_object_unref(s);
}

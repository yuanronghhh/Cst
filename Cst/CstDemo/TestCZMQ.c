#include <TestCZMQ.h>

static void czmq_basic(void) {
  void *ctx = zmq_ctx_new();
  void *res = zmq_socket(ctx, ZMQ_REP);
  int rc    = zmq_bind(res, "tcp://*:5555");

  assert(rc == 0);

  while (1) {
    char buffer[10];

    zmq_recv(res, buffer, 10, 0);

    g_debug("Received Hello");

    Sleep(1000);
    zmq_send(res, STR_LSTR("world"), 0);
  }
  return;
}

void test_czmq_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(czmq_basic);
  }
  UNITY_END();
}

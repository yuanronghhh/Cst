#include "CZMQClient.h"

static void czmq_basic(void) {
  g_debug("Connecting hello server.");
  void *ctx = zmq_ctx_new();
  void *req = zmq_socket(ctx, ZMQ_REQ);
  zmq_connect(req, "tcp://localhost:5555");

  int n;
  for (n = 0; n < 10; n++) {
    char buffer[10];

    g_debug("seding hello");

    zmq_send(req, STR_LSTR("hello"), 0);
    zmq_recv(req, buffer, 10, 0);

    g_debug("Received world %d", n);
  }

  zmq_close(req);
  zmq_ctx_destroy(ctx);
}

void test_czmq_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(czmq_basic);
  }
  UNITY_END();
}

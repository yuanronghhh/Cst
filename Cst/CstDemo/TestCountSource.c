#include "libuv.h"


void test_count_source_line(void) {
  gchar *fname;
  gchar line[4 * 1024];
  gsize tSize = 64 * 1024;
  int lNum = 0;
  gsize nbytes = 0;
  FILE *fp;
  int fd;
  long end, start;
  char *tp;
  char *tend;
  struct statfs fsInfo;

  // gchar *tbuf;

  fname = "/home/greyhound/Desktop/test/out.spec";
  fp = fopen(fname, "rb");
  // fd = fileno(fp);
  // tbuf = g_new0(gchar, tSize);

  // if(fstatfs(fd, &fsInfo) != -1) {
  //   tSize = fsInfo.f_bsize;
  // }
  // tbuf = g_new0(gchar, tSize);

#if 0
  start = g_get_monotonic_time();
  while((nbytes = fread(line, 1, sizeof(line), fp)) > 0) {
    tp = line;
    tend = tp + nbytes;

    while(tp != tend) {
      lNum += *tp == '\n' ? 1 : 0;
      tp++;
    }
  }

  end = g_get_monotonic_time();
  printf("[LineCount 1]%ld\t%d\n", (end - start), lNum);
#endif

#if 1
  start = g_get_monotonic_time();
  while(fgets(line, sizeof(line), fp) != NULL) {
    lNum++;
  }
  end = g_get_monotonic_time();
  printf("[LineCount 2]%ld\t%d\n", (end - start), lNum);
#endif

  // g_free(tbuf);
}

static uv_fs_t open_req1;
static uv_fs_t read_req1;
static uv_loop_t *loop = NULL;
static uv_buf_t iov;
static char buffer[4096];
static int lCount = 0;

static int count_lines(uv_buf_t *buf) {
  char *p = buf->base;
  int i;
  int lines = 0;

  for(i = 0; i < buf->len; i++) {
    if (*(p + i) == '\n') {
      lines++;
    }
  }

  return lines;
}

void on_read(uv_fs_t *req) {
  // printf("[on_read called]\n");
  if (req->result < 0) {
    uv_fs_req_cleanup(req);
    return;
  }

  if(req->result == 0) {
    uv_fs_t close_req;

    printf("[LineCount]%d\n", lCount);
    uv_fs_close(loop, &close_req, open_req1.result, NULL);
  }

  iov.len = req->result;
  lCount += count_lines(&iov);
  uv_fs_read(loop, &read_req1, open_req1.result, &iov, 1, -1, on_read);
}

void on_open(uv_fs_t *req) {
  // printf("[on_open called]\n");

  if(req->result < 0) {
    fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    goto fail;
  }

  iov = uv_buf_init(buffer, sizeof(buffer));
  uv_fs_read(loop, &read_req1, req->result, &iov, 1, -1, on_read);

fail:
  uv_fs_req_cleanup(req);
}

void test_libuv_count_line(void) {
  loop = uv_default_loop();
  gchar *fname;
  long s, e;

  fname = "/home/greyhound/Desktop/test/out.spec";

  s = g_get_monotonic_time();

  uv_fs_open(loop, &open_req1, fname, O_RDONLY, -1, on_open);
  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);

  e = g_get_monotonic_time();
  printf("%ld\n", e - s);
}

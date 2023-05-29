#include "mpdecimal.h"

static void test_mpdec(void) {
  mpd_t *result = NULL;
  mpd_t *a = NULL, *b = NULL;
  mpd_context_t ctx;
  mpd_ssize_t prec = 8;
  uint32_t status = 0;

  mpd_init(&ctx, prec);

  a = mpd_qnew();
  b = mpd_qnew();
  result = mpd_qnew();

  mpd_qset_string(a, "0.1", &ctx, &status);
  mpd_qset_string(b, "0.2", &ctx, &status);

  mpd_qadd(result, a, b, &ctx, &status);
  mpd_addstatus_raise(&ctx, status);

  mpd_print(result);
  mpd_del(a);
  mpd_del(b);
  mpd_del(result);
}


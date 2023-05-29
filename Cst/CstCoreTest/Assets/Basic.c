#line 1 /home/greyhound/Git/Cst/src/CstCoreTest/Assets/Basic.cst
#include <stdio.h>
#define MATH_POWER(t) (t < 0.5 ? Math.pow(t * 2, 6) / 2 : 1 - Math.pow(t * -2 + 2, 6) / 2)

#line 5 /home/greyhound/Git/Cst/src/CstCoreTest/Assets/Basic.c
static void basic_style_func_1(CstWidget *w, CstWidget *pw) {
  SysChar *s_id = "white-btn";
  SysChar *base_ids[] = { "btn", "click-btn" };

  CstStyles *sstyles = cst_styles_parse();
  sys_return_if_fail(sstyles != NULL);

  for(int i = 0; i < ARRAY_SIZE(base_style); i++) {
    SysChar *bs = base_style[i];

    CstStyles *pstyles = sys_hash_table_lookup(global_style_ht, bs);
    if (pstyles != NULL) {
      for(int j = 0; j < pstyles->len; j++) {
        CstStyle *pstyle = pstyles[j];

        cst_styles_add(sstyles, pstyle);
      }
    }

    sys_array_add(global_style_ht, s_id, sstyles);
  }
}

#line 81 /home/greyhound/Git/Cst/src/CstCoreTest/Assets/Basic.cst
static void switch_image(CButton *btn) {
  printf("%s", "ok1");
}

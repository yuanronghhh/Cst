#include <CstCore/Driver/FlexLayout/CstFlexAlgorithm.h>
#include <CstCore/Driver/FlexLayout/CstFlexContext.h>
#include <CstCore/Driver/FlexLayout/CstIFlexItem.h>
#include <CstCore/Driver/FlexLayout/CstFlexLine.h>
#include <CstCore/Driver/FlexLayout/CstFlexContext.h>
#include <CstCore/Driver/CstRenderNode.h>

SYS_DEFINE_TYPE(CstFlexAlgorithm, cst_flex_algorithm, CST_TYPE_ALGORITHM);

static void layout_horizonal(CstAlgorithm* self,
    CstIFlexItem* rnode,
    CstFlexContext* ctx) {

  SysHArray* nodes = cst_i_flex_item_get_nodes(rnode);
  CstIFlexItem *item;

  for(SysUInt i = 0; i < nodes->len; i++) {
    item = nodes->pdata[i];

    sys_debug_N("%s", cst_i_flex_item_get_name(item));
  }

  sys_harray_free(nodes, true);
}

static void cst_flex_algorithm_measure_i(CstAlgorithm* self,
    CstRenderNode* rnode,
    CstRenderContext* ctx) {

}

static void cst_flex_algorithm_layout_i(CstAlgorithm* self,
    CstRenderNode* rnode,
    CstRenderContext* ctx) {
  CstFlexContext* fctx = CST_FLEX_CONTEXT(ctx);

  CstIFlexItem *item = CST_I_FLEX_ITEM(rnode);
  FLEX_DIRECTION_ENUM dr = cst_i_flex_item_get_direction(item);

  switch (dr) {
    case FLEX_DIRECTION_ROW:
      layout_horizonal(self, item, fctx);
      break;
    case FLEX_DIRECTION_ROW_REVERSE:
      break;
    case FLEX_DIRECTION_COLUMN:
      break;
    case FLEX_DIRECTION_COLUMN_REVERSE:
      break;
    default:
      break;
  }
}

/* object api */
static void cst_flex_algorithm_construct(CstAlgorithm *self) {
}

CstAlgorithm* cst_flex_algorithm_new(void) {
  return sys_object_new(CST_TYPE_FLEX_ALGORITHM, NULL);
}

CstAlgorithm* cst_flex_algorithm_new_I(void) {
  CstAlgorithm *o = cst_flex_algorithm_new();

  cst_flex_algorithm_construct(o);

  return o;
}

static void cst_flex_algorithm_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_flex_algorithm_parent_class)->dispose(o);
}

static void cst_flex_algorithm_class_init(CstFlexAlgorithmClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstAlgorithmClass* acls = CST_ALGORITHM_CLASS(cls);

  ocls->dispose = cst_flex_algorithm_dispose;
  acls->layout = cst_flex_algorithm_layout_i;
  acls->measure = cst_flex_algorithm_measure_i;
}

void cst_flex_algorithm_init(CstFlexAlgorithm* self) {
}


#include <CstCore/Driver/Flex/CstFlexAlgorithm.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/Flex/CstFlexLine.h>
#include <CstCore/Driver/Flex/CstFlexContext.h>

SYS_DEFINE_TYPE(CstFlexAlgorithm, cst_flex_algorithm, CST_TYPE_ALGORITHM);


static void layout_horizonal(CstAlgorithm* self,
    CstFlexItem* rnode,
    CstFlexContext* ctx) {

  SysHArray* lines = cst_flex_context_get_lines(ctx);
  CstFlexItem *item;

  for(SysUInt i = 0; i < lines->len; i++) {
    CstFlexLine *line = lines->pdata[i];

    for (SysUInt j = 0; j < line->items.len; j++) {
      item = line->items.pdata[j];

      sys_debug_N("%s", cst_flex_item_get_name(item));
    }
  }
}

static void cst_flex_algorithm_layout_i(CstAlgorithm* self,
    CstRenderNode* rnode,
    CstLayout* layout) {

  CstFlexContext *ctx = cst_flex_context_new_I();
  cst_flex_context_add_nodes(ctx, rnode);

  CstFlexItem *item = CST_FLEX_ITEM(rnode);
  CST_DIRECTION_ENUM dr = cst_flex_item_get_direction(item);

  switch (dr)
  {
    case CST_DIRECTION_HORIZONTAL:
      layout_horizonal(self, item, ctx);
      break;
    case CST_DIRECTION_HORIZONTAL_REVERSE:
      break;
    case CST_DIRECTION_VERTICAL:
      break;
    case CST_DIRECTION_VERTICAL_REVERSE:
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
  CstFlexAlgorithm *self = CST_FLEX_ALGORITHM(o);

  SYS_OBJECT_CLASS(cst_flex_algorithm_parent_class)->dispose(o);
}

static void cst_flex_algorithm_class_init(CstFlexAlgorithmClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstAlgorithmClass* acls = CST_ALGORITHM_CLASS(cls);

  ocls->dispose = cst_flex_algorithm_dispose;
  acls->layout = cst_flex_algorithm_layout_i;
}

void cst_flex_algorithm_init(CstFlexAlgorithm* self) {
}


#include <TestRBTree.h>

/**
 * matrix : 3x3
 *         1 1 1 1
 *         1 1 1 1
 *         1 1 1 1
 *         1 1 1 1
 */
typedef struct _SysPoint SysPoint;
typedef struct _PointData PointData;

typedef enum _SysDataType {
  P_Pointer,
  P_Array,
} SysRBDataType;

struct _PointData {
  SysRBDataType type;

  union rbv {
    SysPointer v_pointer;
    SysPtrArray *v_array;
  } v;
};

struct _SysPoint {
  SysInt x;
  SysInt y;
};

SysPointer rb_data_bubbling(SysRBData data, SysPointer user_data) {
  sys_return_val_if_fail(data != NULL, NULL);

  PointData *od = data;
  switch (od->type)
  {
  case P_Pointer:
    return od->v.v_pointer;
    break;
  case P_Array:
    break;
  default:
    break;
  }

  sys_free_N(od);


  return NULL;
}

static void node_data_free(SysRBData data, SysPointer user_data) {
  PointData *od = data;
  switch (od->type)
  {
    case P_Pointer:
      break;
    case P_Array:
      sys_ptr_array_unref(od->v.v_array);
      break;
    default:
      break;
  }

  sys_free_N(od);
}

void node_set_data(SysRBNode *node, SysPointer ndata, SysPointer user_data) {
  PointData *od = sys_rb_node_get_data(node);

  if (!od) {
	  od = sys_new0_N(PointData, 1);
	  od->type = P_Pointer;
  }

  switch (od->type)
  {
    case P_Pointer:
      if (od->v.v_pointer == NULL) {
        od->v.v_pointer = ndata;

      } else {
        od->type = P_Array;

        SysPtrArray *narray = sys_ptr_array_new();

        sys_ptr_array_add(narray, od->v.v_pointer);
        sys_ptr_array_add(narray, ndata);
        od->v.v_array = narray;
      }
      break;
    case P_Array:
      sys_ptr_array_add(od->v.v_array, ndata);
      break;
    default:
      break;
  }

  sys_rb_node_set_data(node, od);
}

void rb_print_node_data(SysRBTree *tree, SysRBNode *node) {
  sys_return_if_fail(node != NULL);

  PointData *pdata = sys_rb_node_get_data(node);
  SysPoint *p;
  SysPtrArray *parray;
  
  p = pdata->v.v_pointer;
  parray = pdata->v.v_array;

  sys_assert(p != NULL);
  if (pdata->type == P_Pointer) {
    sys_debug_N("(%d, %d)", p->x, p->y);
  } else {
    for (SysUInt i = 0; i < parray->len; i++) {
      p = parray->pdata[i];
      sys_debug_N("(%d, %d)", p->x, p->y);
    }
  }
}

static SysDouble calc_distance(SysPoint *p1, SysPoint *p2) {
  return sqrt((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}

static void test_rbtree_basic(void) {
  SysPoint points[] = { 
    { 0, 0 },
    { 1, 2 },
    { 5, 6 },
    { 7, 8 },
    { 5, 6 },
    { 3, 4 },
  };

  SysRBTree *tree = sys_rb_tree_new_full(NULL, node_data_free, node_set_data, NULL);
  SysDouble dis;
  SysRBNode *node;

  for (int i = 0; i < ARRAY_SIZE(points); i++) {
    dis = calc_distance(&points[0], &points[i]);
    sys_rb_tree_insert(tree, dis, &points[i]);
  }

  sys_rb_node_foreach(tree, rb_print_node_data);

  dis = calc_distance(&points[0], &points[2]);
  node = sys_rb_tree_lookup(tree, dis);
  if (node != NULL) {
    sys_debug_N("%s", "[sys_rb_tree_lookup] ok.");
    // rb_print_node_data(tree, node);
  }

  sys_rb_tree_remove(tree, dis);
  sys_debug_N("%s", "[sys_rb_tree_remove] ok.");
  sys_rb_node_foreach(tree, rb_print_node_data);

  sys_rb_tree_unref(tree);
}

void test_rbtree_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(llvm_gen_call_graph);
    RUN_TEST(test_rbtree_basic);
  }
  UNITY_END();
}

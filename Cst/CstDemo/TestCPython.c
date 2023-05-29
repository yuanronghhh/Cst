#include <TestCPython.h>

#define BYTE            char
typedef BYTE *bitset;

#define testbit(ss, ibit) (((ss)[BIT2BYTE(ibit)] & BIT2MASK(ibit)) != 0)

#define BITSPERBYTE     (8*sizeof(BYTE))
#define BIT2BYTE(ibit)  ((ibit) / BITSPERBYTE)
#define BIT2SHIFT(ibit) ((ibit) % BITSPERBYTE)
#define BIT2MASK(ibit)  (1 << BIT2SHIFT(ibit))


/* Node access functions */
#define NCH(n)          ((n)->n_nchildren)

#define CHILD(n, i)     (&(n)->n_child[i])
#define RCHILD(n, i)    (CHILD(n, NCH(n) + i))
#define TYPE(n)         ((n)->n_type)
#define STR(n)          ((n)->n_str)
#define LINENO(n)       ((n)->n_lineno)

/* Assert that the type of a node is what we expect */
#define REQ(n, type) assert(TYPE(n) == (type))

/* E:\Git\cpython\Include\node.h */


#undef TILDE   /* Prevent clash of our definition with system macro. Ex AIX, ioctl.h */

#define ENDMARKER       0
#define NAME            1
#define NUMBER          2
#define STRING          3
#define NEWLINE         4
#define INDENT          5
#define DEDENT          6
#define LPAR            7
#define RPAR            8
#define LSQB            9
#define RSQB            10
#define COLON           11
#define COMMA           12
#define SEMI            13
#define PLUS            14
#define MINUS           15
#define STAR            16
#define SLASH           17
#define VBAR            18
#define AMPER           19
#define LESS            20
#define GREATER         21
#define EQUAL           22
#define DOT             23
#define PERCENT         24
#define LBRACE          25
#define RBRACE          26
#define EQEQUAL         27
#define NOTEQUAL        28
#define LESSEQUAL       29
#define GREATEREQUAL    30
#define TILDE           31
#define CIRCUMFLEX      32
#define LEFTSHIFT       33
#define RIGHTSHIFT      34
#define DOUBLESTAR      35
#define PLUSEQUAL       36
#define MINEQUAL        37
#define STAREQUAL       38
#define SLASHEQUAL      39
#define PERCENTEQUAL    40
#define AMPEREQUAL      41
#define VBAREQUAL       42
#define CIRCUMFLEXEQUAL 43
#define LEFTSHIFTEQUAL  44
#define RIGHTSHIFTEQUAL 45
#define DOUBLESTAREQUAL 46
#define DOUBLESLASH     47
#define DOUBLESLASHEQUAL 48
#define AT              49
#define ATEQUAL         50
#define RARROW          51
#define ELLIPSIS        52
#define COLONEQUAL      53
#define OP              54
#define AWAIT           55
#define ASYNC           56
#define TYPE_IGNORE     57
#define TYPE_COMMENT    58
#define ERRORTOKEN      59
#define N_TOKENS        63
#define NT_OFFSET       256

/* Special definitions for cooperation with parser */

#define ISTERMINAL(x)           ((x) < NT_OFFSET)
#define ISNONTERMINAL(x)        ((x) >= NT_OFFSET)
#define ISEOF(x)                ((x) == ENDMARKER)

const char * const _PyParser_TokenNames[] = {
    "ENDMARKER",
    "NAME",
    "NUMBER",
    "STRING",
    "NEWLINE",
    "INDENT",
    "DEDENT",
    "LPAR",
    "RPAR",
    "LSQB",
    "RSQB",
    "COLON",
    "COMMA",
    "SEMI",
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
    "VBAR",
    "AMPER",
    "LESS",
    "GREATER",
    "EQUAL",
    "DOT",
    "PERCENT",
    "LBRACE",
    "RBRACE",
    "EQEQUAL",
    "NOTEQUAL",
    "LESSEQUAL",
    "GREATEREQUAL",
    "TILDE",
    "CIRCUMFLEX",
    "LEFTSHIFT",
    "RIGHTSHIFT",
    "DOUBLESTAR",
    "PLUSEQUAL",
    "MINEQUAL",
    "STAREQUAL",
    "SLASHEQUAL",
    "PERCENTEQUAL",
    "AMPEREQUAL",
    "VBAREQUAL",
    "CIRCUMFLEXEQUAL",
    "LEFTSHIFTEQUAL",
    "RIGHTSHIFTEQUAL",
    "DOUBLESTAREQUAL",
    "DOUBLESLASH",
    "DOUBLESLASHEQUAL",
    "AT",
    "ATEQUAL",
    "RARROW",
    "ELLIPSIS",
    "COLONEQUAL",
    "OP",
    "AWAIT",
    "ASYNC",
    "TYPE_IGNORE",
    "TYPE_COMMENT",
    "<ERRORTOKEN>",
    "<COMMENT>",
    "<NL>",
    "<ENCODING>",
    "<N_TOKENS>",
};

/* E:\Git\cpython\Include\token.h */

#define single_input 256
#define file_input 257
#define eval_input 258
#define decorator 259
#define decorators 260
#define decorated 261
#define async_funcdef 262
#define funcdef 263
#define parameters 264
#define typedargslist 265
#define tfpdef 266
#define varargslist 267
#define vfpdef 268
#define stmt 269
#define simple_stmt 270
#define small_stmt 271
#define expr_stmt 272
#define annassign 273
#define testlist_star_expr 274
#define augassign 275
#define del_stmt 276
#define pass_stmt 277
#define flow_stmt 278
#define break_stmt 279
#define continue_stmt 280
#define return_stmt 281
#define yield_stmt 282
#define raise_stmt 283
#define import_stmt 284
#define import_name 285
#define import_from 286
#define import_as_name 287
#define dotted_as_name 288
#define import_as_names 289
#define dotted_as_names 290
#define dotted_name 291
#define global_stmt 292
#define nonlocal_stmt 293
#define assert_stmt 294
#define compound_stmt 295
#define async_stmt 296
#define if_stmt 297
#define while_stmt 298
#define for_stmt 299
#define try_stmt 300
#define with_stmt 301
#define with_item 302
#define except_clause 303
#define suite 304
#define namedexpr_test 305
#define test 306
#define test_nocond 307
#define lambdef 308
#define lambdef_nocond 309
#define or_test 310
#define and_test 311
#define not_test 312
#define comparison 313
#define comp_op 314
#define star_expr 315
#define expr 316
#define xor_expr 317
#define and_expr 318
#define shift_expr 319
#define arith_expr 320
#define term 321
#define factor 322
#define power 323
#define atom_expr 324
#define atom 325
#define testlist_comp 326
#define trailer 327
#define subscriptlist 328
#define subscript 329
#define sliceop 330
#define exprlist 331
#define testlist 332
#define dictorsetmaker 333
#define classdef 334
#define arglist 335
#define argument 336
#define comp_iter 337
#define sync_comp_for 338
#define comp_for 339
#define comp_if 340
#define encoding_decl 341
#define yield_expr 342
#define yield_arg 343
#define func_body_suite 344
#define func_type_input 345
#define func_type 346
#define typelist 347

const static char* kwords[] = {
"single_input",
"file_input",
"eval_input",
"decorator",
"decorators",
"decorated",
"async_funcdef",
"funcdef",
"parameters",
"typedargslist",
"tfpdef",
"varargslist",
"vfpdef",
"stmt",
"simple_stmt",
"small_stmt",
"expr_stmt",
"annassign",
"testlist_star_expr",
"augassign",
"del_stmt",
"pass_stmt",
"flow_stmt",
"break_stmt",
"continue_stmt",
"return_stmt",
"yield_stmt",
"raise_stmt",
"import_stmt",
"import_name",
"import_from",
"import_as_name",
"dotted_as_name",
"import_as_names",
"dotted_as_names",
"dotted_name",
"global_stmt",
"nonlocal_stmt",
"assert_stmt",
"compound_stmt",
"async_stmt",
"if_stmt",
"while_stmt",
"for_stmt",
"try_stmt",
"with_stmt",
"with_item",
"except_clause",
"suite",
"namedexpr_test",
"test",
"test_nocond",
"lambdef",
"lambdef_nocond",
"or_test",
"and_test",
"not_test",
"comparison",
"comp_op",
"star_expr",
"expr",
"xor_expr",
"and_expr",
"shift_expr",
"arith_expr",
"term",
"factor",
"power",
"atom_expr",
"atom",
"testlist_comp",
"trailer",
"subscriptlist",
"subscript",
"sliceop",
"exprlist",
"testlist",
"dictorsetmaker",
"classdef",
"arglist",
"argument",
"comp_iter",
"sync_comp_for",
"comp_for",
"comp_if",
"encoding_decl",
"yield_expr",
"yield_arg",
"func_body_suite",
"func_type_input",
"func_type",
"typelist"
};

typedef struct {
  int          lb_type;
  const char  *lb_str;
} label;

#define EMPTY 0         /* Label number 0 is by definition the empty label */

/* A list of labels */

typedef struct {
  int          ll_nlabels;
  const label *ll_label;
} labellist;

/* An arc from one state to another */

typedef struct {
  short       a_lbl;          /* Label of this arc */
  short       a_arrow;        /* State where this arc goes to */
} arc;

/* A state in a DFA */

typedef struct {
  int          s_narcs;
  const arc   *s_arc;         /* Array of arcs */

  /* Optional accelerators */
  int          s_lower;       /* Lowest label index */
  int          s_upper;       /* Highest label index */
  int         *s_accel;       /* Accelerator */
  int          s_accept;      /* Nonzero for accepting state */
} state;

/* A DFA */

typedef struct {
  int          d_type;        /* Non-terminal this represents */
  char        *d_name;        /* For printing */
  int          d_nstates;
  state       *d_state;       /* Array of states */
  bitset       d_first;
} dfa;

/* A grammar */

typedef struct {
  int          g_ndfas;
  const dfa   *g_dfa;         /* Array of DFAs */
  const labellist g_ll;
  int          g_start;       /* Start symbol of the grammar */
  int          g_accel;       /* Set if accelerators present */
} grammar;


grammar _PyParser_Grammar;
static const arc arcs_0_0[3] = {
    {2, 1},
    {3, 2},
    {4, 1},
};
static const arc arcs_0_1[1] = {
    {0, 1},
};
static const arc arcs_0_2[1] = {
    {2, 1},
};
static state states_0[3] = {
    {3, arcs_0_0},
    {1, arcs_0_1},
    {1, arcs_0_2},
};
static const arc arcs_1_0[3] = {
    {44, 1},
    {2, 0},
    {45, 0},
};
static const arc arcs_1_1[1] = {
    {0, 1},
};
static state states_1[2] = {
    {3, arcs_1_0},
    {1, arcs_1_1},
};
static const arc arcs_2_0[1] = {
    {47, 1},
};
static const arc arcs_2_1[2] = {
    {44, 2},
    {2, 1},
};
static const arc arcs_2_2[1] = {
    {0, 2},
};
static state states_2[3] = {
    {1, arcs_2_0},
    {2, arcs_2_1},
    {1, arcs_2_2},
};
static const arc arcs_3_0[1] = {
    {10, 1},
};
static const arc arcs_3_1[1] = {
    {49, 2},
};
static const arc arcs_3_2[2] = {
    {5, 3},
    {2, 4},
};
static const arc arcs_3_3[2] = {
    {50, 5},
    {51, 6},
};
static const arc arcs_3_4[1] = {
    {0, 4},
};
static const arc arcs_3_5[1] = {
    {2, 4},
};
static const arc arcs_3_6[1] = {
    {50, 5},
};
static state states_3[7] = {
    {1, arcs_3_0},
    {1, arcs_3_1},
    {2, arcs_3_2},
    {2, arcs_3_3},
    {1, arcs_3_4},
    {1, arcs_3_5},
    {1, arcs_3_6},
};
static const arc arcs_4_0[1] = {
    {48, 1},
};
static const arc arcs_4_1[2] = {
    {48, 1},
    {0, 1},
};
static state states_4[2] = {
    {1, arcs_4_0},
    {2, arcs_4_1},
};
static const arc arcs_5_0[1] = {
    {52, 1},
};
static const arc arcs_5_1[3] = {
    {54, 2},
    {55, 2},
    {56, 2},
};
static const arc arcs_5_2[1] = {
    {0, 2},
};
static state states_5[3] = {
    {1, arcs_5_0},
    {3, arcs_5_1},
    {1, arcs_5_2},
};
static const arc arcs_6_0[1] = {
    {38, 1},
};
static const arc arcs_6_1[1] = {
    {56, 2},
};
static const arc arcs_6_2[1] = {
    {0, 2},
};
static state states_6[3] = {
    {1, arcs_6_0},
    {1, arcs_6_1},
    {1, arcs_6_2},
};
static const arc arcs_7_0[1] = {
    {19, 1},
};
static const arc arcs_7_1[1] = {
    {40, 2},
};
static const arc arcs_7_2[1] = {
    {57, 3},
};
static const arc arcs_7_3[2] = {
    {58, 4},
    {59, 5},
};
static const arc arcs_7_4[1] = {
    {60, 6},
};
static const arc arcs_7_5[2] = {
    {61, 7},
    {62, 8},
};
static const arc arcs_7_6[1] = {
    {59, 5},
};
static const arc arcs_7_7[1] = {
    {62, 8},
};
static const arc arcs_7_8[1] = {
    {0, 8},
};
static state states_7[9] = {
    {1, arcs_7_0},
    {1, arcs_7_1},
    {1, arcs_7_2},
    {2, arcs_7_3},
    {1, arcs_7_4},
    {2, arcs_7_5},
    {1, arcs_7_6},
    {1, arcs_7_7},
    {1, arcs_7_8},
};
static const arc arcs_8_0[1] = {
    {5, 1},
};
static const arc arcs_8_1[2] = {
    {50, 2},
    {63, 3},
};
static const arc arcs_8_2[1] = {
    {0, 2},
};
static const arc arcs_8_3[1] = {
    {50, 2},
};
static state states_8[4] = {
    {1, arcs_8_0},
    {2, arcs_8_1},
    {1, arcs_8_2},
    {1, arcs_8_3},
};
static const arc arcs_9_0[3] = {
    {6, 1},
    {64, 2},
    {65, 3},
};
static const arc arcs_9_1[4] = {
    {66, 4},
    {61, 5},
    {65, 6},
    {0, 1},
};
static const arc arcs_9_2[1] = {
    {65, 7},
};
static const arc arcs_9_3[4] = {
    {66, 8},
    {67, 9},
    {61, 5},
    {0, 3},
};
static const arc arcs_9_4[4] = {
    {64, 2},
    {61, 10},
    {65, 11},
    {0, 4},
};
static const arc arcs_9_5[1] = {
    {0, 5},
};
static const arc arcs_9_6[3] = {
    {66, 4},
    {61, 5},
    {0, 6},
};
static const arc arcs_9_7[3] = {
    {66, 12},
    {61, 5},
    {0, 7},
};
static const arc arcs_9_8[6] = {
    {6, 13},
    {64, 2},
    {68, 14},
    {61, 15},
    {65, 3},
    {0, 8},
};
static const arc arcs_9_9[1] = {
    {60, 16},
};
static const arc arcs_9_10[3] = {
    {64, 2},
    {65, 11},
    {0, 10},
};
static const arc arcs_9_11[4] = {
    {66, 4},
    {67, 17},
    {61, 5},
    {0, 11},
};
static const arc arcs_9_12[2] = {
    {61, 5},
    {0, 12},
};
static const arc arcs_9_13[4] = {
    {66, 18},
    {61, 5},
    {65, 19},
    {0, 13},
};
static const arc arcs_9_14[2] = {
    {66, 20},
    {0, 14},
};
static const arc arcs_9_15[5] = {
    {6, 13},
    {64, 2},
    {68, 14},
    {65, 3},
    {0, 15},
};
static const arc arcs_9_16[3] = {
    {66, 8},
    {61, 5},
    {0, 16},
};
static const arc arcs_9_17[1] = {
    {60, 6},
};
static const arc arcs_9_18[4] = {
    {64, 2},
    {61, 21},
    {65, 22},
    {0, 18},
};
static const arc arcs_9_19[3] = {
    {66, 18},
    {61, 5},
    {0, 19},
};
static const arc arcs_9_20[5] = {
    {6, 23},
    {64, 2},
    {61, 24},
    {65, 25},
    {0, 20},
};
static const arc arcs_9_21[3] = {
    {64, 2},
    {65, 22},
    {0, 21},
};
static const arc arcs_9_22[4] = {
    {66, 18},
    {67, 26},
    {61, 5},
    {0, 22},
};
static const arc arcs_9_23[4] = {
    {66, 27},
    {61, 5},
    {65, 28},
    {0, 23},
};
static const arc arcs_9_24[1] = {
    {65, 25},
};
static const arc arcs_9_25[4] = {
    {66, 29},
    {67, 30},
    {61, 5},
    {0, 25},
};
static const arc arcs_9_26[1] = {
    {60, 19},
};
static const arc arcs_9_27[4] = {
    {64, 2},
    {61, 31},
    {65, 32},
    {0, 27},
};
static const arc arcs_9_28[3] = {
    {66, 27},
    {61, 5},
    {0, 28},
};
static const arc arcs_9_29[5] = {
    {6, 33},
    {64, 2},
    {61, 34},
    {65, 25},
    {0, 29},
};
static const arc arcs_9_30[1] = {
    {60, 35},
};
static const arc arcs_9_31[3] = {
    {64, 2},
    {65, 32},
    {0, 31},
};
static const arc arcs_9_32[4] = {
    {66, 27},
    {67, 36},
    {61, 5},
    {0, 32},
};
static const arc arcs_9_33[4] = {
    {66, 37},
    {61, 5},
    {65, 38},
    {0, 33},
};
static const arc arcs_9_34[4] = {
    {6, 33},
    {64, 2},
    {65, 25},
    {0, 34},
};
static const arc arcs_9_35[3] = {
    {66, 29},
    {61, 5},
    {0, 35},
};
static const arc arcs_9_36[1] = {
    {60, 28},
};
static const arc arcs_9_37[4] = {
    {64, 2},
    {61, 39},
    {65, 40},
    {0, 37},
};
static const arc arcs_9_38[3] = {
    {66, 37},
    {61, 5},
    {0, 38},
};
static const arc arcs_9_39[3] = {
    {64, 2},
    {65, 40},
    {0, 39},
};
static const arc arcs_9_40[4] = {
    {66, 37},
    {67, 41},
    {61, 5},
    {0, 40},
};
static const arc arcs_9_41[1] = {
    {60, 38},
};
static state states_9[42] = {
    {3, arcs_9_0},
    {4, arcs_9_1},
    {1, arcs_9_2},
    {4, arcs_9_3},
    {4, arcs_9_4},
    {1, arcs_9_5},
    {3, arcs_9_6},
    {3, arcs_9_7},
    {6, arcs_9_8},
    {1, arcs_9_9},
    {3, arcs_9_10},
    {4, arcs_9_11},
    {2, arcs_9_12},
    {4, arcs_9_13},
    {2, arcs_9_14},
    {5, arcs_9_15},
    {3, arcs_9_16},
    {1, arcs_9_17},
    {4, arcs_9_18},
    {3, arcs_9_19},
    {5, arcs_9_20},
    {3, arcs_9_21},
    {4, arcs_9_22},
    {4, arcs_9_23},
    {1, arcs_9_24},
    {4, arcs_9_25},
    {1, arcs_9_26},
    {4, arcs_9_27},
    {3, arcs_9_28},
    {5, arcs_9_29},
    {1, arcs_9_30},
    {3, arcs_9_31},
    {4, arcs_9_32},
    {4, arcs_9_33},
    {4, arcs_9_34},
    {3, arcs_9_35},
    {1, arcs_9_36},
    {4, arcs_9_37},
    {3, arcs_9_38},
    {3, arcs_9_39},
    {4, arcs_9_40},
    {1, arcs_9_41},
};
static const arc arcs_10_0[1] = {
    {40, 1},
};
static const arc arcs_10_1[2] = {
    {59, 2},
    {0, 1},
};
static const arc arcs_10_2[1] = {
    {60, 3},
};
static const arc arcs_10_3[1] = {
    {0, 3},
};
static state states_10[4] = {
    {1, arcs_10_0},
    {2, arcs_10_1},
    {1, arcs_10_2},
    {1, arcs_10_3},
};
static const arc arcs_11_0[3] = {
    {6, 1},
    {64, 2},
    {70, 3},
};
static const arc arcs_11_1[3] = {
    {66, 4},
    {70, 5},
    {0, 1},
};
static const arc arcs_11_2[1] = {
    {70, 6},
};
static const arc arcs_11_3[3] = {
    {66, 7},
    {67, 8},
    {0, 3},
};
static const arc arcs_11_4[3] = {
    {64, 2},
    {70, 9},
    {0, 4},
};
static const arc arcs_11_5[2] = {
    {66, 4},
    {0, 5},
};
static const arc arcs_11_6[2] = {
    {66, 10},
    {0, 6},
};
static const arc arcs_11_7[5] = {
    {6, 11},
    {64, 2},
    {68, 12},
    {70, 3},
    {0, 7},
};
static const arc arcs_11_8[1] = {
    {60, 13},
};
static const arc arcs_11_9[3] = {
    {66, 4},
    {67, 14},
    {0, 9},
};
static const arc arcs_11_10[1] = {
    {0, 10},
};
static const arc arcs_11_11[3] = {
    {66, 15},
    {70, 16},
    {0, 11},
};
static const arc arcs_11_12[2] = {
    {66, 17},
    {0, 12},
};
static const arc arcs_11_13[2] = {
    {66, 7},
    {0, 13},
};
static const arc arcs_11_14[1] = {
    {60, 5},
};
static const arc arcs_11_15[3] = {
    {64, 2},
    {70, 18},
    {0, 15},
};
static const arc arcs_11_16[2] = {
    {66, 15},
    {0, 16},
};
static const arc arcs_11_17[4] = {
    {6, 19},
    {64, 2},
    {70, 20},
    {0, 17},
};
static const arc arcs_11_18[3] = {
    {66, 15},
    {67, 21},
    {0, 18},
};
static const arc arcs_11_19[3] = {
    {66, 22},
    {70, 23},
    {0, 19},
};
static const arc arcs_11_20[3] = {
    {66, 24},
    {67, 25},
    {0, 20},
};
static const arc arcs_11_21[1] = {
    {60, 16},
};
static const arc arcs_11_22[3] = {
    {64, 2},
    {70, 26},
    {0, 22},
};
static const arc arcs_11_23[2] = {
    {66, 22},
    {0, 23},
};
static const arc arcs_11_24[4] = {
    {6, 27},
    {64, 2},
    {70, 20},
    {0, 24},
};
static const arc arcs_11_25[1] = {
    {60, 28},
};
static const arc arcs_11_26[3] = {
    {66, 22},
    {67, 29},
    {0, 26},
};
static const arc arcs_11_27[3] = {
    {66, 30},
    {70, 31},
    {0, 27},
};
static const arc arcs_11_28[2] = {
    {66, 24},
    {0, 28},
};
static const arc arcs_11_29[1] = {
    {60, 23},
};
static const arc arcs_11_30[3] = {
    {64, 2},
    {70, 32},
    {0, 30},
};
static const arc arcs_11_31[2] = {
    {66, 30},
    {0, 31},
};
static const arc arcs_11_32[3] = {
    {66, 30},
    {67, 33},
    {0, 32},
};
static const arc arcs_11_33[1] = {
    {60, 31},
};
static state states_11[34] = {
    {3, arcs_11_0},
    {3, arcs_11_1},
    {1, arcs_11_2},
    {3, arcs_11_3},
    {3, arcs_11_4},
    {2, arcs_11_5},
    {2, arcs_11_6},
    {5, arcs_11_7},
    {1, arcs_11_8},
    {3, arcs_11_9},
    {1, arcs_11_10},
    {3, arcs_11_11},
    {2, arcs_11_12},
    {2, arcs_11_13},
    {1, arcs_11_14},
    {3, arcs_11_15},
    {2, arcs_11_16},
    {4, arcs_11_17},
    {3, arcs_11_18},
    {3, arcs_11_19},
    {3, arcs_11_20},
    {1, arcs_11_21},
    {3, arcs_11_22},
    {2, arcs_11_23},
    {4, arcs_11_24},
    {1, arcs_11_25},
    {3, arcs_11_26},
    {3, arcs_11_27},
    {2, arcs_11_28},
    {1, arcs_11_29},
    {3, arcs_11_30},
    {2, arcs_11_31},
    {3, arcs_11_32},
    {1, arcs_11_33},
};
static const arc arcs_12_0[1] = {
    {40, 1},
};
static const arc arcs_12_1[1] = {
    {0, 1},
};
static state states_12[2] = {
    {1, arcs_12_0},
    {1, arcs_12_1},
};
static const arc arcs_13_0[2] = {
    {3, 1},
    {4, 1},
};
static const arc arcs_13_1[1] = {
    {0, 1},
};
static state states_13[2] = {
    {2, arcs_13_0},
    {1, arcs_13_1},
};
static const arc arcs_14_0[1] = {
    {71, 1},
};
static const arc arcs_14_1[2] = {
    {72, 2},
    {2, 3},
};
static const arc arcs_14_2[2] = {
    {2, 3},
    {71, 1},
};
static const arc arcs_14_3[1] = {
    {0, 3},
};
static state states_14[4] = {
    {1, arcs_14_0},
    {2, arcs_14_1},
    {2, arcs_14_2},
    {1, arcs_14_3},
};
static const arc arcs_15_0[8] = {
    {73, 1},
    {74, 1},
    {75, 1},
    {76, 1},
    {77, 1},
    {78, 1},
    {79, 1},
    {80, 1},
};
static const arc arcs_15_1[1] = {
    {0, 1},
};
static state states_15[2] = {
    {8, arcs_15_0},
    {1, arcs_15_1},
};
static const arc arcs_16_0[1] = {
    {81, 1},
};
static const arc arcs_16_1[4] = {
    {67, 2},
    {82, 3},
    {83, 4},
    {0, 1},
};
static const arc arcs_16_2[2] = {
    {81, 5},
    {84, 5},
};
static const arc arcs_16_3[1] = {
    {0, 3},
};
static const arc arcs_16_4[2] = {
    {47, 3},
    {84, 3},
};
static const arc arcs_16_5[3] = {
    {67, 2},
    {61, 3},
    {0, 5},
};
static state states_16[6] = {
    {1, arcs_16_0},
    {4, arcs_16_1},
    {2, arcs_16_2},
    {1, arcs_16_3},
    {2, arcs_16_4},
    {3, arcs_16_5},
};
static const arc arcs_17_0[1] = {
    {59, 1},
};
static const arc arcs_17_1[1] = {
    {60, 2},
};
static const arc arcs_17_2[2] = {
    {67, 3},
    {0, 2},
};
static const arc arcs_17_3[2] = {
    {47, 4},
    {84, 4},
};
static const arc arcs_17_4[1] = {
    {0, 4},
};
static state states_17[5] = {
    {1, arcs_17_0},
    {1, arcs_17_1},
    {2, arcs_17_2},
    {2, arcs_17_3},
    {1, arcs_17_4},
};
static const arc arcs_18_0[2] = {
    {85, 1},
    {60, 1},
};
static const arc arcs_18_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_18_2[3] = {
    {85, 1},
    {60, 1},
    {0, 2},
};
static state states_18[3] = {
    {2, arcs_18_0},
    {2, arcs_18_1},
    {3, arcs_18_2},
};
static const arc arcs_19_0[13] = {
    {86, 1},
    {87, 1},
    {88, 1},
    {89, 1},
    {90, 1},
    {91, 1},
    {92, 1},
    {93, 1},
    {94, 1},
    {95, 1},
    {96, 1},
    {97, 1},
    {98, 1},
};
static const arc arcs_19_1[1] = {
    {0, 1},
};
static state states_19[2] = {
    {13, arcs_19_0},
    {1, arcs_19_1},
};
static const arc arcs_20_0[1] = {
    {20, 1},
};
static const arc arcs_20_1[1] = {
    {99, 2},
};
static const arc arcs_20_2[1] = {
    {0, 2},
};
static state states_20[3] = {
    {1, arcs_20_0},
    {1, arcs_20_1},
    {1, arcs_20_2},
};
static const arc arcs_21_0[1] = {
    {29, 1},
};
static const arc arcs_21_1[1] = {
    {0, 1},
};
static state states_21[2] = {
    {1, arcs_21_0},
    {1, arcs_21_1},
};
static const arc arcs_22_0[5] = {
    {100, 1},
    {101, 1},
    {102, 1},
    {103, 1},
    {104, 1},
};
static const arc arcs_22_1[1] = {
    {0, 1},
};
static state states_22[2] = {
    {5, arcs_22_0},
    {1, arcs_22_1},
};
static const arc arcs_23_0[1] = {
    {16, 1},
};
static const arc arcs_23_1[1] = {
    {0, 1},
};
static state states_23[2] = {
    {1, arcs_23_0},
    {1, arcs_23_1},
};
static const arc arcs_24_0[1] = {
    {18, 1},
};
static const arc arcs_24_1[1] = {
    {0, 1},
};
static state states_24[2] = {
    {1, arcs_24_0},
    {1, arcs_24_1},
};
static const arc arcs_25_0[1] = {
    {31, 1},
};
static const arc arcs_25_1[2] = {
    {81, 2},
    {0, 1},
};
static const arc arcs_25_2[1] = {
    {0, 2},
};
static state states_25[3] = {
    {1, arcs_25_0},
    {2, arcs_25_1},
    {1, arcs_25_2},
};
static const arc arcs_26_0[1] = {
    {84, 1},
};
static const arc arcs_26_1[1] = {
    {0, 1},
};
static state states_26[2] = {
    {1, arcs_26_0},
    {1, arcs_26_1},
};
static const arc arcs_27_0[1] = {
    {30, 1},
};
static const arc arcs_27_1[2] = {
    {60, 2},
    {0, 1},
};
static const arc arcs_27_2[2] = {
    {22, 3},
    {0, 2},
};
static const arc arcs_27_3[1] = {
    {60, 4},
};
static const arc arcs_27_4[1] = {
    {0, 4},
};
static state states_27[5] = {
    {1, arcs_27_0},
    {2, arcs_27_1},
    {2, arcs_27_2},
    {1, arcs_27_3},
    {1, arcs_27_4},
};
static const arc arcs_28_0[2] = {
    {105, 1},
    {106, 1},
};
static const arc arcs_28_1[1] = {
    {0, 1},
};
static state states_28[2] = {
    {2, arcs_28_0},
    {1, arcs_28_1},
};
static const arc arcs_29_0[1] = {
    {25, 1},
};
static const arc arcs_29_1[1] = {
    {107, 2},
};
static const arc arcs_29_2[1] = {
    {0, 2},
};
static state states_29[3] = {
    {1, arcs_29_0},
    {1, arcs_29_1},
    {1, arcs_29_2},
};
static const arc arcs_30_0[1] = {
    {22, 1},
};
static const arc arcs_30_1[3] = {
    {108, 2},
    {9, 2},
    {49, 3},
};
static const arc arcs_30_2[4] = {
    {108, 2},
    {9, 2},
    {25, 4},
    {49, 3},
};
static const arc arcs_30_3[1] = {
    {25, 4},
};
static const arc arcs_30_4[3] = {
    {5, 5},
    {6, 6},
    {109, 6},
};
static const arc arcs_30_5[1] = {
    {109, 7},
};
static const arc arcs_30_6[1] = {
    {0, 6},
};
static const arc arcs_30_7[1] = {
    {50, 6},
};
static state states_30[8] = {
    {1, arcs_30_0},
    {3, arcs_30_1},
    {4, arcs_30_2},
    {1, arcs_30_3},
    {3, arcs_30_4},
    {1, arcs_30_5},
    {1, arcs_30_6},
    {1, arcs_30_7},
};
static const arc arcs_31_0[1] = {
    {40, 1},
};
static const arc arcs_31_1[2] = {
    {111, 2},
    {0, 1},
};
static const arc arcs_31_2[1] = {
    {40, 3},
};
static const arc arcs_31_3[1] = {
    {0, 3},
};
static state states_31[4] = {
    {1, arcs_31_0},
    {2, arcs_31_1},
    {1, arcs_31_2},
    {1, arcs_31_3},
};
static const arc arcs_32_0[1] = {
    {49, 1},
};
static const arc arcs_32_1[2] = {
    {111, 2},
    {0, 1},
};
static const arc arcs_32_2[1] = {
    {40, 3},
};
static const arc arcs_32_3[1] = {
    {0, 3},
};
static state states_32[4] = {
    {1, arcs_32_0},
    {2, arcs_32_1},
    {1, arcs_32_2},
    {1, arcs_32_3},
};
static const arc arcs_33_0[1] = {
    {110, 1},
};
static const arc arcs_33_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_33_2[2] = {
    {110, 1},
    {0, 2},
};
static state states_33[3] = {
    {1, arcs_33_0},
    {2, arcs_33_1},
    {2, arcs_33_2},
};
static const arc arcs_34_0[1] = {
    {112, 1},
};
static const arc arcs_34_1[2] = {
    {66, 0},
    {0, 1},
};
static state states_34[2] = {
    {1, arcs_34_0},
    {2, arcs_34_1},
};
static const arc arcs_35_0[1] = {
    {40, 1},
};
static const arc arcs_35_1[2] = {
    {108, 0},
    {0, 1},
};
static state states_35[2] = {
    {1, arcs_35_0},
    {2, arcs_35_1},
};
static const arc arcs_36_0[1] = {
    {23, 1},
};
static const arc arcs_36_1[1] = {
    {40, 2},
};
static const arc arcs_36_2[2] = {
    {66, 1},
    {0, 2},
};
static state states_36[3] = {
    {1, arcs_36_0},
    {1, arcs_36_1},
    {2, arcs_36_2},
};
static const arc arcs_37_0[1] = {
    {27, 1},
};
static const arc arcs_37_1[1] = {
    {40, 2},
};
static const arc arcs_37_2[2] = {
    {66, 1},
    {0, 2},
};
static state states_37[3] = {
    {1, arcs_37_0},
    {1, arcs_37_1},
    {2, arcs_37_2},
};
static const arc arcs_38_0[1] = {
    {15, 1},
};
static const arc arcs_38_1[1] = {
    {60, 2},
};
static const arc arcs_38_2[2] = {
    {66, 3},
    {0, 2},
};
static const arc arcs_38_3[1] = {
    {60, 4},
};
static const arc arcs_38_4[1] = {
    {0, 4},
};
static state states_38[5] = {
    {1, arcs_38_0},
    {1, arcs_38_1},
    {2, arcs_38_2},
    {1, arcs_38_3},
    {1, arcs_38_4},
};
static const arc arcs_39_0[9] = {
    {113, 1},
    {55, 1},
    {53, 1},
    {114, 1},
    {56, 1},
    {115, 1},
    {116, 1},
    {117, 1},
    {118, 1},
};
static const arc arcs_39_1[1] = {
    {0, 1},
};
static state states_39[2] = {
    {9, arcs_39_0},
    {1, arcs_39_1},
};
static const arc arcs_40_0[1] = {
    {38, 1},
};
static const arc arcs_40_1[3] = {
    {114, 2},
    {56, 2},
    {118, 2},
};
static const arc arcs_40_2[1] = {
    {0, 2},
};
static state states_40[3] = {
    {1, arcs_40_0},
    {3, arcs_40_1},
    {1, arcs_40_2},
};
static const arc arcs_41_0[1] = {
    {24, 1},
};
static const arc arcs_41_1[1] = {
    {119, 2},
};
static const arc arcs_41_2[1] = {
    {59, 3},
};
static const arc arcs_41_3[1] = {
    {120, 4},
};
static const arc arcs_41_4[3] = {
    {121, 1},
    {122, 5},
    {0, 4},
};
static const arc arcs_41_5[1] = {
    {59, 6},
};
static const arc arcs_41_6[1] = {
    {120, 7},
};
static const arc arcs_41_7[1] = {
    {0, 7},
};
static state states_41[8] = {
    {1, arcs_41_0},
    {1, arcs_41_1},
    {1, arcs_41_2},
    {1, arcs_41_3},
    {3, arcs_41_4},
    {1, arcs_41_5},
    {1, arcs_41_6},
    {1, arcs_41_7},
};
static const arc arcs_42_0[1] = {
    {33, 1},
};
static const arc arcs_42_1[1] = {
    {119, 2},
};
static const arc arcs_42_2[1] = {
    {59, 3},
};
static const arc arcs_42_3[1] = {
    {120, 4},
};
static const arc arcs_42_4[2] = {
    {122, 5},
    {0, 4},
};
static const arc arcs_42_5[1] = {
    {59, 6},
};
static const arc arcs_42_6[1] = {
    {120, 7},
};
static const arc arcs_42_7[1] = {
    {0, 7},
};
static state states_42[8] = {
    {1, arcs_42_0},
    {1, arcs_42_1},
    {1, arcs_42_2},
    {1, arcs_42_3},
    {2, arcs_42_4},
    {1, arcs_42_5},
    {1, arcs_42_6},
    {1, arcs_42_7},
};
static const arc arcs_43_0[1] = {
    {21, 1},
};
static const arc arcs_43_1[1] = {
    {99, 2},
};
static const arc arcs_43_2[1] = {
    {123, 3},
};
static const arc arcs_43_3[1] = {
    {47, 4},
};
static const arc arcs_43_4[1] = {
    {59, 5},
};
static const arc arcs_43_5[2] = {
    {61, 6},
    {120, 7},
};
static const arc arcs_43_6[1] = {
    {120, 7},
};
static const arc arcs_43_7[2] = {
    {122, 8},
    {0, 7},
};
static const arc arcs_43_8[1] = {
    {59, 9},
};
static const arc arcs_43_9[1] = {
    {120, 10},
};
static const arc arcs_43_10[1] = {
    {0, 10},
};
static state states_43[11] = {
    {1, arcs_43_0},
    {1, arcs_43_1},
    {1, arcs_43_2},
    {1, arcs_43_3},
    {1, arcs_43_4},
    {2, arcs_43_5},
    {1, arcs_43_6},
    {2, arcs_43_7},
    {1, arcs_43_8},
    {1, arcs_43_9},
    {1, arcs_43_10},
};
static const arc arcs_44_0[1] = {
    {32, 1},
};
static const arc arcs_44_1[1] = {
    {59, 2},
};
static const arc arcs_44_2[1] = {
    {120, 3},
};
static const arc arcs_44_3[2] = {
    {124, 4},
    {125, 5},
};
static const arc arcs_44_4[1] = {
    {59, 6},
};
static const arc arcs_44_5[1] = {
    {59, 7},
};
static const arc arcs_44_6[1] = {
    {120, 8},
};
static const arc arcs_44_7[1] = {
    {120, 9},
};
static const arc arcs_44_8[1] = {
    {0, 8},
};
static const arc arcs_44_9[4] = {
    {122, 10},
    {124, 4},
    {125, 5},
    {0, 9},
};
static const arc arcs_44_10[1] = {
    {59, 11},
};
static const arc arcs_44_11[1] = {
    {120, 12},
};
static const arc arcs_44_12[2] = {
    {124, 4},
    {0, 12},
};
static state states_44[13] = {
    {1, arcs_44_0},
    {1, arcs_44_1},
    {1, arcs_44_2},
    {2, arcs_44_3},
    {1, arcs_44_4},
    {1, arcs_44_5},
    {1, arcs_44_6},
    {1, arcs_44_7},
    {1, arcs_44_8},
    {4, arcs_44_9},
    {1, arcs_44_10},
    {1, arcs_44_11},
    {2, arcs_44_12},
};
static const arc arcs_45_0[1] = {
    {34, 1},
};
static const arc arcs_45_1[1] = {
    {126, 2},
};
static const arc arcs_45_2[2] = {
    {66, 1},
    {59, 3},
};
static const arc arcs_45_3[2] = {
    {61, 4},
    {120, 5},
};
static const arc arcs_45_4[1] = {
    {120, 5},
};
static const arc arcs_45_5[1] = {
    {0, 5},
};
static state states_45[6] = {
    {1, arcs_45_0},
    {1, arcs_45_1},
    {2, arcs_45_2},
    {2, arcs_45_3},
    {1, arcs_45_4},
    {1, arcs_45_5},
};
static const arc arcs_46_0[1] = {
    {60, 1},
};
static const arc arcs_46_1[2] = {
    {111, 2},
    {0, 1},
};
static const arc arcs_46_2[1] = {
    {127, 3},
};
static const arc arcs_46_3[1] = {
    {0, 3},
};
static state states_46[4] = {
    {1, arcs_46_0},
    {2, arcs_46_1},
    {1, arcs_46_2},
    {1, arcs_46_3},
};
static const arc arcs_47_0[1] = {
    {128, 1},
};
static const arc arcs_47_1[2] = {
    {60, 2},
    {0, 1},
};
static const arc arcs_47_2[2] = {
    {111, 3},
    {0, 2},
};
static const arc arcs_47_3[1] = {
    {40, 4},
};
static const arc arcs_47_4[1] = {
    {0, 4},
};
static state states_47[5] = {
    {1, arcs_47_0},
    {2, arcs_47_1},
    {2, arcs_47_2},
    {1, arcs_47_3},
    {1, arcs_47_4},
};
static const arc arcs_48_0[2] = {
    {2, 1},
    {4, 2},
};
static const arc arcs_48_1[1] = {
    {129, 3},
};
static const arc arcs_48_2[1] = {
    {0, 2},
};
static const arc arcs_48_3[1] = {
    {45, 4},
};
static const arc arcs_48_4[2] = {
    {130, 2},
    {45, 4},
};
static state states_48[5] = {
    {2, arcs_48_0},
    {1, arcs_48_1},
    {1, arcs_48_2},
    {1, arcs_48_3},
    {2, arcs_48_4},
};
static const arc arcs_49_0[1] = {
    {60, 1},
};
static const arc arcs_49_1[2] = {
    {131, 2},
    {0, 1},
};
static const arc arcs_49_2[1] = {
    {60, 3},
};
static const arc arcs_49_3[1] = {
    {0, 3},
};
static state states_49[4] = {
    {1, arcs_49_0},
    {2, arcs_49_1},
    {1, arcs_49_2},
    {1, arcs_49_3},
};
static const arc arcs_50_0[2] = {
    {132, 1},
    {133, 2},
};
static const arc arcs_50_1[1] = {
    {0, 1},
};
static const arc arcs_50_2[2] = {
    {24, 3},
    {0, 2},
};
static const arc arcs_50_3[1] = {
    {133, 4},
};
static const arc arcs_50_4[1] = {
    {122, 5},
};
static const arc arcs_50_5[1] = {
    {60, 1},
};
static state states_50[6] = {
    {2, arcs_50_0},
    {1, arcs_50_1},
    {2, arcs_50_2},
    {1, arcs_50_3},
    {1, arcs_50_4},
    {1, arcs_50_5},
};
static const arc arcs_51_0[2] = {
    {135, 1},
    {133, 1},
};
static const arc arcs_51_1[1] = {
    {0, 1},
};
static state states_51[2] = {
    {2, arcs_51_0},
    {1, arcs_51_1},
};
static const arc arcs_52_0[1] = {
    {26, 1},
};
static const arc arcs_52_1[2] = {
    {59, 2},
    {69, 3},
};
static const arc arcs_52_2[1] = {
    {60, 4},
};
static const arc arcs_52_3[1] = {
    {59, 2},
};
static const arc arcs_52_4[1] = {
    {0, 4},
};
static state states_52[5] = {
    {1, arcs_52_0},
    {2, arcs_52_1},
    {1, arcs_52_2},
    {1, arcs_52_3},
    {1, arcs_52_4},
};
static const arc arcs_53_0[1] = {
    {26, 1},
};
static const arc arcs_53_1[2] = {
    {59, 2},
    {69, 3},
};
static const arc arcs_53_2[1] = {
    {134, 4},
};
static const arc arcs_53_3[1] = {
    {59, 2},
};
static const arc arcs_53_4[1] = {
    {0, 4},
};
static state states_53[5] = {
    {1, arcs_53_0},
    {2, arcs_53_1},
    {1, arcs_53_2},
    {1, arcs_53_3},
    {1, arcs_53_4},
};
static const arc arcs_54_0[1] = {
    {136, 1},
};
static const arc arcs_54_1[2] = {
    {137, 0},
    {0, 1},
};
static state states_54[2] = {
    {1, arcs_54_0},
    {2, arcs_54_1},
};
static const arc arcs_55_0[1] = {
    {138, 1},
};
static const arc arcs_55_1[2] = {
    {139, 0},
    {0, 1},
};
static state states_55[2] = {
    {1, arcs_55_0},
    {2, arcs_55_1},
};
static const arc arcs_56_0[2] = {
    {28, 1},
    {140, 2},
};
static const arc arcs_56_1[1] = {
    {138, 2},
};
static const arc arcs_56_2[1] = {
    {0, 2},
};
static state states_56[3] = {
    {2, arcs_56_0},
    {1, arcs_56_1},
    {1, arcs_56_2},
};
static const arc arcs_57_0[1] = {
    {127, 1},
};
static const arc arcs_57_1[2] = {
    {141, 0},
    {0, 1},
};
static state states_57[2] = {
    {1, arcs_57_0},
    {2, arcs_57_1},
};
static const arc arcs_58_0[10] = {
    {142, 1},
    {143, 1},
    {144, 1},
    {142, 1},
    {145, 1},
    {146, 1},
    {147, 1},
    {123, 1},
    {148, 2},
    {28, 3},
};
static const arc arcs_58_1[1] = {
    {0, 1},
};
static const arc arcs_58_2[2] = {
    {28, 1},
    {0, 2},
};
static const arc arcs_58_3[1] = {
    {123, 1},
};
static state states_58[4] = {
    {10, arcs_58_0},
    {1, arcs_58_1},
    {2, arcs_58_2},
    {1, arcs_58_3},
};
static const arc arcs_59_0[1] = {
    {6, 1},
};
static const arc arcs_59_1[1] = {
    {127, 2},
};
static const arc arcs_59_2[1] = {
    {0, 2},
};
static state states_59[3] = {
    {1, arcs_59_0},
    {1, arcs_59_1},
    {1, arcs_59_2},
};
static const arc arcs_60_0[1] = {
    {149, 1},
};
static const arc arcs_60_1[2] = {
    {150, 0},
    {0, 1},
};
static state states_60[2] = {
    {1, arcs_60_0},
    {2, arcs_60_1},
};
static const arc arcs_61_0[1] = {
    {151, 1},
};
static const arc arcs_61_1[2] = {
    {152, 0},
    {0, 1},
};
static state states_61[2] = {
    {1, arcs_61_0},
    {2, arcs_61_1},
};
static const arc arcs_62_0[1] = {
    {153, 1},
};
static const arc arcs_62_1[2] = {
    {154, 0},
    {0, 1},
};
static state states_62[2] = {
    {1, arcs_62_0},
    {2, arcs_62_1},
};
static const arc arcs_63_0[1] = {
    {155, 1},
};
static const arc arcs_63_1[3] = {
    {156, 0},
    {157, 0},
    {0, 1},
};
static state states_63[2] = {
    {1, arcs_63_0},
    {3, arcs_63_1},
};
static const arc arcs_64_0[1] = {
    {158, 1},
};
static const arc arcs_64_1[3] = {
    {7, 0},
    {8, 0},
    {0, 1},
};
static state states_64[2] = {
    {1, arcs_64_0},
    {3, arcs_64_1},
};
static const arc arcs_65_0[1] = {
    {159, 1},
};
static const arc arcs_65_1[6] = {
    {160, 0},
    {6, 0},
    {68, 0},
    {161, 0},
    {10, 0},
    {0, 1},
};
static state states_65[2] = {
    {1, arcs_65_0},
    {6, arcs_65_1},
};
static const arc arcs_66_0[4] = {
    {7, 1},
    {8, 1},
    {37, 1},
    {162, 2},
};
static const arc arcs_66_1[1] = {
    {159, 2},
};
static const arc arcs_66_2[1] = {
    {0, 2},
};
static state states_66[3] = {
    {4, arcs_66_0},
    {1, arcs_66_1},
    {1, arcs_66_2},
};
static const arc arcs_67_0[1] = {
    {163, 1},
};
static const arc arcs_67_1[2] = {
    {64, 2},
    {0, 1},
};
static const arc arcs_67_2[1] = {
    {159, 3},
};
static const arc arcs_67_3[1] = {
    {0, 3},
};
static state states_67[4] = {
    {1, arcs_67_0},
    {2, arcs_67_1},
    {1, arcs_67_2},
    {1, arcs_67_3},
};
static const arc arcs_68_0[2] = {
    {39, 1},
    {164, 2},
};
static const arc arcs_68_1[1] = {
    {164, 2},
};
static const arc arcs_68_2[2] = {
    {165, 2},
    {0, 2},
};
static state states_68[3] = {
    {2, arcs_68_0},
    {1, arcs_68_1},
    {2, arcs_68_2},
};
static const arc arcs_69_0[10] = {
    {5, 1},
    {9, 2},
    {11, 2},
    {12, 2},
    {13, 2},
    {14, 3},
    {36, 4},
    {40, 2},
    {41, 2},
    {42, 5},
};
static const arc arcs_69_1[3] = {
    {50, 2},
    {166, 6},
    {84, 6},
};
static const arc arcs_69_2[1] = {
    {0, 2},
};
static const arc arcs_69_3[2] = {
    {167, 2},
    {166, 7},
};
static const arc arcs_69_4[2] = {
    {168, 2},
    {169, 8},
};
static const arc arcs_69_5[2] = {
    {42, 5},
    {0, 5},
};
static const arc arcs_69_6[1] = {
    {50, 2},
};
static const arc arcs_69_7[1] = {
    {167, 2},
};
static const arc arcs_69_8[1] = {
    {168, 2},
};
static state states_69[9] = {
    {10, arcs_69_0},
    {3, arcs_69_1},
    {1, arcs_69_2},
    {2, arcs_69_3},
    {2, arcs_69_4},
    {2, arcs_69_5},
    {1, arcs_69_6},
    {1, arcs_69_7},
    {1, arcs_69_8},
};
static const arc arcs_70_0[2] = {
    {119, 1},
    {85, 1},
};
static const arc arcs_70_1[3] = {
    {66, 2},
    {170, 3},
    {0, 1},
};
static const arc arcs_70_2[3] = {
    {119, 4},
    {85, 4},
    {0, 2},
};
static const arc arcs_70_3[1] = {
    {0, 3},
};
static const arc arcs_70_4[2] = {
    {66, 2},
    {0, 4},
};
static state states_70[5] = {
    {2, arcs_70_0},
    {3, arcs_70_1},
    {3, arcs_70_2},
    {1, arcs_70_3},
    {2, arcs_70_4},
};
static const arc arcs_71_0[3] = {
    {5, 1},
    {108, 2},
    {14, 3},
};
static const arc arcs_71_1[2] = {
    {50, 4},
    {51, 5},
};
static const arc arcs_71_2[1] = {
    {40, 4},
};
static const arc arcs_71_3[1] = {
    {171, 6},
};
static const arc arcs_71_4[1] = {
    {0, 4},
};
static const arc arcs_71_5[1] = {
    {50, 4},
};
static const arc arcs_71_6[1] = {
    {167, 4},
};
static state states_71[7] = {
    {3, arcs_71_0},
    {2, arcs_71_1},
    {1, arcs_71_2},
    {1, arcs_71_3},
    {1, arcs_71_4},
    {1, arcs_71_5},
    {1, arcs_71_6},
};
static const arc arcs_72_0[1] = {
    {172, 1},
};
static const arc arcs_72_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_72_2[2] = {
    {172, 1},
    {0, 2},
};
static state states_72[3] = {
    {1, arcs_72_0},
    {2, arcs_72_1},
    {2, arcs_72_2},
};
static const arc arcs_73_0[2] = {
    {59, 1},
    {60, 2},
};
static const arc arcs_73_1[3] = {
    {173, 3},
    {60, 4},
    {0, 1},
};
static const arc arcs_73_2[2] = {
    {59, 1},
    {0, 2},
};
static const arc arcs_73_3[1] = {
    {0, 3},
};
static const arc arcs_73_4[2] = {
    {173, 3},
    {0, 4},
};
static state states_73[5] = {
    {2, arcs_73_0},
    {3, arcs_73_1},
    {2, arcs_73_2},
    {1, arcs_73_3},
    {2, arcs_73_4},
};
static const arc arcs_74_0[1] = {
    {59, 1},
};
static const arc arcs_74_1[2] = {
    {60, 2},
    {0, 1},
};
static const arc arcs_74_2[1] = {
    {0, 2},
};
static state states_74[3] = {
    {1, arcs_74_0},
    {2, arcs_74_1},
    {1, arcs_74_2},
};
static const arc arcs_75_0[2] = {
    {127, 1},
    {85, 1},
};
static const arc arcs_75_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_75_2[3] = {
    {127, 1},
    {85, 1},
    {0, 2},
};
static state states_75[3] = {
    {2, arcs_75_0},
    {2, arcs_75_1},
    {3, arcs_75_2},
};
static const arc arcs_76_0[1] = {
    {60, 1},
};
static const arc arcs_76_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_76_2[2] = {
    {60, 1},
    {0, 2},
};
static state states_76[3] = {
    {1, arcs_76_0},
    {2, arcs_76_1},
    {2, arcs_76_2},
};
static const arc arcs_77_0[3] = {
    {64, 1},
    {85, 2},
    {60, 3},
};
static const arc arcs_77_1[1] = {
    {127, 4},
};
static const arc arcs_77_2[3] = {
    {66, 5},
    {170, 6},
    {0, 2},
};
static const arc arcs_77_3[4] = {
    {66, 5},
    {59, 7},
    {170, 6},
    {0, 3},
};
static const arc arcs_77_4[3] = {
    {66, 8},
    {170, 6},
    {0, 4},
};
static const arc arcs_77_5[3] = {
    {85, 9},
    {60, 9},
    {0, 5},
};
static const arc arcs_77_6[1] = {
    {0, 6},
};
static const arc arcs_77_7[1] = {
    {60, 4},
};
static const arc arcs_77_8[3] = {
    {64, 10},
    {60, 11},
    {0, 8},
};
static const arc arcs_77_9[2] = {
    {66, 5},
    {0, 9},
};
static const arc arcs_77_10[1] = {
    {127, 12},
};
static const arc arcs_77_11[1] = {
    {59, 13},
};
static const arc arcs_77_12[2] = {
    {66, 8},
    {0, 12},
};
static const arc arcs_77_13[1] = {
    {60, 12},
};
static state states_77[14] = {
    {3, arcs_77_0},
    {1, arcs_77_1},
    {3, arcs_77_2},
    {4, arcs_77_3},
    {3, arcs_77_4},
    {3, arcs_77_5},
    {1, arcs_77_6},
    {1, arcs_77_7},
    {3, arcs_77_8},
    {2, arcs_77_9},
    {1, arcs_77_10},
    {1, arcs_77_11},
    {2, arcs_77_12},
    {1, arcs_77_13},
};
static const arc arcs_78_0[1] = {
    {17, 1},
};
static const arc arcs_78_1[1] = {
    {40, 2},
};
static const arc arcs_78_2[2] = {
    {5, 3},
    {59, 4},
};
static const arc arcs_78_3[2] = {
    {50, 5},
    {51, 6},
};
static const arc arcs_78_4[1] = {
    {120, 7},
};
static const arc arcs_78_5[1] = {
    {59, 4},
};
static const arc arcs_78_6[1] = {
    {50, 5},
};
static const arc arcs_78_7[1] = {
    {0, 7},
};
static state states_78[8] = {
    {1, arcs_78_0},
    {1, arcs_78_1},
    {2, arcs_78_2},
    {2, arcs_78_3},
    {1, arcs_78_4},
    {1, arcs_78_5},
    {1, arcs_78_6},
    {1, arcs_78_7},
};
static const arc arcs_79_0[1] = {
    {174, 1},
};
static const arc arcs_79_1[2] = {
    {66, 2},
    {0, 1},
};
static const arc arcs_79_2[2] = {
    {174, 1},
    {0, 2},
};
static state states_79[3] = {
    {1, arcs_79_0},
    {2, arcs_79_1},
    {2, arcs_79_2},
};
static const arc arcs_80_0[3] = {
    {6, 1},
    {64, 1},
    {60, 2},
};
static const arc arcs_80_1[1] = {
    {60, 3},
};
static const arc arcs_80_2[4] = {
    {131, 1},
    {67, 1},
    {170, 3},
    {0, 2},
};
static const arc arcs_80_3[1] = {
    {0, 3},
};
static state states_80[4] = {
    {3, arcs_80_0},
    {1, arcs_80_1},
    {4, arcs_80_2},
    {1, arcs_80_3},
};
static const arc arcs_81_0[2] = {
    {170, 1},
    {176, 1},
};
static const arc arcs_81_1[1] = {
    {0, 1},
};
static state states_81[2] = {
    {2, arcs_81_0},
    {1, arcs_81_1},
};
static const arc arcs_82_0[1] = {
    {21, 1},
};
static const arc arcs_82_1[1] = {
    {99, 2},
};
static const arc arcs_82_2[1] = {
    {123, 3},
};
static const arc arcs_82_3[1] = {
    {133, 4},
};
static const arc arcs_82_4[2] = {
    {175, 5},
    {0, 4},
};
static const arc arcs_82_5[1] = {
    {0, 5},
};
static state states_82[6] = {
    {1, arcs_82_0},
    {1, arcs_82_1},
    {1, arcs_82_2},
    {1, arcs_82_3},
    {2, arcs_82_4},
    {1, arcs_82_5},
};
static const arc arcs_83_0[2] = {
    {38, 1},
    {177, 2},
};
static const arc arcs_83_1[1] = {
    {177, 2},
};
static const arc arcs_83_2[1] = {
    {0, 2},
};
static state states_83[3] = {
    {2, arcs_83_0},
    {1, arcs_83_1},
    {1, arcs_83_2},
};
static const arc arcs_84_0[1] = {
    {24, 1},
};
static const arc arcs_84_1[1] = {
    {134, 2},
};
static const arc arcs_84_2[2] = {
    {175, 3},
    {0, 2},
};
static const arc arcs_84_3[1] = {
    {0, 3},
};
static state states_84[4] = {
    {1, arcs_84_0},
    {1, arcs_84_1},
    {2, arcs_84_2},
    {1, arcs_84_3},
};
static const arc arcs_85_0[1] = {
    {40, 1},
};
static const arc arcs_85_1[1] = {
    {0, 1},
};
static state states_85[2] = {
    {1, arcs_85_0},
    {1, arcs_85_1},
};
static const arc arcs_86_0[1] = {
    {35, 1},
};
static const arc arcs_86_1[2] = {
    {179, 2},
    {0, 1},
};
static const arc arcs_86_2[1] = {
    {0, 2},
};
static state states_86[3] = {
    {1, arcs_86_0},
    {2, arcs_86_1},
    {1, arcs_86_2},
};
static const arc arcs_87_0[2] = {
    {22, 1},
    {81, 2},
};
static const arc arcs_87_1[1] = {
    {60, 2},
};
static const arc arcs_87_2[1] = {
    {0, 2},
};
static state states_87[3] = {
    {2, arcs_87_0},
    {1, arcs_87_1},
    {1, arcs_87_2},
};
static const arc arcs_88_0[2] = {
    {2, 1},
    {4, 2},
};
static const arc arcs_88_1[2] = {
    {129, 3},
    {61, 4},
};
static const arc arcs_88_2[1] = {
    {0, 2},
};
static const arc arcs_88_3[1] = {
    {45, 5},
};
static const arc arcs_88_4[1] = {
    {2, 6},
};
static const arc arcs_88_5[2] = {
    {130, 2},
    {45, 5},
};
static const arc arcs_88_6[1] = {
    {129, 3},
};
static state states_88[7] = {
    {2, arcs_88_0},
    {2, arcs_88_1},
    {1, arcs_88_2},
    {1, arcs_88_3},
    {1, arcs_88_4},
    {2, arcs_88_5},
    {1, arcs_88_6},
};
static const arc arcs_89_0[1] = {
    {181, 1},
};
static const arc arcs_89_1[2] = {
    {44, 2},
    {2, 1},
};
static const arc arcs_89_2[1] = {
    {0, 2},
};
static state states_89[3] = {
    {1, arcs_89_0},
    {2, arcs_89_1},
    {1, arcs_89_2},
};
static const arc arcs_90_0[1] = {
    {5, 1},
};
static const arc arcs_90_1[2] = {
    {50, 2},
    {182, 3},
};
static const arc arcs_90_2[1] = {
    {58, 4},
};
static const arc arcs_90_3[1] = {
    {50, 2},
};
static const arc arcs_90_4[1] = {
    {60, 5},
};
static const arc arcs_90_5[1] = {
    {0, 5},
};
static state states_90[6] = {
    {1, arcs_90_0},
    {2, arcs_90_1},
    {1, arcs_90_2},
    {1, arcs_90_3},
    {1, arcs_90_4},
    {1, arcs_90_5},
};
static const arc arcs_91_0[3] = {
    {6, 1},
    {64, 2},
    {60, 3},
};
static const arc arcs_91_1[3] = {
    {66, 4},
    {60, 5},
    {0, 1},
};
static const arc arcs_91_2[1] = {
    {60, 6},
};
static const arc arcs_91_3[2] = {
    {66, 7},
    {0, 3},
};
static const arc arcs_91_4[2] = {
    {64, 2},
    {60, 5},
};
static const arc arcs_91_5[2] = {
    {66, 4},
    {0, 5},
};
static const arc arcs_91_6[1] = {
    {0, 6},
};
static const arc arcs_91_7[4] = {
    {6, 8},
    {64, 2},
    {60, 3},
    {0, 7},
};
static const arc arcs_91_8[3] = {
    {66, 9},
    {60, 10},
    {0, 8},
};
static const arc arcs_91_9[2] = {
    {64, 2},
    {60, 10},
};
static const arc arcs_91_10[2] = {
    {66, 9},
    {0, 10},
};
static state states_91[11] = {
    {3, arcs_91_0},
    {3, arcs_91_1},
    {1, arcs_91_2},
    {2, arcs_91_3},
    {2, arcs_91_4},
    {2, arcs_91_5},
    {1, arcs_91_6},
    {4, arcs_91_7},
    {3, arcs_91_8},
    {2, arcs_91_9},
    {2, arcs_91_10},
};
static const dfa dfas[92] = {
    {256, "single_input", 3, states_0,
     "\344\377\377\377\377\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {257, "file_input", 2, states_1,
     "\344\377\377\377\377\027\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {258, "eval_input", 3, states_2,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {259, "decorator", 7, states_3,
     "\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {260, "decorators", 2, states_4,
     "\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {261, "decorated", 3, states_5,
     "\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {262, "async_funcdef", 3, states_6,
     "\000\000\000\000\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {263, "funcdef", 9, states_7,
     "\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {264, "parameters", 4, states_8,
     "\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {265, "typedargslist", 42, states_9,
     "\100\000\000\000\000\001\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {266, "tfpdef", 4, states_10,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {267, "varargslist", 34, states_11,
     "\100\000\000\000\000\001\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {268, "vfpdef", 2, states_12,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {269, "stmt", 2, states_13,
     "\340\377\377\377\377\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {270, "simple_stmt", 4, states_14,
     "\340\373\325\376\270\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {271, "small_stmt", 2, states_15,
     "\340\373\325\376\270\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {272, "expr_stmt", 6, states_16,
     "\340\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {273, "annassign", 5, states_17,
     "\000\000\000\000\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {274, "testlist_star_expr", 3, states_18,
     "\340\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {275, "augassign", 2, states_19,
     "\000\000\000\000\000\000\000\000\000\000\300\377\007\000\000\000\000\000\000\000\000\000\000"},
    {276, "del_stmt", 3, states_20,
     "\000\000\020\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {277, "pass_stmt", 2, states_21,
     "\000\000\000\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {278, "flow_stmt", 2, states_22,
     "\000\000\005\300\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {279, "break_stmt", 2, states_23,
     "\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {280, "continue_stmt", 2, states_24,
     "\000\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {281, "return_stmt", 3, states_25,
     "\000\000\000\200\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {282, "yield_stmt", 2, states_26,
     "\000\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {283, "raise_stmt", 5, states_27,
     "\000\000\000\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {284, "import_stmt", 2, states_28,
     "\000\000\100\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {285, "import_name", 3, states_29,
     "\000\000\000\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {286, "import_from", 8, states_30,
     "\000\000\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {287, "import_as_name", 4, states_31,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {288, "dotted_as_name", 4, states_32,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {289, "import_as_names", 3, states_33,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {290, "dotted_as_names", 2, states_34,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {291, "dotted_name", 2, states_35,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {292, "global_stmt", 3, states_36,
     "\000\000\200\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {293, "nonlocal_stmt", 3, states_37,
     "\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {294, "assert_stmt", 5, states_38,
     "\000\200\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {295, "compound_stmt", 2, states_39,
     "\000\004\052\001\107\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {296, "async_stmt", 3, states_40,
     "\000\000\000\000\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {297, "if_stmt", 8, states_41,
     "\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {298, "while_stmt", 8, states_42,
     "\000\000\000\000\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {299, "for_stmt", 11, states_43,
     "\000\000\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {300, "try_stmt", 13, states_44,
     "\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {301, "with_stmt", 6, states_45,
     "\000\000\000\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {302, "with_item", 4, states_46,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {303, "except_clause", 5, states_47,
     "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\000\000\000\000"},
    {304, "suite", 5, states_48,
     "\344\373\325\376\270\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {305, "namedexpr_test", 4, states_49,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {306, "test", 6, states_50,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {307, "test_nocond", 2, states_51,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {308, "lambdef", 5, states_52,
     "\000\000\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {309, "lambdef_nocond", 5, states_53,
     "\000\000\000\004\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {310, "or_test", 2, states_54,
     "\240\173\000\020\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {311, "and_test", 2, states_55,
     "\240\173\000\020\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {312, "not_test", 3, states_56,
     "\240\173\000\020\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {313, "comparison", 2, states_57,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {314, "comp_op", 4, states_58,
     "\000\000\000\020\000\000\000\000\000\000\000\000\000\000\000\010\000\300\037\000\000\000\000"},
    {315, "star_expr", 3, states_59,
     "\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {316, "expr", 2, states_60,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {317, "xor_expr", 2, states_61,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {318, "and_expr", 2, states_62,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {319, "shift_expr", 2, states_63,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {320, "arith_expr", 2, states_64,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {321, "term", 2, states_65,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {322, "factor", 3, states_66,
     "\240\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {323, "power", 4, states_67,
     "\040\172\000\000\220\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {324, "atom_expr", 3, states_68,
     "\040\172\000\000\220\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {325, "atom", 9, states_69,
     "\040\172\000\000\020\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {326, "testlist_comp", 5, states_70,
     "\340\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {327, "trailer", 7, states_71,
     "\040\100\000\000\000\000\000\000\000\000\000\000\000\020\000\000\000\000\000\000\000\000\000"},
    {328, "subscriptlist", 3, states_72,
     "\240\173\000\024\260\007\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {329, "subscript", 5, states_73,
     "\240\173\000\024\260\007\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {330, "sliceop", 3, states_74,
     "\000\000\000\000\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {331, "exprlist", 3, states_75,
     "\340\173\000\000\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {332, "testlist", 3, states_76,
     "\240\173\000\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {333, "dictorsetmaker", 14, states_77,
     "\340\173\000\024\260\007\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {334, "classdef", 8, states_78,
     "\000\000\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {335, "arglist", 3, states_79,
     "\340\173\000\024\260\007\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {336, "argument", 4, states_80,
     "\340\173\000\024\260\007\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {337, "comp_iter", 2, states_81,
     "\000\000\040\001\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {338, "sync_comp_for", 6, states_82,
     "\000\000\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {339, "comp_for", 3, states_83,
     "\000\000\040\000\100\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {340, "comp_if", 4, states_84,
     "\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {341, "encoding_decl", 2, states_85,
     "\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {342, "yield_expr", 3, states_86,
     "\000\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {343, "yield_arg", 3, states_87,
     "\340\173\100\024\260\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {344, "func_body_suite", 7, states_88,
     "\344\373\325\376\270\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {345, "func_type_input", 3, states_89,
     "\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {346, "func_type", 6, states_90,
     "\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
    {347, "typelist", 11, states_91,
     "\340\173\000\024\260\007\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000"},
};
static const label labels[183] = {
    {0, "EMPTY"},
    {256, 0},
    {4, 0},
    {295, 0},
    {270, 0},
    {7, 0},
    {16, 0},
    {14, 0},
    {15, 0},
    {52, 0},
    {49, 0},
    {1, "False"},
    {1, "None"},
    {1, "True"},
    {9, 0},
    {1, "assert"},
    {1, "break"},
    {1, "class"},
    {1, "continue"},
    {1, "def"},
    {1, "del"},
    {1, "for"},
    {1, "from"},
    {1, "global"},
    {1, "if"},
    {1, "import"},
    {1, "lambda"},
    {1, "nonlocal"},
    {1, "not"},
    {1, "pass"},
    {1, "raise"},
    {1, "return"},
    {1, "try"},
    {1, "while"},
    {1, "with"},
    {1, "yield"},
    {25, 0},
    {31, 0},
    {56, 0},
    {55, 0},
    {1, 0},
    {2, 0},
    {3, 0},
    {257, 0},
    {0, 0},
    {269, 0},
    {258, 0},
    {332, 0},
    {259, 0},
    {291, 0},
    {8, 0},
    {335, 0},
    {260, 0},
    {261, 0},
    {262, 0},
    {334, 0},
    {263, 0},
    {264, 0},
    {51, 0},
    {11, 0},
    {306, 0},
    {58, 0},
    {344, 0},
    {265, 0},
    {35, 0},
    {266, 0},
    {12, 0},
    {22, 0},
    {17, 0},
    {267, 0},
    {268, 0},
    {271, 0},
    {13, 0},
    {294, 0},
    {276, 0},
    {272, 0},
    {278, 0},
    {292, 0},
    {284, 0},
    {293, 0},
    {277, 0},
    {274, 0},
    {273, 0},
    {275, 0},
    {342, 0},
    {315, 0},
    {40, 0},
    {41, 0},
    {46, 0},
    {38, 0},
    {36, 0},
    {37, 0},
    {48, 0},
    {39, 0},
    {44, 0},
    {45, 0},
    {50, 0},
    {43, 0},
    {42, 0},
    {331, 0},
    {279, 0},
    {280, 0},
    {283, 0},
    {281, 0},
    {282, 0},
    {286, 0},
    {285, 0},
    {290, 0},
    {23, 0},
    {289, 0},
    {287, 0},
    {1, "as"},
    {288, 0},
    {296, 0},
    {299, 0},
    {297, 0},
    {300, 0},
    {298, 0},
    {301, 0},
    {305, 0},
    {304, 0},
    {1, "elif"},
    {1, "else"},
    {1, "in"},
    {1, "finally"},
    {303, 0},
    {302, 0},
    {316, 0},
    {1, "except"},
    {5, 0},
    {6, 0},
    {53, 0},
    {308, 0},
    {310, 0},
    {307, 0},
    {309, 0},
    {311, 0},
    {1, "or"},
    {312, 0},
    {1, "and"},
    {313, 0},
    {314, 0},
    {28, 0},
    {20, 0},
    {29, 0},
    {27, 0},
    {21, 0},
    {30, 0},
    {1, "is"},
    {317, 0},
    {18, 0},
    {318, 0},
    {32, 0},
    {319, 0},
    {19, 0},
    {320, 0},
    {33, 0},
    {34, 0},
    {321, 0},
    {322, 0},
    {24, 0},
    {47, 0},
    {323, 0},
    {324, 0},
    {325, 0},
    {327, 0},
    {326, 0},
    {10, 0},
    {26, 0},
    {333, 0},
    {339, 0},
    {328, 0},
    {329, 0},
    {330, 0},
    {336, 0},
    {337, 0},
    {340, 0},
    {338, 0},
    {341, 0},
    {343, 0},
    {345, 0},
    {346, 0},
    {347, 0},
};

grammar _PyParser_Grammar = {
    ARRAY_SIZE(dfas),
    dfas,
    { ARRAY_SIZE(labels), labels},
    256
};

typedef struct _node {
  short               n_type;
  char                *n_str;
  int                 n_lineno;
  int                 n_col_offset;
  int                 n_nchildren;
  struct _node        *n_child;
  int                 n_end_lineno;
  int                 n_end_col_offset;
} node;

int keyword_indexof(char *name) {
  for (int i = 0; i < ARRAY_SIZE(kwords); i++) {
    if (strcmp(kwords[i], name) == 0) {
      return i;
    }
  }

  return -1;
}

/**
* PyGrammar_LabelRepr: E:\Git\cpython\Parser\grammar1.c: line 20
* @param:
*
* Returns:
*/
const char * PyGrammar_LabelRepr(label *lb)
{
  static char buf[100];

  if (lb->lb_type == ENDMARKER)
    return "EMPTY";
  else if (ISNONTERMINAL(lb->lb_type)) {
    if (lb->lb_str == NULL) {
      sys_snprintf(buf, sizeof(buf), "NT%d", lb->lb_type);
      return buf;
    }
    else
      return lb->lb_str;
  }
  else if (lb->lb_type < N_TOKENS) {
    if (lb->lb_str == NULL)
      return _PyParser_TokenNames[lb->lb_type];
    else {
      sys_snprintf(buf, sizeof(buf), "%.32s(%.32s)",
        _PyParser_TokenNames[lb->lb_type], lb->lb_str);
      return buf;
    }
  }
  else {
    fprintf(stderr, "invalid label\n");
    return NULL;
  }
}

/**
* dumptree:  see E:\Git\cpython\Parser\parser.c line 356
* @param:
*
* Returns:
*/
void dumptree(grammar *g, node *n) {
  int i;

  if (n == NULL)
    printf("NIL");
  else {
    label l;
    l.lb_type = TYPE(n);
    l.lb_str = STR(n);
    printf("%s", PyGrammar_LabelRepr(&l));
    if (ISNONTERMINAL(TYPE(n))) {
      printf("(");
      for (i = 0; i < NCH(n); i++) {
        if (i > 0)
          printf(",");
        dumptree(g, CHILD(n, i));
      }
      printf(")");
    }
  }
}

void
showtree(grammar *g, node *n) {
  int i;

  if (n == NULL)
    return;
  if (ISNONTERMINAL(TYPE(n))) {
    for (i = 0; i < NCH(n); i++)
      showtree(g, CHILD(n, i));
  }
  else if (ISTERMINAL(TYPE(n))) {
    printf("%s", _PyParser_TokenNames[TYPE(n)]);
    if (TYPE(n) == NUMBER || TYPE(n) == NAME)
      printf("(%s)", STR(n));
    printf(" ");
  }
  else
    printf("? ");
}

void python_basic(void) {
  grammar *g = &_PyParser_Grammar;
  const dfa *d = g->g_dfa;
  dfa nd;

  // dumptree(g, n);

  for (int i = 0; i < g->g_ndfas; i++) {
     nd = d[i];

     printf("%s\n", nd.d_name);
  }
}

void test_cpython_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(python_basic);
  }
  UNITY_END();
}

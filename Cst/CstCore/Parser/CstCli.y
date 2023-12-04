%{
  #include <System/Platform/Common/SysMemMap.h>
  #include <CstCore/Parser/Ast.h>

  #define yylval (*yylval_param)
%}

%token true_token false_token null_token Component_token GStyle_token import_token from_token
%token int_token double_token
%token source_token id_token string_token

// union struct for store typed data.
%union {
  SysInt v_token;
  SysBool v_bool;
  SysDouble v_double;
  SysInt v_int;
  SysChar *v_string;
  SysPtrArray *v_array;

  AstNode *v_node;
  JNode *v_jnode;
}

%type <v_double> double_token
%type <v_int> int_token
%type <v_string> string_token id_token source_token jkey true_token false_token null_token Component_token GStyle_token import_token from_token
%type <v_node> import component gstyle source top_unit program_unit
%type <v_jnode> jobject jarray jproperty jpair jnode jnumber jstring jnull jbool jsource
%type <v_array> id_list jpair_list jnode_list unit_list

%define parse.error verbose
%locations
%lex-param {CstParser *ps}{YYSTYPE * yylval_param } {yyscan_t yyscanner}
%parse-param {CstParser *ps} {YYSTYPE * yylval_param} {yyscan_t yyscanner}

%destructor {
  ast_node_free($$);
} top_unit import source component gstyle

%destructor {
  sys_ptr_array_free($$, true);
} unit_list

%destructor {
  sys_ptr_array_free($$, true);
} id_list jpair_list jnode_list

%destructor {
  ast_jnode_free($$);
} jobject jarray jproperty jpair jnode jnumber jsource

%destructor {
  sys_free_N($$);
} string_token id_token source_token jkey

%start program_unit
%%
program_unit : unit_list
             {
                AstNode *root = ast_for_root($1);
                $$ = root;

                CstParserContext *ctx = cst_parser_get_ctx(ps);
                cst_parser_context_realize(ctx, root);
                ast_node_free(root);
             }
             ;
unit_list : top_unit
          {
             $$ = sys_ptr_array_new_with_free_func((SysDestroyFunc)ast_node_free);
             sys_ptr_array_add($$, $1);
          }
          | unit_list top_unit
          {
             sys_ptr_array_add($1, $2);
             $$ = $1;
          }
          ;
top_unit: import
        {
            $$ = $1;
        }
        | source
        {
            $$ = $1;
        }
        | gstyle
        {
            $$ = $1;
        }
        | component
        {
            $$ = $1;
        }
        ;
import  : import_token id_list from_token string_token ';'
        {
            $$ = ast_for_import($2, $4);

            CstParserContext *ctx = cst_parser_get_ctx(ps);
            cst_parser_context_import(ctx, $$);
        }
        ;
source  : source_token
        {
            SysInt lineno = yyget_lineno(yyscanner);
            $$ = ast_for_source($1, lineno);
        }
        ;
id_list : id_token
        {
            $$ = sys_ptr_array_new_with_free_func((SysDestroyFunc)sys_free);
            sys_ptr_array_add($$, $1);
        }
        | id_list ',' id_token
        {
            sys_ptr_array_add($1, $3);
            $$ = $1;
        }
        ;
gstyle   : GStyle_token jproperty ':' jobject
        {
           $$ = ast_for_gstyle($2, $4);
        }
        | GStyle_token ':' jobject
        {
           $$ = ast_for_gstyle(NULL, $3);
        }
        ;
component: Component_token jproperty ':' jobject
         {
            $$ = ast_for_component($2, $4);
         }
         | Component_token ':' jobject
         {
            $$ = ast_for_component(NULL, $3);
         }
         ;
jkey : string_token
     {
        $$ = $1;
     }
     | id_token
     {
        $$ = $1;
     }
     ;
jpair : jkey ':' jnode
      {
          $$ = ast_for_jpair($1, NULL, $3);
      }
      | jkey jproperty ':' jnode
      {
          $$ = ast_for_jpair($1, $2, $4);
      }
      ;
jstring: string_token
       {
          $$ = ast_for_jstring($1);
       }
       ;
jbool : true_token
      {
         $$ = ast_for_jbool(true);
      }
      | false_token
      {
         $$ = ast_for_jbool(false);
      }
      ;
jarray : '[' jnode_list ']'
       {
          $$ = ast_for_jarray($2);
       }
       ;
jnumber : int_token
        {
            $$ = ast_for_jint($1);
        }
        | double_token
        {
            $$ = ast_for_jdouble($1);
        }
        ;
jnull: null_token
     {
          $$ = ast_for_jnull();
     }
     ;
jsource: source_token
       {
          $$ = ast_for_jsource($1);
       }
       ;
jnode: jobject
      {
          $$ = ast_for_jnode($1);
      }
      | jarray
      {
          $$ = ast_for_jnode($1);
      }
      | jstring
      {
          $$ = ast_for_jnode($1);
      }
      | jbool
      {
          $$ = ast_for_jnode($1);
      }
      | jnull
      {
          $$ = ast_for_jnode($1);
      }
      | jnumber
      {
          $$ = ast_for_jnode($1);
      }
      | jsource
      {
          $$ = ast_for_jnode($1);
      }
      ;
jpair_list: jpair
          {
              $$ = sys_ptr_array_new_with_free_func((SysDestroyFunc)ast_jnode_free);
              sys_ptr_array_add($$, (SysPointer)$1);
          }
          | jpair_list ',' jpair
          {
              sys_ptr_array_add($1, (SysPointer)$3);
              $$ = $1;
          }
          |
          {
              $$ = NULL;
          }
          ;
jnode_list: jnode
           {
              $$ = sys_ptr_array_new_with_free_func((SysDestroyFunc)ast_jnode_free);
              sys_ptr_array_add($$, (SysPointer)$1);
           }
           | jnode_list ',' jnode
           {
              sys_ptr_array_add($1, (SysPointer)$3);
              $$ = $1;
           }
           |
           {
              $$ = NULL;
           }
           ;
jproperty: '<' jpair_list '>'
         {
            $$ = ast_for_jproperty($2);
         }
         ;
jobject : '{' jpair_list '}'
        {
            $$ = ast_for_jobject($2);
        }
        ;
%%

void yyerror(LEX_PARAMS, const char* msg) {
  SysInt lineno = yyget_lineno(yyscanner);
  const SysChar *filename = cst_parser_get_filename(ps);

  if(yylloc.first_line) {
    sys_warning_N("%s:%d,%d,%d,%d: %s",
    filename,
    yylloc.first_line,
    yylloc.first_column,
    yylloc.last_line,
    yylloc.last_column,
    msg);
  } else {
    sys_warning_N("%s:%d: %s",
    filename,
    lineno, msg);
  }
}

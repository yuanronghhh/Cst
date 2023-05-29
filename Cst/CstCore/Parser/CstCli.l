%option noyywrap
%option yylineno
%option nounistd
%option never-interactive
%option bison-bridge
%option reentrant

WS [ \t\v\n\f]

%{
  #include <CstCore/Parser/CstParserCommon.h>

  struct _SConst {
    SysInt enum_type;
    char* const_value;
    SysInt len;
  };

  static const SConst const_table[] = {
    SCONST(Component),
    SCONST(GStyle),
    SCONST(import),
    SCONST(from),
    SCONST(true),
    SCONST(false),
    SCONST(null)
  };

  static void get_comment(LEX_PARAMS);
  static SysInt get_source(LEX_PARAMS);
  static SysInt get_string(LEX_PARAMS);
  static SysInt get_number(LEX_PARAMS);
  static SysInt get_const_type(LEX_PARAMS);
%}
%%
"/*"                                    { get_comment(LEX_PARAMS_VALS); }
"//".*                                  {}
"{%"                                    { return get_source(LEX_PARAMS_VALS); }
"\""                                    { return get_string(LEX_PARAMS_VALS); }
-{0,1}[0-9.]+                           { return get_number(LEX_PARAMS_VALS); }
[a-zA-Z_0-9-]+                          { return get_const_type(LEX_PARAMS_VALS); }
{WS}+                                   {}
","                                     { SAVE_TOKEN(','); return ','; }
"<"                                     { SAVE_TOKEN('<'); return '<'; }
">"                                     { SAVE_TOKEN('>'); return '>'; }
";"                                     { SAVE_TOKEN(';'); return ';'; }
":"                                     { SAVE_TOKEN(':'); return ':'; }
"["                                     { SAVE_TOKEN('['); return '['; }
"]"                                     { SAVE_TOKEN(']'); return ']'; }
"{"                                     { SAVE_TOKEN('{'); return '{'; }
"}"                                     { SAVE_TOKEN('}'); return '}'; }
.                                       { sys_print("Unknown token!\n"); yyterminate(); }
%%

static SysInt get_source(LEX_PARAMS) {
  SysSize mlen = 200, len = 0;
  SysChar* v_yytext = sys_str_newsize(mlen);

  SysChar s[] = { 0, '\0' };
  SysInt es = false; // escape in nested
  while ((*s = input(yyscanner)) != 0) {
    if(*s == '\\') {
      es = true;
      continue;
    }

    sys_strmcat(&v_yytext, &mlen, &len, s);
    if (*s != '%') {
      if(es == true) {
        es = false;
      }

      continue;
    }
    while ((*s = input(yyscanner)) == '%') {
      sys_strmcat(&v_yytext, &mlen, &len, s);
      continue;
    }

    if(*s == '}') {
      if (es) {
        sys_strmcat(&v_yytext, &mlen, &len, s);
        continue;
      }

      v_yytext[len-1] = '\0';
      SAVE_VALUE(v_string, v_yytext);
      return source_token;
    } else {
      sys_strmcat(&v_yytext, &mlen, &len, s);
    }

    if (*s == 0) {
      break;
    }
  }

  yyerror(LEX_PARAMS_VALS, "unterminated string.");
  return -1;
}

static SysInt get_number(LEX_PARAMS) {
  SysChar* v_yytext = yyget_text(yyscanner);

  if (strstr(v_yytext, ".") != NULL) {
    SAVE_VALUE(v_double, sys_str_to_double(v_yytext));
    return double_token;
  } else {
    SAVE_VALUE(v_int, (SysInt)sys_str_to_int64(v_yytext));
    return int_token;
  }
}

static SysInt get_string(LEX_PARAMS) {
  SysSize mlen = 200, len = 0;
  SysChar* v_yytext = sys_str_newsize(mlen);

  SysChar s[] = { 0, '\0' };
  while ((*s = input(yyscanner)) != 0) {
    if (*s == '\"') {
      SAVE_VALUE(v_string, v_yytext);
      return string_token;
    } else {
      sys_strmcat(&v_yytext, &mlen, &len, s);
    }
  }

  yyerror(LEX_PARAMS_VALS, "unterminated string.");
  return -1;
}

static void get_comment(LEX_PARAMS) {
  SysInt c;

  while ((c = input(yyscanner)) != 0) {
    if (c == '*') {
      while ((c = input(yyscanner)) == '*') {
        continue;
      }

      if (c == '/') {
        return;
      }

      if (c == 0) {
        break;
      }
    }
  }
  yyerror(LEX_PARAMS_VALS, "unterminated comment.");
}

static SysInt get_const_type(LEX_PARAMS) {
  SysChar* v_yytext = yyget_text(yyscanner);

  for(SysInt i = 0; i < ARRAY_SIZE(const_table); i++) {
    SConst sc = const_table[i];
    if(strncmp(sc.const_value, v_yytext, sc.len) == 0) {
      SAVE_VALUE(v_string, sc.const_value);
      return sc.enum_type;
    }
  }

  SAVE_STRING;
  return id_token;
}

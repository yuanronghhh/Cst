%token int_const char_const float_const id string enumeration_const
%%

stmt : if epxr then stmt
     |
     ;

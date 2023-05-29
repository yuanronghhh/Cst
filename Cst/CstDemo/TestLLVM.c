#include <TestLLVM.h>

const char *llvm_get_string(CXString cstring) {
  return clang_getCString(cstring);
}

enum CXChildVisitResult llvm_cursor_visitor(
  CXCursor cursor,
  CXCursor parent,
  CXClientData client_data) {

  CXString s_string =  clang_getCursorSpelling(cursor);
  enum CXCursorKind kind = clang_getCursorKind(cursor);
  CXString s_spell = clang_getCursorKindSpelling(kind);

  const char *sstr = llvm_get_string(s_string);
  const char *spstr = llvm_get_string(s_spell);

  if (kind != CXCursor_FunctionDecl) {
    return CXChildVisit_Recurse;
  }

  if (strncmp(sstr, STR_LSTR("test")) != 0) {
    return CXChildVisit_Recurse;
    printf("debug");
  }

  g_debug("%s,%s", sstr, spstr);

  clang_disposeString(s_string);
  clang_disposeString(s_spell);

  return CXChildVisit_Recurse;
}

void llvm_basic(void) {
  char *filename = "D:/GreyHound/PRIVATE/Git/Cst/src/CstDemo/TestInGtk.c";
  int excludeDeclsFromPCH = 1, displayDiagnostics = 1;
  CXIndex index;
  CXTranslationUnit trans_unit;
  CXCursor cursor;

  index = clang_createIndex(excludeDeclsFromPCH, displayDiagnostics);

  trans_unit  = clang_parseTranslationUnit(index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);
  if (trans_unit == NULL) {
    char *emsg = g_strdup_printf("parse file %s failed.", filename);
    perror(emsg);
    return;
  }


  cursor = clang_getTranslationUnitCursor(trans_unit);
  clang_visitChildren(cursor, llvm_cursor_visitor, NULL);

  clang_disposeTranslationUnit(trans_unit);
  clang_disposeIndex(index);
}

void llvm_gen_call_graph(void) {
  LLVMPassManagerRef pm = LLVMCreatePassManager();
  LLVMModuleRef m = LLVMModuleCreateWithName("call graph");
  LLVMValueRef main = LLVMGetNamedFunction(m, "main");

  LLVMValueRef fn;
  LLVMBasicBlockRef block;
  LLVMValueRef inst;

  if (main) {
    fn = LLVMGetFirstFunction(m);
  } else {
    fn = main;
  }

  while (fn) {
    while ((block = LLVMGetFirstBasicBlock(fn))) {
      while ((inst = LLVMGetFirstInstruction(block))) {

        inst = LLVMGetNextInstruction(inst);
      }

      block = LLVMGetNextBasicBlock(block);
    }

    fn = LLVMGetNextFunction(fn);
  }

  LLVMRunPassManager(pm, m);
}

void test_llvm_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(llvm_gen_call_graph);
    RUN_TEST(llvm_basic);
  }
  UNITY_END();
}

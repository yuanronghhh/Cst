include ./MakeTools/MakeCmake.makefile
BUILD_TYPE:=Debug
VIM_SESSION:=GVIM
project:=Framework \
				 FrameworkTest \
				 CstCoreTest \
				 CstCore \
				 Mini \
				 CstCli \
				 CstClient

ifeq ($(OS), Linux)
	BIN_FILE=${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME}
else
	BIN_FILE=${BUILD_DIR}/Cst/${PROJ_NAME}/${BUILD_TYPE}/${PROJ_NAME}${BIN_SURFIX} ${ARGS}
endif

# -------------------- CstCli start --------------------
cst-cli-gen:
	@#gen ${PROJ_NAME}.yy.c ${PROJ_NAME}.yy.h
	@#bison --debug -v -d ${PROJ_NAME}.y -o ${PROJ_NAME}.yy.c
	@bison -o './CstCore/Parser/CstCli.yy.c' -d './CstCore/Parser/CstCli.y'
	@#gen lex.yy.c
	@flex -o ./CstCore/Parser/lex.yy.c ./CstCore/Parser/CstCli.l
	@sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './CstCore/Parser/lex.yy.c'

${project}:
	@make PROJ_NAME=$@ build-${PLATFORM}-prj

.PHONY: ${project}

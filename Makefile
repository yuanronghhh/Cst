include ./MakeTools/MakeCmake.makefile
BUILD_TYPE:=Debug
VIM_SESSION:=GVIM
project := Framework \
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
	@#gen ${PROJ}.yy.c ${PROJ}.yy.h
	@#bison --debug -v -d ${PROJ}.y -o ${PROJ}.yy.c
	@bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y
	@#gen lex.yy.c
	@flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
	@/usr/bin/sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './Cst/CstCore/Parser/lex.yy.c'

${project}:
	@make -C ./build -j8 $@

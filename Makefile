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
	BIN_FILE=${BUILD_DIR}/${PROJ_NAME}/${PROJ_NAME}
else
	BIN_FILE=${BUILD_DIR}/${PROJ_NAME}/${BUILD_TYPE}/${PROJ_NAME}${BIN_SURFIX} ${ARGS}
endif

# -------------------- CstCli start --------------------
cst-cli-gen:
	@#gen ${PROJ_NAME}.yy.c ${PROJ_NAME}.yy.h
	@#bison --debug -v -d ${PROJ_NAME}.y -o ${PROJ_NAME}.yy.c
	@bison -o './CstCore/Parser/CstCli.yy.c' -d './CstCore/Parser/CstCli.y'
	@#gen lex.yy.c
	@flex -o ./CstCore/Parser/lex.yy.c ./CstCore/Parser/CstCli.l
	@sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './CstCore/Parser/lex.yy.c'

config:
	@${CMAKE_CONFIG}
	@sed -i 's/;/\n-I/g' compile_flags.txt
	@sed -i '/^-I[[:space:]]*$$/d' compile_flags.txt
	@cat compile_flags.txt|sort |uniq > compile_flags2.txt
	@mv compile_flags2.txt compile_flags.txt

re-config:
	@rm -rf build/CMakeCache.txt
	@make config


${project}:
	@make PROJ_NAME=$@ build-${PLATFORM}-prj

.PHONY: ${project}

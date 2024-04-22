PROJECT_DIR:=$(shell pwd -P)
BUILD_DIR:=${PROJECT_DIR}/build
OS:=$(shell uname -s)
PLATFORM=
OS_NCASE:=$(shell uname -s | /usr/bin/tr '[A-Z]' '[a-z]')
BUILD_TYPE:=Debug
PROJ_NAME_FILE:=
SURFIX=
ARGS=
OS_BUILD_TYPE=${BUILD_TYPE}
VS_ENV=

ifeq ($(OS), Linux)
	OS_BUILD_TYPE=
	PLATFORM=linux
else
	VS_ENV=-G "Visual Studio 17 2022" -A x64
	# VS_ENV=-G "Ninja"
	PLATFORM=win32
	SURFIX=.exe
endif

BUILD_CMAKE_ARGS:=$(BUILD_CMAKE_ARGS)
CMAKE_CONFIG = cmake $(BUILD_CMAKE_ARGS) \
                      -H"$(PROJECT_DIR)" \
                      -B"$(BUILD_DIR)" \
                      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ${VS_ENV}

build-all: build-${PLATFORM}

env-win32:
	@"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvars64.bat"

config:
	@${CMAKE_CONFIG}
	@sed -i 's/;/\n-I/g' compile_flags.txt
	@cat compile_flags.txt|sort |uniq > compile_flags2.txt
	@mv compile_flags2.txt compile_flags.txt

build-linux: config
	@${MAKE} -C "$(BUILD_DIR)" -s -j4

build-win32: config
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE}

build-win32-prj: config
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE} --target ${PROJ_NAME}

build-linux-prj: config
	@${MAKE} -C "$(BUILD_DIR)" -s -j8 ${PROJ_NAME}

re-config:
	@/usr/bin/rm -rf build/CMakeCache.txt
	@make config

clean:
	@cd ./build/ && make clean

run-linux:
	@#export LSAN_OPTIONS=verbosity=1:log_threads=1
	@${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME} ${ARGS}

prof-linux:
	@${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME} ${ARGS}
	@gprof ${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME} ${ARGS} > perf.log

run-win32:
	@${BUILD_DIR}/Cst/${PROJ_NAME}/${BUILD_TYPE}/${PROJ_NAME}${SURFIX} ${ARGS}

debug-linux:
	@gvim --servername VIM --remote-send ':DbgDebug c ${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME}<cr>'

debug-win32:
	# @gdb ${BUILD_DIR}/Cst/${PROJ_NAME}/${BUILD_TYPE}/${PROJ_NAME}${SURFIX}
	# @gvim --remote-send ':Debug ${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME}${SURFIX}<cr>'

remote-debug:
	@gdb-server '-ex "set substitute-path ./debian/build/deb/ /home/greyhound/Git/glib/_build/glib/" ${PROJ_NAME_FILE}'

build-tags:
	@echo "building tags..."
	@find ./ -type f -name '*.c' -or -name '*.h' -or -name '*.cpp' \
		-or -name '*.hpp' -or -name '*.py' -or -name '*.cs' \
		-or -name '*.js' -or -name 'CMakeLists.txt' -or -name '*.cmake' \
		-or -name '*.lua' | grep -v 'nuklear' | grep -v 'clewn' | grep -v 'old' \
		| grep -v 'tcc' | grep -v '.bak' > file.log
	@ctags${SURFIX} -a -L file.log

check-linux:
	@export G_DEBUG=gc-friendly
	@export G_SLICE=always-malloc
	valgrind --leak-check=full \
		--log-file=./check.log \
		--leak-resolution=high \
		--show-leak-kinds=all \
		--show-reachable=no \
		--suppressions=/usr/share/glib-2.0/valgrind/glib.supp  \
		--suppressions=cst.supp  \
		${BUILD_DIR}/Cst/${PROJ_NAME}/${PROJ_NAME} ${ARGS}

# -------------------- core start --------------------
cst-test-build:
	@make PROJ_NAME="CstCoreTest" build-${PLATFORM}-prj

cst-test-debug: cst-test-build
	@make PROJ_NAME="CstCoreTest" debug-${PLATFORM}

cst-test-run: cst-test-build
	@make PROJ_NAME="CstCoreTest" run-${PLATFORM}

cst-test-prof: cst-test-build
	@make PROJ_NAME="CstCoreTest" prof-${PLATFORM}

cst-test-check: cst-test-build
	@make PROJ_NAME="CstCoreTest" check-${PLATFORM}

system-build:
	@make PROJ_NAME="SystemTestSuite" build-${PLATFORM}-prj

system-check:
	@export G_DEBUG=gc-friendly
	@export G_SLICE=always-malloc
	valgrind --leak-check=full \
		--log-file=./check.log \
		--leak-resolution=high \
		--show-leak-kinds=all \
		--show-reachable=no \
		--suppressions=/usr/share/glib-2.0/valgrind/glib.supp  \
		--suppressions=cst.supp  \
		./build/Cst/System/TestSuite/SystemTestSuite ${ARGS}

system-run: system-build
	@./build/Cst/System/TestSuite/${BUILD_TYPE}/SystemTestSuite

system-debug: system-build
	@gvim --servername GVIM1 --remote-send ':DbgDebug c ./build/Cst/System/TestSuite/SystemTestSuite<cr>'

# -------------------- CstCli start --------------------
cst-cli-gen:
	@#gen ${PROJ}.yy.c ${PROJ}.yy.h
	@#bison --debug -v -d ${PROJ}.y -o ${PROJ}.yy.c
	@bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y
	@#gen lex.yy.c
	@flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
	@/usr/bin/sed -i '1i#include <CstCore/Parser/CstParserCommon.h>' './Cst/CstCore/Parser/lex.yy.c'

cst-cli-check: cst-cli-build
	@make PROJ_NAME="CstCli" check-${PLATFORM}

cst-cli-build:
	@make PROJ_NAME="CstCli" build-${PLATFORM}-prj

cst-cli-debug: cst-cli-build
	@make PROJ_NAME="CstCli" PROJ_NAME_FILE=${BUILD_DIR}/CstCli/CstCli debug-${PLATFORM}

cst-cli-run: cst-cli-build
	@make PROJ_NAME="CstCli" run-${PLATFORM}

mini-build:
	@make PROJ_NAME="Mini" build-${PLATFORM}-prj

mini-debug: mini-build
	@make PROJ_NAME="Mini" debug-${PLATFORM}

mini-run: mini-build
	@make PROJ_NAME="Mini" run-${PLATFORM}

mini-check: mini-build
	@make PROJ_NAME="Mini" check-${PLATFORM}

.PHONY: build run debug clean build-all build-tags system

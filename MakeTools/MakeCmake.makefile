PROJECT_DIR:=$(shell pwd -P)
BUILD_DIR:=${PROJECT_DIR}/build
OS:=$(shell uname -s)
PLATFORM=
OS_NCASE:=$(shell uname -s | /usr/bin/tr '[A-Z]' '[a-z]')
BUILD_TYPE:=Debug
BIN_SURFIX:=
ARGS+=
BIN_FILE:=

ifeq ($(OS), Linux)
	PLATFORM=linux
	BIN_SURFIX=
else
	PLATFORM=win32
	BIN_SURFIX=.exe
endif

BUILD_CMAKE_ARGS:=$(BUILD_CMAKE_ARGS)
CMAKE_CONFIG = cmake $(BUILD_CMAKE_ARGS) \
                      -H"$(PROJECT_DIR)" \
                      -B"$(BUILD_DIR)" \
                      -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

build-all: build-${PLATFORM}

build-linux:
	@${MAKE} -C "$(BUILD_DIR)" -s -j8

build-win32:
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE}

build-win32-prj:
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE} --target ${PROJ_NAME}

build-linux-prj:
	@${MAKE} -C "$(BUILD_DIR)" -s -j8 ${PROJ_NAME}

debug:
	@gvim --servername ${VIM_SESSION} --remote-send ':DbgDebug c ${BIN_FILE}<cr>'

run:
	#export LSAN_OPTIONS=verbosity=1:log_threads=1
	@${BIN_FILE} ${ARGS}

prof-linux:
	@${BIN_FILE} ${ARGS}
	@gprof ${BIN_FILE} ${ARGS} > perf.log

remote-debug:
	gdb-server '-ex "set substitute-path ./debian/build/deb/ /home/greyhound/Git/glib/_build/glib/" ${PROJ_NAME_FILE}'

build-tags:
	@echo "building tags..."
	@find ./ -type f -name '*.c' -or -name '*.h' -or -name '*.cpp' \
		-or -name '*.hpp' -or -name '*.py' -or -name '*.cs' \
		-or -name '*.js' -or -name 'CMakeLists.txt' -or -name '*.cmake' \
		-or -name '*.lua' | grep -v 'nuklear' | grep -v 'clewn' | grep -v 'old' \
		| grep -v 'tcc' | grep -v '.bak' > file.log
	@ctags${BIN_SURFIX} -a -L file.log

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
		${BIN_FILE} ${ARGS}

%-debug: %
	@make  PROJ_NAME=$< debug

%-run: %
	@make PROJ_NAME=$< run

%-check: %
	@make PROJ_NAME=$< check-${PLATFORM}

.PHONY: config clean build-all build-tags

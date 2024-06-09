PROJECT_DIR:=$(shell pwd -P)
BUILD_DIR:=${PROJECT_DIR}/build
OS:=$(shell uname -s)
PLATFORM:=
OS_NCASE:=$(shell uname -s | /usr/bin/tr '[A-Z]' '[a-z]')
BUILD_TYPE:=Debug
BIN_SURFIX:=
ARGS:=
BIN_FILE:=
DEBUGGER:=gdb

ifeq ($(OS), Linux)
	PLATFORM:=linux
	BIN_SURFIX=
else
	PLATFORM:=win32
	BIN_SURFIX:=.exe
endif

BUILD_CMAKE_ARGS:=$(BUILD_CMAKE_ARGS)
CMAKE_CONFIG = cmake $(BUILD_CMAKE_ARGS) \
                      -H"$(PROJECT_DIR)" \
                      -B"$(BUILD_DIR)" \
                      -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

build-all: build-${PLATFORM}

config:
	@${CMAKE_CONFIG}
	@make editor-config

editor-config:
	@sed -i 's/;/\n-I/g' compile_flags.txt
	@sed -i '/^-I[[:space:]]*$$/d' compile_flags.txt
	@cat compile_flags.txt|sort|uniq > compile_flags2.txt
	@mv compile_flags2.txt compile_flags.txt

re-config:
	@rm -rf build/CMakeCache.txt
	@make config

build-linux:
	@${MAKE} -C "$(BUILD_DIR)" -s -j8

build-win32:
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE}

build-win32-prj:
	@cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE} --target ${PROJ_NAME}

build-linux-prj:
	@${MAKE} -C "$(BUILD_DIR)" -s -j8 ${PROJ_NAME}

debug:
	@gvim --servername ${VIM_SESSION} --remote-send ':DbgDebug ${DEBUGGER} ${ARGS}<cr>'

debugcli:
	@echo ${ARGS}
	@${DEBUGGER} ${ARGS}

run:
	@echo ${ARGS}
	@${ARGS}

clean:
	@make -C "${BUILD_DIR}" clean

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

check-leak:
	@export G_DEBUG=gc-friendly
	@export G_SLICE=always-malloc
	valgrind --leak-check=full \
		--log-file=./check.log \
		--leak-resolution=high \
		--show-leak-kinds=all \
		--show-reachable=no \
		--suppressions=/usr/share/glib-2.0/valgrind/glib.supp  \
		--suppressions=cst.supp  \
		${ARGS}

check-thread:
	@export G_DEBUG=gc-friendly
	@export G_SLICE=always-malloc
	valgrind --tool=helgrind \
		--log-file=./check.log \
		--suppressions=/usr/share/glib-2.0/valgrind/glib.supp  \
		--suppressions=cst.supp  \
		${ARGS}

.PHONY: editor-config re-config config clean build-all build-tags

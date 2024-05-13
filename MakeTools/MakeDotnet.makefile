PROJECT_DIR:=$(shell pwd -P)
OS:=$(shell uname -s)
OS_NCASE:=$(shell uname -s | /usr/bin/tr '[A-Z]' '[a-z]')
CC=dotnet
PROJ_NAME:=
PLATFORM=
ARGS=
VIM_SESSION=GVIM
BUILD_DIR:=${PROJ_NAME}/bin/Debug/net8.0
BIN_FILE=${PROJ_NAME}/bin/Debug/net8.0/${PROJ_NAME}.dll

debug:
	@gvim --servername ${VIM_SESSION} --remote-send ':DbgDebug csharp ${BIN_FILE} ${ARGS}<cr>'

run:
	@${CC} run --project ${PROJ_NAME}

build:
	@${CC} build ./${PROJ_NAME}

clean:
	@${CC} clean

%-build: %
	@make PROJ_NAME=$< build

%-run: %
	@make PROJ_NAME=$< run

%-debug: %
	@make PROJ_NAME=$< debug

.PHONY: build debug run clean

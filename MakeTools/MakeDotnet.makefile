PROJECT_DIR:=$(shell pwd -P)
OS:=$(shell uname -s)
OS_NCASE:=$(shell uname -s | /usr/bin/tr '[A-Z]' '[a-z]')
CC:=dotnet
PROJ_NAME:=
PLATFORM=
VIM_SESSION:=GVIM
BIN_FILE:=
ARGS:=

build:
	@${CC} build ./${PROJ_NAME}

run:
	@${CC} run --project ${PROJ_NAME}

clean:
	@${CC} clean

%-build: %
	@make PROJ_NAME=$< build

%-run: %
	@make PROJ_NAME=$< run

.PHONY: build run clean

BUILD_DIR_CTRL              := build
OPIL_DIR                    := ../..
HOST_COMM_SOCK_SERVER_IP    := 127.0.0.1
HOST_COMM_SOCK_SERVER_PORT  := 8090

ifeq ($(OS),Windows_NT)
	RM := del /q
	BUILD_DIR_PLECS_OPIL := plecs\build
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		RM := rm -rf
		BUILD_DIR_PLECS_OPIL := plecs/build
	else
		$(error Unsupported platform: $(UNAME_S))
	endif
endif

default: all

all:
	make -f ${OPIL_DIR}/sw/plecs/make_opil_plecs.mk BUILD_DIR=${BUILD_DIR_PLECS_OPIL} OPIL=${OPIL_DIR} HOST_COMM_SOCK_SERVER_IP=${HOST_COMM_SOCK_SERVER_IP} HOST_COMM_SOCK_SERVER_PORT=${HOST_COMM_SOCK_SERVER_PORT}
	make -f make_ctrl.mk BUILD_DIR=${BUILD_DIR_CTRL} OPIL=${OPIL_DIR} TARGET_COMM_SOCK_SERVER_PORT=${HOST_COMM_SOCK_SERVER_PORT}

clean:
	${RM} ${BUILD_DIR_CTRL}
	${RM} ${BUILD_DIR_PLECS_OPIL}

BUILD_DIR := build
OPIL      := ../..
OBJS      := build/ctlrif.o build/targetCommSock.o build/opiltarget.o build/buckcontrol.o
TARGET_COMM_SOCK_SERVER_PORT := 8090

ifeq ($(OS),Windows_NT)
	PLAT := win
	COMM_DIR := win
	DIR_GUARD := mkdir -p ${BUILD_DIR}
	SOCK_LIB  := ws2_32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		PLAT := linux
		COMM_DIR := linux
		DIR_GUARD := mkdir -p ${BUILD_DIR}
	else
		$(error Unsupported platform: $(UNAME_S))
	endif
endif

main: ${OBJS} src/main.c
	gcc -O2 -c src/main.c -I${OPIL}/ -o build/main.o
ifeq ($(PLAT),win)
	gcc build/main.o ${OBJS} -l${SOCK_LIB} -o build/opiltarget.exe
else
	gcc build/main.o ${OBJS} -o build/opiltarget.so
endif

build/ctlrif.o: ${OPIL}/ctlrif/ctlrif.c
	${DIR_GUARD}
	gcc -O2 -c ${OPIL}/ctlrif/ctlrif.c -I${OPIL}/ -I. -o build/ctlrif.o
	
build/targetCommSock.o: ${OPIL}/comm/${COMM_DIR}/targetCommSock.c
	${DIR_GUARD}
	gcc -O2 -c ${OPIL}/comm/${COMM_DIR}/targetCommSock.c -I${OPIL}/ -I${OPIL}/comm/ -o build/targetCommSock.o -DTARGET_COMM_SOCK_SERVER_PORT=${TARGET_COMM_SOCK_SERVER_PORT}

build/opiltarget.o: ${OPIL}/opiltarget.c
	${DIR_GUARD}
	gcc -O2 -c ${OPIL}/opiltarget.c -I${OPIL}/ -o build/opiltarget.o

build/buckcontrol.o: src/buckcontrol.c
	${DIR_GUARD}
	gcc -O2 -c src/buckcontrol.c -I${OPIL}/ -I. -o build/buckcontrol.o

clean:
	del /q build
	


//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "targetCommPynqSock.h"

#include "stdio.h"

/* Device and drivers */
#include "xparameters.h"
#include "netif/xadapter.h"
#include "xil_printf.h"
#include "xil_types.h"

/* lwip */
#include "lwip/dhcp.h"
#include "lwip/sockets.h"
#include "lwipopts.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define TARGET_COMM_PYNQ_SOCK_SERVER_PORT	8090
#define TARGET_COMM_PYNQ_SOCK_BACKLOG		5

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================

/**
 * @brief Prints the specified IP4.
 * @param msg Message to print before printing the IP.
 * @param ip IP to be printed.
 */
static void targetCommZynqSockPrintIP(char *msg, ip_addr_t *ip);
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static int serverSocket, clientSocket;

//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t targetCommPynqInitialize(void){

	int status;

	struct sockaddr_in address;

	memset(&address, 0, sizeof(address));

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( serverSocket < 0 ){
		printf("Failed to create server socket (err %d)\n\r\n\r", serverSocket);
		return TARGET_COMM_PYNQ_SOCK_ERR_SOCK_INI;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(TARGET_COMM_PYNQ_SOCK_SERVER_PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	status = lwip_bind(serverSocket, (struct sockaddr *)&address, sizeof (address));
	if ( status < 0){
		printf("Failed to bind server socket (err %d)\n\r", status);
		return TARGET_COMM_PYNQ_SOCK_ERR_SOCK_BIND;
	}

	return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommPynqSockOpenConnection(void *params){

	int size;
	struct sockaddr_in remote;

	lwip_listen(serverSocket, 0);

	size = sizeof(remote);

	clientSocket = lwip_accept( serverSocket, (struct sockaddr *)&remote, (socklen_t *)&size );

	if( clientSocket <= 0 ){
		printf("Failed to accept client connection (err %d)\n\r", clientSocket);
		return TARGET_COMM_PYNQ_SOCK_ERR_SOCK_CLIENT_CONN;
	}

	printf("Received connection from");
	targetCommZynqSockPrintIP(": ", (ip_addr_t *)&remote.sin_addr);

	return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommPynqSockCloseConnection(void *params){

	int32_t status;

	status = lwip_close(clientSocket);
	if( status != 0 ){
        printf("Failed to close client socket (err %d)\n\r", (int)status);
        status = TARGET_CONN_PYNQ_SOCK_ERR_CLIENT_SOCK_CLOSE;
	}

	return status;
}
//-----------------------------------------------------------------------------
int32_t targetCommPynqSockSendData(void *buffer, int32_t size){

	int32_t sent, totalSent;
	uint8_t *p = (uint8_t *)buffer;

	totalSent = 0;

	while( totalSent < size ){

		sent = lwip_send(clientSocket, (void *)p, size - totalSent, 0);

		if( sent < 0 ){
	        printf("Send failed (err %d)\n\rConnection will be closed...\n\r", (int)sent);
	        targetCommPynqSockCloseConnection(0);
	        return TARGET_CONN_PYNQ_SOCK_ERR_SOCK_SEND;
		}

		p = p + sent;
		totalSent += sent;
	}

	return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommPynqSockReceiveData(void *buffer, int32_t size){

	int32_t received, totalReceived;
	uint8_t *p = (uint8_t *)buffer;

	totalReceived = 0;
	while( totalReceived < size ){

		received = recv(clientSocket, (void*)p, size - totalReceived, 0);

		if( received == 0 ){
			printf("Connection closed by client.\n\r");
			return TARGET_CONN_PYNQ_SOCK_ERR_CLIENT_DISCONNECTED;
		}
		else if( received < 0 ){
	        printf("Recv failed (err %d)\n\rConnection will be closed...\n\r", (int)received);
	        targetCommPynqSockCloseConnection(0);
	        return TARGET_CONN_PYNQ_SOCK_ERR_SOCK_RECV;
		}

		p = p + received;
		totalReceived += received;
	}

	return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static void targetCommZynqSockPrintIP(char *msg, ip_addr_t *ip){

	printf(msg);
	printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}
//-----------------------------------------------------------------------------
//=============================================================================

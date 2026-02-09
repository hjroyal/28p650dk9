//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "targetCommSock.h"

#include "stdio.h"
#include "winsock2.h"
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static WSADATA wsaData;
static SOCKET server_socket = INVALID_SOCKET, client_socket = INVALID_SOCKET;
static struct sockaddr_in server_addr, client_addr;
static int client_len;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t targetCommSockOpenConnection(void *params){
    
    (void)params;
    int status;
    int nodelay_flag = 1;
    
    /* Initializes Winsock */
    if( WSAStartup(MAKEWORD(2,2),&wsaData) != 0 ){
        LogError( ("WSAStartup failed with error code %d", WSAGetLastError()) );
        return 1;
    }

    /* Creates the server socket */
    if( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET ){
        LogError( ("Creating socket failed with error code %d", WSAGetLastError()) );
        WSACleanup();
        return 1;
    }

    /* Sets up the server address structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TARGET_COMM_SOCK_SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    /* Binds the server socket to the address */
    if( bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR ){
        LogError( ("Socket bind failed with error code %d", WSAGetLastError()) );
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    /* Listens for incoming connections */
    if( listen(server_socket, TARGET_COMM_SOCK_BACKLOG) == SOCKET_ERROR ){
        LogError( ("Socket listen failed with error code %d", WSAGetLastError()) );
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    /* Waits for a connection */
    client_len = sizeof(client_addr);
    if( (client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == INVALID_SOCKET ) {
        LogError( ("Socket accept failed with error code %d", WSAGetLastError()) );
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    status = setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay_flag, sizeof(nodelay_flag));
    if( status == SOCKET_ERROR ){
        LogError(("setsockopt TCP_NODELAY failed with error code %d", WSAGetLastError()));
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommSockCloseConnection(void *params){

    (void)params;
    int32_t status;

    status = shutdown(client_socket, SD_SEND);
    if( status == SOCKET_ERROR ){
        LogError( ("Shutdown failed with error code %d", WSAGetLastError()) );
        status = TARGET_CONN_SOCK_ERR_SOCK_SHUTDOWN;
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return status;
}
//-----------------------------------------------------------------------------
int32_t targetCommSockSendData(void *buffer, int32_t size){

    int32_t sent, totalSent;
    uint8_t *p = (uint8_t *)buffer;

    totalSent = 0;

    while( totalSent < size ){

        sent = send(client_socket, (void *)p, size - totalSent, 0);

        if( sent == SOCKET_ERROR ){
            LogError( ("Send failed with error code %d", WSAGetLastError()) );
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return TARGET_CONN_SOCK_ERR_SOCK_SEND;
        }

        p = p + sent;
        totalSent += sent;
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommSockReceiveData(void *buffer, int32_t size){

    int32_t received, totalReceived;
    uint8_t *p = (uint8_t *)buffer;

    totalReceived = 0;
    while( totalReceived < size ){

        received = recv(client_socket, (void*)p, size - totalReceived, 0);

        if( received == 0 ){
            return TARGET_CONN_SOCK_ERR_CLIENT_DISCONNECTED;
        }
        else if( received < 0 ){
            LogError( ("Receive failed with error code %d", WSAGetLastError()) );
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return TARGET_CONN_SOCK_ERR_SOCK_RECV;
        }

        p = p + received;
        totalReceived += received;
    }

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

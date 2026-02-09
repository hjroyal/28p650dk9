
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "hostCommSock.h"

#include "stdio.h"
#include "winsock2.h"
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static WSADATA wsaData;
static SOCKET sock = INVALID_SOCKET;
static struct sockaddr_in server_addr;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t hostCommSockOpenConnection(void *params){

    int nodelay_flag = 1;
    (void)params;
    int status;

    /* Initializes Winsock */
    status = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( status != 0 ) {
        LogError( ("WSAStartup failed failed with error code %d", status) );
        return HOST_COMM_SOCK_ERR_SOCK_INI;
    }

    /* Creates the socket on this side */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        LogError( ("Socket creation failed with error code %d", WSAGetLastError()) );
        WSACleanup();
        return HOST_COMM_SOCK_ERR_SOCK_CREATE;
    }

    status = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay_flag, sizeof(nodelay_flag));
    if( status == SOCKET_ERROR ){
        LogError(("setsockopt TCP_NODELAY failed with error code %d", WSAGetLastError()));
    }

    /* Server data */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST_COMM_SOCK_SERVER_IP);
    server_addr.sin_port = htons(HOST_COMM_SOCK_SERVER_PORT);

    /* Connects to the server */
    status = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if( status == SOCKET_ERROR ) {
        LogError( ("Connect failed with error code %d", WSAGetLastError()) );
        closesocket(sock);
        WSACleanup();
        return HOST_COMM_SOCK_ERR_SOCK_SERVER_CONN;
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t hostCommSockCloseConnection(void *params){

    (void)params;
    int32_t status;

    status = shutdown(sock, SD_SEND);
    if( status == SOCKET_ERROR ){
        LogError( ("Shutdown failed with error code %d", WSAGetLastError()) );
        status = HOST_CONN_SOCK_ERR_SOCK_SHUTDOWN;
    }

    closesocket(sock);
    WSACleanup();

    return status;
}
//-----------------------------------------------------------------------------
int32_t hostCommSockSendData(void *buffer, int32_t size){

    int32_t sent, totalSent;
    uint8_t *p = (uint8_t *)buffer;

    totalSent = 0;

    while( totalSent < size ){

        sent = send(sock, (void *)p, size - totalSent, 0);

        if( sent == SOCKET_ERROR ){
            LogError( ("Send failed with error code %d", WSAGetLastError()) );
            closesocket(sock);
            WSACleanup();
            return HOST_CONN_SOCK_ERR_SOCK_SEND;
        }

        p = p + sent;
        totalSent += sent;
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t hostCommSockReceiveData(void *buffer, int32_t size){

    int32_t received, totalReceived;
    uint8_t *p = (uint8_t *)buffer;

    totalReceived = 0;
    while( totalReceived < size ){

        received = recv(sock, (void*)p, size - totalReceived, 0);

        if( received == 0 ){
            return HOST_CONN_SOCK_ERR_SERVER_DISCONNECTED;
        }
        else if( received < 0 ){
            LogError( ("Socket recv failed with error code %d", WSAGetLastError()) );
            closesocket(sock);
            WSACleanup();
            return HOST_CONN_SOCK_ERR_SOCK_RECV;
        }

        p = p + received;
        totalReceived += received;
    }

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

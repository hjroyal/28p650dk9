
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "hostCommSock.h"

#include "stdio.h"

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
int32_t hostCommSockOpenLocalConnection(void);
int32_t hostCommSockOpenExternalConnection(void);
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define HOST_COMM_SOCKET_PATH "/tmp/opil_unix_socket" STR(HOST_COMM_SOCK_SERVER_PORT)

static int sock;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t hostCommSockOpenConnection(void *params){

    (void)params;
    int status;

    if( 
        ( strcmp(HOST_COMM_SOCK_SERVER_IP, "127.0.0.1") == 0 ) ||
        ( strcmp(HOST_COMM_SOCK_SERVER_IP, "localhost") == 0 )
    ) status = hostCommSockOpenLocalConnection();
    else status = hostCommSockOpenExternalConnection();

    return status;
}
//-----------------------------------------------------------------------------
int32_t hostCommSockCloseConnection(void *params){

    (void)params;
    int32_t status;

    status = shutdown(sock, SHUT_RDWR);
    if( status < 0 ){
        LogError( ("Shutdown failed with error code %d", status) );
        status = HOST_CONN_SOCK_ERR_SOCK_SHUTDOWN;
    }

    close(sock);

    return status;
}
//-----------------------------------------------------------------------------
int32_t hostCommSockSendData(void *buffer, int32_t size){

    int32_t sent, totalSent;
    uint8_t *p = (uint8_t *)buffer;

    totalSent = 0;

    while( totalSent < size ){

        sent = send(sock, (void *)p, size - totalSent, 0);

        if( sent < 0 ){
            LogError( ("Send failed with error code %d", sent) );
            close(sock);
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
            LogError( ("Socket recv failed with error code %d", received) );
            close(sock);
            return HOST_CONN_SOCK_ERR_SOCK_RECV;
        }

        p = p + received;
        totalReceived += received;
    }

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t hostCommSockOpenLocalConnection(void){
    
    int32_t status;
    struct sockaddr_un server_addr;

    /* Creates the socket on this side */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        LogError( ("Socket creation failed with error code %d", sock) );
        return HOST_COMM_SOCK_ERR_SOCK_CREATE;
    }

    /* Server data */
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, HOST_COMM_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    /* Connects to the server */
    status = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if( status < 0 ) {
        LogError( ("Connect failed with error code %d", status) );
        close(sock);
        return HOST_COMM_SOCK_ERR_SOCK_SERVER_CONN;
    }

    return status;  
}
//-----------------------------------------------------------------------------
int32_t hostCommSockOpenExternalConnection(void){

    int nodelay_flag = 1;
    int32_t status;
    struct sockaddr_in server_addr;

    /* Creates the socket on this side */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LogError( ("Connect failed with error code %d", sock) );
        return HOST_COMM_SOCK_ERR_SOCK_CREATE;
    }

    status = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &nodelay_flag, sizeof(nodelay_flag));
    if( status < 0 ){
        LogError(( "Failed to set TCP_NODELAY -- run time will be affected." ));
    }

    /* Server data */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST_COMM_SOCK_SERVER_IP);
    server_addr.sin_port = htons(HOST_COMM_SOCK_SERVER_PORT);

    /* Connects to the server */
    status = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if( status < 0 ) {
        LogError( ("Connect failed with error code %d", status) );
        close(sock);
        return HOST_COMM_SOCK_ERR_SOCK_SERVER_CONN;
    }
    
    return status;
}
//-----------------------------------------------------------------------------
//=============================================================================

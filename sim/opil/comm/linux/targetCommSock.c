
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "targetCommSock.h"

#include <stdio.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define TARGET_COMM_SOCKET_PATH "/tmp/opil_unix_socket" STR(TARGET_COMM_SOCK_SERVER_PORT)

static int server_socket, client_socket;
struct sockaddr_un server_addr;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t targetCommSockOpenConnection(void *params){

    (void)params;
    int status;

    /* 
     * Creates the server socket. It is created as UNIX socket because the
     * target is running on the local machine.
     */
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if( server_socket < 0 ){
        LogError( ("Creating socket failed with error code %d", server_socket) );
        return 0;
    }

    unlink(TARGET_COMM_SOCKET_PATH);

    /* Sets up the server address structure */
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, TARGET_COMM_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);


    /* Binds the server socket to the address */
    status = bind( server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr) );
    if( status < 0 ){
        LogError( ("Socket bind failed with error code %d", status) );
        return 0;
    }

    /* Listens for incoming connections */
    status = listen(server_socket, TARGET_COMM_SOCK_BACKLOG); 
    if( status < 0 ){
        LogError( ("Socket listen failed with error code %d", status) );
        close(server_socket);
        return 0;
    }

    /* Waits for a connection */
    socklen_t client_len = sizeof(struct sockaddr_un);
    client_socket = accept(server_socket, NULL, &client_len);

    if( client_socket < 0 ) {
        LogError( ("Socket accept failed with error code %d", client_socket) );
        close(server_socket);
        return -1;
    }

    return 0;
}
//-----------------------------------------------------------------------------
int32_t targetCommSockCloseConnection(void *params){

    (void)params;
    int32_t status;

    status = shutdown(client_socket, SHUT_RDWR);
    if( status < 0 ){
        LogError( ("Shutdown failed with error code %d", status) );
        status = TARGET_CONN_SOCK_ERR_SOCK_SHUTDOWN;
    }

    close(client_socket);
    close(server_socket);

    return status;
}
//-----------------------------------------------------------------------------
int32_t targetCommSockSendData(void *buffer, int32_t size){

    int32_t sent, totalSent;
    uint8_t *p = (uint8_t *)buffer;

    totalSent = 0;

    while( totalSent < size ){

        sent = send(client_socket, (void *)p, size - totalSent, 0);

        if( sent < 0 ){
            LogError( ("Send failed with error code %d", sent) );
            close(client_socket);
            close(server_socket);
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
            LogError( ("Receive failed with error code %d", received) );
            close(client_socket);
            close(server_socket);
            return TARGET_CONN_SOCK_ERR_SOCK_RECV;
        }

        p = p + received;
        totalReceived += received;
    }

    return 0;
}
//-----------------------------------------------------------------------------
//=============================================================================

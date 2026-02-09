
#ifndef TARGET_COMM_SOCK_H_
#define TARGET_COMM_SOCK_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
#include "stddef.h"

//=============================================================================

//=============================================================================
/*-------------------------------- Logging ----------------------------------*/
//=============================================================================
#include "logging/logging_levels.h"

#ifndef LIBRARY_LOG_NAME
#define LIBRARY_LOG_NAME    "OPiL target"
#endif

#ifndef LIBRARY_LOG_LEVEL
#define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

#include "logging/logging_stack.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define TARGET_COMM_SOCK_ERR_SOCK_INI               -1
#define TARGET_COMM_SOCK_ERR_SOCK_CREATE            -2
#define TARGET_COMM_SOCK_ERR_SOCK_SERVER_CONN       -3
#define TARGET_CONN_SOCK_ERR_SOCK_SEND              -4
#define TARGET_CONN_SOCK_ERR_SOCK_RECV              -5
#define TARGET_CONN_SOCK_ERR_SOCK_SHUTDOWN          -6
#define TARGET_CONN_SOCK_ERR_CLIENT_DISCONNECTED    -7

#ifndef TARGET_COMM_SOCK_SERVER_PORT
#define TARGET_COMM_SOCK_SERVER_PORT	            8090
#endif
#define TARGET_COMM_SOCK_BUFFER_SIZE	            512
#define TARGET_COMM_SOCK_BACKLOG		            5
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t targetCommSockOpenConnection(void *params);
//-----------------------------------------------------------------------------
int32_t targetCommSockCloseConnection(void *params);
//-----------------------------------------------------------------------------
int32_t targetCommSockSendData(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
int32_t targetCommSockReceiveData(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* TARGET_COMM_SOCK_H_ */

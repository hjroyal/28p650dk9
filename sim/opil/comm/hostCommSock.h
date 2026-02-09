
#ifndef HOST_COMM_SOCK_H_
#define HOST_COMM_SOCK_H_

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
#define LIBRARY_LOG_NAME    "OPiL host"
#endif

#ifndef LIBRARY_LOG_LEVEL
#define LIBRARY_LOG_LEVEL    LOG_INFO
#endif

#include "logging/logging_stack.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define HOST_COMM_SOCK_ERR_SOCK_INI             -1
#define HOST_COMM_SOCK_ERR_SOCK_CREATE          -2
#define HOST_COMM_SOCK_ERR_SOCK_SERVER_CONN     -3
#define HOST_CONN_SOCK_ERR_SOCK_SEND            -4
#define HOST_CONN_SOCK_ERR_SOCK_RECV            -5
#define HOST_CONN_SOCK_ERR_SOCK_SHUTDOWN        -6
#define HOST_CONN_SOCK_ERR_SERVER_DISCONNECTED  -7

#ifndef HOST_COMM_SOCK_SERVER_IP
#define HOST_COMM_SOCK_SERVER_IP                "127.0.0.1"
#endif

#ifndef HOST_COMM_SOCK_SERVER_PORT
#define HOST_COMM_SOCK_SERVER_PORT              8090
#endif

#define HOST_COMM_SOCK_BUFFER_SIZE              512
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes and opens the client socket.
 *
 * @param params Not used, can be given as 0 or NULL.
 * @return 0 if socket was opened successfully, otherwise an error code.
 */
int32_t hostCommSockOpenConnection(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Closes the client socket previously opened.
 *
 * @param params Not used, can be given as 0 or NULL.
 * @return 0 if socket was closed successfully, an error code otherwise.
 */
int32_t hostCommSockCloseConnection(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Sends data through the previously opened socket.
 *
 * @param buffer Pointer to buffer holding data to be sent.
 * @param size Size of buffer, in bytes.
 * @return 0 if data was sent successfully, an error code otherwise.
 */
int32_t hostCommSockSendData(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Receives data from the previously opened socket.
 *
 * @param buffer Pointer to buffer to hold data.
 * @param size Number of bytes to receive.
 * @return 0 if data was received successfully, an error code otherwise.
 */
int32_t hostCommSockReceiveData(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* HOST_COMM_SOCK_H_ */

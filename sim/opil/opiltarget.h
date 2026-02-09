#ifndef OPILTARGET_H_
#define OPILTARGET_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
#include "stddef.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Size of buffer used to temporarily store received data (in bytes).
 */
#define OPIL_TARGET_CONFIG_BUFFER_SIZE	512
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to open a host connection.
 *
 * @param params Generic parameters to be passed to the function.
 * @return The function should return 0 if the connection was successful.
 */
typedef int32_t (*otCommOpenConn_t)(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to close a host connection.
 *
 * @param params Generic parameters to be passed to the function.
 * @return The function should return 0 if the disconnection was successful.
 */
typedef int32_t (*otCommCloseConn_t)(void *params);
//-----------------------------------------------------------------------------
/**
 * @ brief Signature of function to send data to the host.
 *
 * @param buffer Pointer to buffer holding data to be sent.
 * @param size Size of buffer, in bytes.
 * @return The function should return 0 if data was sent successfully.
 */
typedef int32_t (*otCommSendData_t)(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to receive data from the host.
 *
 * @param buffer Pointer to buffer to hold data.
 * @param size Number of bytes to receive.
 * @return The function should return 0 if data was received successfully.
 */
typedef int32_t (*otCommReceiveData_t)(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to write the measurements data.
 *
 * @param meas Pointer to the measurements data.
 * @param size Size of measurements data, in bytes.
 * @return Should return 0 if measurements was updated successfully.
 */
typedef int32_t (*otUpdateMeas_t)(void *meas, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to write the (additional) simulation data.
 *
 * @param simData Pointer to the simulation data.
 * @param size Size of simulation data, in bytes.
 * @return Should return 0 if simulation data was updated successfully.
 */
typedef int32_t (*otUpdateSimData_t)(void *simData, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Callback of function to initialize the controller.
 *
 * This function is called whenever a new connection is opened.
 */
typedef void (*otInitControl_t)(void);
//-----------------------------------------------------------------------------
/**
 * @brief Callback of function that runs the controller.
 *
 * This function is called whenever new data is received.
 */
typedef void (*otRunControl_t)(void);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to get the control data.
 *
 * @param control Address of pointer where the address of the control data
 * 			      should be written to.
 * @return The size of the control data structure, in bytes.
 */
typedef int32_t (*otGetControl_t)(void **control);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to get the controller data.
 *
 * @param control Address of pointer where the address of the controller data
 * 			      should be written to.
 * @return The size of the controller data structure, in bytes.
 */
typedef int32_t (*otGetControllerData_t)(void **controllerData);
//-----------------------------------------------------------------------------
/**
 * @brief Target communication settings.
 *
 * Config structure to set the callbacks for target communication.
 */
typedef struct{
	otCommOpenConn_t openConn;
	otCommCloseConn_t closeConn;
	otCommSendData_t sendData;
	otCommReceiveData_t receiveData;
}opiltargetCommConfig_t;
//-----------------------------------------------------------------------------
/**
 * @brief Target settings.
 *
 * Config structure to set the callbacks to interface with the embedded
 * controller.
 */
typedef struct{
	otUpdateMeas_t updateMeas;
	otUpdateSimData_t updateSimData;

	otInitControl_t initControl;
	otRunControl_t runControl;

	otGetControl_t getControl;
	otGetControllerData_t getControllerData;
}opiltargetControlConfig_t;
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes the opiltarget library.
 *
 * @param commConfig Callbacks for communication.
 * @param targetConfig Callbacks for interfacing with the embedded controller.
 *
 * @return Always returns 0.
 */
int32_t opiltargetInitialize(
		opiltargetCommConfig_t *commConfig,
		opiltargetControlConfig_t* targetConfig
		);
//-----------------------------------------------------------------------------
/**
 * @brief Connects to the Host.
 *
 * @param params Parameters to be passed to the otCommOpenConn_t callback.
 *
 * @return 0 if connection was successful, an error code otherwise.
 */
int32_t opiltargetConnectToHost(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Exchanges data with the host.
 *
 * This function control the main data flow implemented by the opil library.
 * Essentially, here, the following steps happen:
 *
 *	- Data is received from the host and the internal structures are updated
 *	- The control algorithm is executed
 *	- The new control and controller data are sent to the host
 *
 * @return 0 if data was exchanged successfully, an error code otherwise.
 */
int32_t opiltargetExchangeDataHost(void);
//-----------------------------------------------------------------------------
/**
 * @brief Disconnects from the host.
 *
 * @param params Parameters to be passed to the otCommCloseConn_t callback.
 * @return 0 if disconnection was successful, an error code otherwise.
 */
int32_t opiltargetDisconnectFromHost(void *params);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* OPILTARGET_H_ */

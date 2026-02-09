
#ifndef OPILHOST_H_
#define OPILHOST_H_

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
#define OPIL_HOST_CONFIG_BUFFER_SIZE	512
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to open a target connection.
 *
 * @param params Generic parameters to be passed to the function.
 * @return The function should return 0 if the connection was successful.
 */
typedef int32_t (*ohCommOpenConn_t)(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to close a target connection.
 *
 * @param params Generic parameters to be passed to the function.
 * @return The function should return 0 if the disconnection was successful.
 */
typedef int32_t (*ohCommCloseConn_t)(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to send data to the target.
 *
 * @param buffer Pointer to buffer holding data to be sent.
 * @param size Size of buffer, in bytes.
 * @return The function should return 0 if data was sent successfully.
 */
typedef int32_t (*ohCommSendData_t)(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to receive data from the target.
 *
 * @param buffer Pointer to buffer to hold data.
 * @param size Number of bytes to receive.
 * @return The function should return 0 if data was received successfully.
 */
typedef int32_t (*ohCommReceiveData_t)(void *buffer, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to update simulation data.
 *
 * This function essentially copies the outside data into the internal
 * structures.
 */
typedef void (*ohUpdateSim_t)(void);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to get the measurements data.
 *
 * @param meas Address of pointer where the address of the measurements data
 * 			   should be written to.
 * @return The size of the measurements data structure, in bytes.
 */
typedef int32_t (*ohGetMeas_t)(void **meas);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to get the (additional) simulation data.
 *
 * @param simdata Address of pointer where the address of the simulation data
 * 			      should be written to.
 * @return The size of the simulation data structure, in bytes.
 */
typedef int32_t (*ohGetSimData_t)(void **simdata);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to write the control data.
 *
 * @param control Pointer to the control data.
 * @param size Size of control data, in bytes.
 * @return Should return 0 if control data was updated successfully.
 */
typedef int32_t (*ohUpdateControl_t)(void *control, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to write the controller data.
 *
 * @param controllerdata Pointer to the controller data.
 * @param size Size of controller data, in bytes.
 * @return Should return 0 if controller data was updated successfully.
 */
typedef int32_t (*ohUpdateControllerData_t)(void *controllerdata, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Signature of function to apply the control and controller data to
 * the simulation.
 */
typedef void (*ohApplyControl_t)(void);
//-----------------------------------------------------------------------------
/**
 * @brief Host communication settings.
 *
 * Config structure to set the callbacks for host communication.
 */
typedef struct{
	ohCommOpenConn_t openConn;
	ohCommCloseConn_t closeConn;
	ohCommSendData_t sendData;
	ohCommReceiveData_t receiveData;
}opilhostCommConfig_t;
//-----------------------------------------------------------------------------
/**
 * @brief Simulation settings.
 *
 * Config structure to set the callbacks to interface with the simulation.
 */
typedef struct{
	ohUpdateSim_t updateSim;
	ohGetMeas_t getMeas;
	ohGetSimData_t getSimData;

	ohUpdateControl_t updateControl;
	ohUpdateControllerData_t updateControllerData;
	ohApplyControl_t applyControl;
}opilhostSimConfig_t;
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes the opilhost library.
 *
 * @param commConfig Callbacks for communication.
 * @param simConfig Callbacks for interfacing with the simulation.
 *
 * @return Always returns 0.
 */
int32_t opilhostInitialize(
		opilhostCommConfig_t *commConfig,
		opilhostSimConfig_t* simConfig
		);
//-----------------------------------------------------------------------------
/**
 * @brief Connects to the target.
 *
 * @param params Parameters to be passed to the ohCommOpenConn_t callback.
 *
 * @return 0 if connection was successful, an error code otherwise.
 */
int32_t opilhostConnectToTarget(void *params);
//-----------------------------------------------------------------------------
/**
 * @brief Exchanges data with the target.
 *
 * This function control the main data flow implemented by the opil library.
 * Essentially, here, the following steps happen:
 *
 * - The internal structures are updated with new simulation data
 * - Updated simulation data is sent to the target
 * - Control and controller data is read from the target
 * - Simulation is updated with the control and controller data.
 *
 * @return 0 if data was exchanged successfully, an error code otherwise.
 */
int32_t opilhostExchangeDataTarget(void);
//-----------------------------------------------------------------------------
/**
 * @brief Disconnects from the target.
 *
 * @param params Parameters to be passed to the ohCommCloseConn_t callback.
 * @return 0 if disconnection was successful, an error code otherwise.
 */
int32_t opilhostDisconnectFromTarget(void *params);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* OPILHOST_H_ */

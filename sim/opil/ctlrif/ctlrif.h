
#ifndef CONTROLLERINTERFACE_H_
#define CONTROLLERINTERFACE_H_

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "stdint.h"
#include "stddef.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef void (*ctlrifControlInit_t)(void);
typedef void (*ctlrifControlRun_t)(
		void *meas,
		void *simData,
		void *control,
		void *controllerData
		);
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
/**
 * @brief Initializes the controller interface.
 */
void ctlrifInitialize(
		ctlrifControlInit_t controlInit, ctlrifControlRun_t controlRun
		);
//-----------------------------------------------------------------------------
/**
 * @brief Updates measurements data.
 *
 * The internal measurements structure is updated with the new data.
 *
 * @param meas Pointer to measurements data.
 * @param size Size of the measurements data, in bytes.
 * @return Always returns 0.
 */
int32_t ctlrifUpdateMeasurements(void *meas, int32_t size);
//-----------------------------------------------------------------------------
/**
 * @brief Updates simulation data.
 *
 * The internal simulation data structure is updated with the new data.
 *
 * @param simData Pointer to simulation data.
 * @param size Size of the simulation data, in bytes.
 * @return Always returns 0.
 */
int32_t ctlrifUpdateSimData(void *simData, int32_t size);
//-----------------------------------------------------------------------------
void ctlrifInitializeControl(void);
//-----------------------------------------------------------------------------
/**
 * @brief Runs the control algorithm.
 */
void ctlrifRunControl(void);
//-----------------------------------------------------------------------------
/**
 * @brief Gets control data.
 *
 * The location of the structure holding the control data is stored in control.
 *
 * @param control Address of pointer to hold the address of the
 * 			      control data structure.
 * @return The size of the control data structure, in bytes.
 */
int32_t ctlrifGetControl(void **control);
//-----------------------------------------------------------------------------
/**
 * @brief Gets controller data.
 *
 * The location of the structure holding the controller data is stored in
 * controllerData.
 *
 * @param controllerData Address of pointer to hold the address of the
 * 			             controller data structure.
 * @return The size of the control data structure, in bytes.
 */
int32_t ctlrifGetControllerData(void **controllerData);
//-----------------------------------------------------------------------------
//=============================================================================

#endif /* CONTROLLERINTERFACE_H_ */

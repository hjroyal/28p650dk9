
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "simif.h"
#include "stypes.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static stypesMeasurements_t xhMeasurements;
static stypesSimData_t xhSimData;
static stypesControl_t xhControl;
static stypesControllerData_t xhControllerData;

static void *vMeas;
static void *vSimData;
static void *vControl;
static void *vControllerData;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void simifInitialize(
		void *meas, void *simData,
		void *control, void *controllerData){

	vMeas = meas;
	vSimData = simData;
	vControl = control;
	vControllerData = controllerData;
}
//-----------------------------------------------------------------------------
void simifUpdateSimulation(void){

	uint8_t *dst, *src;
	uint32_t k;

	src = (uint8_t *)( vMeas );
	dst = (uint8_t *)( &xhMeasurements );
	k = sizeof(stypesMeasurements_t);
	while(k--) *dst++ = *src++;

	src = (uint8_t *)( vSimData );
	dst = (uint8_t *)( &xhSimData );
	k = sizeof(stypesSimData_t);
	while(k--) *dst++ = *src++;
}
//-----------------------------------------------------------------------------
int32_t simifGetMeasurements(void **meas){

	*meas = (void *)( &xhMeasurements );

	return sizeof(stypesMeasurements_t);
}
//-----------------------------------------------------------------------------
int32_t simifGetSimData(void **simData){

	*simData = (void *)( &xhSimData );

	return sizeof(stypesSimData_t);
}
//-----------------------------------------------------------------------------
int32_t simifUpdateControl(void *control, int32_t size){

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xhControl );
	src = (uint8_t *)( control );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t simifUpdateControllerData(void *controllerData, int32_t size){

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xhControllerData );
	src = (uint8_t *)( controllerData );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
void simifApplyControl(void){

	uint8_t *src, *dst;
	uint32_t k;

	dst = (uint8_t *)( vControl );
	src = (uint8_t *)( &xhControl );
	k = sizeof(stypesControl_t);
	while(k--) *dst++ = *src++;

	dst = (uint8_t *)( vControllerData );
	src = (uint8_t *)( &xhControllerData );
	k = sizeof(stypesControllerData_t);
	while(k--) *dst++ = *src++;
}
//-----------------------------------------------------------------------------
//=============================================================================


//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "ctlrif.h"

#include "stypes.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static stypesMeasurements_t xtMeasurements;
static stypesSimData_t xtSimData;
static stypesControl_t xtControl;
static stypesControllerData_t xtControllerData;

static ctlrifControlInit_t xcontrolInit = 0;
static ctlrifControlRun_t xcontrolRun = 0;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void ctlrifInitialize(
		ctlrifControlInit_t controlInit, ctlrifControlRun_t controlRun
		){

	xcontrolInit = controlInit;
	xcontrolRun = controlRun;
}
//-----------------------------------------------------------------------------
int32_t ctlrifUpdateMeasurements(void *meas, int32_t size){

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xtMeasurements );
	src = (uint8_t *)( meas );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t ctlrifUpdateSimData(void *simData, int32_t size){

	uint8_t *src, *dst;

	dst = (uint8_t *)( &xtSimData );
	src = (uint8_t *)( simData );

	while(size--) *dst++ = *src++;

	return 0;
}
//-----------------------------------------------------------------------------
void ctlrifInitializeControl(void){

	if( xcontrolInit ) xcontrolInit();
}
//-----------------------------------------------------------------------------
void ctlrifRunControl(void){

	if( xcontrolRun )
		xcontrolRun(
				(void *)&xtMeasurements,
				(void *)&xtSimData,
				(void *)&xtControl,
				(void *)&xtControllerData
				);
}
//-----------------------------------------------------------------------------
int32_t ctlrifGetControl(void **control){

	*control = (void *)( &xtControl );

	return sizeof(stypesControl_t);
}
//-----------------------------------------------------------------------------
int32_t ctlrifGetControllerData(void **controllerData){

	*controllerData = (void *)( &xtControllerData );

	return sizeof(stypesControllerData_t);
}
//-----------------------------------------------------------------------------
//=============================================================================

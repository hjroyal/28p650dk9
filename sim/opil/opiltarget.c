
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "opiltarget.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
	otCommOpenConn_t openConn;
	otCommCloseConn_t closeConn;
	otCommSendData_t sendData;
	otCommReceiveData_t receiveData;

	otUpdateMeas_t updateMeas;
	otUpdateSimData_t updateSimData;

	otInitControl_t initControl;
	otRunControl_t runControl;

	otGetControl_t getControl;
	otGetControllerData_t getControllerData;

	uint8_t buffer[OPIL_TARGET_CONFIG_BUFFER_SIZE];

}opiltargetControl_t;
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static opiltargetControl_t xotControl;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t opiltargetInitialize(
		opiltargetCommConfig_t *commConfig,
		opiltargetControlConfig_t* controlConfig
		){

	xotControl.openConn = commConfig->openConn;
	xotControl.closeConn = commConfig->closeConn;
	xotControl.sendData = commConfig->sendData;
	xotControl.receiveData = commConfig->receiveData;

	xotControl.updateMeas = controlConfig->updateMeas;
	xotControl.updateSimData = controlConfig->updateSimData;

	xotControl.initControl = controlConfig->initControl;
	xotControl.runControl = controlConfig->runControl;

	xotControl.getControl = controlConfig->getControl;
	xotControl.getControllerData = controlConfig->getControllerData;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t opiltargetConnectToHost(void *params){

	int32_t status;

	status = xotControl.openConn(params);

	if( status == 0 ) xotControl.initControl();

	return status;
}
//-----------------------------------------------------------------------------
int32_t opiltargetExchangeDataHost(void){

	void *p = 0;
	int32_t size;

	/* Receives and updates measurements and simulation data */
	if(	xotControl.receiveData((void *)&size, 4) != 0 )
		return -1;
	if( xotControl.receiveData((void *)xotControl.buffer, size) )
		return -1;
	xotControl.updateMeas(xotControl.buffer, size);

	if(	xotControl.receiveData((void*)&size, 4) != 0 )
		return -1;
	if( xotControl.receiveData((void*)xotControl.buffer, size) != 0 )
		return -1;
	xotControl.updateSimData(xotControl.buffer, size);

	/* Runs control algorithm */
	xotControl.runControl();

	/* Gets control and controller data and sends it to host */
	size = xotControl.getControl( (void **)(&p) );
	xotControl.sendData( (void *)(&size), 4 );
	xotControl.sendData(p, size);

	size = xotControl.getControllerData( (void **)(&p) );
	xotControl.sendData( (void *)(&size), 4 );
	xotControl.sendData(p, size);

	return 0;
}
//-----------------------------------------------------------------------------
int32_t opiltargetDisconnectFromHost(void *params){

	return xotControl.closeConn(params);
}
//-----------------------------------------------------------------------------
//=============================================================================



//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "opilhost.h"

//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
typedef struct{
	ohCommOpenConn_t openConn;
	ohCommCloseConn_t closeConn;
	ohCommSendData_t sendData;
	ohCommReceiveData_t receiveData;

	ohUpdateSim_t updateSim;
	ohGetMeas_t getMeas;
	ohGetSimData_t getSimData;

	ohUpdateControl_t updateControl;
	ohUpdateControllerData_t updateControllerData;
	ohApplyControl_t applyControl;

	uint8_t buffer[OPIL_HOST_CONFIG_BUFFER_SIZE];

}opilhostControl_t;
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
static opilhostControl_t xohControl;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
int32_t opilhostInitialize(
		opilhostCommConfig_t *commConfig,
		opilhostSimConfig_t* simConfig
		){

	xohControl.openConn = commConfig->openConn;
	xohControl.closeConn = commConfig->closeConn;
	xohControl.sendData = commConfig->sendData;
	xohControl.receiveData = commConfig->receiveData;

	xohControl.updateSim = simConfig->updateSim;
	xohControl.getMeas = simConfig->getMeas;
	xohControl.getSimData = simConfig->getSimData;

	xohControl.updateControl = simConfig->updateControl;
	xohControl.updateControllerData = simConfig->updateControllerData;
	xohControl.applyControl = simConfig->applyControl;

	return 0;
}
//-----------------------------------------------------------------------------
int32_t opilhostConnectToTarget(void *params){

	return xohControl.openConn(params);
}
//-----------------------------------------------------------------------------
int32_t opilhostExchangeDataTarget(void){

	void *p;
	int32_t size;

	/* Updates simulation signals */
	xohControl.updateSim();

	/* Sends measurements and sim data to target */
	size = xohControl.getMeas( (void **)(&p) );
	xohControl.sendData( (void *)(&size), 4 );
	xohControl.sendData(p, size);

	size = xohControl.getSimData( (void **)(&p) );
	xohControl.sendData( (void *)(&size), 4 );
	xohControl.sendData(p, size);

	/* Receives and update control and controller data */
	if( xohControl.receiveData((void*)&size, 4) != 0 )
		return -1;
	if( xohControl.receiveData((void*)xohControl.buffer, size) != 0 )
		return -1;
	xohControl.updateControl(xohControl.buffer, size);

	if( xohControl.receiveData((void*)&size, 4) != 0 )
		return -1;
	if( xohControl.receiveData((void*)xohControl.buffer, size) != 0 )
		return -1;
	xohControl.updateControllerData(xohControl.buffer, size);

	/* Applies control and controller data to simulation */
	xohControl.applyControl();

	return 0;
}
//-----------------------------------------------------------------------------
int32_t opilhostDisconnectFromTarget(void *params){

	return xohControl.closeConn(params);
}
//-----------------------------------------------------------------------------
//=============================================================================

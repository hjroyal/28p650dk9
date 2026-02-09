
//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include <ctlrif/ctlrif.h>
#include <ctlrif/pynq/pil.h>
#include "FreeRTOS.h"
#include "task.h"

/* OPiL */
#include "opiltarget.h"
#include "comm/pynq/targetCommPynqSock.h"

/* Control */
//#include "invcontrol.h"
//#include "mmccontrol.h"
#include "buckcontrol.h"
//=============================================================================

//=============================================================================
/*--------------------------------- Defines ---------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================

//=============================================================================

//=============================================================================
/*-------------------------------- Prototypes -------------------------------*/
//=============================================================================
static void pilInitialize(void);
//=============================================================================

//=============================================================================
/*---------------------------------- Task -----------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void pil(void *param){

	pilInitialize();

	while(1){
		if( opiltargetConnectToHost(0) != 0 ) continue;

		while( opiltargetExchangeDataHost() == 0 );

		opiltargetDisconnectFromHost(0);
	}
}
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
/*---------------------------- Static functions -----------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
static void pilInitialize(void){

	/* Creates the server socket */
	if( targetCommPynqInitialize() != 0 )
		vTaskDelay(2000 / portTICK_PERIOD_MS);

	/* Initializes controller interface */
	//ctlrifInitialize(invcontrolInitialize, invcontrol);
	//ctlrifInitialize(mmccontrolInitialize, mmccontrol);
	ctlrifInitialize(buckcontrolInitialize, buckcontrol);

	/* Initializes opil */
	opiltargetCommConfig_t comm;
	opiltargetControlConfig_t control;

	comm.openConn = targetCommPynqSockOpenConnection;
	comm.closeConn = targetCommPynqSockCloseConnection;
	comm.sendData = targetCommPynqSockSendData;
	comm.receiveData = targetCommPynqSockReceiveData;

	control.updateMeas = ctlrifUpdateMeasurements;
	control.updateSimData = ctlrifUpdateSimData;

	control.initControl = ctlrifInitializeControl;
	control.runControl = ctlrifRunControl;

	control.getControl = ctlrifGetControl;
	control.getControllerData = ctlrifGetControllerData;

	opiltargetInitialize(&comm, &control);
}
//-----------------------------------------------------------------------------
//=============================================================================


//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "DllHeader.h"

#include "stdint.h"

#include "stypes.h"

#include "opilhost.h"
#include "comm/hostCommSock.h"
#include "simif/simif.h"
//=============================================================================

//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================
#define CONFIG_MEAS_SIZE        (sizeof(stypesMeasurements_t) / 4)
#define CONFIG_SIM_DATA_SIZE    (sizeof(stypesSimData_t) / 4)
#define CONFIG_CONTROL_SIZE     (sizeof(stypesControl_t) / 4)
#define CONFIG_CONTROLLER_SIZE  (sizeof(stypesControllerData_t) / 4)
//=============================================================================

//=============================================================================
/*--------------------------------- Globals ---------------------------------*/
//=============================================================================
float measurements[CONFIG_MEAS_SIZE];
float simdata[CONFIG_SIM_DATA_SIZE];
float control[CONFIG_CONTROL_SIZE];
float controllerdata[CONFIG_CONTROLLER_SIZE];

int32_t conn;
//=============================================================================

//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
DLLEXPORT void plecsSetSizes(struct SimulationSizes* aSizes)
{
   aSizes->numInputs = (CONFIG_MEAS_SIZE + CONFIG_SIM_DATA_SIZE);
   aSizes->numOutputs = (CONFIG_CONTROL_SIZE + CONFIG_CONTROLLER_SIZE);
   aSizes->numStates = 0;
   aSizes->numParameters = 0; //number of user parameters passed in
}
//-----------------------------------------------------------------------------
//This function is automatically called at the beginning of the simulation
DLLEXPORT void plecsStart(struct SimulationState* aState)
{
    simifInitialize((void *)( &measurements ), (void *)( &simdata ), (void *)( &control ), (void *)( &controllerdata ));

    opilhostCommConfig_t comm;
    opilhostSimConfig_t sim;

    comm.openConn = hostCommSockOpenConnection;
    comm.closeConn = hostCommSockCloseConnection;
    comm.sendData = hostCommSockSendData;
    comm.receiveData = hostCommSockReceiveData;

    sim.updateSim = simifUpdateSimulation;
    sim.getMeas = simifGetMeasurements;
    sim.getSimData = simifGetSimData;
    sim.updateControl = simifUpdateControl;
    sim.updateControllerData = simifUpdateControllerData;
    sim.applyControl = simifApplyControl;

    opilhostInitialize(&comm, &sim);

    conn = opilhostConnectToTarget(0);
}
//-----------------------------------------------------------------------------
//This function is automatically called every sample time
//output is written to DLL output port after the output delay
DLLEXPORT void plecsOutput(struct SimulationState* aState)
{	
    if( conn == 0 )
    {
	    int k;

	    for(k = 0; k < CONFIG_MEAS_SIZE; k++)
	    {
		    measurements[k] = (float)aState->inputs[k];
	    }

	    for(k = CONFIG_MEAS_SIZE; k < (CONFIG_MEAS_SIZE + CONFIG_SIM_DATA_SIZE); k++)
	    {
		    simdata[k-CONFIG_MEAS_SIZE] = (float)aState->inputs[k];
	    }

	    opilhostExchangeDataTarget();

	    for(k = 0; k < CONFIG_CONTROL_SIZE; k++)
	    {
		    aState->outputs[k] = control[k];
	    }

	    for(k = CONFIG_CONTROL_SIZE; k < (CONFIG_CONTROL_SIZE + CONFIG_CONTROLLER_SIZE); k++)
	    {
		    aState->outputs[k] = controllerdata[k - CONFIG_CONTROL_SIZE];
	    }
	    
    }

}
//-----------------------------------------------------------------------------
DLLEXPORT void plecsTerminate(struct SimulationState* aState){
    
    if( conn == 0 ) {opilhostDisconnectFromTarget(0);}
}
//-----------------------------------------------------------------------------
//=============================================================================

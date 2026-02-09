/*
 * main.c
 *
 *  Created on: 6 de mai de 2023
 *      Author: LRS
 */

#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

#include "opiltarget.h"

#include "ctlrif/ctlrif.h"
#include "comm/targetCommSock.h"

#include "buckcontrol.h"

int main(int argc, char **argv){

    char uinput, d;
    
    /* Initializes target */
    ctlrifInitialize(buckcontrolInitialize, buckcontrol);
    
    /* Initializes opil */
    opiltargetCommConfig_t comm;
    opiltargetControlConfig_t control;
    
    comm.openConn = targetCommSockOpenConnection;
    comm.closeConn = targetCommSockCloseConnection;
    comm.sendData = targetCommSockSendData;
    comm.receiveData = targetCommSockReceiveData;
    
    control.updateMeas = ctlrifUpdateMeasurements;
    control.updateSimData = ctlrifUpdateSimData;
    
    control.initControl = ctlrifInitializeControl;
    control.runControl = ctlrifRunControl;

    control.getControl = ctlrifGetControl;
    control.getControllerData = ctlrifGetControllerData;

    opiltargetInitialize(&comm, &control);
    
    /* Runs opil target */

    printf("%s: OPiL target module ready to run.\r\n\r\n", __FUNCTION__);
    fflush( stdout );
    
    while( 1 ){

    printf("%s: Waiting for a connection...\r\n", __FUNCTION__);
    fflush( stdout );
    opiltargetConnectToHost(0);

    printf("%s: Connection received! Running OPiL target...\r\n", __FUNCTION__);
    fflush( stdout );
    while( opiltargetExchangeDataHost() == 0 );

    printf("%s: Host disconnected.\r\n", __FUNCTION__);
    fflush( stdout );
    
    opiltargetDisconnectFromHost(0);
    
    printf("%s: Run again? [y/n]", __FUNCTION__);
    fflush( stdout );
    uinput = getchar();
    while ((d = getchar()) != '\n' && d != EOF);

    if( uinput != 'y' ) break;
    printf("\n");
    fflush( stdout );
    }
    
    printf("%s: Closing OPiL target module.\r\n", __FUNCTION__);
    fflush( stdout );

    return 0;
}

/*
 * buckcontrol.c
 *
 *  Created on: 09.05.2023
 *      Author: LRS
 */

//=============================================================================
/*-------------------------------- Includes ---------------------------------*/
//=============================================================================
#include "buckcontrol.h"
#include "stdio.h"

#include "stypes.h"
//=============================================================================
/*------------------------------- Definitions -------------------------------*/
//=============================================================================

//#define DEBUG

// Control parameters
#define k1           0.01861111f
#define k2           -0.06860324
#define kr           (1.0f / 59.53091245f)

// ADC
#define max_i            8.0f
#define min_i            -8.0f
#define range_i          (max_i - min_i)
#define adc_res_i        4095.0f
#define adc_gain_i       (adc_res_i/range_i)

#define max_v_out        35.0f
#define min_v_out        0.0f
#define range_v_out      (max_v_out - min_v_out)
#define adc_res_v_out    4095.0f
#define adc_gain_v_out   (adc_res_v_out/range_v_out)
//=============================================================================
/*-------------------------------- Functions --------------------------------*/
//=============================================================================
//-----------------------------------------------------------------------------
void buckcontrolInitialize(void)
{

}
//-----------------------------------------------------------------------------
void buckcontrol
(
    void *ms,
    void *sd,
    void *ctl,
    void *ctlrdata
)
{
    stypesMeasurements_t *meas = (stypesMeasurements_t *)ms;
	stypesSimData_t *simdata = (stypesSimData_t *)sd;
	stypesControl_t *control = (stypesControl_t *)ctl;
	stypesControllerData_t *controllerdata = (stypesControllerData_t *)ctlrdata;
    
	float d, i, v;

    i = meas->i / adc_gain_i + min_i;
    v = meas->v / adc_gain_v_out + min_v_out;

	d = simdata->v_ref * kr - k1 * i - k2 * v;
	
	if(d < 0.0f)
		control->d = 0.0f;
	else if(d > 1.0f)
		control->d = 1.0f;
	else
		control->d = d;
	
	controllerdata->t_exec = 42.0f;
	
#ifdef DEBUG
	printf("i=%f, v_out=%f, V_ref=%f\n", i, v, simdata->v_ref);
#endif //DEBUG
}
//-----------------------------------------------------------------------------
//=============================================================================

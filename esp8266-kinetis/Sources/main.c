/* ###################################################################
**     Filename    : main.c
**     Project     : esp8266-kinetis
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-02-22, 18:42, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "SI7005_I2C.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "wifi.h"
#include "si7005.h"
#include "stdio.h"

LDD_TDeviceData *si7005_device_data_p;

uint32_t i = 0;
uint8_t rH, temperat;
uint8_t temperature[2] = {0};
uint8_t humidity[2] = {0};
    /*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  si7005_open(&si7005_device_data_p);
  for(i=0; i<100000; i++);
  read_temperature(&temperat);
  for(i=0; i<100000; i++);
  read_rH( &rH );
  temperature[0] = '0' + temperat/10;
  temperature[1] = '0' + temperat%10;
  humidity[0] = '0' + rH/10;
  humidity[1] = '0' + rH%10;

//  sprintf(sLCDBuffer,"%02i%02i", temperat, rH);

  wifi_open();
  esp8266_init();
  wifi_network_connect();
  wifi_socket_open();
  wifi_send_data(temperature, 2, humidity, 2);
  wifi_socket_close();

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

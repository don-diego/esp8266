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
#include "ASerialLdd1.h"
#include "SI7005_I2C.h"
#include "LED_Red.h"
#include "LED_Green.h"
#include "LED_Blue.h"
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
uint8_t rssi_str[4];
uint8_t rH_raw_value;
uint32_t temperature_raw_value;
uint8_t temperature[5] = {0, 0, '.', 0, 0};
uint8_t humidity[2] = {0};
    /*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
while(1)
{
  LED_Blue_ClrVal(NULL);
  si7005_open( &si7005_device_data_p );
  for(i=0; i<100000; i++);
  read_temperature( &temperature_raw_value );
  for(i=0; i<100000; i++); // wait some time
  read_rH( &rH_raw_value );
  temperature[0] = '0' + temperature_raw_value / 1000;
  temperature[1] = '0' + (temperature_raw_value % 1000)/100;
  temperature[3] = '0' + ((temperature_raw_value % 1000)%100)/10;
  temperature[4] = '0' + ((temperature_raw_value % 1000)%100)%10;
  humidity[0] = '0' + rH_raw_value / 10;
  humidity[1] = '0' + rH_raw_value % 10;
  LED_Blue_SetVal(NULL);
  LED_Red_ClrVal(NULL);
  wifi_open();
  esp8266_init();
  wifi_network_connect();
  wifi_socket_open();
  wifi_send_data(temperature, 5, humidity, 2, rssi_str, 3);
  wifi_socket_close();
  LED_Red_SetVal(NULL);
  LED_Green_ClrVal(NULL);
  for(i=0; i<150000000; i++);
  LED_Green_SetVal(NULL);
}
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

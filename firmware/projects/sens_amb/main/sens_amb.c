/*! @mainpage Sensado Ambiental 
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 * 
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 * 
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 31/10/2025 | Document creation		                         |
 *
 * @author Joaquín Machado (joaquin.machado@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>   // printf
#include <string.h>  // strcmp
#include <inttypes.h>  // PRIx64

#include "sen5x_i2c.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
/*==================[macros and definitions]=================================*/
#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
void convert_and_print_serial(uint16_t* serial_raw) {
    uint64_t serial_as_int = 0;
    sensirion_common_to_integer((uint8_t*)serial_raw, (uint8_t*)&serial_as_int,
                                LONG_INTEGER, 6);
    printf("0x%" PRIx64, serial_as_int);
}
/*==================[external functions definition]==========================*/
void app_main(void){
	// Init
	sensirion_i2c_hal_init(); // init I2C 400 kHz
}
/*==================[end of file]============================================*/
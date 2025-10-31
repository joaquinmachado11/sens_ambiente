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
int16_t error_sen5x = 0;
int16_t error_scd4x = 0;
uint16_t mass_concentration_pm1p0;
uint16_t mass_concentration_pm2p5;
uint16_t mass_concentration_pm4p0;
uint16_t mass_concentration_pm10p0;
int16_t ambient_humidity;
int16_t ambient_temperature;
int16_t voc_index;
int16_t nox_index;
uint16_t co2_concentration;
int32_t temperature;
int32_t relative_humidity;
bool data_ready;
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

	// Start Measurement
    error_sen5x = sen5x_start_measurement();
    if (error_sen5x) {
        printf("Error executing sen5x_start_measurement(): %i\n", error_sen5x);
    }

	data_ready = false;
	co2_concentration = 0;
	temperature = 0;
	relative_humidity = 0;

    while(0) {
        // Read Measurement
        sensirion_i2c_hal_sleep_usec(1000000);

		// Manejo de excepciones scd4x
		error_scd4x = scd4x_get_data_ready_status(&data_ready);
        if (error_scd4x != NO_ERROR) {
            printf("error executing get_data_ready_status(): %i\n", error_scd4x);
            continue;
        }
        while (!data_ready) {
            sensirion_hal_sleep_us(100000);
            error_scd4x = scd4x_get_data_ready_status(&data_ready);
            if (error_scd4x != NO_ERROR) {
                printf("error executing get_data_ready_status(): %i\n", error_scd4x);
                continue;
            }
        }
        error_scd4x = scd4x_read_measurement(&co2_concentration, &temperature,
                                       &relative_humidity);
        if (error_scd4x != NO_ERROR) {
            printf("error executing scd4x read_measurement(): %i\n", error_scd4x);
        } else {	// Muestra de datos scd4x
			printf("CO2 concentration [ppm]: %u\n", co2_concentration);
			printf("Temperature scd4x [m°C] : %i\n", (int)temperature);
			printf("Humidity scd4x [mRH]: %i\n", (int)relative_humidity);
		}
        
        error_sen5x = sen5x_read_measured_values(
            &mass_concentration_pm1p0, &mass_concentration_pm2p5,
            &mass_concentration_pm4p0, &mass_concentration_pm10p0,
            &ambient_humidity, &ambient_temperature, &voc_index, &nox_index);

        if (error_sen5x) {
            printf("Error executing sen5x_read_measured_values(): %i\n", error_sen5x);
        } else {
            printf("Mass concentration pm1p0: %.1f µg/m³\n",
                   mass_concentration_pm1p0 / 10.0f);
            printf("Mass concentration pm2p5: %.1f µg/m³\n",
                   mass_concentration_pm2p5 / 10.0f);
            printf("Mass concentration pm4p0: %.1f µg/m³\n",
                   mass_concentration_pm4p0 / 10.0f);
            printf("Mass concentration pm10p0: %.1f µg/m³\n",
                   mass_concentration_pm10p0 / 10.0f);
            if (ambient_humidity == 0x7fff) {
                printf("Ambient humidity: n/a\n");
            } else {
                printf("Ambient humidity: %.1f %%RH\n",
                       ambient_humidity / 100.0f);
            }
            if (ambient_temperature == 0x7fff) {
                printf("Ambient temperature: n/a\n");
            } else {
                printf("Ambient temperature: %.1f °C\n",
                       ambient_temperature / 200.0f);
            }
            if (voc_index == 0x7fff) {
                printf("Voc index: n/a\n");
            } else {
                printf("Voc index: %.1f\n", voc_index / 10.0f);
            }
            if (nox_index == 0x7fff) {
                printf("Nox index: n/a\n");
            } else {
                printf("Nox index: %.1f\n", nox_index / 10.0f);
            }
        }
    }

    error_sen5x = sen5x_stop_measurement();
    if (error_sen5x) {
        printf("Error executing sen5x_stop_measurement(): %i\n", error_scd4x);
    }

    return 0;
}
/*==================[end of file]============================================*/
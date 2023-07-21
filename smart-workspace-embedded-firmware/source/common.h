/*
 * common.h
 *
 *  Created on: Oct 21, 2020
 *      Author: ddka
 */

#ifndef SOURCE_COMMON_H_
#define SOURCE_COMMON_H_

#include "stdbool.h"

typedef struct{

	/* User Preferences obtained from AWS */
	bool isManualOverrideEnabled;
	char* user_name;
	int light_duty_cycle;
	int fan_duty_cycle;

	/* Updated by RFID Task */
	int rfid_number;

	/* Updated by DHT22 Task */
	int temperature;
	int humidity;

	/* Updated by CO2 Task */
	int co2;

	/* Radar updates the value if person is detected */
	bool isPersonDetected;

} control_t;


typedef struct{
	int temperature;
	int humidity;
} dht_sensor_t;


typedef struct{
	bool isManualOverrideEnabled;
	char* user_name;
	int light_duty_cycle;
	int fan_duty_cycle;
} user_prefs_t;


void common_set_dht_structure(int temperature, int humidity);
void common_get_dht_structure(dht_sensor_t *data);

void common_set_light_duty_cycle(int duty_cycle);
int common_get_light_duty_cycle();

void common_set_fan_duty_cycle(int duty_cycle);
int common_get_fan_duty_cycle();

void common_set_co2_value(int value);
int common_get_co2_value();

void common_set_person_status();
void common_clear_person_status();

void common_set_user_prefs(user_prefs_t *prefs);
void common_get_user_prefs(user_prefs_t *prefs);

void commmon_print_structure();

#endif /* SOURCE_COMMON_H_ */

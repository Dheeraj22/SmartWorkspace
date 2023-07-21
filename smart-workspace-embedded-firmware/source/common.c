/*
 * common.c
 *
 *  Created on: Oct 21, 2020
 *      Author: ddka
 */


#include "common.h"


static control_t controller = {0};


void common_set_dht_structure(int temperature, int humidity){
	controller.temperature = temperature;
	controller.humidity = humidity;
}

void common_get_dht_structure(dht_sensor_t *data){
	data->temperature = controller.temperature;
	data->humidity = controller.humidity;
}

void common_set_light_duty_cycle(int duty_cycle){
	controller.light_duty_cycle = duty_cycle;
}

int common_get_light_duty_cycle(){
	return controller.light_duty_cycle;
}

void common_set_fan_duty_cycle(int duty_cycle){
	controller.fan_duty_cycle = duty_cycle;
}

int common_get_fan_duty_cycle(){
	return controller.fan_duty_cycle;
}

void common_set_co2_value(int value){
	controller.co2 = value;
}

int common_get_co2_value(){
	return controller.co2;
}

void common_set_person_status(){
	controller.isPersonDetected = true;
}

void common_clear_person_status(){
	controller.isPersonDetected = false;
}

void common_set_user_prefs(user_prefs_t *prefs){
	controller.isManualOverrideEnabled = prefs->isManualOverrideEnabled;
	controller.light_duty_cycle = prefs->light_duty_cycle;
	controller.fan_duty_cycle = prefs->fan_duty_cycle;
	controller.user_name = prefs->user_name;
}

void common_get_user_prefs(user_prefs_t *prefs){
	prefs->isManualOverrideEnabled = controller.isManualOverrideEnabled;
	prefs->light_duty_cycle = controller.light_duty_cycle;
	prefs->fan_duty_cycle = controller.fan_duty_cycle;
	prefs->user_name = controller.user_name;
}

void commmon_print_structure(){
	printf("User Name: %s", controller.user_name);
}

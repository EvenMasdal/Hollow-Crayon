#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include "elev.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


int moving;
int last_floor; 
int last_dir;


void ctrl_init(void){
	q_clear_queue();
	elev_init();
	timer_reset();
	
	int floor_sensor = elev_get_floor_sensor_signal();

	if(floor_sensor == -1){
		elev_set_motor_direction(DIRN_UP);
		moving = 1;
		last_dir = 1;
	}

	while(1){
		floor_sensor = elev_get_floor_sensor_signal();

	// Sjekker hvilken etg den har truffet. Setter etg.indikator, stopper motor, skriver at heis er i stopp og setter last_floor til den respektive etg.
		if(floor_sensor != -1){
			elev_set_floor_indicator(floor_sensor);
			elev_set_motor_direction(DIRN_STOP);
			moving = 0;
			last_floor = floor_sensor;
			last_dir = 1;
			break;
		}
	}
}


/*
void ctrl_emergency_stop(void){
	int stop_status = elev_get_stop_signal();

	if(timer_get_status() == 0){
		elev_set_motor_direction(DIRN_STOP);
		moving = 0;
		elev_set_stop_lamp(stop_status);
		if(elev_get_floor_sensor_signal != -1){
			elev_open_door();
			timer_start(3);
			last_floor = elev_get_floor_sensor_signal;
		}
		q_clear_queue();
	}
}
*/

void ctrl_requests(void){
	elev_button_type_t button_type;
	int posDir;
	for(button_type = BUTTON_CALL_UP; button_type <= BUTTON_COMMAND; button_type++){
		for(int floor = 0; floor < 4; floor++){
			switch(button_type){
				case BUTTON_CALL_UP:
					if(floor == 3){ //denne knappen finnes ikke
						break;
					}
					if(elev_get_button_signal(button_type, floor) == 1 && floor != elev_get_floor_sensor_signal()){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						posDir = q_floor_and_dir_to_posDir(floor, 1); 	//1 er retning opp
						q_set_request(posDir);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_CALL_DOWN:
					if(floor == 0){	//denne knappen finnes ikke
						break;
					}
					if(elev_get_button_signal(button_type, floor) == 1 && floor != elev_get_floor_sensor_signal()){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						posDir = q_floor_and_dir_to_posDir(floor, -1); 	//1 er retning opp
						q_set_request(posDir);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_COMMAND:
					if(elev_get_button_signal(button_type, floor) == 1 && floor != elev_get_floor_sensor_signal()){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						posDir = q_floor_and_dir_to_posDir(floor, 1); 	//retning opp
						q_set_request(posDir);
						posDir = q_floor_and_dir_to_posDir(floor, -1);	//retning ned
						q_set_request(posDir);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				default:
					break;
			}
		}
	}
}


void ctrl_move(void){
	if(timer_get_status() < 3.0){
		return;
	}
	timer_reset();
	elev_set_door_open_lamp(0);
	int next_dir = q_get_next_direction(last_floor, last_dir);
	switch(next_dir){
		case -1:
			elev_set_motor_direction(DIRN_DOWN);	//kan elev_set_motor_dir(enum) ta inn int isteden for enum?
			moving = 1;
			last_dir = -1;
			break;
		case 0:
			elev_set_motor_direction(DIRN_STOP); 	//Nødvendig?
			moving = 0;
			break;
		case 1:
			elev_set_motor_direction(DIRN_UP);
			moving = 1;
			last_dir = 1;
			break;
		default:
			elev_set_motor_direction(DIRN_STOP);	//nødvendig?
			moving = 0;
			break;
	}
}

void ctrl_hit_floor(int floor){

	elev_set_floor_indicator(floor);
	if(floor == q_get_next_floor(last_floor,last_dir) && timer_on == 0 && moving == 1){
		elev_set_motor_direction(DIRN_STOP);
		moving = 0;
		elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
		timer_start();
		elev_set_door_open_lamp(1);
		switch(floor){
			case 0: 
				q_clear_floor(0);
				elev_set_button_lamp(BUTTON_CALL_UP, 0, 0);
				break;
			case 1: 
				q_clear_floor(1);
				q_clear_floor(5);
				elev_set_button_lamp(BUTTON_CALL_UP, 1, 0);
				elev_set_button_lamp(BUTTON_CALL_DOWN, 1, 0);
				break;
			case 2: 
				q_clear_floor(2);
				q_clear_floor(4);
				elev_set_button_lamp(BUTTON_CALL_UP, 2, 0);
				elev_set_button_lamp(BUTTON_CALL_DOWN, 2, 0);
				break;
			case 3: 
				q_clear_floor(3);
				elev_set_button_lamp(BUTTON_CALL_DOWN, 3, 0);
				break;
			default:
				break;
		}

	}
	if(timer_on == 0){
		elev_set_door_open_lamp(0);
	}
	last_floor = floor;
	
}





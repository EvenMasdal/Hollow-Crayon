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
int door_status;
int emergency_stop;


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


void ctrl_emergency_stop(void){
	int stop_status = elev_get_stop_signal();
	
	elev_set_motor_direction(DIRN_STOP);
	moving = 0;
	emergency_stop = 1;

	q_clear_queue();
	elev_clear_all_lights();

	elev_set_stop_lamp(stop_status);

	if(elev_get_floor_sensor_signal() != -1){
		elev_set_door_open_lamp(1);
		last_floor = elev_get_floor_sensor_signal();
	}
	while(elev_get_stop_signal()){

	}
	timer_start();
	elev_set_stop_lamp(0);

}


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
					if(elev_get_button_signal(button_type, floor) == 1 /*&& !(floor == elev_get_floor_sensor_signal() && door_status ==1 )*/ ){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						posDir = q_floor_and_dir_to_posDir(floor, 1); 	//1 er retning opp
						q_set_request(posDir);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_CALL_DOWN:
					if(floor == 0){	//denne knappen finnes ikke
						break;
					}
					if(elev_get_button_signal(button_type, floor) == 1 /*&& !(floor == elev_get_floor_sensor_signal() && door_status ==1 )*/){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						posDir = q_floor_and_dir_to_posDir(floor, -1); 	//1 er retning opp
						q_set_request(posDir);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_COMMAND:
					if(elev_get_button_signal(button_type, floor) == 1 /*&& !(floor == elev_get_floor_sensor_signal() && door_status ==1 )*/){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
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

	int next_floor = q_get_next_floor(last_floor, last_dir);
	
	if(next_floor == -1){
		return;
	}

	printf("Moving from %d to %d\n", last_floor, next_floor);
	
	if(emergency_stop == 1 && next_floor == last_floor){
		printf("Nødstopphendelse\n");
		elev_set_motor_direction(-last_dir);
		emergency_stop = 0;
	}else{
		last_dir = q_get_next_direction(last_floor,last_dir);
		elev_set_motor_direction(last_dir);
	}
	moving = 1;
}

void ctrl_hit_floor(int floor){

	elev_set_floor_indicator(floor);

	if(floor == q_get_next_floor(last_floor,last_dir) && timer_on == 0 && door_status == 0){
		elev_set_motor_direction(DIRN_STOP);
		moving = 0;
		door_status = 1;
		timer_start();
		elev_set_door_open_lamp(1);
	}
	if(floor == q_get_next_floor(last_floor,last_dir) && door_status == 1){
		elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
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
	if(timer_get_status() > 3.0){
		timer_reset();
		elev_set_door_open_lamp(0);
		door_status = 0;
	}
	last_floor = floor;
}





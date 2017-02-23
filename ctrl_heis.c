#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include "elev.h"
#include <stdio.h>

bool moving;
static int last_floor; 
static int last_dir;


void ctrl_init(void){
	q_clear_queue();
	elev_init();
	timer_stop();

	int floor_sensor = elev_get_floor_sensor_signal();

	if(floor_sensor == -1){
		elev_set_motor_direction(DIRN_UP);
		moving = true
	}
	
	bool loop = true;

	while(loop){
		floor_sensor = elev_get_floor_sensor_signal();


// Sjekker hvilken etg den har truffet. Setter etg.indikator, stopper motor, skriver at heis er i stopp og setter last_floor til den respektive etg.
		switch(floor_sensor){
			case 0: 
				elev_set_indicator(floor_sensor);
				elev_set_motor_direction(DIRN_STOP);
				moving = false;
				last_floor = floor_sensor;
				//last_dir = 1;  ???????????????????????
				loop = false;
				break;
			case 1:
				elev_set_indicator(floor_sensor);
				elev_set_motor_direction(DIRN_STOP);
				moving = false;
				last_floor = floor_sensor;
				//last_dir = 1;  ???????????????????????
				loop = false;
				break;
			case 2:
				elev_set_indicator(floor_sensor);
				elev_set_motor_direction(DIRN_STOP);
				moving = false;
				last_floor = floor_sensor;
				//last_dir = 1;  ???????????????????????
				loop = false;
				break;
			case 3:
				elev_set_indicator(floor_sensor);
				elev_set_motor_direction(DIRN_STOP);
				moving = false;
				last_floor = floor_sensor;
				//last_dir = 1;  ???????????????????????
				loop = false;
				break;
			default: //går ut i loop igjen helt til floor_Sensor gir utslag
				break;
		}
	}
}



void ctrl_emergency_stop(void){
	int stop_status = elev_get_stop_signal();

	if(timer_get_status() == false){
		elev_set_motor_direction(DIRN_STOP);
		moving = false;
		elev_set_stop_lamp(stop_status);
		if(elev_get_floor_sensor_signal != -1){
			elev_open_door();
			timer_start(3);
			last_floor = elev_get_floor_sensor_signal;
		}
		q_clear_queue();
	}
}


void ctrl_requests(void){
	enum elev_button_type_t button_type;
	int floor;
	int button_signal;
	int array_int;

	for(button_type = BUTTON_CALL_UP; button_type =< BUTTON_COMMAND; button_type++){
		for(floor = 0; floor < 4; floor++){
			switch(button_type){
				case BUTTON_CALL_UP:
					if(elev_get_button_signal(button_type, floor) == 1){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						array_int = q_floor_and_dir_to_array_int(floor, 1); 	//1 er retning opp
						q_set_request(array_int);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_CALL_DOWN:
					if(elev_get_button_signal(button_type, floor) == 1){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						array_int = q_floor_and_dir_to_array_int(floor, -1); 	//1 er retning opp
						q_set_request(array_int);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				case BUTTON_COMMAND:
					if(elev_get_button_signal(button_type, floor) == 1){ 		//hvis trykket. Kanskje & //get_q(floor) !=1 ?
						array_int = q_floor_and_dir_to_array_int(floor, 1); 	//retning opp
						q_set_request(array_int);
						array_int = q_floor_and_dir_to_array_int(floor, -1);	//retning ned
						q_set_request(array_int);
						elev_set_button_lamp(button_type, floor, 1);
					}
					break;
				default:
					break;
			}
		}
	}
	
	q_set_request(floordir);
	elev_set_button_lamp(button_type, floor, verdi);

}


void ctrl_move(void){
	int next_dir = q_get_next_dir(last_floor, last_dir);

	switch(next_dir){
		case -1:
			elev_set_motor_direction(DIRN_DOWN);	//kan elev_set_motor_dir(enum) ta inn int isteden for enum?
			moving = -1;
			break;
		case 0:
			elev_set_motor_direction(DIRN_STOP); 	//Nødvendig?
			moving = 0;
			break;
		case 1:
			elev_set_motor_direction(DIRN_UP);
			moving = 1;
			break;
		default:
			elev_set_motor_direction(DIRN_STOP);	//nødvendig?
			moving = 0;
			break;
	}
	
}



void ctrl_floor_sensor(void){
	int current_floor_sensor = elev_get_floor_sensor_signal();
	int next_floor_array_int = q_get_next_floor(last_floor, last_dir); 
	current_floor_and_dir = q_floor_and_dir_to_array_int(current_floor_sensor, last_dir);
	
	if(next_floor_array_int == current_floor_and_dir){
		elev_set_motor_direction(DIRN_STOP);
		q_clear_order(current_floor_and_dir);		//tar inn array int
		elev_set_button_lamp(BUTTON_COMMAND, current_floor_sensor, 0);
		
		if(current_floor_and_dir = 1 || current_floor_and_dir > 3){
			elev_set_button_lamp(BUTTON_CALL_UP, current_floor_sensor, 0);
		}
		else if(current_floor_and_dir > 1 & current_floor_and_dir < 4){
			elev_set_button_lamp(BUTTON_CALL_DOWN, current_floor_sensor, 0);
		}
		elev_open_door();
		timer_start(3);
	}
	elev_set_indicator(current_floor_sensor);
	last_floor = current_floor_sensor;
}

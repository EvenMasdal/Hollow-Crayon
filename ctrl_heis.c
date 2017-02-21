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

	if(floor_sensor == -1 ){
		elev_set_motor_direction(DIRN_UP);
		moving = true
	}
	
	bool loop = true;

	while(loop){
		floor_sensor = elev_get_floor_sensor_signal();

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
			default:
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
	//for logikk
}

void ctrl_move(void){

}

void ctrl_floor_sensor(void){
	int current_floor_sensor = elev_get_floor_sensor_signal();
	int next_floor = q_get_next_floor(last_floor, last_dir); 
	int floor_and_dir;

//FOR-LOOP ISTEDEN??
switch(current_floor_sensor){
	case 0:
		floor_and_dir = q_floor_and_dir_to_array_int(current_floor_sensor, last_dir);
		if(next_floor == current_floor_and_dir){
			elev_set_motor_direction(DIRN_STOP);
			elev_set_indicator(current_floor_sensor);
			q_clear_order(current_floor_sensor); 			//tar denne inn floor eller array int???????
			elev_open_door();
			timer_start(3);
		}
		elev_set_indicator(current_floor_sensor);
		break;
	case 1:
		
		break;
	case 2:
		
		break;
	case 3:
		
		break;
	default:
		break;
		}
}


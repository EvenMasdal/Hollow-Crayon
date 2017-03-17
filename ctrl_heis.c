#include "ctrl_heis.h"
#include "queue_heis.h"
#include "timer_heis.h"
#include "elev.h"
#include <stdio.h>



int moving;
int last_floor;
int last_dir;
int door_status;
int emergency_stop_count;		//Sier hvor mange ganger på rad heisen har stoppet mellom etasjer.
int emergency_stop_dir;


void ctrl_init(void){
	queue_clear_queue();
	elev_init();
	timer_reset();
	emergency_stop_count = 0;
	
	int floor_sensor = elev_get_floor_sensor_signal();

	if(floor_sensor == -1){
		elev_set_motor_direction(DIRN_UP);
		moving = 1;
		last_dir = 1;
	}

	while(1){
		floor_sensor = elev_get_floor_sensor_signal();
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

	elev_set_motor_direction(DIRN_STOP);
	moving = 0;
	emergency_stop_count++;
	timer_reset();

	printf("Nødstopp\n");

	queue_clear_queue();
	elev_clear_all_lights();						//Denne lagde vi selv og la til i elev

	elev_set_stop_lamp(1);

	if(elev_get_floor_sensor_signal() != -1){
		elev_set_door_open_lamp(1);
		door_status = 1;
		last_floor = elev_get_floor_sensor_signal();
	}
	if(elev_get_floor_sensor_signal() == -1 && emergency_stop_count == 1){
		emergency_stop_dir = last_dir;
	}
	while(elev_get_stop_signal()){

	}
	if(elev_get_floor_sensor_signal() != -1){
		timer_start();
	}
	elev_set_stop_lamp(0);
}


void ctrl_update_requests(void){
	elev_button_type_t button_type;

	for(button_type = BUTTON_CALL_UP; button_type <= BUTTON_COMMAND; button_type++){
		for(int floor = 0; floor < 4; floor++){
			if(!((button_type == BUTTON_CALL_UP && floor == 3) || (button_type == BUTTON_CALL_DOWN && floor == 0))){ //ignorerer knapper som ikke finnes
				if(elev_get_button_signal(button_type, floor) == 1){
					elev_set_button_lamp(button_type, floor, 1);
					//Å trykke en etasjeknapp er det samme som å trykke begge bestillingsknappene i en etasje.
					if(button_type == BUTTON_CALL_DOWN || button_type == BUTTON_COMMAND){
						queue_set_request(queue_floor_and_dir_to_queuePos(floor, -1));
					}
					if(button_type == BUTTON_CALL_UP || button_type == BUTTON_COMMAND){
						queue_set_request(queue_floor_and_dir_to_queuePos(floor, 1));
					}
				}
			}
		}
	}
}



void ctrl_move(void){

	int next_floor = queue_get_next_floor(last_floor, last_dir);
	if(next_floor != -1){
		printf("Kjører fra %d. til %d.\n", last_floor +1 , next_floor+1 );
	}

	
	if(emergency_stop_count > 0 && next_floor == last_floor){					//Hvis det har vært en nødstopp mellom etasjer tror heisen den er i 																				//Hvis det har vært én nødstopp mellom etasjer skal heisen kjøre tilbake
		last_dir = -emergency_stop_dir;											//en etasje. Da må den kjøre "baklengs" for å fullføre bestillingen.
		elev_set_motor_direction(last_dir);
		moving = 1;
	}
	else{
		int next_dir = queue_get_next_direction(last_floor,last_dir);
		elev_set_motor_direction(next_dir);
		if(next_dir != 0){
			moving = 1;
			last_dir = next_dir;
		}
	}
}

void ctrl_hit_floor(int floor){
	emergency_stop_count = 0;
	elev_set_floor_indicator(floor);
	int next_floor = queue_get_next_floor(last_floor,last_dir);

	if(floor == next_floor){
		if(moving == 1){
			elev_set_motor_direction(DIRN_STOP);
			moving = 0;
			printf("Stoppet i %d.\n", floor + 1);
		}
		if(moving == 0 && timer_on == 0 && door_status == 0){
			door_status = 1;
			elev_set_door_open_lamp(1);
			printf("Åpner dør\n");
		}
	}
	if(door_status == 1){														//Hvis døra er åpen i en etasje slettes alle bestillinger i etasjen.
		if(floor == next_floor){
			timer_start();
		}
		queue_clear_floor(floor);
		elev_clear_floor_button_light(floor);
	}
	
	if(timer_get_status() > 3.0 && timer_on == 1){								//lukker døra hvis timeren har vært på i mer enn 3 sekunder
		elev_set_door_open_lamp(0);
		door_status = 0;
		timer_reset();
		printf("Lukker dør\n");
	}
	last_floor = floor;
}
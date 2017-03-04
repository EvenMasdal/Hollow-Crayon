#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include "elev.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


int moving;				//forteller om heisen beveger seg elle rikke
int last_floor; 		//Sier hva forrige passerte etasje er(heisen stoppet ikke nødvendigvis der)
int last_dir;			//Sier hva forrige retning på heisen var
int door_status;		//Sier om døra er åpen
int emergency_stop;		//Sier om heisen holder på å håndtere en nødstopp

/*_____________________________________________________________________________________________________________________________*/

void ctrl_init(void){
	q_clear_queue();		//Sletter køen
	elev_init();			//initialiserer heisen
	timer_reset();			//resetter timeren
	
	int floor_sensor = elev_get_floor_sensor_signal(); //Skaffer etasjeinformasjon

	if(floor_sensor == -1){					//Kjører opp om heisen ikke er i en etasje, setter tilstanden moving til 1 og siste retning til opp
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

/*_____________________________________________________________________________________________________________________________*/



void ctrl_emergency_stop(void){
	//int stop_status = elev_get_stop_signal();
	
	elev_set_motor_direction(DIRN_STOP);
	moving = 0;
	emergency_stop = 1;
	timer_reset();

	q_clear_queue();
	elev_clear_all_lights();

	elev_set_stop_lamp(1);

	if(elev_get_floor_sensor_signal() != -1){
		elev_set_door_open_lamp(1);
		last_floor = elev_get_floor_sensor_signal();
	}
	while(elev_get_stop_signal()){

	}
	if(elev_get_floor_sensor_signal() != -1){
		timer_start();
	}
	elev_set_stop_lamp(0);

}


/*_____________________________________________________________________________________________________________________________*/


void ctrl_requests(void){
	elev_button_type_t button_type;

	for(button_type = BUTTON_CALL_UP; button_type <= BUTTON_COMMAND; button_type++){		//itererer gjennom knappetypene
		for(int floor = 0; floor < 4; floor++){												//itererer gjennom etasjene
			if(!((button_type == BUTTON_CALL_UP && floor == 3) || (button_type == BUTTON_CALL_DOWN && floor == 0))){	//hopper over knapper som ikke finnes opp i 4. og ned i 1.
				if(elev_get_button_signal(button_type, floor) == 1){			//Sjekker om knappen er trykket
					elev_set_button_lamp(button_type, floor, 1);				//Skrur lyset på
					if(button_type == BUTTON_CALL_DOWN){						//Sjekker om en av nedknappene har blitt trykket.
						q_set_request(q_floor_and_dir_to_posDir(floor, -1));	//Setter en request i køen med riktig etasje og retning ned
					}else if(button_type == BUTTON_CALL_UP){
						q_set_request(q_floor_and_dir_to_posDir(floor, 1));		//Setter en request i køen med riktig etasje og retning opp
					}else{
						q_set_request(q_floor_and_dir_to_posDir(floor, 1));		//Setter en request i riktig etasje for både opp og ned. På den måten
						q_set_request(q_floor_and_dir_to_posDir(floor, -1));	//Stopper heisen alltid for å slippe folk av i riktig etasje.
					}
				}
			}
		}
	}
}


/*_____________________________________________________________________________________________________________________________*/


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
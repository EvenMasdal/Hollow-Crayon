#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include "elev.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>


int moving;						//forteller om heisen beveger seg elle rikke
int last_floor; 				//Sier hva forrige passerte etasje er(heisen stoppet ikke nødvendigvis der)
int last_dir;					//Sier hva forrige retning på heisen var
int door_status;				//Sier om døra er åpen
int emergency_stop_count;		//Sier hvor mange ganger heisen har stoppet mellom etasjer på rad

/*_____________________________________________________________________________________________________________________________*/

void ctrl_init(void){
	q_clear_queue();															//Sletter køen
	elev_init();																//initialiserer heisen
	timer_reset();																//resetter timeren
	emergency_stop_count = 0;
	
	int floor_sensor = elev_get_floor_sensor_signal(); 							//Skaffer etasjeinformasjon

	if(floor_sensor == -1){														//Kjører opp om heisen ikke er i en etasje, 
		elev_set_motor_direction(DIRN_UP);										//setter tilstanden moving til 1 og siste retning til opp
		moving = 1;
		last_dir = 1;
	}

	while(1){
		floor_sensor = elev_get_floor_sensor_signal();							//Oppdaterer floor_sensor en gang per loop for å unngå å
		if(floor_sensor != -1){													//få forskjellige verdier når loopen kjører
			elev_set_floor_indicator(floor_sensor);								//Sjekker hvilken etg heisen har truffet. Setter etg.indikator
			elev_set_motor_direction(DIRN_STOP);								//stopper motor, oppdaterer moving og setter last_floor til den respektive etg.
			moving = 0;
			last_floor = floor_sensor;
			last_dir = 1;
			break;
		}
	}
}

/*_____________________________________________________________________________________________________________________________*/



void ctrl_emergency_stop(void){

	elev_set_motor_direction(DIRN_STOP);										//Stopper motoren
	moving = 0;																	//Setter moving til 0 og emergency stopp til 0
	emergency_stop_count++;															
	timer_reset();																//resetter timeren		
	printf("Nødstopp\n");

	q_clear_queue();															//sletter køen
	elev_clear_all_lights();													//slår av alle knappelys

	elev_set_stop_lamp(1);														//skrur på stopplampa

	if(elev_get_floor_sensor_signal() != -1){									//Åpner døra hvis heisen stoppet i en etasje
		elev_set_door_open_lamp(1);
		last_floor = elev_get_floor_sensor_signal();							//Oppdaterer last floor
	}
	while(elev_get_stop_signal()){												//venter på at bruker skal slippe stoppknapp

	}
	if(elev_get_floor_sensor_signal() != -1){									//starter dørtimeren hvis brukeren slipper stoppknappen
		timer_start();															//i en etasje
	}
	elev_set_stop_lamp(0);														//skrur av stopplampen
}


/*_____________________________________________________________________________________________________________________________*/


void ctrl_requests(void){
	elev_button_type_t button_type;

	for(button_type = BUTTON_CALL_UP; button_type <= BUTTON_COMMAND; button_type++){		//itererer gjennom knappetypene
		for(int floor = 0; floor < 4; floor++){												//itererer gjennom etasjene
			if(!((button_type == BUTTON_CALL_UP && floor == 3) || (button_type == BUTTON_CALL_DOWN && floor == 0))){
																				//hopper over knapper som ikke finnes. Det er opp i 4. og ned i 1. etasje
				if(elev_get_button_signal(button_type, floor) == 1){			//Sjekker om knappen er trykket
					elev_set_button_lamp(button_type, floor, 1);				//Skrur lyset på i gjeldene knapp på

					if(button_type == BUTTON_CALL_DOWN || button_type == BUTTON_COMMAND){						//Sjekker om en av nedknappene har blitt trykket.
						q_set_request(q_floor_and_dir_to_queuePos(floor, -1));	//Setter en request i køen med etasje "floor" og retning ned
					}
					else if(button_type == BUTTON_CALL_UP || button_type == BUTTON_COMMAND){
						q_set_request(q_floor_and_dir_to_queuePos(floor, 1));		//Setter en request i køen med etasje "floor" og retning opp
					}
				}
			}
		}
	}
}


/*_____________________________________________________________________________________________________________________________*/


void ctrl_move(void){

	int next_floor = q_get_next_floor(last_floor, last_dir);					//Ber køen om neste etasje
	
	if(next_floor == -1){														//returnerer hvis neste etasje er gjeldene etasje
		return;
	}

	printf("Kjører fra %d. til %d.\n", last_floor +1 , next_floor+1 );			//Printer i Terminal
	
	if(emergency_stop_count > 0 && next_floor == last_floor){					//Hvis det har vært en nødstopp og heisen tror at den allerede er i 
		if(emergency_stop_count == 1){											//Hvis det har vært én nødstopp mellom etasjer skal heisen kjøre tilbake
			last_dir = -last_dir;												//dit den kom fra for å kjøre i riktig retning.
		}
		elev_set_motor_direction(last_dir);										//Kjører heisen
		moving = 1;			 													//Setter tilstanden moving til 1.
	}else{
		int next_dir = q_get_next_direction(last_floor,last_dir);				//Hvis tilfellet over ikke har intruffet skal heisen kjøre som normalt.
		elev_set_motor_direction(next_dir);										//Den henter neste retning fra køen og oppdaterer moving og last direction.
		if(next_dir != 0){														//Usikker på om det er nødvendig med if, er for lost.
			moving = 1;
			last_dir = next_dir;
		}
	}
}

void ctrl_hit_floor(int floor){													//Håndterer at heisen har kommet til en etasje
	emergency_stop_count = 0;
	elev_set_floor_indicator(floor);											//Setter etasjelys i riktig etasje
	int next_floor = q_get_next_floor(last_floor,last_dir);						//får neste etasje fra køen.
	if(floor == next_floor){													//Sjekker om den er i etasjen den skal stoppe i.
		if(moving == 1){														//Hvis den er i bevegelse stopper den og setter moving til 0.
			elev_set_motor_direction(DIRN_STOP);
			moving = 0;
			printf("Stoppet i %d.\n", floor + 1);								//Printer ut hvilken etasje heisen har stoppet i.
		}
		if(moving == 0 && timer_on == 0 && door_status == 0){					//Hvis heisen står stille, timeren ikke er aktiv og døra er lukket
			door_status = 1;													//Åpner døra og setter dørstatus til 1. Og skriver at døra er åpen
			elev_set_door_open_lamp(1);
			printf("Åpner dør\n");
		}
		if(door_status == 1){													//Hvis døra er åpen slettes alle bestillinger i gjeldene etasje og timeren startes
			timer_start();
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
	}
	
	if(timer_get_status() > 3.0 && timer_on == 1){								//Er timeren på og det har gått mer enn 3 sekunder vil heisen
		elev_set_door_open_lamp(0);												//lukke døra, resette/skru av timeren og printe at døra er lukket.
		door_status = 0;
		timer_reset();
		printf("Lukker dør\n");
	}
	last_floor = floor;															//Oppdaterer last floor til forrige etasje som ble passert
}
#include "q_heis.h"
#include <stdio.h>
#include <stdbool.h>

/*
Køsystemet fungerer ved å ha et array på 6 elementer
	queue_arr = [0,0,0,0,0,0]
Elementene er satt opp slik
	["opp i 1." , "opp i 2." , "opp i 3." , "ned i 4." ,"ned i 3." , "ned i 2."]
Og de er 0 for ingen bestilling og 1 for bestilling.
Vi har kalt tallet som representerer hvilket element det er snakk om posDir, siden det inneholder informasjon om både retning og posisjon.
Ved å iterere gjennom lista kan vi enkelt finne ut hvilken bestilling heisen skal først.

Vi har to funksjoner som gjør om fra etasje og retning til "posDir" og omvendt.


*/
//static int q_number_of_floors = 4;

int q_array_size = 6 ; 	//static?

int queue_arr[6] = {0};

/*______________________________________________________________________________________________________________________________________________________*/


void q_set_request(int posDir){											//setter en request etasje og retning i køen
	queue_arr[posDir] = 1;
}

/*______________________________________________________________________________________________________________________________________________________*/


int q_get_next_floor(int last_floor, int last_dir){						//returnerer etasjen til neste ettall i køen.

	int start_pos = q_floor_and_dir_to_posDir(last_floor, last_dir); 	//finner gjeldene posisjon i køsystemet
	int queue_pos = start_pos;											//lagrer startposisjonen for å ikke iterere mer enn én gang gjennom køen

	while (true){														//Bruker while og ikke for siden vi itererer fra en ubestemt posisjon		
		if (queue_arr[queue_pos] == 1){									//Ser om det er en bestilling i gjeldene plass i køen
			return q_posDir_to_floor(queue_pos);						//returnerer neste etasje i køen
		}
		queue_pos++;													//går til neste posisjon i køen
		if (queue_pos == 6){											//begynner fra starten av køen hvis den når slutten 
			queue_pos = 0;
		}
		if (queue_pos == start_pos){									//stopper loopen når det er iterert en runde
			return -1;													//returnerer -1 hvis det ikke er noen bestillinger
		}
	}
}
/*______________________________________________________________________________________________________________________________________________________*/

//henter ut neste retning som motor skal bruke. Heisen vil her stå i ro i en etasje for å finne ut hvilken vei den skal kjøre.
int q_get_next_direction(int last_floor, int last_dir){				

	int next_floor = q_get_next_floor(last_floor, last_dir);			//bruker get_next floor
	
	if(next_floor == -1){												//sier at motoren skal stå stille om køen er tom
		return 0;
	}
	else if(last_floor - next_floor < 0){								//returnerer 1 hvis neste etasje er over forrige etasje
		return 1;
	}
	else if (last_floor - next_floor > 0){								//returnerer -1 hvis neste etasje er under forrige etasje
		return -1;
	}
	else {
	    return 0;														//returnerer 0 hvis neste etasje er samme som forrige.
     }
}



/*______________________________________________________________________________________________________________________________________________________*/

//Tar inn en int 0-5(etasje og retning)

void q_clear_floor(int floor){
	queue_arr[floor] = 0;												//Gjør det motsatte av set request, fjerner en request
}


/*______________________________________________________________________________________________________________________________________________________*/


void q_clear_queue(void){												//Itererer gjennom kø-arrayet og setter alle elementene til 0. Sletter køen
	for (int element = 0; element < q_array_size ; element = element + 1){
		queue_arr[element] = 0;
	}
}


/*______________________________________________________________________________________________________________________________________________________*/


int q_floor_and_dir_to_posDir(int floor, int dir){						//returnerer posDir som samsvarer med etasje og retning. 
	if (dir != 1 && dir !=-1){											//Returnerer -1 dersom dir != -1 eller 1.
		return -1;
	}
	switch(floor){ 														//konverterer retning og etasje til posDir
		
		case 0:
			return 0;

		case 1: 
			if (dir == 1){
				return 1;
			}else {
				return 5;
			}

		case 2:
			if (dir == 1){
				return 2;
			}else {
				return 4;
			}

		case 3:
			return 3;
		
		default:
			return -1;
	}
}


/*______________________________________________________________________________________________________________________________________________________*/

int q_posDir_to_floor(int posDir){										//Får in posDir mellom 0 og 5 og returnerer en etasje mellom 0 og 3.
	switch(posDir){
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return 2;
		case 5:
			return 1;
		default:
			return -1;
	}
}

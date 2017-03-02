#include "q_heis.h"
#include <stdio.h>
#include <stdbool.h>


//static int q_number_of_floors = 4;

//static int q_array_size = 6 ; 	//endre her?

static int queue_arr[6] = {0};

/*______________________________________________________________________________________________________________________________________________________*/

//setter etasje og retning i køen
void q_set_request(int posDir){
	queue_arr[posDir] = 1;
}

/*______________________________________________________________________________________________________________________________________________________*/

//henter neste 1'er i køen, BØR TENKE PÅ FEIL DERSOM LAST_DIR = 0!!!!!!!
//returnerer array int.
int q_get_next_floor(int last_floor, int last_dir){

	int queue_pos = q_floor_and_dir_to_posDir(last_floor, last_dir); //finner gjeldene posisjon i køsystemet
	int start_pos = queue_pos;

	while (true){
		queue_pos++;									//starter på neste etasje
		if (queue_arr[queue_pos] == 1){					//Ser etter en bestilling i etasjen
			return q_posDir_to_floor(queue_pos);	//returnerer neste etasje i køen
		}

		if (queue_pos == 5){							//begynner fra starten av arrayet hvis den når slutten 
			queue_pos = 0;
		}

		if (queue_pos == start_pos){					//stopper loop om den har gått en runde
			return -1;
		}
	}
}
/*______________________________________________________________________________________________________________________________________________________*/

//henter ut neste retning som motor skal bruke. Heisen vil her stå i ro i en etasje for å finne ut hvilken vei den skal kjøre.
int q_get_next_direction(int last_floor, int last_dir){

	int posDir = q_get_next_floor(last_floor, last_dir);
	int next_floor = q_posDir_to_floor(posDir);

	if(last_floor - next_floor > 0){			//gå opp hvis neste etasje er over
		return 1;
	}
	else if (last_floor - next_floor< 0){		//gå ned hvis neste etasje er under
		return -1;
	}
	else {return 0;} 							//dersom den står i den etasjen  den skal til
}



/*______________________________________________________________________________________________________________________________________________________*/

//Tar inn en int 0-5(etasje og retning)

void q_clear_order(int posDir){
	queue_arr[posDir] = 0; 
	
}


/*______________________________________________________________________________________________________________________________________________________*/

//setter alle element i lista til 0
void q_clear_queue(void){
	for (int element = 0; element < q_array_size ; element = element + 1){
		queue_arr[element] = 0;
	}
}


/*______________________________________________________________________________________________________________________________________________________*/

//returnerer posDir som samsvarer med floor og dir. Returnerer -1 dersom dir != -1 eller 1.
int q_floor_and_dir_to_posDir(int floor, int dir){
	if (dir != 1 || dir !=-1){
		return -1;
	}

	switch(floor){ 				//konverterer retning og etasje til arrayint
		
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

int q_posDir_to_floor(int posDir){
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

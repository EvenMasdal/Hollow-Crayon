#include "q_heis.h"
#include <stdio.h>


static int q_number_of_floors = 4;

static int q_array_size = (q_number_of_floors * 2) - 2 ;

static int queue_arr[q_array_size] = {0};

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
		queue_pos++; 			//starter på neste etasje
		if (queue_arr[queue_pos] == 1){	//Ser etter en bestilling i etasjen
			return q_posDir_to_floor(int queue_pos); //returnerer neste etasje i køen
		}

		if (queue_pos == 5){		//starter 
			queuePos = 0;
		}

		if (queue_pos == start_pos){		//stopper loop om den har gått en runde
			return -1;
		}

	}
}
/*______________________________________________________________________________________________________________________________________________________*/

//henter ut neste retning som motor skal bruke. Heisen vil her stå i ro i en etasje for å finne ut hvilken vei den skal kjøre.
int q_get_next_direction(int last_floor, int last_dir){
	int array_int = q_get_next_floor(last_floor, last_dir);
	//er dette nødvendig?? Kanskje vi bare skal lage logikk for array_int?
	// evt fiske logikk i q_next_floor til å returnere 0-3
	int next_floor = q_array_int_to_floor(array_int);

	if(last_floor - next_floor > 0){
		return 1;
	}
	else if (last_floor - next_floor< 0){
		return -1;
	}
	else {return 0}; 		//dersom den står i den etasjen  den skal til!!!!!!!!!!!!!
}



/*______________________________________________________________________________________________________________________________________________________*/

//Tar inn en int 0-5(etasje og retning)

void q_clear_order(int floorDir){
	queue_arr[floorDir] = 0; 
	
}


/*______________________________________________________________________________________________________________________________________________________*/

//setter alle element i lista til 0
void q_clear_queue(void){
	for (element = 0; element < q_array_size ; element = element + 1){
		queue_arr[element] = 0;
	}
}


/*______________________________________________________________________________________________________________________________________________________*/

//returnerer array int som samsvarer med floor og dir. Returnerer -1 dersom dir != -1 eller 1.
int q_floor_and_dir_to_array_int(int floor, int dir){
	switch(floor){ 				//konverterer retning og etasje til arrayint
		case 0:
			if (dir != 1 & dir != -1 ){return -1;}
			return 0;

		case 1: 
			if (dir == 1){return 1;}
			else if (dir == -1){return 5;}
			else {return -1;}

		case 2:
			if (dir == 1){return 2;}
			else if (dir == -1;){return 4;}
			else {return -1;}
		case 3: 
			if (dir != 1 || dir != ){return -1;}
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

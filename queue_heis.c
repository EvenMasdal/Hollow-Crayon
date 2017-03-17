#include "queue_heis.h"
#include <stdio.h>


int queue_array_size = 6;

int queue_arr[6] = {0};


void queue_set_request(int queuePos){
	queue_arr[queuePos] = 1;
}


int queue_get_next_floor(int last_floor, int last_dir){	

	int start_pos = queue_floor_and_dir_to_queuePos(last_floor, last_dir);
	int queue_pos = start_pos;

	while (1){		
		if (queue_arr[queue_pos] == 1){
			return queue_queuePos_to_floor(queue_pos);
		}
		queue_pos++;
		if (queue_pos == 6){
			queue_pos = 0;
		}
		if (queue_pos == start_pos){
			return -1;				//returnerer 1 hvis køen er tom.									
		}
	}
}


int queue_get_next_direction(int last_floor, int last_dir){				

	int next_floor = queue_get_next_floor(last_floor, last_dir);
	
	if(next_floor == -1){					//Dette skjer om køen er tom
		return 0;
	}
	else if(last_floor - next_floor < 0){
		return 1;
	}
	else if (last_floor - next_floor > 0){
		return -1;
	}
	else {
	    return 0;
     }
}


void queue_clear_floor(int floor){
	switch(floor){
		case 0:
			queue_arr[0] = 0;
			break;
		case 1:
			queue_arr[1] = 0;
			queue_arr[5] = 0;
			break;
		case 2:
			queue_arr[2] = 0;
			queue_arr[4] = 0;
			break;
		case 3:
			queue_arr[3] = 0;
			break;
	}
}



void queue_clear_queue(void){
	for (int element = 0; element < queue_array_size ; element = element + 1){
		queue_arr[element] = 0;
	}
}


int queue_floor_and_dir_to_queuePos(int floor, int dir){ 
	if (dir != 1 && dir !=-1){
		return -1;
	}
	switch(floor){
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



int queue_queuePos_to_floor(int queuePos){
	if(queuePos == 5){
		return 1;
	}else if(queuePos == 4){
		return 2;
	}else{
		return queuePos;
	}
}
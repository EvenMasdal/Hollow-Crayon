#include "elev.h"
#include "ctrl_heis.h"
#include "q_heis.h"
#include <stdio.h>
#include <stdbool.h>

int etasje=1;

int main() {
    ctrl_init();	
    while (1){
        ctrl_requests();
	if(moving == 0){
		ctrl_move();
	}
	//for(int i=0; i<6; i++){printf("%d ", queue_arr[i]);}
	etasje = elev_get_floor_sensor_signal();
	//printf(" %d", etasje);		
	if(etasje != -1 ){
		ctrl_hit_floor(etasje);
	}
	if(elev_get_stop_signal()){
		elev_set_motor_direction(0);
		break;
	}
	
    }printf("\n \n");
}


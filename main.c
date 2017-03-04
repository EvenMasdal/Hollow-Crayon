#include "elev.h"
#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include <stdio.h>
#include <stdbool.h>

int etasje=1;

int main() {
    ctrl_init();	
    while (1){
    	if(elev_get_stop_signal() == 1){
    		ctrl_emergency_stop();
    	}

        ctrl_requests();
	
		if(moving == 0){
			ctrl_move();
		}

		etasje = elev_get_floor_sensor_signal();		
		if(etasje != -1){
			ctrl_hit_floor(etasje);
		}
    }
}


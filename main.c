#include "elev.h"
#include "ctrl_heis.h"
#include <stdio.h>



int main() {
    ctrl_init();
    int floor_sensor;
    while (1){
    	if(elev_get_stop_signal() == 1){
    		ctrl_emergency_stop();
    	}

        ctrl_update_requests();
	
		if(moving == 0 && door_status == 0){
			ctrl_move();
		}

		floor_sensor = elev_get_floor_sensor_signal();
		if(floor_sensor != -1){
			ctrl_hit_floor(floor_sensor);
		}
    }
}
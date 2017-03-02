#include "elev.h"
#include "ctrl_heis.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum { false, true } bool;

int main() {
    ctrl_init();
/*
    while (1){
        if (elev_get_stop_signal()){
            ctrl_emergency_stop();
        }
        ctrl_request();
        if (moving == 0){
            ctrl_move();
        }
        if (elev_get_floor_sensor() != -1){
            ctrl_floor_sensor();
        }
    }
*/
}
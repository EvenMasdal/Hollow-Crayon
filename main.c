#include "elev.h"
#include "ctrl_heis.h"
#include "q_heis.h"
#include "timer_heis.h"
#include <stdio.h>
#include <stdbool.h>

int etasje=1;

int main() {
    ctrl_init();									//initialiserer heisen
    while (1){										//Kjører i en evig loop
    	if(elev_get_stop_signal() == 1){			//Kjører nødstopp om stoppknappen trykkes
    		ctrl_emergency_stop();
    	}

        ctrl_requests();							//Oppdaterer alt av kø og kølys
	
		if(moving == 0 && timer_on == 0){			//Ber motoren kjøre om heisen står stille og timeren er av
			ctrl_move();
		}
		etasje = elev_get_floor_sensor_signal();	//Oppdaterer etasjen en gang per loop så etasje signalet ikke
		if(etasje != -1){							//endrer seg midt i en funksjon
			ctrl_hit_floor(etasje);					//Kjører om heisen treffer en etasje
		}
    }
}


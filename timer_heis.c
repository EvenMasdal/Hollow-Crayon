#include "timer_heis.h"
#include <stdio.h>
#include <time.h>


static clock_t start;
int timer_on = 0;

void timer_start(void){
 	start = clock();
	timer_on = 1;
}

void timer_reset(void){
	timer_on = 0;
}

double timer_get_status(void){
 	clock_t diff = clock()- start;
  	double time_elapsed = (double)(diff)/CLOCKS_PER_SEC;
  return time_elapsed;
}
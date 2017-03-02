#include "timer_heis.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

static clock_t seconds;

void timer_start(){
  clock_t start = clock();
}

int timer_get_status(void){
  clock_t diff = clock() - start;
  int msec = diff * 1000 / CLOCKS_PER_SEC;
  return (msec) > 3000;
}

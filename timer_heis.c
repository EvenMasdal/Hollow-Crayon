#include "timer_heis.h"
#include <stdio.h>
#include <time.h>

static time_t seconds

void timer_start(){

  seconds = time(NULL);
  
  return(0);
}

bool timer_get_status(void){
  return (time(NULL) - seconds) > 3;
}

main(){
  timer_start()
  int b;
  for(int i = 0; i<10000; i+=1){
      b = 1 + 1;
  }
  timer_get_status();
  
}

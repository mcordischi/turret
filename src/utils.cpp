#include "utils.h"
#include<unistd.h>
#include <sys/timeb.h> //time in milliseconds

void wait(int seconds){
    usleep(seconds * 1000000);
}


double getNow(){
    timeb tb;
    ftime(&tb);
    return  (double)(tb.millitm + (tb.time & 0xfffff) * 1000)/1000.0; //now

}

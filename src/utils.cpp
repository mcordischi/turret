#include "utils.h"
#include<unistd.h>

void wait(int seconds){
    usleep(seconds * 1000000);
}

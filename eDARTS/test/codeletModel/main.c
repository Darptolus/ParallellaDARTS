#include <stdlib.h>
#include <stdio.h>
#include <e-hal.h>
#include "darts_print_server.h"

#define INIT_SIGNAL 0x6004
#define FINAL_SIGNAL 0x6008

int main(int argc, char *argv[]){
    e_platform_t platform;
    e_epiphany_t dev;

    //Initalize Epiphany device
    e_init(NULL);
    e_reset_system();//reset Epiphany
    e_get_platform_info(&platform);
    e_open(&dev, 0, 0, 4, 4);
    start_printing_server();

    unsigned number = 0;

    e_load_group("e_darts_tp_test.elf", &dev, 0, 0, 4, 4, E_FALSE);

    // Set the initial value for the flags
    e_write(&dev,0,0, INIT_SIGNAL,&number,sizeof(number)); // startSignal
    e_write(&dev,0,0, FINAL_SIGNAL,&number,sizeof(number)); // startSignal

    e_start_group(&dev);
    // wait until consumer is done
    while(number != 1)
    {
        e_read(&dev,0,0,FINAL_SIGNAL,&number,sizeof(number));
    }

    stop_printing_server();
    e_close(&dev);
    e_finalize();
    return EXIT_SUCCESS;
}


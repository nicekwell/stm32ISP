#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stm32isp.h>

int main(int argc, char *argv[])
{
    unsigned char get[100];
    int i;

    stm32isp_init("/dev/ttyUSB0", 57600, 8, 1, 'N', 30);
    
    printf("syncing...");
    if(stm32isp_sync()) printf("sync ok\n");
    else printf("sync fail\n\n");

    stm32isp_get_command();
    stm32isp_get_ID_command();

    printf("erasing flash...\n");
    if(stm32isp_erase_all()) printf("erase flash done\n");
    else printf("erase flash fail\n\n");

    printf("starting download...\n");
    if(stm32isp_write_bin(argv[1])) printf("down success\n");
    else printf("down fail\n\n");

    printf("starting verify...\n");
    if(stm32isp_verify(argv[1])) printf("verify success\n");
    else printf("verify fail\n");
    
    stm32isp_close();
    return 0;
}

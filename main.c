#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stm32isp.h>

int main(int argc, char *argv[])
{
    if(argc != 3){
        printf("parameter error!\nexample: stm32isp /dev/ttyUSB0 stm32_test.bin\n");
        return 0;
    }

    stm32isp_init(argv[1], 57600, 8, 1, 'N', 30);
    
    printf("syncing...");
    if(stm32isp_sync()) printf("sync ok\n");
    else printf("sync fail\n\n");

    stm32isp_get_command();
    stm32isp_get_ID_command();

    printf("erasing flash...\n");
    if(stm32isp_erase_all()) printf("erase flash done\n");
    else printf("erase flash fail\n\n");

    printf("starting download...\n");
    if(stm32isp_write_bin(argv[2])) printf("download success\n");
    else printf("download fail\n\n");

    printf("starting verify...\n");
    if(stm32isp_verify(argv[2])) printf("verify success\n");
    else printf("verify fail\n");
    
    stm32isp_close();
    return 0;
}

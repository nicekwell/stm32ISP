#include <stdio.h>
#include <unistd.h>
#include <serial.h>

int main(int argc, char *argv[])
{
    unsigned char get[100];
    int num, i;

    printf("open\n");
    uart_open("/dev/ttyUSB0", 115200, 8, 1, 'N', 30);
    
    printf("send\n");
    uart_writeB(0x55);
    usleep(1000000);

    printf("check\n");
    printf("in waiting: %d\n", uart_inWaiting());
    
    printf("read\n");
    num = uart_read(get, 10);
    printf("get num %d\n", num);
    for(i=0;i<num;i++)
        printf("%x\n", get[i]);

    printf("close\n");
    uart_close();
    return 0;
}

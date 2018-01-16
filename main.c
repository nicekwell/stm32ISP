#include <stdio.h>
#include <unistd.h>
#include <wiringSerial.h>

int fd;

int main(int argc, char *argv[])
{
    unsigned char get[100];
    int num, i;

    printf("open\n");
//    fd = serialOpen("/dev/ttyUSB0", 57600, );
    fd = serialOpen("/dev/ttyUSB0", 57600, 8, 1, 'N', 30);
    
    printf("send\n");
    get[0] = 0x55;
    get[1] = 0xcc;
    serialWrite(fd, get, 2);
    usleep(10000);

    printf("check\n");
    printf("in waiting: %d\n", serialDataAvail(fd));
    
    printf("read\n");
    num = serialRead(fd, get, 3);
    printf("get num %d\n", num);
    for(i=0;i<num;i++)
        printf("%x\n", get[i]);

    printf("close\n");
    serialClose(fd);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <wiringSerial.h>

int fd;

typedef struct {
    unsigned char bootloaderversion;    //1字节，bootloader版本
    unsigned char cmd_count;    //支持的所有指令个数
    unsigned char cmd[16];      //支持的所有指令
    unsigned int PID;       //product ID
} stm32info_t;
stm32info_t stm32info;

static int waitACK()    //等待接收数据，第一个字节如果是0x79则认为ok返回1，如果是0x1f则认为失败返回0
{
    while(1){
        if(0 != serialDataAvail(fd)) {
            if(0x79 == serialGetchar(fd))
                return 1;
            else
                return 0;
        }
        usleep(1000);
    }
}

int stm32_sync()
{
    printf("stm32_sync\n");
    serialFlush(fd);
    while(1) {
        serialPutchar(fd, 0x7f);
        if(waitACK()) {
            printf("sync ok\n");
            return 1;
        }
        usleep(10000);
    }
    serialFlush(fd);
}
int stm32_get_command()
{
    unsigned char get[32];
    int i, j;
    printf("%s\n", __func__);
    serialPutchar(fd, 0x00);
    serialPutchar(fd, 0xff);
    waitACK();
    //下面开始接收数据
    i=0;
    while(1) {
        get[i] = serialGetchar(fd);     //读取数据，如果没有数据的话则会等待
        if(get[i] == 0x79)      //结束
            break;
        else if(get[i] == 0x1f)
            return -1;
        i++;
    }
    //数据处理
    stm32info.bootloaderversion = get[1];
    stm32info.cmd_count = get[0];
    for(j=0;j < stm32info.cmd_count;j++)
        stm32info.cmd[j] = get[j+2];

    serialFlush(fd);
    return 1;
}
int stm32_get_ID_command()
{
    unsigned char get[16];
    int i, j;
    printf("%s\n", __func__);
    serialPutchar(fd, 0x02);
    serialPutchar(fd, 0xfd);
    waitACK();
    //接收数据
    serialGetchar(fd);
    stm32info.PID = serialGetchar(fd);
    stm32info.PID <<= 8;
    stm32info.PID += serialGetchar(fd);
    waitACK();
    
    serialFlush(fd);
    return 1;    
}
int stm32_erase_all()
{
    printf("%s\n", __func__);
    serialPutchar(fd, 0x43);
    serialPutchar(fd, 0xbc);
    waitACK();
    serialPutchar(fd, 0xff);
    serialPutchar(fd, 0x00);
    waitACK();
    printf("erase flash done\n");
    serialFlush(fd);
    return 1;
}

int main(int argc, char *argv[])
{
    unsigned char get[100];
    int i;

    printf("open\n");
    fd = serialOpen("/dev/ttyUSB0", 57600, 8, 1, 'N', 30);
    
    stm32_sync();
    
    stm32_get_command();
    printf("get bootloader version is 0x%x\n", stm32info.bootloaderversion);
    printf("get %d support commands: ", stm32info.cmd_count);
    for(i=0;i<stm32info.cmd_count;i++)
        printf("0x%x ", stm32info.cmd[i]);
    printf("\n");

    stm32_get_ID_command();
    printf("get PID is 0x%x\n", stm32info.PID);

    stm32_erase_all();

    serialClose(fd);
    return 0;
}

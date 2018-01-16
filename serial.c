#include <serial.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>

#define FALSE  -1
#define TRUE   0

static int fd = FALSE;

static int _UART_Open(int fd,char* port)
{
    fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);
    if (FALSE == fd){
        perror("Can't Open Serial Port");
        return(FALSE);
    }
    if(fcntl(fd, F_SETFL, 0) < 0){
        printf("fcntl failed!\n");
        return(FALSE);
    } else {
        // printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(0 == isatty(STDIN_FILENO)){
        printf("standard input is not a terminal device\n");
        return(FALSE);
    }
    return fd;
}
static void _UART_Close(int fd)
{
    close(fd);
}

static int _UART_Set(int fd,int speed,int databits,int stopbits,int parity, int timeout)
{ 
    int   i; 
    // int   status; 
    int   speed_arr[] = { B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
    int   name_arr[] = {230400, 115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400, 19200,  9600, 4800, 2400, 1200,  300 };  
    struct termios options; 

    if(tcgetattr( fd,&options)  !=  0){  
        perror("SetupSerial 1");     
        return(FALSE);  
    }
    for(i= 0;i < sizeof(speed_arr) / sizeof(int);i++) { 
        if  (speed == name_arr[i]) {        
            cfsetispeed(&options, speed_arr[i]);  
            cfsetospeed(&options, speed_arr[i]);   
        }
    }  
    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;

//    switch(flow_ctrl){
    switch(0){
    case 0 :
        options.c_cflag &= ~CRTSCTS;
        break;
    case 1 :
        options.c_cflag |= CRTSCTS;
        break;
    case 2 :
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    }
    
    options.c_cflag &= ~CSIZE; 
    switch (databits){   
    case 5 :
        options.c_cflag |= CS5;
        break;
    case 6 :
        options.c_cflag |= CS6;
        break;
    case 7 :     
        options.c_cflag |= CS7; 
        break;
    case 8:     
        options.c_cflag |= CS8;
        break;   
    default:    
        fprintf(stderr,"Unsupported data size\n"); 
        return (FALSE);
    }
    switch (parity) {   
    case 'n':
    case 'N': 
        options.c_cflag &= ~PARENB;  
        options.c_iflag &= ~INPCK;     
        break;  
    case 'o':   
    case 'O':    
        options.c_cflag |= (PARODD | PARENB);  
        options.c_iflag |= INPCK;              
        break;  
    case 'e':  
    case 'E':   
        options.c_cflag |= PARENB;        
        options.c_cflag &= ~PARODD;        
        options.c_iflag |= INPCK;       
        break;
    case 's': 
    case 'S': 
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;  
    default:   
        fprintf(stderr,"Unsupported parity\n");    
        return (FALSE); 
    }  
    switch (stopbits){
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");  
        return (FALSE); 
    } 
                                        
    options.c_oflag &= ~OPOST; 
//    options.c_cc[VTIME] = 1;
    options.c_cc[VTIME] = timeout;
    options.c_cc[VMIN] = 1;
    tcflush(fd,TCIFLUSH);
    if(tcsetattr(fd,TCSANOW,&options) != 0){ 
        perror("com set error!\n");   
        return (FALSE);  
    } 
    return (TRUE);  
}

static int _UART_Recv(int fd, unsigned char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;
    
    struct timeval time;
    
    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);
    
    time.tv_sec = 10;
    time.tv_usec = 0;
    
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel){
        len = read(fd,(char *)rcv_buf,data_len);
        return len;
    } else {
        return FALSE;
    }
}

static int _UART_Send(int fd, unsigned char *send_buf,int data_len)
{
    int ret;
    ret = write(fd,(char *)send_buf,data_len);
    if (data_len == ret ){
        return ret;
    } else {    
        tcflush(fd,TCOFLUSH);
        return FALSE;
    }
}

/*************************** 接口函数 *******************/
/*
  打开端口。timeout的单位是百毫秒。
  例：uart_open("/dev/tty.usbserial", 115200, 8, 1, 'N', 30)
 */
void uart_open(char *filename, int baudrate, int databits, int stopbits, char parity, int timeout)
{
    int ret;
    fd = _UART_Open(fd, filename);
    if(FALSE == fd){
        printf("open error\n");
        exit(1);
    }
    ret = _UART_Set(fd, baudrate, databits, stopbits, parity, timeout);
    if (FALSE == ret){
        printf("Set Port Error\n");
        exit(1);
    }    
}
void uart_write(unsigned char *p, int len)
{
    _UART_Send(fd, p, len);
}
void uart_writeB(unsigned char dat)
{
    uart_write(&dat, 1);
}
int uart_read(unsigned char *p, int len)
{
    return _UART_Recv(fd, p, len);
}
void uart_close()
{
    if(fd)
        _UART_Close(fd);
}
int uart_inWaiting()    //获取缓冲区里已经接收到的数据字节数
{
    int num;
    ioctl(fd, FIONREAD, &num);
    return num;
}


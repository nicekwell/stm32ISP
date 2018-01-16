#ifndef __SERIAL_H__
#define __SERIAL_H__

/*
  和python的串口有所区别。
  具体过程已经封装在了文件里，不用关心，使用方法：
  1、uart_init初始化串口，指定串口的设备文件名称和波特率，波特率取值范围：
  230400, 115200, 57600, 38400, 19200,  9600,  4800,  2400,  1200,  300,
  38400, 19200,  9600, 4800, 2400, 1200,  300
  2、调用uart_write发送一串数据或uart_writeB发送一个字节。
  这两个发送函数是把数据放到缓冲区，执行速度是很快的，但发送完成还需要时间。
  3、调用uart_read接收数据。
  和python的串口一样，接收到数据是不会有中断事件的，需要程序查询。
  关于此函数具体用法看函数的介绍。
  4、程序结束时调用uart_close关闭串口。
*/

/*
  打开端口。timeout的单位是百毫秒。
  例：uart_open("/dev/tty.usbserial", 115200, 8, 1, 'N', 30)
 */
void uart_open(char *filename, int baudrate, int databits, int stopbits, char parity, int timeout);
void uart_write(unsigned char *p, int len);
void uart_writeB(unsigned char dat);
int uart_read(unsigned char *p, int len);
void uart_close();


#endif

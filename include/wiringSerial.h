/*
 * wiringSerial.h:
 *	Handle a serial port
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

//extern int   serialOpen      (const char *device, const int baud) ;   //wiringPi默认
/* nicek
   例：serialOpen("/dev/tty.usbserial", 115200, 8, 1, 'N', 30)
   关于timeout：
     1、单位是100ms
     2、缓冲区有数据时，即使数据个数少于read传入的个数，也会立即读取数据后返回，不会延时等待；
        仅当缓冲区里没有任何数据时，才会进行延时等待，等到有数据或者超时后返回。
*/
extern int   serialOpen      (const char *device, const int baud, const int databits, const int stopbits, const char parity, const int timeout);
extern void  serialClose     (const int fd) ;
extern void  serialFlush     (const int fd) ;   //清空缓冲区，发送和接收都清空
extern void  serialPutchar   (const int fd, const unsigned char c) ;    //发送一个字符
extern void  serialPuts      (const int fd, const char *s) ;    //发送字符串，s是字符串，不发送字符串结尾'\0'
extern void  serialWrite     (const int fd, const char *s, const int count);    //nicek 写入数据块
extern void  serialPrintf    (const int fd, const char *message, ...) ; //类似于printf
extern int   serialDataAvail (const int fd) ;   //检查缓冲区里的数据个数
extern int   serialGetchar   (const int fd) ;   //返回读取的字符
extern int   serialRead      (const int fd, char *p, const int count);        //nicek 读取数据块，写入到p，返回读取到的个数
    //如果缓冲区里没有数据，则会阻塞等待，直到有一个数据立马读出返回
    //如果缓冲区里有数据n，且n<count，则读出n个数据后返回，不会等待
    //如果缓冲区里有数据n，且n>count，则读出count个数据后返回，缓冲区里有剩余数据

#ifdef __cplusplus
}
#endif

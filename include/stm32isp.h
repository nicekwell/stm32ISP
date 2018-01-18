#ifndef __STM32_H__
#define __STM32_H__

/*
  本驱动基于已经ok的串口驱动，包括串口初始化、关闭、收发、缓冲区操作。
 */
int stm32isp_init(const char *device, const int baud, const int databits, const int stopbits, const char parity, const int timeout); //成功返回1，失败返回0
void stm32isp_close();


int stm32isp_sync();               //同步波特率，成功返回1，失败返回0
int stm32isp_get_command();        //获取ID和command列表，信息保存在驱动内部结构体，不输出，但会进行打印
                                //成功返回1，失败返回0
int stm32isp_get_ID_command();     //获取PID，信息保存在驱动内部结构体，不输出，但会进行打印
                                //成功返回1，失败返回0
int stm32isp_erase_all();          //全擦flash，成功返回1，失败返回0
int stm32isp_write_bin(char *p);   //传入bin文件路径，写入bin文件到flash，成功返回1，失败返回0
int stm32isp_verify(char *p);      //传入bin文件路径，根据bin文件大小读取flash内容，并把两个比较
                                //验证成功返回1，失败返回0


#endif

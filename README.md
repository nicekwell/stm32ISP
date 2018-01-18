
# 概述

stm32串口ISP程序，基于c语言。  
在ubuntu和mac下测试都ok，ubuntu下需要修改/dev/ttyUSB0权限，mac下不需要修改。

使用方法：  
**stm32isp /dev/ttyUSB0 stm32_test.bin**  
mac下的usb串口节点是 /dev/tty.usbserial

注：

1. 波特率固定为57600不允许修改。实际上测试了各种波特率，只有57600和38400可以稳定下载（程序写的还不太稳定吧），所以波特率固定为了57600。
2. **下载的是bin文件，不是hex文件**。bin文件是纯粹的编码，hex文件包含了地址信息。keil默认不生成bin文件，生成bin文件的方法可网上查找。


# 下载协议

stm32官方文档已提交到本工程 documents 目录下。<https://github.com/nicekwell/stm32ISP/raw/master/documents/stm32isp%20application%20note.pdf>

## 1、硬件

## 2、sync

## 3、get command

## 4、get version & read protection

## 5、get ID command

## 6、Erase Memory command

## 7、Write Memory command

## 8、Read Memory command

## 9、Go command


# 代码说明
根据上面的下载协议，封装成了模块，这里介绍本程序结构和模块的使用，以便移植到其他地方。

## 程序结构

**wiringSerial.c wiringSerial.h**  
串口驱动，在树莓派[wiringPi](https://github.com/WiringPi/WiringPi)基础上修改，增加了数据块读写代码，增加了更多设置项。  
这部分驱动在mac、桌面ubuntu、树莓派上都可以使用。

**stm32isp.c stm32isp.h**  
下载逻辑代码，基于串口驱动，串口的打开、关闭操作也归于这部分控制。  
以上两个部分，serial完全归于stm32isp，外部使用stm32isp时不需要再管串口了。

**main.c**  
程序交互的实现，调用上面stm32isp接口实现下载。

下面着重介绍的是stm32isp部分的接口函数。

## stm32isp接口
stm32isp使用前提是串口工作正常。

### stm32isp_init
`int stm32isp_init(const char *device, const int baud, const int databits, const int stopbits, const char parity, const int timeout);`  
使用前需要先调用此函数，主要是完成串口初始化（串口的打开和关闭也交给stm32isp模块管理）。  
例：`stm32isp_init("/dev/ttyUSB0", 57600, 8, 1, 'N', 30);`  
注：此驱动测试只有57600和38400两个波特率可以正常下载。

### stm32isp_close
`void stm32isp_close();     //成功返回1，失败返回0`  
下载结束后调用此函数，主要是完成串口关闭（串口的打开和关闭也交给stm32isp模块管理）。  

### stm32isp_sync
`int stm32isp_sync();`  
同步波特率，成功返回1，失败返回0。
打开串口后第一步就需要sync，和单片机同步波特率。

### stm32isp_get_command
`int stm32isp_get_command()`  
获取ID和command列表，信息保存在驱动内部结构体，不输出，但会进行打印信息。成功返回1，失败返回0。  
这一步不是下载必须执行的。

### stm32isp_get_ID_command
`int stm32isp_get_ID_command();`  
获取PID，信息保存在驱动内部结构体，不输出，但会进行打印。成功返回1，失败返回0。  
这一步不是下载必须执行的。

### stm32isp_erase_all
`int stm32isp_erase_all();`
全擦flash，成功返回1，失败返回0。  
下载之前先擦除flash。

### stm32isp_write_bin
`int stm32isp_write_bin(char *p);`  
传入bin文件路径，写入bin文件到flash，成功返回1，失败返回0。  
注意写入的是bin文件，不是hex文件。  
bin文件路径可能由main函数的argv参数传入。

### stm32isp_verify
`int stm32isp_verify(char *p);`  
传入bin文件路径，根据bin文件大小读取flash相应大小内容，并把两个比较，验证成功返回1，失败返回0。  
这一步可选，为了保险可下载后验证一下。






# 概述

stm32串口ISP程序，基于c语言。  
在ubuntu和mac下测试都ok，ubuntu下需要修改/dev/ttyUSB0权限，mac下不需要修改。  
本程序在stm32f103c8t6上ok，根据官方文档，其他型号单片机下载协议相同，但没有测试过。

使用方法：  
**stm32isp /dev/ttyUSB0 stm32_test.bin**  
mac下的usb串口节点是 /dev/tty.usbserial

注：

1. 波特率固定为57600不允许修改。实际上测试了各种波特率，只有57600和38400可以稳定下载（程序写的还不太稳定吧），所以波特率固定为了57600。
2. **下载的是bin文件，不是hex文件**。bin文件是纯粹的编码，hex文件包含了地址信息。keil默认不生成bin文件，生成bin文件的方法可网上查找。

# 广告

老婆做毕业设计，给我下了死命令，一定要找300个程序员研究一下，请各位同行做个调查问卷，可扫描下面二维码或者直接点击此链接：<https://www.wjx.cn/jq/17710478.aspx>，多谢！

![ad.jpg](https://github.com/nicekwell/stm32ISP/raw/master/documents/ad.jpg)

# 下载协议

stm32官方文档已提交到本工程 documents 目录下。<https://github.com/nicekwell/stm32ISP/raw/master/documents/stm32isp%20application%20note.pdf>

## 1、硬件
首先要让stm32进入bootloader启动：

引脚 | 电平
:-: | :-: 
BOOT0 | 高电平
BOOT1 | 低电平

这样启动后就会从system分区启动，开始接收串口数据。

连接串口1：

引脚 | 功能
:-: | :-:
PA9 | TXD，连接host RXD
PA10 | RXD，连接host TXD

## 2、sync
以下数据指的是响应host或device端发送的数据。

host | device | note
:-: | :-: | :-:
0x7f | -  | 发送0x7f，单片机收到后会自动匹配波特率。
\- | 0x79(ACK)/0x1F(NACK) | device返回ACK或NACK，表示对host的反应。
 
 适配波特率这一步是无条件执行的，执行完这一步之后就可以接收各种指令。  
下面就介绍各个指令的功能和数据协议。

## 3、get command

在上面已经适配波特率情况下可以执行此指令。  
【指令码】0x00  
【功能】获取stm32里bootloader版本号，以及所有支持的指令代码。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x00 + 0xff | - | 
\- | 0x79(ACK)/0x1F(NACK) | 
\- | N | 1字节，表示下面要接收到的字节数。bootloaderversion字节数 + 所有指令字节数 = N+1
\- | bootloader version | 1字节，如0x21代表2.1版本
\- | 所有支持的指令 | 多个字节，每个字节数据都表示一个支持的指令
\- | 0x79(ACK)/0x1F(NACK) | 指令执行结束后会返回0x79

## 4、get version & read protection
【指令码】0x01  
【功能】获取stm32里bootloader版本号，读取保护状态。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x01+0xfe | - |
\- | 0x79(ACK)/0x1F(NACK) | 
\- | bootloader version | 
\- | 2个字节 | 这两个字节和保护状态有关
\- | 0x79(ACK)/0x1F(NACK) |

## 5、get ID command
【指令码】0x02  
【功能】获取stm32 PID（product ID，不是芯片唯一识别码）。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x02+0xfd | - | 
\- | 0x79(ACK)/0x1F(NACK) | 
\- | N | 1字节，表示下面 PID字节数 - 1
\- | PID | 多字节(上一个字节已指明字节数)，先传高位后传低位。我这次用的stm32f103c8t6是2字节。
\- | 0x79(ACK)/0x1F(NACK) | 

## 6、Erase Memory command
【指令码】0x43  
【功能】擦除flash，可以全擦或擦除一部分，这里只介绍全擦。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x43+0xbc | - |
\- | 0x79(ACK)/0x1F(NACK) | 
0xff+0x00 | \- | 这是全擦指令
\- | 0x79(ACK)/0x1F(NACK) | 

## 7、Write Memory command
【指令码】0x31  
【功能】  
写存储器，可以写任意的RAM、flash，我们写程序就用这个。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x31+0xCE | - | 
\- | 0x79(ACK)/0x1F(NACK) | 
addr | - | 4字节，下载地址。用户flash起始地址是0x08000000。**先发高位，后发低位**
addr checksum | - | 1字节，地址的checksum，就是上面4字节数据的异或。
\- | 0x79(ACK)/0x1F(NACK) | 
count | - | 1字节，表示后面将要传输的字节数，范围(0, 255]。**字节数 = 这个值+1**，也就是说最大传输256字节。
data | - | 多字节，字节数 = count + 1，最大256字节。**这里下载进去的是bin文件，不是hex。。**
checksum | - | 1字节，上面的data数据，以及数据个数count的checksum。注意这里的checksum包含**数据和个数**。
\- | 0x79(ACK)/0x1F(NACK) | 

注：

1. 对于写flash，用户flash的起始地址是0x08000000。
2. 对于写flash，这里最多一次写256字节，写一个bin文件需要多次使用此协议写入。

## 8、Read Memory command
【指令码】0x11  
【功能】  
读取stm32内存，可以读取stm32任意地址的RAM、flash等数据。  
我们常在下载完成后把flash内容读取出来验证。  
【数据协议】

host | device | note
:-: | :-: | :-:
0x11+0xEE | 0x79(ACK)/0x1F(NACK) | 
addr | - | 4字节，下载地址。用户flash起始地址是0x08000000。**先发高位，后发低位**
addr checksum | - | 1字节，地址的checksum，就是上面4字节数据的异或。
\- | 0x79(ACK)/0x1F(NACK) | 
count | - | 1字节，将要读取的数据个数，0~255。count+1就是将要读取的字节数，最多读取256字节。
checksum | - | 1字节，count的按位取反。
\- | 0x79(ACK)/0x1F(NACK) | 
\- | data | count+1个字节，这就是要读取的数据。

注：

1. 对于读取flash，用户flash的起始地址是0x08000000。
2. 一次最多读取256字节，要读取大段内容就重复执行这个读取操作。

## 9、Go command
略。

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





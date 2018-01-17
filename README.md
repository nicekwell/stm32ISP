
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

stm32官方文档已提交到本工程 documents 目录下。

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
串口驱动，在树莓派[wiringPi]<https://github.com/WiringPi/WiringPi>基础上修改，增加了数据块读写代码，增加了更多设置项。  
这部分驱动在mac、桌面ubuntu、树莓派上都可以使用。

**stm32isp.c stm32isp.h**  
下载逻辑代码，基于串口驱动，串口的打开、关闭操作也归于这部分控制。  
以上两个部分，serial完全归于stm32isp，外部使用stm32isp时不需要再管串口了。

**main.c**  
程序交互的实现，调用上面stm32isp接口实现下载。

下面着重介绍的是stm32isp部分的接口函数。

## stm32isp接口
stm32isp使用前提是串口工作正常。











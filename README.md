
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






# 驱动说明







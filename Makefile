stm32isp: main.c wiringSerial.c
	gcc main.c wiringSerial.c -Iinclude -o stm32isp

clean:
	rm stm32isp


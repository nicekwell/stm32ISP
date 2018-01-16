stm32isp: main.c serial.c
	gcc main.c serial.c -Iinclude -o stm32isp

clean:
	rm stm32isp


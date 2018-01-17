stm32isp: main.c wiringSerial.c stm32.c
	gcc main.c wiringSerial.c stm32.c -Iinclude -o stm32isp

clean:
	rm stm32isp


stm32isp: main.c wiringSerial.c stm32isp.c
	gcc main.c wiringSerial.c stm32isp.c -Iinclude -o stm32isp

clean:
	rm stm32isp


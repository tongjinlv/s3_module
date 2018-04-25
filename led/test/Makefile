obj-m :=led_test.o
all:
	arm-linux-gnueabi-gcc  led_test.c -o led_test
clean:
	rm -rf *.ko *.o *.order *symvers

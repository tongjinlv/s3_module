obj-m :=gpio_test.o
all:
	arm-linux-gnueabi-gcc  gpio_test.c -o gpio_test
clean:
	rm -rf *.ko *.o *.order *symvers

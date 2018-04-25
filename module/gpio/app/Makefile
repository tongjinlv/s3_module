obj-m :=gpio_app.o
all:
	arm-linux-gnueabi-gcc  gpio_app.c -o gpio_app
clean:
	rm -rf *.ko *.o *.order *symvers

obj-m :=gpio_app.o
all:
	arm-linux-gnueabi-gcc  gpio_app.c -o gpio_app
	cp gpio_app /home/tom/BUILD/install/bin/
clean:
	rm -rf *.ko *.o *.order *symvers

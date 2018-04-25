export PATH=/home/tom/work/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi/bin:$PATH
obj-m :=led_app.o
all:
	arm-linux-gnueabi-gcc  led_app.c -o led_app
clean:
	rm -rf *.ko *.o *.order *symvers

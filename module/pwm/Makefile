
obj-m :=pwm.o pwm-sunxi.o
KDIR :=../../linux-3.4
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm
clean:
	rm -rf *.ko *.o *.order *symvers

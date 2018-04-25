
obj-m :=pwm.o
KDIR :=../../linux-3.4
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm
	sz pwm.ko
clean:
	rm -rf *.ko *.o *.order *symvers

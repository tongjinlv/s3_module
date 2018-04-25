obj-m :=gpio.o
KDIR :=../../linux-3.4
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm
	cp gpio.ko /home/tom/BUILD/install/module/
clean:
	rm -rf *.ko *.o *.order *symvers

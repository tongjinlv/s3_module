
obj-m :=pwm.o 
KDIR :=/root/SDK/s3/linux-3.4
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm
	cp pwm.ko /home/tom/BUILD/install/module/
clean:
	rm -rf *.ko *.o *.order *symvers

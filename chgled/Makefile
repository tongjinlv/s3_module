
obj-m :=chgled.o 
KDIR :=/root/SDK/build_s3evb/linux-3.4
all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm
	cp chgled.ko /home/tom/BUILD/install/module/
clean:
	rm -rf *.ko *.o *.order *symvers

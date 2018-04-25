#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <unistd.h>
#include<stdlib.h>

void write_led(int fd,int index,char h)
{
    unsigned char ledbuf[2];
    ledbuf[0]=index;
    ledbuf[1]=h;
    write(fd,ledbuf,2);
    printf("set %d=%d\r\n",ledbuf[0],ledbuf[1]);
}
int main(int argc, const char *argv[])
{   
    int fd;
    unsigned char ledbuf[2];
    if(argc<3){printf("not found\r\n");return 0;}
    int a=(int)atoi(argv[1]);
    int b=(int)atoi(argv[2]);
    fd = open("/dev/myled", O_RDWR);
    ioctl(fd, 0x30);
    write_led(fd,a,b);
    return 0;
}

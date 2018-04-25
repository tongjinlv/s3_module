#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


void write_led(int fd,int index,char h)
{
    unsigned char ledbuf[2];
    ledbuf[0]=index;
    ledbuf[1]=h;
    write(fd,ledbuf,2);
}
int main(int argc, const char *argv[])
{
    int fd;
    unsigned char ledbuf[2];
    fd = open("/dev/myled", O_RDWR);
    printf("iFd=%d\r\n",fd);
    ioctl(fd, 0x30);
    ledbuf[0]=0;
    ledbuf[1]=1;
    int ret=read(fd,ledbuf, 0);
    while(1)
    {
         sleep(1);
         write_led(fd,0,1);
         write_led(fd,1,1);
         write_led(fd,2,1);
         sleep(1);
         write_led(fd,0,0);
         write_led(fd,1,0);
         write_led(fd,2,0);
    }
    printf("ret=%d\r\n",ret);
    return 0;
}

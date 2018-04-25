#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

void write_led(int fd,char *cmd)
{
    int length=strlen(cmd);
    write(fd,cmd,length);
}
void read_key(int fd,char *cmd)
{
    int length=strlen(cmd);
    int v=read(fd,cmd,length);
    printf("valud=%d\r\n",v);
}
int main(int argc, const char *argv[])
{
    int fd;
    unsigned char ledbuf[2];
    fd = open("/dev/mygpio", O_RDWR);
    printf("iFd=%d\r\n",fd);
    ioctl(fd, 0x30);
    while(1)
    {
         write_led(fd,"PB4|L");
         write_led(fd,"PE19|L");
         sleep(1);
         write_led(fd,"PE15|H");
         write_led(fd,"PE19|H");
         sleep(1);
         //read_key(fd,"PE15|H");
    }
    close(fd);
    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

void write_led(int fd,const char *cmd)
{
    int length=strlen(cmd);
    write(fd,cmd,length);
}
int  read_key(int fd,const char *cmd)
{
    int length=strlen(cmd);
    int v=read(fd,cmd,length);
    return v;
}
int main(int argc, const char *argv[])
{
    int fd;
    unsigned char ledbuf[2];
    fd = open("/dev/mygpio", O_RDWR);
    printf("iFd=%d\r\n",fd);
    ioctl(fd, 0x30);
    if(argv[1][0]=='W')write_led(fd,argv[2]);
    if(argv[1][0]=='R')return read_key(fd,argv[2]);
    close(fd);
    return 0;
}

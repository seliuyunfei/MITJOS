#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<stdio.h>
#define __NR_schello 385
int main(int argc,char *argv[])
{
//666666666666666
syscall(__NR_schello);
printf("ok!run dmesg|grep hello in terminal!\n");
return 0;
}

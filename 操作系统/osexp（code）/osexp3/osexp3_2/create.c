#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

int main()
{

    sem_t *mutex;
    
    int ret, fd;
    char buf[100] = {0};
    
    sem_unlink("mutex");
    mutex=sem_open("mutex", O_CREAT, 0666, 1);

    //创建有名管道
    ret = mkfifo("fifo.tmp", O_CREAT | O_EXCL);     
    if (-1 == ret)      
    {
        perror("fifo");
        exit(1);
    }

    fd = open("fifo.tmp", O_RDONLY);        //以只读方式打开
    if(-1 == fd)        //打开失败
    {
        perror("read");
        exit(1);
    }
    
    while(1)
    {
    	printf("read data:\n");
        sem_wait(mutex);
        ret = read(fd, buf, sizeof(buf));       //读出缓冲区中的内容
        if(ret == -1)
        {
            perror("read");
            exit(1);
        }
        if(!strncmp(buf, "exit", 4))     //以bye结束
        {
            break;      //跳出循环
        }

        printf("%s\n",buf);
        memset(buf, 0, sizeof(buf));    //清空缓冲区
        sem_post(mutex);
    }
    unlink("fifo.tmp"); //关掉管道文件，方便程序下次运行
    return 0;
}

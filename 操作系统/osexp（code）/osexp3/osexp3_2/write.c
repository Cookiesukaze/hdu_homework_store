#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int main()
{
    sem_t *mutex;
    sem_unlink("mutex");
    mutex=sem_open("mutex", O_CREAT, 0666, 1);
    
    char buf[100] = {0};
    int fd, ret;

    fd = open("fifo.tmp", O_WRONLY);        //以只写方式打开
    if(-1 == fd)
    {
        perror("open");
        exit(1);
    }

    while(1)
    {
    	sem_wait(mutex);
    	printf("input data:");
        scanf("%s",buf);
        ret = write(fd, buf, strlen(buf));      //往缓冲区里面写
        if(-1 == ret)
        {
            perror("write");
            exit(1);
        }

        if(!strncmp(buf, "exit", 4))     //以bye结束
        {
            break;
        }
    	memset(buf, 0, sizeof(buf));        //清空缓冲区
    	sem_post(mutex);
    }
    return 0;
}

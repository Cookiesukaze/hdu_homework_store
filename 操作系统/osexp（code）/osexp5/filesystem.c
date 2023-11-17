#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<time.h>

#define BLOCKSIZE 1024  //磁盘块大小
#define SIZE 1024000  //虚拟磁盘空间大小
#define END 65535 //FAT中的文件结束状态
#define FREE 0  //FAT中盘块空闲标志
#define ROOTBLOCKNUM 2  //根目录区所占盘块总数
#define MAXOPENFILE 10  //最多同时打开文件个数
#define MAXFILESIZE 10000   //最多同时打开文件大小

/* 文件控制块FCB */
typedef struct FCB
{
    char filename[8];   //文件名
    char exname[3];     //文件扩展名
    unsigned char attribute;    //文件属性字段
    unsigned short time;    //文件创建时间
    unsigned short date;    //文件创建日期
    unsigned short first;   //文件起始盘块号
    unsigned long length;   //文件长度
    char free;      //表示目录项是否为空
}fcb;

/* 文件分配表FAT */
typedef struct FAT
{
    unsigned short id;
}fat;

/* 用户打开文件表USEROPEN */
typedef struct USEROPEN
{
    char filename[8];               //文件名
    char exname[3];                 //文件扩展名
    unsigned char attribute;        //文件属性
    unsigned short time;            //文件创建时间
    unsigned short date;            //文件创建日期
    unsigned short first;           //文件起始盘块号
    unsigned long length;           //文件长度
    char free;      //表示目录项是否为空
    int father;    //表示父目录的文件描述符
    int dirno;      //相应打开文件的目录项在父目录文件中的盘块号
    int diroff;     //相应打开文件的目录项在父目录文件的dirno盘块中的目录项序号
    char dir[MAXOPENFILE][80];      //相应打开文件所在的目录名，方便快速检查指定文件是否已经打开
    int count;      //读写指针在文件中的位置
    char fcbstate;  //是否修改了文件的FCB内容
    char topenfile; //表示该用户打开表项是否为空
}useropen;

/* 引导块BLOCK0 */
typedef struct BLOCK0
{
    //存储一些描述信息，如磁盘块大小、磁盘块数量、最多打开文件数等
    unsigned short fbnum;   //文件魔数、文件类型

    char information[200];
    unsigned short root;    //根目录文件的起始盘块号
    unsigned char *startblock;  //虚拟盘块上数据区开始位置

}block0;

/* 全局变量定义 */
unsigned char *myvhard;     //指向虚拟磁盘的起始地址
useropen openfilelist[MAXOPENFILE];     //用户打开文件表数组
useropen *ptrcurdir;        //指向用户打开文件表中的当前目录所在打开文件表项的位置
char currentdir[80];        //记录当前目录的目录名
unsigned char *startp;      //记录虚拟磁盘上数据区开始位置
char path[] = "c:\\myfsys";     //虚拟空间保存路径
int curfd;


/* 主要命令及函数 */
void startsys();        //进入文件系统函数
void my_format();       //磁盘格式化函数
void my_cd(char *dirname);          //更改当前目录函数
void my_mkdir(char *dirname);       //创建子目录函数
void my_rmdir(char *dirname);       //删除子目录函数
void my_ls(void);       //显示目录函数
void my_create(char *filename);     //创建文件函数
void my_rm(char *filename);         //删除文件函数
int my_open(char *filename);        //打开文件函数
int my_close(int fd);  //关闭文件函数
int my_write(int fd);   //写文件函数
int do_write(int fd, char *text, int len, char wstyle);    //实际写文件函数
int my_read(int fd, int len);       //读文件函数
int do_read(int fd, int len, char *text);   //实际读文件函数
void my_exitsys();      //退出文件系统函数

unsigned short findFree();      //查找下一个空闲磁盘块
int findFree0();    //寻找空闲文件表项




/* 具体函数实现 */

/* 进入文件系统函数 */
void startsys()
{
    FILE *fp;
    int i;
    unsigned char buffer[SIZE];
    myvhard = (unsigned char *)malloc(SIZE);
    memset(myvhard, 0, SIZE);	//初始化为0
    if(fp = fopen(path, "r"))   //打开文件成功
    {
        fread(buffer, SIZE, 1, fp);
        fclose(fp);
        if(buffer[0]!=0XAA || buffer[1]!=0XAA)  //不是文件系统类型10101010
        {
            printf("myfsys文件系统不存在, 现在开始创建文件系统......\n");
            my_format();    //格式化
        }else{      //是文件系统
            printf("myfsys文件系统打开成功！\n");
            for(i=0;i<SIZE;i++)
                myvhard[i] = buffer[i];     //将虚拟磁盘中的内容保存到文件系统中
        }
    }else{      //打开文件失败
        printf("myfsys文件系统不存在, 现在开始创建文件系统......\n");
        my_format();    //格式化
    }

    //初始化，文件打开项0给根目录
    strcpy(openfilelist[0].filename, "root");   //设置根目录
    strcpy(openfilelist[0].exname, "di");       //设置文件扩展名
    openfilelist[0].attribute=0X2D;     //设置文件属性：存档、卷标、系统文件、只读
    openfilelist[0].time = ((fcb *)(myvhard+5*BLOCKSIZE))->time;    //创建时间
    openfilelist[0].date = ((fcb *)(myvhard+5*BLOCKSIZE))->date;    //创建日期
    openfilelist[0].first = ((fcb *)(myvhard+5*BLOCKSIZE))->first;      //起始盘块号
    openfilelist[0].length = ((fcb *)(myvhard+5*BLOCKSIZE))->length;    //文件长度
    openfilelist[0].free = 1;       //已分配
    openfilelist[0].dirno = 5;      //根目录所在起始块号
    openfilelist[0].diroff = 0;     //块内偏移量
    openfilelist[0].count = 0;      //读写指针在文件中的位置
    openfilelist[0].fcbstate = 0;   //文件修改位
    openfilelist[0].topenfile = 0;  //打开文件表项为空
    openfilelist[0].father = 0;     //父目录

    memset(currentdir, 0, sizeof((currentdir)));    //清空当前路径
    strcpy(currentdir, "\\root\\");        //当前路径为根目录
    strcpy(openfilelist->dir[0], currentdir);       //存储当前表项
    startp = ((block0 *)myvhard)->startblock;       //磁盘空间内数据区首地址
    ptrcurdir = &openfilelist[0];       //指文件表项向该用户打开的
    curfd = 0;      //当前文件标识符

}

 /* 格式化函数 */
void my_format()
{
    FILE *fp;
    fat *fat1, *fat2;
    block0 *b0;
    time_t *now;
    struct tm *nowtime;
    unsigned char *p;
    fcb *root;
    int i;

    p = myvhard;
    b0 = (block0 *)p;
    fat1 = (fat *)(p+BLOCKSIZE);
    fat2 = (fat *)(p+BLOCKSIZE*3);

    b0->fbnum = 0XAAAA; //文件系统类型
    b0->root = 5;       //根目录起始盘块号
    strcpy(b0->information, "GPB File System\nBlocksize = 1KB  Whole size = 1000KB  BlockNum = 1000  RootBlockNum = 2\n");

    //FAT1 FAT2 前5个磁盘块已经分配，标记为END
    for(i=1;i<=7;i++)
    {
        if(i==6)    //从第6块进入数据区
        {
            fat1->id = 6;
            fat2->id = 6;
        }else{
            fat1->id = END;
            fat2->id = END;
        }
        fat1++;
        fat2++;
    }

    //将数据区的标记变为空闲状态
    for(i=7;i<SIZE/BLOCKSIZE;i++)
    {
        (*fat1).id = FREE;
        (*fat2).id = FREE;
        fat1++;
        fat2++;
    }

    /*  创建根目录文件root，将数据区的第一块分配给根目录区
        在给磁盘上创建两个特殊的目录项：".",".."，
        除了文件名之外，其它都相同 */
    p += BLOCKSIZE*5;
    root = (fcb *)p;

    //当前目录
    strcpy(root->filename, ".");
    strcpy(root->exname, "di");
    root->attribute = 0X28;
    now = (time_t *)malloc(sizeof(time_t));
    time(now);
    nowtime = localtime(now);
    root->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    root->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    root->first = 5;
    root->length = 2*sizeof(fcb);
    root++;

    //上一级目录
    strcpy(root->filename, "..");
    strcpy(root->exname, "di");
    root->attribute = 0X28;
    time(now);
    nowtime = localtime(now);
    root->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    root->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    root->first = 5;
    root->length = 2*sizeof(fcb);
    root++;

    for(i=2;i<BLOCKSIZE*2/sizeof(fcb);i++,root++)
    {
        root->filename[0] = '\0';
    }
    fp = fopen(path, "w");
    b0->startblock = p + BLOCKSIZE*4;
    fwrite(myvhard, SIZE, 1, fp);
    free(now);
    fclose(fp);

}

/* 更改当前目录函数 */
void my_cd(char *dirname)
{
    char *dir, text[MAXFILESIZE];
    int fd, i;
    dir = strtok(dirname, "\\");
    if(strcmp(dir, ".") == 0)
        return;
    if(strcmp(dir, "..") == 0)
    {
        fd = openfilelist[curfd].father;
        my_close(curfd);
        curfd = fd;
        ptrcurdir = &openfilelist[curfd];
        return;
    }
    if(strcmp(dir, "root") == 0)
    {
        for(i=1;i<MAXOPENFILE;i++)
        {
            if(openfilelist[i].free)
                my_close(i);
        }
        ptrcurdir = &openfilelist[0];
        curfd = 0;
        dir = strtok(NULL, "\\");
    }

    while(dir)
    {
        if((fd = my_open(dir)) > 0)
        {
            ptrcurdir = &openfilelist[fd];
            curfd = fd;
        }else
            return ;
        dir = strtok(NULL, "\\");
    }

}


/* 创建子目录函数 */
void my_mkdir(char *dirname)
{
    fcb *dir_fcb, *pcb_tmp;
    int size, fd, i;
    unsigned short block_num;
    char text[MAXFILESIZE], *p;
    time_t *now;
    struct tm *nowtime;

    //将当前的文件信息读到text中，size是实际读取的字节数
    openfilelist[curfd].count = 0;
    size = do_read(curfd, openfilelist[curfd].length, text);
    dir_fcb = (fcb *)text;

    //检查是否有相同的目录名
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(dirname, dir_fcb->filename) == 0)
        {
            printf("错误: 该目录名已存在!\n");
            return ;
        }
        dir_fcb++;
    }

    //分配一个空闲的打开文件表项
    dir_fcb = (fcb *)text;
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(dir_fcb->filename, "") == 0)
            break;
        dir_fcb++;
    }
    openfilelist[curfd].count = i*sizeof(fcb);

    if((fd = findFree0()) < 0)
        return ;

    //寻找空闲盘块
    block_num = findFree();
    if(block_num == END)
        return ;

    pcb_tmp = (fcb *)malloc(sizeof(fcb));
    now = (time_t *)malloc(sizeof(time_t));

    //在当前目录下新建目录项
    pcb_tmp->attribute = 0X30;      //属性：存档、子目录
    time(now);
    nowtime = localtime(now);
    pcb_tmp->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    pcb_tmp->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    strcpy(pcb_tmp->filename, dirname);
    strcpy(pcb_tmp->exname, "di");
    pcb_tmp->first = block_num;
    pcb_tmp->length = 2*sizeof(fcb);

    openfilelist[fd].attribute = pcb_tmp->attribute;
    openfilelist[fd].count = 0;
    openfilelist[fd].date = pcb_tmp->date;
    strcpy(openfilelist[fd].dir[fd], openfilelist[curfd].dir[curfd]);

    p = openfilelist[fd].dir[fd];
    while(*p != '\0')
        p++;
    strcpy(p, dirname);
    while(*p != '\0')
        p++;
    *p = '\\';
    p++;
    *p = '\0';

    openfilelist[fd].dirno = openfilelist[curfd].first;
    openfilelist[fd].diroff = i;
    strcpy(openfilelist[fd].filename, pcb_tmp->filename);
    strcpy(openfilelist[fd].exname, pcb_tmp->exname);
    openfilelist[fd].fcbstate = 1;      //修改位置为1
    openfilelist[fd].first = pcb_tmp->first;
    openfilelist[fd].length = pcb_tmp->length;
    openfilelist[fd].free = 1;
    openfilelist[fd].time = pcb_tmp->time;
    openfilelist[fd].topenfile = 1;     //打开表项为1

    //建立.特殊目录项
    do_write(curfd, (char *)pcb_tmp, sizeof(fcb), 2);
    pcb_tmp->attribute = 0X28;      //属性：存档、卷标
    time(now);
    nowtime = localtime(now);
    pcb_tmp->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    pcb_tmp->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    strcpy(pcb_tmp->filename, ".");
    strcpy(pcb_tmp->exname, "di");
    pcb_tmp->first = block_num;
    pcb_tmp->length = 2*sizeof(fcb);

    //建立..特殊目录项
    do_write(fd, (char *)pcb_tmp, sizeof(fcb), 2);
    pcb_tmp->attribute = 0X28;
    time(now);
    nowtime = localtime(now);
    pcb_tmp->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    pcb_tmp->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    strcpy(pcb_tmp->filename, "..");
    strcpy(pcb_tmp->exname, "di");
    pcb_tmp->first = openfilelist[curfd].first;
    pcb_tmp->length = openfilelist[curfd].length;

    //覆盖写调用do_write写入FCB内容
    do_write(fd, (char *)pcb_tmp, sizeof(fcb), 2);

    openfilelist[curfd].count = 0;
    do_read(curfd, openfilelist[curfd].length, text);

    pcb_tmp = (fcb *)text;
    pcb_tmp->length = openfilelist[curfd].length;
    my_close(fd);

    openfilelist[curfd].count = 0;
    do_write(curfd, text, pcb_tmp->length, 2);

}

/* 删除子目录函数 */
void my_rmdir(char *dirname)
{
    int size, fd;
    char text[MAXFILESIZE];
    fcb *fcb_ptr, *fcb_tmp1, *fcb_tmp2;
    unsigned short block_num;
    int i, j;
    fat *fat1, *fat_ptr;
    if(strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
    {
        printf("错误: 无法删除该目录!\n");
        return ;
    }
    fat1 = (fat *)(myvhard + BLOCKSIZE);
    openfilelist[curfd].count = 0;
    size = do_read(curfd, openfilelist[curfd].length, text);
    fcb_ptr = (fcb *)text;
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(dirname, fcb_ptr->filename) == 0)
            break;
        fcb_ptr++;
    }
    if(i >= size/sizeof(fcb))
    {
        printf("错误: 该目录不存在!\n");
        return ;
    }

    block_num = fcb_ptr->first;
    fcb_tmp2 = fcb_tmp1 = (fcb *)(myvhard+block_num*BLOCKSIZE);
    for(j=0;j<fcb_tmp1->length/sizeof(fcb);j++)
    {
        if(strcmp(fcb_tmp2->filename, ".") && strcmp(fcb_tmp2->filename, "..") && fcb_tmp2->filename[0]!='\0')
        {
            printf("错误: 该目录不为空!\n");
            return ;
        }
        fcb_tmp2++;
    }

    while(block_num != END)
    {
        fat_ptr = fat1 + block_num;
        block_num = fat_ptr->id;
        fat_ptr->id = FREE;
    }

    //清空
    strcpy(fcb_ptr->filename, "");
    strcpy(fcb_ptr->exname, "");
    fcb_ptr->first = END;
    openfilelist[curfd].count = 0;
    do_write(curfd, text, openfilelist[curfd].length, 2);

}

/* 显示目录函数 */
void my_ls(void)
{
    fcb *fcb_ptr;
    int i;
    char text[MAXFILESIZE];
    unsigned short block_num;
    openfilelist[curfd].count = 0;
    do_read(curfd, openfilelist[curfd].length, text);
    fcb_ptr = (fcb *)text;
    for(i=0;i<(int)(openfilelist[curfd].length/sizeof(fcb));i++)
    {
        if(fcb_ptr->filename[0] != '\0')
        {
            if(fcb_ptr->attribute & 0X20)   //目录项
            {
                if(strlen(fcb_ptr->filename) >= 7)
                    printf("%s\\\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n", fcb_ptr->filename, ((fcb_ptr->date)>>9)+1980, ((fcb_ptr->date)>>5) & 0X000F, (fcb_ptr->date) & 0X001F, fcb_ptr->time>>11, (fcb_ptr->time>>5) & 0X003F, fcb_ptr->time & 0X001F*2);
                else
                    printf("%s\\\t\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n", fcb_ptr->filename, ((fcb_ptr->date)>>9)+1980, ((fcb_ptr->date)>>5) & 0X000F, (fcb_ptr->date) & 0X001F, fcb_ptr->time>>11, (fcb_ptr->time>>5) & 0X003F, fcb_ptr->time & 0X001F*2);
            }else{
                if(strlen(fcb_ptr->filename) + strlen(fcb_ptr->exname) >= 7)
                    printf("%s.%s\t%ld B\t\t%d/%d/%d\t%02d:%02d:%02d\t\n", fcb_ptr->filename, fcb_ptr->exname, fcb_ptr->length, ((fcb_ptr->date)>>9)+1980, (fcb_ptr->date)>>5 & 0X000F, fcb_ptr->date & 0X001F, fcb_ptr->time>>11, (fcb_ptr->time>>5) & 0X003F, fcb_ptr->time & 0X001F*2);
                else
                    printf("%s.%s\t\t%ld B\t\t%d/%d/%d\t%02d:%02d:%02d\t\n", fcb_ptr->filename, fcb_ptr->exname, fcb_ptr->length, ((fcb_ptr->date)>>9)+1980, (fcb_ptr->date)>>5 & 0X000F, fcb_ptr->date & 0X001F, fcb_ptr->time>>11, (fcb_ptr->time>>5) & 0X003F, fcb_ptr->time & 0X001F*2);
            }
        }
        fcb_ptr++;
    }
    openfilelist[curfd].count = 0;
}

/* 创建文件函数 */
void my_create(char *filename)
{
    char *fname, *exname, text[MAXFILESIZE];
    int fd, size, i;
    fcb *filefcb, *fcb_tmp;
    time_t *now;
    struct tm *nowtime;
    unsigned short block_num;
    fat *fat1, *fat_ptr;

    fat1 = (fat *)(myvhard+BLOCKSIZE);
    fname = strtok(filename, ".");
    exname = strtok(NULL, ".");
    if(strcmp(fname, "") == 0)
    {
        printf("错误: 创建文件必须要有文件名!\n");
        return ;
    }
    if(!exname)
    {
        printf("错误: 创建文件必须要有文件扩展名!\n");
        return ;
    }

    openfilelist[curfd].count = 0;
    size = do_read(curfd, openfilelist[curfd].length, text);
    filefcb = (fcb *)text;
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(fname, filefcb->filename)==0 && strcmp(exname, filefcb->exname)==0)
        {
            printf("错误: 该文件名已经存在!\n");
            return ;
        }
        filefcb++;
    }

    filefcb = (fcb *)text;
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(filefcb->filename, "") == 0)
            break;
        filefcb++;
    }
    openfilelist[curfd].count = i*sizeof(fcb);

    block_num = findFree();
    if(block_num == END)
        return ;

    fcb_tmp = (fcb *)malloc(sizeof(fcb));
    now = (time_t *)malloc(sizeof(time_t));

    fcb_tmp->attribute = 0X00;
    time(now);
    nowtime = localtime(now);
    fcb_tmp->time = nowtime->tm_hour*2048 + nowtime->tm_min*32 + nowtime->tm_sec/2;
    fcb_tmp->date = (nowtime->tm_year-80)*512 + (nowtime->tm_mon+1)*32 + nowtime->tm_mday;
    strcpy(fcb_tmp->filename, fname);
    strcpy(fcb_tmp->exname, exname);
    fcb_tmp->first = block_num;
    fcb_tmp->length = 0;

    do_write(curfd, (char *)fcb_tmp, sizeof(fcb), 2);
    free(fcb_tmp);
    free(now);
    openfilelist[curfd].count = 0;
    do_read(curfd, openfilelist[curfd].length, text);
    fcb_tmp = (fcb *)text;
    fcb_tmp->length = openfilelist[curfd].length;
    openfilelist[curfd].count = 0;
    do_write(curfd, text, openfilelist[curfd].length, 2);
    openfilelist[curfd].fcbstate = 1;
    fat_ptr = (fat *)(fat1 + block_num);
    fat_ptr->id = END;

    printf("创建文件成功!\n");
}


/* 删除文件函数 */
void my_rm(char *filename)
{
    char *fname, *exname;
    char text[MAXFILESIZE];
    fcb *fcb_ptr;
    int i,size;
    unsigned short block_num;
    fat *fat1, *fat_ptr;

    fat1 = (fat *)(myvhard+BLOCKSIZE);
    fname = strtok(filename, ".");
    exname = strtok(NULL, ".");
    if(!fname || strcmp(fname, "") == 0)
    {
        printf("错误: 删除文件必须要有正确的文件名!\n");
        return ;
    }
    if(!exname)
    {
        printf("错误: 删除文件必须要有正确的文件扩展名!\n");
        return ;
    }

    openfilelist[curfd].count = 0;
    size = do_read(curfd, openfilelist[curfd].length, text);
    fcb_ptr = (fcb *)text;
    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(fname, fcb_ptr->filename)==0 && strcmp(exname, fcb_ptr->exname)==0)
            break;
        fcb_ptr++;
    }
    if(i >= size/sizeof(fcb))
    {
        printf("错误: 该文件名不存在!\n");
        return ;
    }

    block_num = fcb_ptr->first;
    while(block_num != END)
    {
        fat_ptr = fat1 + block_num;
        block_num = fat_ptr->id;
        fat_ptr->id = FREE;
    }

    strcpy(fcb_ptr->filename, "");
    strcpy(fcb_ptr->exname, "");
    fcb_ptr->first = END;
    openfilelist[curfd].count = 0;
    do_write(curfd, text, openfilelist[curfd].length, 2);

}

/* 打开文件函数 */
int my_open(char *filename)
{
    int i, fd, size;
    char text[MAXFILESIZE], *p, *fname, *exname;
    fcb *fcb_ptr;
    char exnamed = 0;
    fname = strtok(filename, ".");
    exname = strtok(NULL, ".");
    if(!exname)
    {
        exname = (char *)malloc(3);
        strcpy(exname, "di");
        exnamed = 1;
    }
    for(i=0;i<MAXOPENFILE;i++)
    {
        if(strcmp(openfilelist[i].filename, filename)==0 && strcmp(openfilelist[i].exname, exname)==0 && i!=curfd)
        {
            printf("错误: 该文件已经被打开!\n");
            return -1;
        }
    }
    openfilelist[curfd].count = 0;
    size = do_read(curfd, openfilelist[curfd].length, text);
    fcb_ptr = (fcb *)text;

    for(i=0;i<size/sizeof(fcb);i++)
    {
        if(strcmp(filename, fcb_ptr->filename)==0 && strcmp(fcb_ptr->exname, exname)==0)
            break;
        fcb_ptr++;
    }
    if(i >= size/sizeof(fcb))
    {
        printf("错误: 该文件名不存在!\n");
        return curfd;
    }
    if(exnamed)
        free(exname);

    if((fd = findFree0()) == -1)
        return -1;

    strcpy(openfilelist[fd].filename, fcb_ptr->filename);
    strcpy(openfilelist[fd].exname, fcb_ptr->exname);
    openfilelist[fd].attribute = fcb_ptr->attribute;
    openfilelist[fd].count = 0;
    openfilelist[fd].date = fcb_ptr->date;
    openfilelist[fd].first = fcb_ptr->first;
    openfilelist[fd].length = fcb_ptr->length;
    openfilelist[fd].time = fcb_ptr->time;
    openfilelist[fd].father = curfd;
    openfilelist[fd].dirno = openfilelist[curfd].first;
    openfilelist[fd].diroff = i;
    openfilelist[fd].fcbstate = 0;
    openfilelist[fd].free = 1;
    openfilelist[fd].topenfile = 1;
    strcpy(openfilelist[fd].dir[fd], openfilelist[curfd].dir[curfd]);
    p = openfilelist[fd].dir[fd];
    while(*p != '\0')
        p++;
    strcpy(p, filename);
    while(*p != '\0')
        p++;

    if(openfilelist[fd].attribute & 0X20)       //属性：存档
    {
        *p = '\\';
        p++;
        *p = '\0';
    }else{
        *p = '.';
        p++;
        strcpy(p, openfilelist[fd].exname);
    }
    return fd;

}


/* 关闭文件函数 */
int my_close(int fd)
{
    fcb *fa_fcb;
    char text[MAXFILESIZE];
    int fa;

    if(fd > MAXOPENFILE || fd <= 0)
    {
        printf("错误: 该文件名不存在!\n");
        return -1;
    }

    fa = openfilelist[fd].father;
    if(openfilelist[fd].fcbstate)
    {
        fa = openfilelist[fd].father;
        openfilelist[fa].count = 0;
        do_read(fa, openfilelist[fa].length, text);

        fa_fcb = (fcb *)(text+sizeof(fcb)*openfilelist[fd].diroff);
        fa_fcb->attribute = openfilelist[fd].attribute;
        fa_fcb->date = openfilelist[fd].date;
        fa_fcb->first = openfilelist[fd].first;
        fa_fcb->length = openfilelist[fd].length;
        fa_fcb->time = openfilelist[fd].time;
        strcpy(fa_fcb->filename, openfilelist[fd].filename);
        strcpy(fa_fcb->exname, openfilelist[fd].exname);

        openfilelist[fa].count = 0;
        do_write(fa, text, openfilelist[fa].length, 2);

    }

    openfilelist[fd].attribute = 0;
    openfilelist[fd].count = 0;
    openfilelist[fd].date = 0;
    strcpy(openfilelist[fd].dir[fd], "");
    strcpy(openfilelist[fd].filename, "");
    strcpy(openfilelist[fd].exname, "");
    openfilelist[fd].length = 0;
    openfilelist[fd].time = 0;
    openfilelist[fd].free = 0;
    openfilelist[fd].topenfile = 0;

    return fa;
}


/* 写文件函数 */
int my_write(int fd)
{
    int wstyle=-1, wlen=0;
    fat *fat1, *fat_ptr;
    unsigned short block_num;
    unsigned char *block_ptr;
    char text[MAXFILESIZE];
    fat1 = (fat *)(myvhard + BLOCKSIZE);
    if(fd > MAXOPENFILE)
    {
        printf("该文件名不存在!\n");
        return -1;
    }
    while(wstyle<0 || wstyle>3)
    {
        printf("请选择写文件的方式:\n(1) 截断写     (2) 覆盖写    （3）追加写    (0) 返回\n");
        scanf("%d", &wstyle);
        getchar();
        switch(wstyle)
        {
            case 1:     //截断写，，将第一个数据块外所有的fat的id改为free，文件长度和读写指针位置归零
            {
                block_num = openfilelist[fd].first;
                fat_ptr = fat1 + block_num;
                while(fat_ptr->id != END)
                {
                    block_num = fat_ptr->id;
                    fat_ptr->id = FREE;
                    fat_ptr = fat1 + block_num;
                }
                fat_ptr->id = FREE;
                block_num = openfilelist[fd].first;
                fat_ptr = fat1 + block_num;
                fat_ptr->id = END;
                openfilelist[fd].length = 0;
                openfilelist[fd].count = 0;
                break;
            }
            case 2:     //覆盖写
            {                   //直接将读写指针归零，文件长度没变
                openfilelist[fd].count = 0;
                break;
            }
            case 3:     //追加写
            {
                openfilelist[fd].count = openfilelist[fd].length;//将读写指针位置放在文件长度的位置
                break;
            }
            case 0:
            {
                return -1;
            }
            default:
                break;
        }
    }
    printf("请输入写入的数据:(##END##结束输入)\n");
    while(gets(text))
    {
        if(strcmp(text, "##END##") == 0)
            break;
        int len = strlen(text);
        text[len++] = '\n';
        text[len] = '\0';
        if(do_write(fd, text, len, wstyle) > 0)
            wlen += len;
        else
            return -1;
    }
    if(openfilelist[fd].count > openfilelist[fd].length)
        openfilelist[fd].length = openfilelist[fd].count;

    openfilelist[fd].fcbstate = 1;
    return wlen;
}

/* 实际写文件函数 */
int do_write(int fd, char *text, int len, char wstyle)
{
    unsigned char *buf;
    unsigned short block_num;
    int offset, len_tmp1=0, len_tmp2=0, rwptr, i;
    unsigned char *block_ptr, *p;
    char *text_tmp, flag;
    fat *fat1, *fat_ptr;
    fat1 = (fat *)(myvhard + BLOCKSIZE);
    text_tmp = text;

    //申请临时缓冲区
    buf = (unsigned char *)malloc(BLOCKSIZE);
    if(buf == NULL)
    {
        printf("分配内存空间失败!\n");
        return -1;
    }

    rwptr = offset = openfilelist[fd].count;
    block_num = openfilelist[fd].first;
    fat_ptr = fat1 + block_num;
    while(offset >= BLOCKSIZE)  //根据读写指针大小确定它在哪个磁盘块
    {
        offset = offset - BLOCKSIZE;
        block_num = fat_ptr->id;
        if(block_num == END)//等于时表示当前磁盘块恰巧写满，需开辟新的磁盘块
        {
            block_num = fat_ptr->id = findFree();//找不到磁盘已满return出去
            if(block_num == END)
                return -1;
            fat_ptr = fat1 + block_num;
            fat_ptr->id = END;
        }
        fat_ptr = fat1 + block_num;
    }

    fat_ptr->id = END;
    block_ptr = (unsigned char *)(myvhard + block_num*BLOCKSIZE);
    while(len_tmp1 < len)               //写入的长度小于要写入的长度
    {
        for(i=0;i<BLOCKSIZE;i++)//初始化缓冲区
            buf[i] = 0;
        for(i=0;i<BLOCKSIZE;i++)    //将尾部磁盘块的没满数据写入缓冲区
        {
            buf[i] = block_ptr[i];
            len_tmp2++;
            if(len_tmp2 == openfilelist[curfd].length)
                break;
        }

        for(p=buf+offset;p<buf+BLOCKSIZE;p++)//将传入的文件字符写入缓冲区的后边
        {
            *p = *text_tmp;
            len_tmp1++;
            text_tmp++;
            offset++;
            if(len_tmp1 == len)
                break;
        }
        for(i=0;i<BLOCKSIZE;i++)        //将缓冲区一次性写入虚拟磁盘
            block_ptr[i] = buf[i];
        openfilelist[fd].count = rwptr + len_tmp1;
        if(offset >= BLOCKSIZE)
        {
            offset = offset - BLOCKSIZE;
            block_num = fat_ptr->id;
            if(block_num == END)
            {
                block_num = fat_ptr->id = findFree();
                if(block_num == END)
                    return -1;
                fat_ptr = fat1 + block_num;
                fat_ptr->id = END;
            }
            fat_ptr = fat1 + block_num;
            block_ptr = (unsigned char *)(myvhard+block_num*BLOCKSIZE);
        }
    }
    free(buf);
    if(openfilelist[fd].count > openfilelist[fd].length)
        openfilelist[fd].length  = openfilelist[fd].count;
    openfilelist[fd].fcbstate = 1;

    return len_tmp1;
}


/* 寻找下一个空闲盘块号 */
unsigned short findFree()
{
    unsigned short i;
    fat *fat1, *fat_ptr;

    fat1 = (fat *)(myvhard + BLOCKSIZE);
    for(i=6;i<END;i++)
    {
        fat_ptr = fat1 + i;
        if(fat_ptr->id == FREE)
            return i;
    }
    printf("错误: 无法找到空闲盘块!\n");
    return END;
}

/* 寻找空闲文件表项 */
int findFree0()
{
    int i;
    for(i=0;i<MAXOPENFILE;i++)
    {
        if(openfilelist[i].free == 0)
            return i;
    }
    printf("错误: 无法找到空闲文件表项!\n");
    return -1;
}


/* 读文件函数 */
int my_read(int fd, int len)
{
    char text[MAXFILESIZE];
    if(fd > MAXOPENFILE)
    {
        printf("该文件名不存在!\n");
        return -1;
    }
    openfilelist[curfd].count = 0;
    if(do_read(fd, len, text)>0)
    {
        text[len] = '\0';
        printf("%s\n", text);
    }else if(len == 0){

    }else{
        printf("读错误!\n");
        return -1;
    }
    return len;
}

/* 实际读文件函数 */
int do_read(int fd, int len, char *text)
{
    unsigned char *buf;
    unsigned short block_num;
    int offset, len_tmp, i;
    unsigned char *block_ptr, *p;
    char *text_tmp;
    fat *fat1, *fat_ptr;
    fat1 = (fat *)(myvhard + BLOCKSIZE);
    len_tmp = len;
    text_tmp = text;

    //申请缓冲区
    buf = (unsigned char *)malloc(1024);
    if(buf == NULL)
    {
        printf("分配内存空间失败!\n");
        return -1;
    }

    offset = openfilelist[fd].count;
    block_num = openfilelist[fd].first;
    fat_ptr = fat1 + block_num;
    while(offset >= BLOCKSIZE)
    {
        offset = offset - BLOCKSIZE;
        block_num = fat_ptr->id;
        fat_ptr = fat1 + block_num;
        if(block_num == END)
        {
            printf("错误: 该磁盘块已经存在!\n");
            return -1;
        }
    }

    block_ptr = (unsigned char *)(myvhard + block_num*BLOCKSIZE);
    for(i=0;i<BLOCKSIZE;i++)    //将所在磁盘块一次性读入缓冲区
        buf[i] = block_ptr[i];

    while(len > 0)
    {
        if(BLOCKSIZE - offset > len)
        {   //将从offset开始的buf中的len长度读到text中,即这是最后一个缓冲区，不需要全部读出
            for(p=buf+offset;len>0;p++,text_tmp++)
            {
                *text_tmp = *p;
                len--;
                offset++;
                openfilelist[fd].count++;
            }
        }else{  //将从offset开始的剩余内容读到text中
            for(p=buf+offset;p<buf+BLOCKSIZE;p++,text_tmp++)
            {
                *text_tmp = *p;
                len--;
                openfilelist[fd].count++;
            }
            offset = 0;
            block_num = fat_ptr->id;
            fat_ptr = fat1 + block_num;
            block_ptr = (unsigned char *)(myvhard + block_num*BLOCKSIZE);
            for(i=0;i<BLOCKSIZE;i++)
                buf[i] = block_ptr[i];
        }
    }
    free(buf);
    return len_tmp - len;
}

/* 退出文件系统函数 */
void my_exitsys()
{
    FILE *fp;
    fcb *rootfcb;
    char text[MAXFILESIZE];
    while(curfd)
        curfd = my_close(curfd);

    if(openfilelist[curfd].fcbstate)
    {   //保存虚拟磁盘空间
        openfilelist[curfd].count = 0;
        do_read(curfd, openfilelist[curfd].length, text);
        rootfcb = (fcb *)text;
        rootfcb->length = openfilelist[curfd].length;
        openfilelist[curfd].count = 0;
        do_write(curfd, text, openfilelist[curfd].length, 2);
    }
    fp = fopen(path, "w");
    fwrite(myvhard, SIZE, 1, fp);
    free(myvhard);      //释放虚拟空间
    fclose(fp);
}

/* 开始页面 */
void StartPage()
{
    printf("*****************************File System*****************************\n\n");
	printf("命令名\t\t命令参数\t\t命令说明\n\n");
    printf("ls\t\t无\t\t\t显示当前目录下的目录和文件\n");
    printf("cd\t\t目录名\t\t\t切换当前目录到指定目录\n");
	printf("mkdir\t\t目录名\t\t\t在当前目录创建新目录\n");
	printf("rmdir\t\t目录名\t\t\t在当前目录删除指定目录\n");
	printf("create\t\t文件名\t\t\t在当前目录下创建指定文件\n");
	printf("rm\t\t文件名\t\t\t在当前目录下删除指定文件\n");
	printf("open\t\t文件名\t\t\t在当前目录下打开指定文件\n");
    printf("close\t\t无\t\t\t在打开文件状态下，关闭该文件\n");
    printf("read\t\t无\t\t\t在打开文件状态下，读取该文件\n");
    printf("write\t\t无\t\t\t在打开文件状态下，写该文件\n");
	printf("exit\t\t无\t\t\t退出系统\n\n");
	printf("*********************************************************************\n\n");

}

/* 主函数 */
void main()
{
    char cmd[12][10] = {"ls", "cd", "mkdir", "rmdir", "create", "rm", "open", "close", "read", "write", "exit"};
    char buf[30], *str;
    int cmd_num, i;

    startsys();     //进入文件系统
    StartPage();    //开始页面

    while(1)
    {
        printf("%s > ", openfilelist[curfd].dir[curfd]);
        gets(buf);
        cmd_num = -1;
        if(strcmp(buf, ""))
        {
            str = strtok(buf, " ");
            for(i=0;i<12;i++)
            {
                if(strcmp(str, cmd[i]) == 0)
                {
                    cmd_num = i;
                    break;
                }
            }
            switch(cmd_num)
            {
                case 0:{    //ls
                    if(openfilelist[curfd].attribute & 0X20)
                    {
                        my_ls();
                        putchar('\n');
                    }else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 1:{    //cd
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                        my_cd(str);
                    else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 2:{    //mkdir
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                        my_mkdir(str);
                    else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 3:{    //rmdir
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                        my_rmdir(str);
                    else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 4:{    //create
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                        my_create(str);
                    else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 5:{    //rm
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                        my_rm(str);
                    else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 6:{    //open
                    str = strtok(NULL, " ");
                    if(str && openfilelist[curfd].attribute & 0X20)
                    {
                        if(strchr(str, '.'))
                            curfd = my_open(str);
                        else
                            printf("打开文件需要有文件扩展名!\n");
                    }else
                        printf("请输入合法的命令!\n");
                    break;
                }
                case 7:{    //close
                    if(!(openfilelist[curfd].attribute & 0X20))
                        curfd = my_close(curfd);
                    else
                        printf("该文件并没有被打开!\n");
                    break;
                }
                case 8:{    //read
                    if(!(openfilelist[curfd].attribute & 0X20))
                        my_read(curfd, openfilelist[curfd].length);
                    else
                        printf("该文件并没有被打开!\n");
                    break;
                }
                case 9:{    //write
                    if(!(openfilelist[curfd].attribute & 0X20))
                        my_write(curfd);
                    else
                        printf("该文件并没有被打开!\n");
                    break;
                }
                case 10:{   //exit
                    if(openfilelist[curfd].attribute & 0X20)
                    {
                        my_exitsys();
                        return ;
                    }else
                        printf("请输入合法的命令!\n");
                    break;
                }
                default:{
                    printf("请输入合法的命令!\n");
                    break;
                }
            }
        }
    }

}


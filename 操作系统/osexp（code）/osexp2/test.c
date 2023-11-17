#include <unistd.h>
#include <sys/syscall.h> //引用头文件,以使用syscall函数
#include <stdio.h>
#include <stdlib.h>
#define EFAULT 14 //查询可知EFAULT = 14 :Bad address

int main() {
  int pid, flag, nicevalue; 
  int prev_prio, prev_nice, cur_prio, cur_nice;
  int result1, result2;

  system("ps -l"); //长格式打印当前进程
  printf("\n");
  while (1) { //循环输入
    printf("请依次输入pid, flag, nicevalue:");
    scanf("%d %d %d", &pid, &flag, &nicevalue);
    
    //首先查询修改前进程的prio与nice值
    result1 = syscall(335, pid, 0, nicevalue, &prev_prio, &prev_nice); 
    if (result1 == EFAULT) {
      printf("错误,获取进程信息失败!\n");
      continue;
    }
    
    if (flag == 1){
      //使用系统调用修改指定进程的nice值,并打印返回结果
      result2 = syscall(335, pid, 1, nicevalue, &cur_prio, &cur_nice);
      if (result2 == 0) {
        printf("设置nice值为%d成功!\n", nicevalue);  
      } else {
        printf("设置nice值失败,错误码:%d\n", result2);
      }
    }
    else if (flag == 0){
      //使用系统调用查询指定进程的nice值,并打印返回结果  
      result2 = syscall(335, pid, 0, nicevalue, &cur_prio, &cur_nice);
      if (result2 == 0) {
        printf("进程nice值为:%d\n", cur_nice);
      } else {
        printf("查询nice值失败,错误码:%d\n", result2);
      }
    }
  }
  
  return 0;
}
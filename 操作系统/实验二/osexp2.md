![image-20231103112751766](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231103112751766.png)



声明服务例程

​	asmlinkage long sys_mysetnice(pid_t pid, int flag, int nicevalue, void __user*prio, void __user*nice);

实现服务例程

SYSCALL_DEFINE5(mysetnice, pid_t, pid, int, flag, int, nicevalue, void __user*, prio, void __user*, nice){

  int cur_prio, cur_nice;

  struct pid *_pid;

  struct task_struct *pcb;

  _pid = find_get_pid(pid);      

  pcb = pid_task(_pid, PIDTYPE_PID);

  if (flag == 1)       
        set_user_nice(pcb, nicevalue);
  else if (flag != 0)        
        return EFAULT;

  cur_prio = task_prio(pcb);     

  cur_nice = task_nice(pcb);

  copy_to_user(prio, &cur_prio, sizeof(cur_prio));

  copy_to_user(nice, &cur_nice, sizeof(cur_nice));

  return 0;

}

![image-20231104135110831](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104135110831.png)

![image-20231104180525741](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104180525741.png)

![image-20231104193139653](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104193139653.png)

![image-20231104195134590](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104195134590.png)

![image-20231104220458912](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104220458912.png)

![image-20231104222401159](C:\Users\张奉静\AppData\Roaming\Typora\typora-user-images\image-20231104222401159.png)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

// 初始化函数
static int hello_init(void)
{
    pr_alert("list start !\n");
    struct task_struct *p;
    printk(KERN_ALERT"名称\t进程号\t状态\t优先级\t父进程号");
    int kernel_thread_num = 0;
    for_each_process(p)
    {
        if(p->mm == NULL){ //内核线程的mm成员为空
            kernel_thread_num++;
            pr_info("%s\t%d\t%ld\t%d\n",p->comm,p->pid, p->stats,p->rt_priority,p->parent->pid);
        }
    }
    pr_info("kernel thread num is %d\n",kernel_thread_num);
    return 0;
}
// 清理函数
static void hello_exit(void)
{
}

// 函数注册
module_init(hello_init);  
module_exit(hello_exit);  

// 模块许可申明
MODULE_LICENSE("GPL");  
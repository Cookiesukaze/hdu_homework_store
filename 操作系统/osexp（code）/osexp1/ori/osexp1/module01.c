#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

// 初始化函数
static int hello_init(void)
{
struct task_struct *p;
printk("-----------------------------------------------------------------------");
printk(KERN_ALERT"名称          进程    状态    优先级  父进程");
for_each_process(p)
 {
if(p->mm == NULL){ //内核线程的mm成员为空
printk(KERN_ALERT"%s\t%d\t%ld\t%d\n",p->comm,p->pid,p->stats,p->normal_prio,p->parent->pid);
  }
 }
return 0;
}

// 清理函数
static void hello_exit(void)
{
printk(KERN_ALERT"goodbye!\n");
}

// 函数注册
module_init(hello_init);
module_exit(hello_exit);

// 模块许可申明
MODULE_LICENSE("GPL");


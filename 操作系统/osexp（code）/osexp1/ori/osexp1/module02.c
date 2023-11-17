#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
MODULE_LICENSE("GPL");
static pid_t pid;
module_param(pid,int,0644);

static int hello_init(void)
{
  struct task_struct *p;
  struct list_head *pp;
  struct task_struct *psibling;
  printk("--------------------------------------------------------");
 //当前进程的 PID
 
  p = pid_task(find_vpid(pid), PIDTYPE_PID);
  printk("me:%s %d %ld\n",p->comm, p->pid, p->stats);

 // 父进程
  if(p->parent == NULL) {
        printk("No Parent\n");

    }
    else {
        printk("Parent:%s %d %ld\n",p->parent->comm, p->parent->pid, p->parent->stats);
    }                             
 // 兄弟进程                      
   list_for_each(pp, &p->parent->children)
    {                             
        psibling = list_entry(pp, struct task_struct, sibling);
        printk("sibling %s %d %ld \n",psibling->comm, psibling->pid, psibling->stats);
    }

// 子进程
   list_for_each(pp, &p->children)
    {
        psibling = list_entry(pp, struct task_struct, sibling);
        printk("children %s %d %ld \n", psibling->comm,psibling->pid, psibling->stats);
    }

    return 0;
}
static void hello_exit(void)
{
    printk(KERN_ALERT"goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

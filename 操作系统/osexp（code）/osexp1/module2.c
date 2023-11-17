#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

static pid_t  pid = 1;
module_param(pid,int,0644);

// 初始化函数
static int hello_init(void)
{
    pr_alert("list start !\n");
    struct task_struct *task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    struct task_struct *parent = task->parent;
    struct task_struct *sibling;
    struct list_head *list;
    // List it own process
    pr_info("Root: Name: %s, PID: %d, State: %ld\n",task->comm,task->pid,task->stats);

    // List parent process
    pr_info("Parent: Name: %s, PID: %d, State: %ld\n", parent->comm, parent->pid, parent->stats);

    // List child processes
    list_for_each(list, &task->children) {
        sibling = list_entry(list, struct task_struct, sibling);
        pr_info("Child: Name: %s, PID: %d, State: %ld\n", sibling->comm, sibling->pid, sibling->stats);
    }

    // List sibling processes
    list_for_each(list, &parent->children) {
        sibling = list_entry(list, struct task_struct, sibling);
        if (sibling != task) {
            pr_info("Sibling: Name: %s, PID: %d, State: %ld\n", sibling->comm, sibling->pid, sibling->stats);
        }
    }
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
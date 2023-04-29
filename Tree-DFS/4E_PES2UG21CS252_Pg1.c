#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>



char buffer[256];

char * get_task_state(long __state)
{
    switch (__state) {
        case TASK_RUNNING:
            return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
        case __TASK_STOPPED:
            return "__TASK_STOPPED()";
        case __TASK_TRACED:
            return "__TASK_TRACED";
        default:
        {
            sprintf(buffer, "Unknown Type\t");
            return buffer;
        }
    }
}

static void print_process_tree(struct task_struct *task, int depth) {
    struct task_struct *child;
    struct list_head *list;

    // print task info
    pr_info("%*s|- %s\tPID:%d\tStatus Code:%s\tState:%ld\n ", depth*4, "", task->comm, task->pid, get_task_state(task->__state),task->__state);

    // iterate over children
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        print_process_tree(child, depth+1); // recursively print children
    }
}

static int my_init(void)
{
    struct task_struct *task_list;

    pr_info("%s: In init\n", __func__);
    pr_info("Process Tree:\n");
    print_process_tree(&init_task, 0);
    return 0;
}

static void my_exit(void)
{
    pr_info("%s: In exit\n", __func__);
}
MODULE_LICENSE("GNU General Public License v3.0");
MODULE_DESCRIPTION("Listing tasks in Tree Structure");
MODULE_AUTHOR("Lakshya Singh");
module_init(my_init);
module_exit(my_exit);


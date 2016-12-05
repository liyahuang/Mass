#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/moduleparam.h>

static int pid;
module_param(pid,int,0644);

static int display_init(void)
{
	struct task_struct *p,*temp_p;
	struct list_head *pos;
	p = pid_task(find_get_pid(pid),PIDTYPE_PID);
	if(p != NULL)
	{
		
		temp_p = p->real_parent;
		printk(KERN_ALERT"Parent:\n%20s%10d\n",temp_p->comm,temp_p->pid);
		
		printk(KERN_ALERT"Sibling:\n");
		list_for_each(pos,&(p->sibling))
		{
			temp_p = list_entry(pos,struct task_struct,sibling);
			printk(KERN_ALERT"%20s %10d",temp_p->comm,temp_p->pid);
		}

		printk(KERN_ALERT"Children:\n");
		list_for_each(pos,&(p->children))
		{
			temp_p = list_entry(pos,struct task_struct,children);
			printk(KERN_ALERT"%20s %10d",temp_p->comm,temp_p->pid);
		}
	}
	else
	{
		printk(KERN_ALERT"NO THIS TASK!\n");
	}
}

static void display_exit(void)
{
	printk(KERN_ALERT"Exit\n");
}

module_init(display_init);
module_exit(display_exit);
MODULE_LICENSE("GPL");


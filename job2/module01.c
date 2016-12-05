#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>
static int display_init(void)
{
	struct task_struct *p=NULL;

	for_each_process(p)
	{
		if (p->mm !=NULL)
		{
			continue;
		}
		printk(KERN_ALERT"%20s%20d%10d",p->comm,p->pid,p->state);
	}
	return 0;
}

static void display_exit(void)
{
	printk(KERN_ALERT"Exit\n");
}

module_init(display_init);
module_exit(display_exit);
MODULE_LICENSE("GPL");


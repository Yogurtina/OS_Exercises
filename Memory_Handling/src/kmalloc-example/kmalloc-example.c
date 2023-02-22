# include "linux/slab.h"
#include "asm/processor.h"
#include "linux/list.h"
#include "linux/nodemask.h"
#include "linux/printk.h"
#include "linux/sched.h"
#include <linux/kernel.h>
#include <linux/module.h>


MODULE_DESCRIPTION("Memory allocation");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

char *my_memory ;


static int my_init(void)
{       

        my_memory = kmalloc (4096 * sizeof (char), GFP_KERNEL );
        if (!my_memory) printk("Error in allocating memory!");

        int length = sizeof(my_memory)/sizeof(my_memory[0]);

        printk("Length: %d!\n", length);

        for (int i=0; i<50; i++){
            printk("%c ", my_memory[i]);
        }
        return 0;
}

static void my_exit(void)
{
        kfree(my_memory);
        printk("Memory freed\n");
}

module_init(my_init);
module_exit(my_exit);


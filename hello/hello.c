#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohit Sharma");
MODULE_DESCRIPTION("Hello world simple module");
MODULE_VERSION("0.69");

static char *name = "world"; // Example of argument 
module_param(name, charp, S_IRUGO);  // Parametre descrtiopn, name, chap = char pointer.S_IRUGO = Read and not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

// Initialisation function

int __init helloBBB_init(void){
	printk(KERN_INFO "ELK : Hello %s from the Kernel world \n", name);
	return 0;
}

//Brief cleanup function

static void __exit helloBBB_exit(void){
	printk(KERN_INFO " EBB: Goodbye %s from Kernel space LKM \n ", name);
}

// Module must use the module_init() and module_exit macros from linux/init.h
// Which helps in identifying initialise and cleanup function

module_init(helloBBB_init);
module_exit(helloBBB_exit);
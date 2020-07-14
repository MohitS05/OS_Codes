/*

 @file chard.c
 @author Mohit Sharma
 @date 30 June 2020
 @version 0.1
 @brief An character driver to read and write from user space to this LKM

 */

#include <linux/init.h>		// __init and __exit
#include <linux/module.h>	// Core header to load LKMs to kernel
#include <linux/device.h>	//Header to support kernel driver module
#include <linux/kernel.h>	// Types, macros, functions for kernel
#include <linux/fs.h>		// Header for linux file system support
#include <linux/uaccess.h>		//Copy to user functions

#include<linux/mutex.h> 		// For the multithreads

#define DEVICE_NAME "Mcharr"
#define CLASS_NAME "ebb"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohit Sharma");
MODULE_DESCRIPTION("Hello world simple module");
MODULE_VERSION("0.69");

static int majorNumber;				//Store device number- Automated determination
static char message[256] = {0};		//Memory for string that is to be passed
static short size_of_message;		//Used to store string lenght
static int numberOpens = 0;  		//Counts number of times device has been opened

static struct class* m1charclass = NULL;	// Device driver class struct pointer
static struct device* m1chardevice = NULL;	// Device driver device struct pointer

static DEFINE_MUTEX(mych_mutex);			//Declare new mutex

// Prototype functions

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode* , struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/*
	As devices are represented as file structure in kernel. File_operations structure from
	linux/fs.h lists the callback functions.
*/

static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

/*
	The LKM intialisation function
	The static keyword restricts visibility of the function within this C file.

	@return returns 0 if successful.
*/

static int __init chard__init(void){
	printk(KERN_INFO "LKMchar: intialisation of the chardriver LKM\n");

	//Dynamically allocate a major number for the device 
	majorNumber = register_chrdev(0,DEVICE_NAME, &fops);
	if(majorNumber<0){
		printk(KERN_ALERT "LKMchar failed to register a major number\n");
		return majorNumber;
	}

	printk(KERN_INFO "LKMchar: registered a major number successfully %d \n",majorNumber);

	//Register the device class
	m1charclass = class_create(THIS_MODULE, CLASS_NAME);

	if(IS_ERR(m1charclass)){		//Check for error and cleanup if error is present
	unregister_chrdev(majorNumber,DEVICE_NAME);
	printk(KERN_ALERT "Failed to register device class \n");
	return PTR_ERR(m1charclass);
	}

	printk(KERN_INFO "LKMchar: Device class registered correctly \n"); // Device class registered

	//Register device driver
	m1chardevice = device_create(m1charclass, NULL, MKDEV(majorNumber,0),NULL,DEVICE_NAME);
	if (IS_ERR(m1chardevice)){
		class_destroy(m1charclass);				// Cleanup in case of failure
		unregister_chrdev(majorNumber,DEVICE_NAME);
		printk(KERN_ALERT " Failed to create a device \n");
		return PTR_ERR(m1chardevice);
	}
	printk(KERN_INFO "LKMchar: Device created and initialised correctly \n"); //Device initalised
	mutex_init(&mych_mutex);  // dynamic init of mutex

	return 0;
}

static void __exit chard__exit(void){
	mutex_destroy(&mych_mutex);
	device_destroy(m1charclass,MKDEV(majorNumber,0));
	class_unregister(m1charclass);
	class_destroy(m1charclass);
	unregister_chrdev(majorNumber,DEVICE_NAME);
	printk(KERN_INFO "LKMchar: Goodbye from the chardriver LKM! \n ");
}

/*
	@brief The device open function that is czlled each time device is opened
	this will increment numberOpens.
	@param inodep A pointer to an inode object (defined in linux/fs.h)
	@param filep A pointer to a file object (defined in linux/fs.h)
 */

static int dev_open(struct inode * inodep , struct file * filep){

	if(!mutex_trylock(&mych_mutex)){		// return 0 if fail to aquire mutex
		printk(KERN_ALERT "LKMchar: Device use by another process");
		return -EBUSY;
	}
	//If aquired
	numberOpens++;
	printk(KERN_INFO "LKMchar: Device has been opened %d times \n", numberOpens);
	return 0;
}

/*
	@brief This function is called whenever device is being read from user space i.e. data 
	is being sent from device to the user.  copy_to_user() syscall will be used.

	@param filep A pointer to a file object (defined in linux/fs.h)
 	@param buffer The pointer to the buffer to which this function writes the data
 	@param len The length of the b
 	@param offset The offset if required
*/

static ssize_t dev_read(struct file * filep, char * buffer, size_t len, loff_t *offset){
	int error_count = 0;
	error_count = copy_to_user(buffer, message, size_of_message);

	if (error_count == 0){
		printk(KERN_INFO "LKMchar: Sent %d characters to the user \n", size_of_message);
		return ( size_of_message = 0); // clear the position to start
	}
	else{
		printk( KERN_INFO "LKMchar: Failed to send %d characters to user \n", error_count);
		return -EFAULT; // Failed- Return bad address message ( -14)		
	}
}


/* @brief This function is called whenever the device is being written to from user space i.e. 
	data is being sent to the device from the user. 
	ram filep A pointer to a file object

 	@param buffer The buffer to that contains the string to write to the device
 	@param len The length of the array of data that is being passed in the const char buffer
 	@param offset The offset if required
*/

static ssize_t dev_write (struct file *filep, const char *buffer, size_t len, loff_t *offset){
	sprintf(message,"%s(%zu letters)",buffer, len);	// appending recieved string with it's length
	size_of_message = strlen(message);				// storing the lenght of the message recieved
	printk(KERN_INFO "LKMchar: Recieved %zu characters from the user \n", len);
	return len;
}

/* @brief The device release function. Needs to be called whenever device is being
	released/closed by the user space program.
	@param inodep A pointer to inode object
	@param filep A pointer to file object
*/
static int dev_release(struct inode * inodep, struct file *filep){
	mutex_unlock(&mych_mutex);
	printk(KERN_INFO "LKMchar: Device successfully closed\n");
	return 0;
}

// Defining the init and cleanup function

module_init(chard__init);
module_exit(chard__exit);
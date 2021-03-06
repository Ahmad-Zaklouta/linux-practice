#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define EOK 0;
volatile static int is_open = 0; // keep track if the device is open

static char message[1024];
int num_bytes = 0;

static int devnum;

ssize_t hello_read (struct file *filep, char __user *outb, size_t nbytes, loff_t *offset)
{
	int bytes_read = 0;

	if (offset == NULL) return -EINVAL;

	if (*offset >= num_bytes) return 0;  

	while ((bytes_read < nbytes) && (*offset < num_bytes)) 
	{
		put_user(message[*offset], &outb[bytes_read]);
		*offset = *offset + 1;
		bytes_read++;
	}

	return bytes_read;
}

ssize_t hello_write (struct file *filep, const char __user *inpb, size_t nbytes, loff_t *offset)
{
	int bytes_write = 0;

	if (offset == NULL) return -EINVAL;

	if (*offset >= num_bytes) return -EINVAL;  

	while ((bytes_write < nbytes) && (*offset < 1023)) 
	{
		get_user(message[*offset], &inpb[bytes_write]);
		*offset = *offset + 1;
		bytes_write++;
	}

	num_bytes = *offset;
	printk(KERN_INFO "(%s)\n", message);
	return bytes_write;
}

int hello_open (struct inode *inodep, struct file *filep)
{
	if (is_open == 1)
	{
		printk(KERN_INFO "Error - hello is already open\n");
		return -EBUSY;
	}
	is_open = 1;

	try_module_get(THIS_MODULE);
	return EOK;
}

int hello_release (struct inode *inodep, struct file *filep)
{
	if (is_open == 0)
	{
		printk(KERN_INFO "Error - device wasn't open\n");
		return -EBUSY;
	}
	is_open = 0;
	
	module_put(THIS_MODULE);  // prevent the module from unloading if something is waitnig for it.
	return EOK;
}

struct file_operations fops =
{
	read: hello_read,
	write: hello_write,
	open: hello_open,
	release: hello_release
};

static int __init hello_start(void)
{
	printk(KERN_INFO "Hello, I'm here to help\n");

	strncpy(message, "Hello world", 1023);
	num_bytes = strlen(message);

	// int register_chrdev(unsigned int major, const char *name, struct file_operations *fops);
	devnum = register_chrdev(0, "hello", &fops);
	printk(KERN_INFO "the hello device is major: %d\n", devnum);

	return 0;
}

static void __exit hello_end(void)
{
	
	unregister_chrdev(devnum, "hello");
	printk(KERN_INFO "Goodbye, I hope I was helpfull\n");
}

module_init(hello_start);
module_exit(hello_end);
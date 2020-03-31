#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>   
#include<linux/random.h> //to generate random no.
#include<linux/errno.h>


#define MAGIC_NUMBER 'A'
#define ch_SELECT_SEQNO 1
#define RESULT_ALIGNMENT_SEQNO 2

#define ch_SELECT _IOW(MAGIC_NUMBER, ch_SELECT_SEQNO, unsigned int)
#define RESULT_ALIGNMENT _IOW(MAGIC_NUMBER, RESULT_ALIGNMENT_SEQNO, unsigned int)


static dev_t first;		//Global variable for the first device number

static struct cdev c_dev;	//Global variable for the Creation of device file
static struct class *cls;	//Global variable for the Creation of device file


uint16_t ch;
uint16_t align;

//Step 4 : Driver callback functions

static int My_Open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "ADC8 device : open()\n");
	return 0;
}

static int my_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "ADC8 device : close()\n");
	return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	uint16_t result;
	printk(KERN_INFO "ADC8 device : read()\n");
	get_random_bytes(&result, 2);
	printk(KERN_INFO "ADC8 device : - %d\n",result);
	result = result%1024;
	copy_to_user(buf, &result, sizeof(result));
	printk(KERN_INFO "ADC8 device : read value form ch - %d is %d\n",ch, result);
	return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{

	switch(cmd){
		case ch_SELECT:
		printk(KERN_INFO "ADC8 device : IOCTL - ch_SELECT\n");
		copy_from_user(&ch, (uint16_t *) arg, sizeof(ch));
		printk(KERN_INFO "ADC8 device : Selected ch - %d\n",ch);
		break;
		case RESULT_ALIGNMENT:
		printk(KERN_INFO "ADC8 device : IOCTL - RESULT_ALIGNMENT\n");
		copy_from_user(&align, (uint16_t *) arg, sizeof(align));
		printk(KERN_INFO "ADC8 device : Selected Alignment - %d\n",align);
		break;
		default:
		return -ENOTTY;
		}
	return 0;
}

static struct file_operations fops = 
				{
				.owner	= THIS_MODULE,
				.open	= My_Open,
				.release= my_close,
				.read	= my_read,
				.unlocked_ioctl = my_ioctl
				};

static int __init adc_init(void)
{
	printk(KERN_INFO "8-ch ADC Device Registration\n\n");

	//Step-1 : Reserve Major ans Minor Numbers for ADC8
	if(alloc_chrdev_region(&first, 0, 1, "adc8") < 0)
	{
		return -1;
	}
	printk(KERN_INFO "<Major , Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));

	//Step-2 : Creation of ADC8 device file
	if((cls = class_create(THIS_MODULE, "chardev")) == NULL)
	{
		unregister_chrdev_region(first,1);
		return -1;
	}
	
	if(device_create(cls, NULL, first, NULL, "adc8") == NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}

	//Step-3 : Link fops and cdev to the device node
	cdev_init(&c_dev, &fops);
	if(cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}

	return 0;
}

static void __exit adc_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "8-ch ADC Device Unregistrated");	
}

module_init(adc_init);
module_exit(adc_exit);


MODULE_AUTHOR("C.RATNA KUMARI - 2019H1400555H");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("8-ch ADC Char DD");


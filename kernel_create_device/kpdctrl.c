#include <mach/hal_pub_kpd.h>
#include <linux/kpd.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/aio.h>
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/compat.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#include <linux/device.h>

#include <linux/kpd.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <mach/eint.h>
#include <mach/board.h>



#include <mach/hal_pub_kpd.h>
#include <mach/hal_priv_kpd.h>
#include <mach/mt_clkmgr.h>


int kpdctrl_major = 555;
int kpdctrl_minor = 0;
int number_of_devices = 1;

static int kpdctrl_open(struct inode *inode, struct file *filp)
{
	printk("via kpd spidev_open\n");
	return 0;

}


static ssize_t kpdctrl_write(struct file *filp, const char __user *buf,size_t count)
{
	printk("via kpd kpdctrl_write buf: %s\n",buf);
	if(!strcmp(buf,"kpdctrl"))
	{
		printk("via kpd kpdctrl_write kpdctrl start\n");
	}
	return -EFAULT;
}

static ssize_t kpdctrl_read(struct file *filp, char __user *buf, size_t count)
{
	printk("via kpd kpdctrl_read buf: %s\n",buf);
	return 0;
}







static int kpdctrl_release(struct inode *inode, struct file *filp)
{
	printk("via kpd kpdctrl_release\n");
	return 0;
}


static struct file_operations kpdctrl_fops = {
	.owner      =   THIS_MODULE,
	.open       =   kpdctrl_open,
	.read           =   kpdctrl_read,
	.write          =   kpdctrl_write,
	.release    =   kpdctrl_release,
};



static int __init kpdctrl_init(void)
{
	printk("via kpd kpdctrl_init\n");

	struct file *fp;
	fp = filp_open("/sdcard/kpdctrl",O_RDWR | O_CREAT,0644);
	if(IS_ERR(fp))
		printk("via kpd create open error\r\n");
	else
	{
		char buf[128] = "ttttt";
		loff_t pos;
		set_fs(KERNEL_DS);
		pos = 0;
		printk("via kpd vsf_write\n");
		vfs_write(fp,buf,sizeof(buf),&pos);
	}



	struct class *myclass = class_create(THIS_MODULE, "kpdctrl");

	device_create(myclass, NULL, MKDEV(555, 0), NULL, "kpdctrl");
	if (IS_ERR(myclass)) {
		printk("via kpd class_create failed\n");
	}
	else
		printk("via kpd class_create successed\n");
	int status;
	status = register_chrdev(555, "kpdctrl", &kpdctrl_fops);
	if (status < 0)
	{
		printk("via kpd kpdctrl_init failed\n");
		return status;
	}




	return status;
}

static void __exit kpdctrl_exit(void)
{	
	printk("via kpd kpdctrl_exit\n");
	unregister_chrdev(0, "kpdctrl");

	return;
}




module_init(kpdctrl_init);
module_exit(kpdctrl_exit);

MODULE_DESCRIPTION ( "mt kpd device driver" );
MODULE_AUTHOR ( "Ranran Lu <ranran.lu@mediatek.com>" );
MODULE_LICENSE("GPL");




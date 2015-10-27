/*
 * Version.c - Thomas Moussajee 
 */
#include <linux/module.h>
#include <generated/compile.h>
#include <linux/miscdevice.h>
#include <linux/syscalls.h>
#include <linux/utsname.h>
#include <linux/slab.h>
#include "version.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMAS MOUSSAJEE");
MODULE_DESCRIPTION("KERNEL VERSION DEVICE");

static const char linux_proc_banner2[] =
	"%s version %s"
	" (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ")"
	" (" LINUX_COMPILER ") %s\n";

static struct file_operations fops =
	{
	  .owner = THIS_MODULE,
	  .read = device_read,
	  .write = device_write,
	  .open = device_open,
	  .release = device_release,
	  .llseek = device_lseek,
	  .unlocked_ioctl = device_ioctl,
	};

static struct miscdevice misc_dev = 
	{ 
	  .minor = MISC_DYNAMIC_MINOR,
	  .name = DEVICE_NAME,
	  .fops = &fops,
	  .mode = S_IRWXUGO, 
	};

static char used = 0;
static char trust = 0;
static char init = 0;
static char *version;
static char *message;

static int get_version(void)
{
	struct new_utsname *info;

	if (!init) {
	  message = kmalloc(BUF_LEN, GFP_KERNEL);
	  version = kmalloc(BUF_LEN, GFP_KERNEL);
	  if (!message || !version)
	    return -1;

	  memset(message, 0, BUF_LEN);
	  memset(version, 0, BUF_LEN);
	  info = utsname();
	  if (!info)
	    return -1;

	  if (sprintf(version, linux_proc_banner2, info->sysname,
		      info->release, info->version) < 0)
	    return -1;
	  init = 1;
	}

	if (sprintf(message, "%s", version) < 0)
	  return -1;
	trust = 1;

	return 0;
}

static void clean_message(void)
{
	memset(message, 0, BUF_LEN);
	trust = 0;
}  

int init_module(void)
{
	int ret = 0;
  
	ret = get_version();
	if (ret)
	  return ret;

	ret = misc_register(&misc_dev);
	return ret;
}

void cleanup_module(void)
{
	misc_deregister(&misc_dev);
}

static int device_open(struct inode *inode, struct file *file)
{
	if (used)
	  return -EBUSY;

	used++;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	used--;
	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file * file, char __user * buf, size_t count, loff_t *ppos)
{
	int maxbytes;
	int bytes_to_do;
	int nbytes;

	maxbytes = BUF_LEN - 1 - *ppos;
	if (maxbytes > count)
	  bytes_to_do = count;
	else
	  bytes_to_do = maxbytes;

	if (bytes_to_do == 0)
	  return 0;

	nbytes = bytes_to_do - copy_to_user(buf,  message + *ppos, bytes_to_do );
	*ppos += nbytes;
	return nbytes; 
}

static ssize_t device_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	int nbytes;
	int bytes_to_do;
	int maxbytes;

	if (!(*ppos))
	  clean_message();
  
	maxbytes = BUF_LEN - 1 - *ppos;

	if (maxbytes > count)
	  bytes_to_do = count;
	else
	  bytes_to_do = maxbytes;

	if (bytes_to_do == 0)
	  return 0;
  
	nbytes = bytes_to_do - copy_from_user(message + *ppos, buf, bytes_to_do);
	*ppos += nbytes;
	return nbytes;
}

static loff_t device_lseek(struct file *file, loff_t ppos, int orig)
{
	loff_t new_pos = 0;

	switch(orig)
	  {
	  case 0: /* SEEK_SET: */
	    new_pos = ppos;
	    break;
	  case 1: /* SEEK_CUR: */
	    new_pos = file->f_pos + ppos;
	    break;
	  case 2: /* SEEK_END: */
	    new_pos = BUF_LEN - 1 - ppos;
	    break;
	  }
	if (new_pos > BUF_LEN - 1)
	  new_pos = BUF_LEN - 1;
	if (new_pos < 0)
	  new_pos = 0;
	file->f_pos = new_pos;
	return new_pos;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
  
	switch (cmd) {
	case VERSION_MODIFIED:
	  {
	    if (!trust)
	      ret = 1;
	    break;
	  }
	case VERSION_RESET:
	  {
	    clean_message();
	    ret = get_version();
	    break;
	  }
	default:
	  ret = -EINVAL; 
	}
	return ret;
}

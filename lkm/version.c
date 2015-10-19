/*
 * Version.c - Thomas Moussajee 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> 
#include <linux/major.h> 
#include <linux/capability.h> 
#include <asm/uaccess.h> 
#include <linux/miscdevice.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/utsname.h>
#include <linux/string.h>
#include "version.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMAS MOUSSAJEE");
MODULE_DESCRIPTION("KERNEL VERSION DEVICE");

static struct file_operations fops =
  {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
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
static char message[BUF_LEN] = {0};
static char *msg_ptr;

static int get_version(void)
{
  struct new_utsname *info;

  info = utsname();
  if (!info)
    return -1;
  
  sprintf(message, "%s\n", info->release);
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
  if (ret) {
    printk("Unable get kernel version\n");
    return ret;
  }
  
  printk(KERN_ALERT "module init \n");
  ret = misc_register(&misc_dev);
  if (ret)
    printk("Unable to register misc dev\n");

  return ret;
}

void cleanup_module(void)
{
  misc_deregister(&misc_dev);
}

/* 
 * Called when a process open the device file.
 */
static int device_open(struct inode *sinode, struct file *sfile)
{
  if (used)
    return -EBUSY;

  used++;
  msg_ptr = message;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
  used--;
  printk("device closed\n");
  module_put(THIS_MODULE);
  
  return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file * file, char __user * buf, size_t count, loff_t *ppos)
{
  int bytes_read = 0;

  if (!(*msg_ptr))
    return 0;

  while (count && *msg_ptr)  {
    put_user(*(msg_ptr++), buf++);
    count--;
    bytes_read++;
  }

  return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/version
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
  int i;

  clean_message();
  for (i = 0; i < len && i < BUF_LEN; i++)
    get_user(message[i], buff + i);
  
  msg_ptr = message;
  
  return i;
}

/*
 * Called when a process use ioclt on dev file
 */
static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
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
      get_version();
      break;
    }
  default:
    ret = -EINVAL; 
  }
  return ret;
}

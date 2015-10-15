/*
 * Version.c - Thomas Moussajee 
 */
#ifndef VERSION_H
#define VERSION_H

#define SUCCESS 0
#define DEVICE_NAME "chardev"/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80/* Max length of the message from the device */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#endif
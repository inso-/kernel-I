/*
 * Version.h - Thomas Moussajee 
 */
#ifndef VERSION_H
#define VERSION_H

#define SUCCESS 0
#define DEVICE_NAME "version"
#define BUF_LEN 256 

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int get_version(void);
static void clean_message(void);
#endif

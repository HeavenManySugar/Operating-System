#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_NAME "seconds"

static unsigned long start_jiffies;

static int seconds_show(struct seq_file *m, void *v) {
    seq_printf(m, "%lu\n", (jiffies - start_jiffies) / HZ);
    return 0;
}

static int seconds_open(struct inode *inode, struct file *file) {
    return single_open(file, seconds_show, NULL);
}

static const struct proc_ops seconds_ops = {
    .proc_open = seconds_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init seconds_init(void) {
    start_jiffies = jiffies;
    proc_create(PROC_NAME, 0, NULL, &seconds_ops);
    return 0;
}

static void __exit seconds_exit(void) { remove_proc_entry(PROC_NAME, NULL); }

MODULE_LICENSE("GPL");
module_init(seconds_init);
module_exit(seconds_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(birthday_list);

void add_birthday(int day, int month, int year) {
    struct birthday *new_birthday;
    new_birthday = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    new_birthday->day = day;
    new_birthday->month = month;
    new_birthday->year = year;
    INIT_LIST_HEAD(&new_birthday->list);
    list_add_tail(&new_birthday->list, &birthday_list);
}

/* This function is called when the module is loaded. */
int birthday_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    add_birthday(1, 2, 3);
    add_birthday(4, 5, 6);
    add_birthday(7, 8, 0);
    add_birthday(2, 5, 6);
    add_birthday(5, 5, 6);

    struct birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list) {
        printk(KERN_INFO "day %d month %d year %d\n", ptr->day, ptr->month, ptr->year);
    }
    return 0;
}

/* This function is called when the module is removed. */
void birthday_exit(void) {
    struct birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(birthday_init);
module_exit(birthday_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List Test Module");
MODULE_AUTHOR("sheep");

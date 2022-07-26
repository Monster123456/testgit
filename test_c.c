Skip to content
Search or jump to…
Pull requests
Issues
Marketplace
Explore
 
@Monster123456 
autoas
/
as
Public
Code
Issues
11
Pull requests
Actions
Projects
Wiki
Security
Insights
as/com/as.infrastructure/system/dev/device.c
@parai
parai bring up vfs on device, prepare for vfs_mount
Latest commit 7f491f0 on 3 Mar 2019
 History
 1 contributor
113 lines (102 sloc)  2.77 KB

/**
 * AS - the open source Automotive Software on https://github.com/parai
 *
 * Copyright (C) 2019 AS <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
/* ============================ [ INCLUDES  ] ====================================================== */
#include "device.h"
/* ============================ [ MACROS    ] ====================================================== */
#ifndef DEV_LOCK
#define DEV_LOCK() do { imask_t mask; Irq_Save(mask)
#endif

#ifndef DEV_UNLOCK
#define DEV_UNLOCK() Irq_Restore(mask); } while(0)
#endif
/* ============================ [ TYPES     ] ====================================================== */
typedef struct device_list
{
	const device_t* dev;
	TAILQ_ENTRY(device_list) entry;
} device_list_t;
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
static TAILQ_HEAD(device_head, device_list) device_list = TAILQ_HEAD_INITIALIZER(device_list);
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
void device_init(void)
{
	TAILQ_INIT(&device_list);
}

int device_register(const device_t* device)
{
	int ercd = 0;
	device_list_t * devL = NULL;

	DEV_LOCK();
	TAILQ_FOREACH(devL, &device_list, entry)
	{
		if(0 == strcmp(devL->dev->name, device->name))
		{
			ercd = EEXIST;
			break;
		}
	}
	DEV_UNLOCK();

	if(0 == ercd)
	{
		devL = malloc(sizeof(*device));
		if(NULL != devL)
		{
			devL->dev = device;
			DEV_LOCK();
			TAILQ_INSERT_TAIL(&device_list, devL, entry);
			DEV_UNLOCK();
		}
		else
		{
			ercd = ENOMEM;
		}
	}

	return ercd;
}

int device_unregister(const device_t* device)
{
	int ercd = 0;
	device_list_t * devL;

	DEV_LOCK();
	TAILQ_FOREACH(devL, &device_list, entry)
	{
		if(device == devL->dev)
		{
			TAILQ_REMOVE(&device_list, devL, entry);
			free(devL);
			break;
		}
	}
	DEV_UNLOCK();

	return 0;
}

const device_t* device_find(const char* name)
{
	const device_t * device = NULL;
	device_list_t * devL;

	DEV_LOCK();
	TAILQ_FOREACH(devL, &device_list, entry)
	{
		if(0 == strcmp(devL->dev->name, name))
		{
			device = devL->dev;
			break;
		}
	}
	DEV_UNLOCK();

	return device;
}
Footer
© 2022 GitHub, Inc.
Footer navigation
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About

/*
 * Copyright (c) Nordic Semiconductor ASA. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor ASA.
 * The use, copying, transfer or disclosure of such information is prohibited except by
 * express written agreement with Nordic Semiconductor ASA.
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/poweroff.h>

int main(void)
{
	k_sleep(K_MSEC(1000));

	volatile int i = 10000000;
	while(i){
		i--;
	}

	sys_poweroff();

	return 0;
}

/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file implements the OpenThread module initialization and state change handling.
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>

#include "platform/platform-zephyr.h"
#include "openthread.h"

#if defined(CONFIG_OPENTHREAD_NAT64_TRANSLATOR)
#include <openthread/nat64.h>
#endif /* CONFIG_OPENTHREAD_NAT64_TRANSLATOR */

#define PKT_IS_IPv4(_p) ((NET_IPV6_HDR(_p)->vtc & 0xf0) == 0x40)

#define OT_module_SIZE (CONFIG_OPENTHREAD_THREAD_module_SIZE)

#if defined(CONFIG_OPENTHREAD_THREAD_PREEMPTIVE)
#define OT_PRIORITY K_PRIO_PREEMPT(CONFIG_OPENTHREAD_THREAD_PRIORITY)
#else
#define OT_PRIORITY K_PRIO_COOP(CONFIG_OPENTHREAD_THREAD_PRIORITY)
#endif

#if defined(CONFIG_OPENTHREAD_NETWORK_NAME)
#define OT_NETWORK_NAME CONFIG_OPENTHREAD_NETWORK_NAME
#else
#define OT_NETWORK_NAME ""
#endif

#if defined(CONFIG_OPENTHREAD_CHANNEL)
#define OT_CHANNEL CONFIG_OPENTHREAD_CHANNEL
#else
#define OT_CHANNEL 0
#endif

#if defined(CONFIG_OPENTHREAD_PANID)
#define OT_PANID CONFIG_OPENTHREAD_PANID
#else
#define OT_PANID 0
#endif

#if defined(CONFIG_OPENTHREAD_XPANID)
#define OT_XPANID CONFIG_OPENTHREAD_XPANID
#else
#define OT_XPANID ""
#endif

#if defined(CONFIG_OPENTHREAD_NETWORKKEY)
#define OT_NETWORKKEY CONFIG_OPENTHREAD_NETWORKKEY
#else
#define OT_NETWORKKEY ""
#endif

#if defined(CONFIG_OPENTHREAD_JOINER_PSKD)
#define OT_JOINER_PSKD CONFIG_OPENTHREAD_JOINER_PSKD
#else
#define OT_JOINER_PSKD ""
#endif

#if defined(CONFIG_OPENTHREAD_PLATFORM_INFO)
#define OT_PLATFORM_INFO CONFIG_OPENTHREAD_PLATFORM_INFO
#else
#define OT_PLATFORM_INFO ""
#endif

#if defined(CONFIG_OPENTHREAD_POLL_PERIOD)
#define OT_POLL_PERIOD CONFIG_OPENTHREAD_POLL_PERIOD
#else
#define OT_POLL_PERIOD 0
#endif

#define ZEPHYR_PACKAGE_NAME "Zephyr"
#define PACKAGE_VERSION	    KERNEL_VERSION_STRING

static void openthread_process(struct k_work *work)
{
	struct openthread_context *ot_context =
		CONTAINER_OF(work, struct openthread_context, api_work);

	openthread_api_mutex_lock(ot_context);

	while (otTaskletsArePending(ot_context->instance)) {
		otTaskletsProcess(ot_context->instance);
	}

	otSysProcessDrivers(ot_context->instance);

	openthread_api_mutex_unlock(ot_context);
}

int openthread_module_state_changed_cb_register();
int openthread_module_state_changed_cb_unregister();

struct openthread_context *openthread_module_get_default_context();
struct otInstance *openthread_module_get_default_instance();

int openthread_module_init(openthread_module_context *context)
{
	struct k_work_queue_config q_cfg = {
		.name = "openthread",
		.no_yield = true,
	};
	otError err = OT_ERROR_NONE;

	k_mutex_init(&context->lock);
	k_work_init(&context->work, openthread_process);
}

int openthread_module_start();

void openthread_module_api_mutex_lock(openthread_module_context *context)
{
	(void)k_mutex_lock(&context->lock, K_FOREVER);
}

int openthread_module_api_mutex_try_lock(openthread_module_context *context)
{
	return k_mutex_lock(&context->lock, K_NO_WAIT);
}

void openthread_module_api_mutex_unlock(openthread_module_context *context)
{
	(void)k_mutex_unlock(&context->lock);
}
/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file is the public header file for the OpenThread module.
 *
 */

#ifndef MODULES_OPENTHREAD_OPENTHREAD_H_
#define MODULES_OPENTHREAD_OPENTHREAD_H_

#include <openthread/openthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct openthread_module_context {
	otInstance *instance;
	struct k_mutex lock;
	struct k_work_q work_q;
	struct k_work work;
	sys_slist_t state_change_cbs;
};

int openthread_module_state_changed_cb_register();
int openthread_module_state_changed_cb_unregister();

struct openthread_context *openthread_module_get_default_context();
struct otInstance *openthread_module_get_default_instance();

int openthread_module_init(openthread_module_context *context);
int openthread_module_start();

void openthread_module_api_mutex_lock();
int openthread_module_api_mutex_try_lock();
void openthread_module_api_mutex_unlock();

#ifdef __cplusplus
}
#endif

#endif /* MODULES_OPENTHREAD_OPENTHREAD_H_ */

/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is licensed under the terms of the BSD 3-clause
 * license. See the file LICENSE distributed with this software for
 * full license information.
 ********************************************************************/

#include "osal_log.h"
#include "base/task/Mutex.h"
#include "bsp-interface/di/console.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace
{
	char _buffer[1024];
	base::task::Mutex _mutex{};

} // namespace

void os_log(uint8_t type, char const *fmt, ...)
{
	base::task::MutexGuard g{_mutex};

	// 初始化缓冲区
	_buffer[0] = '\0';

	// 根据日志级别添加前缀
	switch (LOG_LEVEL_GET(type))
	{
	case LOG_LEVEL_DEBUG:
		snprintf(_buffer, sizeof(_buffer), "%10ld [DEBUG] ", xTaskGetTickCount());
		break;
	case LOG_LEVEL_INFO:
		snprintf(_buffer, sizeof(_buffer), "%10ld [INFO ] ", xTaskGetTickCount());
		break;
	case LOG_LEVEL_WARNING:
		snprintf(_buffer, sizeof(_buffer), "%10ld [WARN ] ", xTaskGetTickCount());
		break;
	case LOG_LEVEL_ERROR:
		snprintf(_buffer, sizeof(_buffer), "%10ld [ERROR] ", xTaskGetTickCount());
		break;
	case LOG_LEVEL_FATAL:
		snprintf(_buffer, sizeof(_buffer), "%10ld [FATAL] ", xTaskGetTickCount());
		break;
	default:
		return; // 如果是未知的日志级别，直接返回
	}

	// 获取当前缓冲区的长度
	int len = strlen(_buffer);

	// 将格式化后的消息追加到缓冲区
	va_list list{};
	va_start(list, fmt);
	vsnprintf(_buffer + len, sizeof(_buffer) - len, fmt, list);
	va_end(list);

	bsp::di::Console().WriteLine(_buffer);
}

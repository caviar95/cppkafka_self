/*
 * @Author: Caviar
 * @Date: 2025-01-13 23:46:47
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-13 23:54:06
 * @Description: 
 */

#pragma once

#if defined(_WIN32) && !defined(CPPKAFKA_STATIC)
    #ifdef cppkafka_EXPORTS
        #define CPPKAFKA_API __declspec(dllexport)
    #else
        #define CPPKAFKA_API __declspec(dllimport)
    #endif
#else
    #define CPPKAFKA_API
#endif

#define RD_KAFKA_QUEUE_REFCOUNT_BUG_VERSION 0x000b0500 // v0.11.5
#define RD_KAFKA_HEADERS_SUPPORT_VERSION 0x000b0402 // v0.11.4.2
#define RD_KAFKA_ADMIN_API_SUPPORT_VERSION 0x000b0500 // v0.11.5
#define RD_KAFKA_MESSAGE_LATENCY_SUPPORT_VERSION 0x000b0000 // v0.11.0.00
#define RD_KAFKA_EVENT_STATS_SUPPORT_VERSION 0x000b0000 // v0.11.0.00
#define RD_KAFKA_MESSAGE_STATUS_SUPPORT_VERSION 0x01000002 // v1.0.0.2
#define RD_KAFKA_STORE_OFFSETS_SUPPORT_VERSION 0x00090501 // v0.9.5.1
#define RD_KAFKA_DESTROY_FLAGS_SUPPORT_VERSION 0x000b0600 // v0.11.6

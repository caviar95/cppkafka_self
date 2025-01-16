/*
 * @Author: Caviar
 * @Date: 2025-01-15 14:26:26
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 14:38:36
 * @Description: 
 */

#pragma once

#include <string>
#include <memory>
#include <librdkafka/rdkafka.h>
#include "macros.h"

namespace cppkafka {

class CPPKAFKA_API Topic {
public:
    static Topic make_non_owning(rd_kafka_topic_t* handle);

    Topic();

    Topic(rd_kafka_topic_t* handle);

    std::string get_name() const;

    bool is_partition_available(int partition) const;

    explicit operator bool() const {
        return handle_ != nullptr;
    }

    rd_kafka_topic_t* get_handle() const;

private:
    using HandlePtr = std::unique_ptr<rd_kafka_topic_t, decltype(&rd_kafka_topic_destroy)>;
    struct NonOwningTag {};

    Topic(rd_kafka_topic_t* handle, NonOwningTag);

    HandlePtr handle_;
};

} // namespace cppkafka
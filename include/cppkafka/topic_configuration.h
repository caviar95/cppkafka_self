/*
 * @Author: Caviar
 * @Date: 2025-01-15 10:44:06
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 12:00:12
 * @Description: 
 */

#pragma once

#include <string>
#include <functional>
#include <initializer_list>
#include <librdkafka/rdkafka.h>
#include "clonable_ptr.h"
#include "configuration_base.h"
#include "macros.h"

namespace cppkafka {

class Topic;
class Buffer;

class CPPKAFKA_API TopicConfiguration : public ConfigurationBase<TopicConfiguration> {
public:
    using PartitionerCallback = std::function<int32_t(const Topic&, const Buffer& key, int32_t partition_count)>;
    using ConfigurationBase<TopicConfiguration>::set;
    using ConfigurationBase<TopicConfiguration>::get;

    TopicConfiguration();

    TopicConfiguration(const std::vector<ConfigurationOption> &options);

    TopicConfiguration(const std::initializer_list<ConfigurationOption>& options);

    TopicConfiguration& set(const std::string& name, const std::string& value);

    TopicConfiguration& set_partitioner_callback(PartitionerCallback callback);

    TopicConfiguration& set_as_opaque();

    const PartitionerCallback& get_partitioner_callback() const;

    bool has_property(const std::string& name) const;

    std::string get(const std::string& name) const;

    std::map<std::string, std::string> get_all() const;

    rd_kafka_topic_conf_t* get_handle() const;

private:
    using HandlePtr = ClonablePtr<rd_kafka_topic_conf_t,
                                  decltype(&rd_kafka_topic_conf_destroy),
                                  decltype(&rd_kafka_topic_conf_dup)>;

    TopicConfiguration(rd_kafka_topic_conf_t* ptr);
    static HandlePtr make_handle(rd_kafka_topic_conf_t* ptr);

    HandlePtr handle_;
    PartitionerCallback partitioner_callback_;
};

} // namespace cppkafka

/*
 * @Author: Caviar
 * @Date: 2025-01-15 10:32:47
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 15:03:59
 * @Description: 
 */

#pragma once

#include <memory>
#include <string>
#include <functional>
#include <initializer_list>
#include <chrono>
#include <boost/optional.hpp>
#include <librdkafka/rdkafka.h>
#include "topic_partition_list.h"
#include "topic_configuration.h"
#include "clonable_ptr.h"
#include "configuration_base.h"
#include "macros.h"
#include "event.h"

namespace cppkafka {

class Message;
class Error;
class Producer;
class Consumer;
class KafkaHandleBase;

class CPPKAFKA_API Configuration : public ConfigurationBase<Configuration> {
public:
    using DeliveryReportCallback = std::function<void(Producer &producer, const Message&)>;
    using OffsetCommitCallback = std::function<void(Consumer& consumer, Error error, const TopicPartitionList& topic_partitions)>;
    using ErrorCallback = std::function<void(KafkaHandleBase& handle, int error, const std::string& reason)>;
    using ThrottleCallback = std::function<void(KafkaHandleBase& handle, const std::string& broker_name, int32_t broker_id, std::chrono::milliseconds throttle_time)>;
    using LogCallback = std::function<void(KafkaHandleBase& handle, int level, const std::string& facility, const std::string& message)>;
    using StatsCallback = std::function<void(KafkaHandleBase& handle, const std::string& json)>;
    using SocketCallback = std::function<int(int domain, int type, int protocol)>;
    using BackgroundEventCallback = std::function<void(KafkaHandleBase& handle, Event)>;
    using ConfigurationBase::set;
    using ConfigurationBase::get;

    Configuration();

    Configuration(const std::vector<ConfigurationOption>& options);

    Configuration(const std::initializer_list<ConfigurationOption>& options);

    Configuration& set(const std::string& name, const std::string& value);

    Configuration& set_delivery_report_callback(DeliveryReportCallback callback);

    Configuration& set_offset_commit_callback(OffsetCommitCallback callback);

    Configuration& set_error_callback(ErrorCallback callback);

    Configuration& set_throttle_callback(ThrottleCallback callback);

    Configuration& set_log_callback(LogCallback callback);

    Configuration& set_stats_callback(StatsCallback callback);

    Configuration& set_socket_callback(SocketCallback callback);

#if RD_KAFKA_VERSION >= RD_KAFKA_ADMIN_API_SUPPORT_VERSION
    Configuration& set_background_event_callback(BackgroundEventCallback callback);

    Configuration& set_events(int events);
#endif

    Configuration& set_default_topic_configuration(TopicConfiguration config);

    bool has_property(const std::string& name) const;

    rd_kafka_conf_t* get_handle() const;

    std::string get(const std::string& name) const;

    std::map<std::string, std::string> get_all() const;

    const DeliveryReportCallback& get_delivery_report_callback() const;

    const OffsetCommitCallback& get_offset_commit_callback() const;

    const ErrorCallback& get_error_callback() const;

    const ThrottleCallback& get_throttle_callback() const;

    const LogCallback& get_log_callback() const;

    const StatsCallback& get_stats_callback() const;

    const SocketCallback& get_socket_callback() const;

    const BackgroundEventCallback& get_background_event_callback() const;

    const boost::optional<TopicConfiguration>& get_default_topic_configuration() const;

    boost::optional<TopicConfiguration>& get_default_topic_configuration();

private:
    using HandlePtr = ClonablePtr<rd_kafka_conf_t, decltype(&rd_kafka_conf_destroy), decltype(&rd_kafka_conf_dup)>;

    Configuration(rd_kafka_conf_t* ptr);

    static HandlePtr make_handle(rd_kafka_conf_t* ptr);

    HandlePtr handle_;
    boost::optional<TopicConfiguration> default_topic_config_;
    DeliveryReportCallback delivery_report_callback_;
    OffsetCommitCallback offset_commit_callback_;
    ErrorCallback error_callback_;
    ThrottleCallback throttle_callback_;
    LogCallback log_callback_;
    StatsCallback stats_callback_;
    SocketCallback socket_callback_;
    BackgroundEventCallback background_event_callback_;
};

} // namespace cppkafka
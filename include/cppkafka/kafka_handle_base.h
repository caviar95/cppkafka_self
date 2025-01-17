#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <map>
#include <mutex>
#include <tuple>
#include <librdkafka/rdkafka.h>
#include "group_information.h"
#include "topic_partition.h"
#include "topic_partition_list.h"
#include "topic_configuration.h"
#include "configuration.h"
#include "macros.h"
#include "logging.h"
#include "queue.h"

namespace cppkafka {

class Topic;
class Metadata;
class TopicMetadata;
class GroupInformation;
class CPPKAFKA_API KafkaHandleBase {
public:
    using OffsetTuple = std::tuple<int64_t, int64_t>;
    using TopicPartitionsTimestampsMap = std::map<TopicPartition, std::chrono::milliseconds>;

    virtual ~KafkaHandleBase() = default;
    KafkaHandleBase(const KafkaHandleBase &) = delete;
    KafkaHandleBase(KafkaHandleBase &&) = delete;
    KafkaHandleBase& operator=(const KafkaHandleBase &) = delete;
    KafkaHandleBase& operator=(KafkaHandleBase &&) = delete;

    void pause_partitions(const TopicPartitionList& topic_partitions);

    void pause(const std::string& topic);

    void resume_partitions(const TopicPartitionList& topic_partitions);

    void resume(const std::string& topic);

    void set_timeout(std::chrono::milliseconds timeout);

    void set_log_level(LogLevel level);

    void add_brokers(const std::string& broker);

    OffsetTuple query_offsets(const TopicPartition& topic_partition) const;

    OffsetTuple query_offsets(const TopicPartition& topic_partition, std::chrono::milliseconds timeout) const;

    rd_kafka_t* get_handle() const;

    Topic get_topic(const std::string& name);

    Topic get_topic(const std::string& name, TopicConfiguration config);

    Metadata get_metadata(bool all_topics = true) const;

    Metadata get_metadata(bool all_topics, std::chrono::milliseconds timeout) const;

    TopicMetadata get_metadata(const Topic& topic) const;

    TopicMetadata get_metadata(const Topic& topic, std::chrono::milliseconds timeout) const;

    GroupInformation get_consumer_group(const std::string& name);

    GroupInformation get_consumer_group(const std::string& name, std::chrono::milliseconds timeout);

    GroupInformationList get_consumer_groups();

    GroupInformationList get_consumer_groups(std::chrono::milliseconds timeout);

    TopicPartitionList get_offsets_for_times(const TopicPartitionsTimestampsMap& queries) const;

    TopicPartitionList get_offsets_for_times(const TopicPartitionsTimestampsMap& queries, std::chrono::milliseconds timeout) const;

    std::string get_name() const;

    std::chrono::milliseconds get_timeout() const;

#if RD_KAFKA_VERSION >= RD_KAFKA_ADMIN_API_SUPPORT_VERSION
    Queue get_background_queue() const {
        return Queue::make_queue(rd_kafka_queue_get_background(handle_.get()));
    }
#endif

    int get_out_queue_length() const;

#if RD_KAFKA_VERSION >= RD_KAFKA_DESTROY_FLAGS_SUPPORT_VERSION
    void set_destroy_flags(int destroy_flags);
    int get_destroy_flags() const;
#endif

    void yield() const;

protected:
    KafkaHandleBase(Configuration config);

    void set_handle(rd_kafka_resp_err_t* handle);
    void check_error(rd_kafka_resp_err_t error) const;
    void check_error(rd_kafka_resp_err_t error, const rd_kafka_topic_partition_list_t& list_ptr) const;
    rd_kafka_conf_t* get_configuration_handle() const;

private:
    static const std::chrono::milliseconds DEFAULT_TIMEOUT;

    struct CPPKAFKA_API HandleDeleter {
        explicit HandleDeleter(const KafkaHandleBase* handle_base_ptr) : handle_base_ptr_(handle_base_ptr) {}

        void operator()(rd_kafka_t* handle);

    private:
        const KafkaHandleBase* handle_base_ptr_;
    };

    using HandlePtr = std::unique_ptr<rd_kafka_t, HandleDeleter>;
    using TopicConfigurationMap = std::unordered_map<std::string, TopicConfiguration>;

    Topic get_topic(const std::string& name, rd_kafka_topic_conf_t* conf);
    Metadata get_metadata(bool all_topics, rd_kafka_topic_t* topic_ptr, std::chrono::milliseconds timeout) const;

    GroupInformationList fetch_consumer_groups(const char* name, std::chrono::milliseconds timeout);
    void save_topic_config(const std::string& name, TopicConfiguration config);

    std::chrono::milliseconds timeout_ms_;
    Configuration config_;
    TopicConfigurationMap topic_configurations_;
    std::mutex topic_configurations_mutex_;
    HandlePtr handle_;
    int destroy_flags_;
};

} // namespace cppkafka

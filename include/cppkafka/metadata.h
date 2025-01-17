#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <librdkafka/rdkafka.h>
#include "macros.h"

namespace cppkafka {

class Error;

class CPPKAFKA_API PartitionMetadata {
public:
    PartitionMetadata(const rd_kafka_metadata_partition& partition);

    uint32_t get_id() const;

    Error get_error() const;

    int32_t get_leader() const;

    const std::vector<int32_t>& get_replicas() const;

    const std::vector<int32_t>& get_in_sync_replica_broker() const;

private:
    int32_t id_;
    rd_kafka_resp_err_t error_;
    int32_t leader_;
    std::vector<int32_t> replicas_;
    std::vector<int32_t> isrs_;
};

class CPPKAFKA_API TopicMetadata {
public:
    TopicMetadata(const rd_kafka_metadata_topic& topic);

    std::string get_name() const;

    Error get_error() const;

    const std::vector<PartitionMetadata>& get_partitions() const;

private:
    std::string name_;
    rd_kafka_resp_err_t error_;
    std::vector<PartitionMetadata> partitions_;
};

class CPPKAFKA_API BrokerMetadata {
public:
    BrokerMetadata(const rd_kafka_metadata_broker& broker);

    const std::string& get_host() const;

    int32_t get_id() const;

    uint16_t get_port() const;

private:
    const std::string host_;
    int32_t id_;
    uint16_t port_;
};

class CPPKAFKA_API Metadata {
public:
    static Metadata make_non_owning(const rd_kafka_metadata_t* handle);

    Metadata();

    Metadata(const rd_kafka_metadata_t* handle);

    std::vector<BrokerMetadata> get_brokers() const;

    std::vector<TopicMetadata> get_topics() const;

    std::vector<TopicMetadata> get_topics(const std::unordered_set<std::string>& topics) const;

    std::vector<TopicMetadata> get_topics_prefixed(const std::string& prefix) const;

    explicit operator bool() const;

    const rd_kafka_metadata_t* get_handle() const;

private:
    using HandlePtr = std::unique_ptr<const rd_kafka_metadata_t, decltype(&rd_kafka_metadata_destroy)>;

    struct NonOwningTag {};

    Metadata(const rd_kafka_metadata_t* handle, NonOwningTag);

    HandlePtr handle_;
};

} // naemspace cppkafka

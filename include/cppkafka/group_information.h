#pragma once

#include <vector>
#include <cstdint>
#include "macros.h"
#include "metadata.h"
#include "error.h"
#include "topic_partition_list.h"

namespace cppkafka {

class CPPKAFKA_API MemberAssignmentInformation {
public:
    MemberAssignmentInformation(const std::vector<uint8_t>& data);

    uint16_t get_version() const;

    const TopicPartitionList& get_topic_partitions() const;

private:
    uint16_t version_;
    TopicPartitionList topic_partitions_;
};

class CPPKAFKA_API GroupMemberInformation {
public:
    GroupMemberInformation(const rd_kafka_group_member_info& info);

    const std::string& get_member_id() const;

    const std::string& get_client_id() const;

    const std::string& get_client_host() const;

    const std::vector<uint8_t>& get_member_metadata() const;

    const std::vector<uint8_t>& get_member_assignment() const;

private:
    std::string member_id_;
    std::string client_id_;
    std::string client_host_;
    std::vector<uint8_t> member_metadata_;
    std::vector<uint8_t> member_assignment_;
};

class CPPKAFKA_API GroupInformation {
public:
    GroupInformation(const rd_kafka_group_info& info);

    const BrokerMetadata& get_broker() const;

    const std::string& get_name() const;

    Error get_error() const;

    const std::string& get_protocol_type() const;

    const std::string& get_state() const;

    const std::string& get_protocol() const;

    const std::vector<GroupMemberInformation>& get_members() const;

private:
    BrokerMetadata broker_;
    std::string name_;
    Error error_;
    std::string state_;
    std::string protocol_type_;
    std::string protocol_;
    std::vector<GroupMemberInformation> members_;
};

using GroupInformationList = std::vector<GroupInformation>;
} // namespace cppkafka

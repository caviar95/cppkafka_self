/*
 * @Author: Caviar
 * @Date: 2025-01-15 12:01:19
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 12:11:25
 * @Description:
 */

#pragma once

#include <memory>
#include <iosfwd>
#include <algorithm>
#include <vector>
#include <set>
#include <librdkafka/rdkafka.h>
#include "macros.h"

namespace cppkafka {

class TopicPartition;
class PartitionMetadata;

using TopicPartitionListPtr = std::unique_ptr<rd_kafka_topic_partition_list_t,
                                               decltype(&rd_kafka_topic_partition_list_destroy)>;

using TopicPartitionList = std::vector<TopicPartition>;

CPPKAFKA_API TopicPartitionListPtr convert(const TopicPartitionList& topic_partitions);
CPPKAFKA_API TopicPartitionList convert(const TopicPartitionListPtr& topic_partitions);
CPPKAFKA_API TopicPartitionList convert(rd_kafka_topic_list_t* topic_partitions);
CPPKAFKA_API TopicPartitionList convert(const std::string& topic, const std::vector<PartitionMetadata>& partition_metadata);
CPPKAFKA_API TopicPartitionListPtr make_handle(rd_kafka_topic_partition_list_t* handle);
CPPKAFKA_API TopicPartitionList find_matches(const TopicPartitionList& partitions,
                                             const std::set<std::string>& topics);
CPPKAFKA_API TopicPartitionList find_matches(const TopicPartitionList& partitions,
                                             const std::set<int>& ids);
CPPKAFKA_API std::ostream& operator<<(std::ostream& output, const TopicPartitionList& rhs);

} // namespace cppkafka

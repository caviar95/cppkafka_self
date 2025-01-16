/*
 * @Author: Caviar
 * @Date: 2025-01-15 12:14:03
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 14:26:07
 * @Description: 
 */

#pragma once

#include <string>
#include <iosfwd>
#include <cstdint>
#include "macros.h"

namespace cppkafka {

class CPPKAFKA_API TopicPartition {
public:
    enum Offset {
        OFFSET_BEGINNING = -2,
        OFFSET_END = -1,
        OFFSET_STORED = -1000,
        OFFSET_INVALID = -1001
    };

    TopicPartition();
    TopicPartition(const char* topic);
    TopicPartition(std::string topic);
    TopicPartition(std::string topic, int partition);
    const std::string& get_topic() const;
    int get_partition() const;
    int64_t get_offset() const;
    void set_partition(int partition);
    void set_offset(int64_t offset);
    bool operator<(const TopicPartition& rhs) const;
    bool operator==(const TopicPartition& rhs) const;
    bool operator!=(const TopicPartition& rhs) const;

    CPPKAFKA_API friend std::ostream& operator<<(std::ostream& output, const TopicPartition& rhs);

private:
    std::string topic_{};
    int partition_{};
    int64_t offset_{};
};

} // namespace cppkafka
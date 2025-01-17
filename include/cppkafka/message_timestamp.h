#pragma once

#include <chrono>
#include <librdkafka/rdkafka.h>
#include "macros.h"

namespace cppkafka {

class CPPKAFKA_API MessageTimestamp {
    friend class Message;

public:
    enum TimestampType {
        CREATE_TIME = RD_KAFKA_TIMESTAMP_CREATE_TIME,
        LOG_APPEND_TIME = RD_KAFKA_TIMESTAMP_LOG_APPEND_TIME,
    };

    TimestampType get_type() const;

private:
    MessageTimestamp(TimestampType type);

    std::chrono::milliseconds timestamp_;
    TimestampType type_;
};

} // namepsace cppkafka

/*
 * @Author: Caviar
 * @Date: 2025-01-14 08:48:30
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-14 16:41:38
 * @Description: 
 */

#pragma once

#include <string>
#include <iosfwd>
#include <librdkafka/rdkafka.h>
#include "macros.h"

namespace cppkafka {

class Error {
public:
    Error() = default;
    Error(rd_kafka_resp_err_t error);
    rd_kafka_resp_err_t get_error() const;
    std::string to_string() const;
    explicit operator bool() const;
    bool operator==(const Error& rhs) const;
    bool operator!=(const Error& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Error& error);

private:
    rd_kafka_resp_err_t error_{RD_KAFKA_RESP_ERR_NO_ERROR};
};

} // namespace cppkafka

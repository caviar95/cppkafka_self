/*
 * @Author: Caviar
 * @Date: 2025-01-14 16:35:50
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-14 16:41:08
 * @Description: 
 */

#include "error.h"

#include <iostream>

using std::string;
using std::ostream;

namespace cppkafka {

Error::Error(rd_kafka_resp_err_t error) : error_(error) {}

rd_kafka_resp_err_t Error::get_error() const { return error_; }

string Error::to_string() const
{
  return rd_kafka_err2str(error_);
}

Error::operator bool() const { return error_ != RD_KAFKA_RESP_ERR_NO_ERROR; }

bool Error::operator==(const Error& rhs) const
{
    return error_ == rhs.error_;
}

bool Error::operator!=(const Error& rhs) const
{
    return !(*this == rhs);
}

ostream& operator<<(ostream& output, const Error& rhs)
{
    return output << rhs.to_string();
}

} // namespace cppkafka

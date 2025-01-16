/*
 * @Author: Caviar
 * @Date: 2025-01-14 16:45:32
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 00:06:50
 * @Description: 
 */

#include "exceptions.h"

using std::string;
using std::to_string;

namespace cppkafka {

Exception::Exception(string message) : message_(std::move(message)) {}

const char* Exception::what() const noexcept
{
    return message_.data();
}

ConfigException::ConfigException(const string& config_name, const string& error)
    : Exception("Failed to set " + config_name + ": " + error) {}

ConfigOptionNotFound::ConfigOptionNotFound(const string& config_name)
    : Exception(config_name + " not found") {}

InvalidConfigOptionType::InvalidConfigOptionType(const string& config_name, const string& type)
    : Exception(config_name + " could not be converted to " + type) {}

ElementNotFound::ElementNotFound(const string& element_type, const string& name)
    : Exception("Could not find " + element_type + " with name: " + name) {}

ParseException::ParseException(const string& message)
    : Exception(message) {}

UnexpectVersion::UnexpectVersion(uint32_t version)
    : Exception("Unexpected version " + to_string(version)) {}

HandleException::HandleException(Error error) 
    : Exception(error.to_string()), error_(error) {}

Error HandleException::get_error() const
{
    return error_;
}

ConsumerException::ConsumerException(Error error)
    : Exception(error.to_string()), error_(error) {}

Error ConsumerException::get_error() const
{
    return error_;
}

QueueException::QueueException(Error error)
    : Exception(error.to_string()), error_(error) {}

Error QueueException::get_error() const
{
    return error_;
}

ActionTerminatedException::ActionTerminatedException(const string& error)
    : Exception(error) {}

} // naemspace cppkafka

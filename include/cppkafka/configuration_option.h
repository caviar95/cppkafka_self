/*
 * @Author: Caviar
 * @Date: 2025-01-15 00:08:47
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 11:25:36
 * @Description: 
 */

#pragma once

#include <string>
#include <type_traits>
#include "macros.h"

namespace cppkafka {

class CPPKAFKA_API ConfigurationOption {
public:
    ConfigurationOption(const std::string &key, const std::string &value);
    ConfigurationOption(const std::string &key, const char* value);
    ConfigurationOption(const std::string &key, bool value);

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    ConfigurationOption(const std::string &key, T value) : ConfigurationOption(key, std::to_string(value)) {}

    const std::string &get_key() const;
    const std::string &get_value() const;

private:
    std::string key_{};
    std::string value_{};
};
} // namepsace cppkafka

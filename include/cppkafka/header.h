
#pragma once

#include <string>
#include <assert.h>

#include "macros.h"
#include "buffer.h"

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION

namespace cppkafka {

template <typename BufferType>
class Header {
public:
    using ValueType = BufferType;

    Header() = default;

    Header(std::string name, const BufferType& value);

    Header(std::string name, BufferType&& value);

    const std::string& get_name() const;

    const BufferType& get_value() const;

    BufferType& get_value();

    operator bool() const;

private:
    template <typename T>
    T make_value(const T& other);

    Buffer make_value(const Buffer& other);

    std::string name_;
    BufferType value_;
};

template <typename BufferType>
bool operator==(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return std::tie(lhs.get_name(), lhs.get_value()) == std::tie(rhs.get_name(), rhs.get_value());
}

template <typename BufferType>
bool operator!=(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return !(lhs == rhs);
}

template <typename BufferType>
bool operator<(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return std::tie(lhs.get_name(), lhs.get_value()) < std::tie(rhs.get_name(), rhs.get_value());
}

template <typename BufferType>
bool operator>(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return std::tie(lhs.get_name(), lhs.get_value()) > std::tie(rhs.get_name(), rhs.get_value());
}

template <typename BufferType>
bool operator<=(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return !(lhs > rhs);
}

template <typename BufferType>
bool operator>=(const Header<BufferType>& lhs, const Header<BufferType>& rhs) {
    return !(lhs < rhs);
}

template <typename BufferType>
Header<BufferType>::Header(std::string name, const BufferType& value) : name_(std::move(name)), value_(std::move(value)) {}

template <typename BufferType>
Header<BufferType>::Header(std::string name, BufferType&& value) : name_(std::move(name)), value_(std::move(value)) {}

template <typename BufferType>
const std::string& Header<BufferType>::get_name() const {
    return name_;
}

template <typename BufferType>
const BufferType& Header<BufferType>::get_value() const {
    return value_;
}

template <typename BufferType>
BufferType& Header<BufferType>::get_value() {
    return value_;
}

template <typename BufferType>
Header<BufferType>::operator bool() const {
    return !value_.empty();
}

template <>
inline Header<Buffer>::operator bool() const {
    return value_.get_size() > 0;
}

template <typename BufferType>
template <typename T>
T Header<BufferType>::make_value(const T& other) {
    return other;
}

template <typename BufferType>
Buffer Header<BufferType>::make_value(const Buffer& other) {
    return Buffer(other.get_data(), other.get_size());
}

} // namespace cppkafka

#endif

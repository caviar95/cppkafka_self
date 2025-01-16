/*
 * @Author: Caviar
 * @Date: 2025-01-13 23:41:25
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-14 13:55:00
 * @Description: 
 */

#pragma once

#include <cstddef>
#include <array>
#include <vector>
#include <iosfwd>
#include <algorithm>
#include "macros.h"
#include "exceptions.h"

namespace cppkafka {

class CPPKAFKA_API Buffer {
public:
    using DataType = unsigned char;
    using const_iterator = const DataType*;

    Buffer();

    template <typename T>
    Buffer(const T* data, size_t size) : data_(reinterpret_back<const DataType *>(data)), size_(size) {
        static_assert(sizeof(T) == sizeof(DataType), "sizeof(T) != sizeof(DataTYpe)");

        if ((data_ == nullptr) && (size_ > 0)) {
            throw Exception("Invalid buffer configuration");
        }
    }

    template <typename Iter>
    Buffer(const Iter first, const Iter last) : Buffer(&*first, std::distance(first, last)) {}

    template <typename T>
    Buffer(const std::vector<T>& data) : data_(data.data()), size_(data.size()) {
        static_assert(sizeof(T) == sizeof(DataType), "sizeof(T) != sizeof(DataTYpe)");
    }
    
    template <typename T>
    Buffer(std::vector<T>&& data) = delete;

    template <typename T, size_t N>
    Buffer(const std::array<T, N>& data) : data_(data.data()), size_(data.size()) {
        static_assert(sizeof(T) == sizeof(DataType), "sizeof(T) != sizeof(DataTYpe)");
    }

    template <typename T, size_t N>
    Buffer(const std::array<T, N>&& data) = delete;

    template <typename T, size_t N>
    Buffer(const T(&data)[N]) : Buffer(data, N) {
    }

    template <typename T, size_t N>
    Buffer(const T(&&data)[N]) = delete;


    Buffer(const std::string &data);

    Buffer(std::string &&data) = delete;
    

    // 删除拷贝语义
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = default;

    const DataType *get_data() const;

    size_t get_size() const;

    const_iterator begin() const;
    const_iterator end() const;

    explicit operator bool() const;
    
    operator std::string() const; // convert the contents of the buffer into a string

    template <typename T>
    operator std::vector<T>() const {
        static_assert(sizeof(T) == sizeof(DataType), "sizeof(T) != sizeof(DataTYpe)");
        return std::vector<T>(data_, data_ + size_);
    }

    CPPKAFKA_API friend std::ostream& operator<<(std::ostream& os, const Buffer& buf);

private:
    const DataType* data_;
    size_t size_;
};

CPPKAFKA_API bool operator<(const Buffer& lhs, const Buffer& rhs);
CPPKAFKA_API bool operator<=(const Buffer& lhs, const Buffer& rhs);
CPPKAFKA_API bool operator>(const Buffer& lhs, const Buffer& rhs);
CPPKAFKA_API bool operator>=(const Buffer& lhs, const Buffer& rhs);

} // namespace cppkafka
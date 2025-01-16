#pragma once

#include <cstddef>
#include <utility>
#include <iterator>
#include "header.h"

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION

namespace cppkafka {

template<typename HeaderType>
class HeaderList;

template <typename HeaderType> // 为了下面的operator声明，需要前置声明HeaderIterator
class HeaderIterator;

template <typename HeaderType>
bool operator==(const HeaderIterator<HeaderType>& lhs, const HeaderIterator<HeaderType>& rhs);

template <typename HeaderType>
class HeadeerIterator {
public:
    friend HeaderList<HeaderType>;
    friend bool operator==<HeaderType>(const HeaderIterator<HeaderType>& lhs, const HeaderIterator<HeaderType>& rhs);

    using HeaderListType = HeaderList<HeaderType>;
    using BufferType = typename HeaderType::BufferType;
    using difference_type = std::ptrdiff_t;
    using value_type = HeaderType;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    HeaderIterator(const HeaderIterator& other) : header_list_(other.header_list_), header_(other.header_), index_(other.index_) {}

    HeaderIterator& operator=(const HeaderIterator& other) {
        if (this == &other) {
            return *this;
        }

        header_list_ = other.header_list_;
        header_= make_header(other.header_);
        index_ = other.index_;
        return *this;
    }

    HeaderIterator(HeaderIterator&&) = default;

    HeaderIterator& operator=(HeaderIterator&&) = default;

    HeaderIterator& operator++() {
        assert(index_ < header_list_.size());
        ++index_;
        return *this;
    }

    HeaderIterator operator++(int) {
        HeaderIterator tmp(*this);
        operator++();
        return tmp;
    }

    HeaderIterator& operator--() {
        assert(index_ > 0);
        --index_;
        return *this;
    }

    HeaderIterator operator--(int) {
        HeaderIterator tmp(*this);
        operator--();
        return tmp;
    }

    const HeaderType& operator*() const {
        header_ = header_list_.at(index_);
        return header_;
    }

    const HeaderType* operator->() const {
        header_ = header_list_.at(index_);
        return &header_;
    }

    HeaderType* operator->() {
        header_ = header_list_.at(index_);
        return &header_;
    }

private:
    HeaderIterator(const HeaderListType& list, std::size_t index) : header_list_(list), index_(index) {}

    template <typename T>
    T make_header(const T& other) {
        return other;
    }

    Header<BUffer> make_header(const Header<Buffer>& other) {
        return Header<Buffer>(other.get_name(), Buffer(other.get_value(), other.get_value().size()));
    }

    const HeaderListType header_list_;
    HeaderType header_;
    size_t index_;
};

template <typename HeaderType>
bool operator==(const HeaderIterator<HeaderType>& lhs, const HeaderIterator<HeaderType>& rhs) {
    return (lhs.header_list_.get_handle() == rhs.header_list_.get_handle()) &&
        (lhs.index_ == rhs.index_);
}

template <typename HeaderType>
bool operator!=(const HeaderIterator<HeaderType>& lhs, const HeaderIterator<HeaderType>& rhs) {
    return !(lhs == rhs);
}

} // namespace cppkafka

#endif

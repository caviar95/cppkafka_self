#pragma once

#include <chrono>
#include "buffer.h"
#include "topic.h"
#include "macros.h"
#include "message.h"
#include "header_list.h"

namespace cppkafka {

template <typename BufferType, typename Concrete>
class BasicMessageBuilder {
public:

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    using HeaderType = Header<BufferType>;
    using HeaderListType = HeaderList<BufferType>;
#endif

    BasicMessageBuilder(std::string topic);
    BasicMessageBuilder(const Message& message);

    template <typename OtherBufferType, typename OtherConcrete>
    BasicMessageBuilder(const BasicMessageBuilder<OtherBufferType, OtherConcrete>& other);

    template <typename OtherBufferType, typename OtherConcrete>
    BasicMessageBuilder(BasicMessageBuilder<OtherBufferType, OtherConcrete>&& other);

    BasicMessageBuilder(BasicMessageBuilder&&) = default;
    BasicMessageBuilder(const BasicMessageBuilder&) = default;
    BasicMessageBuilder& operator=(BasicMessageBuilder&&) = default;
    BasicMessageBuilder& operator=(const BasicMessageBuilder&) = default;

    Concrete& topic(std::string value);
    Concrete& partition(int value);

    Concrete& key(const BufferType& value);
    Concrete& key(BufferType&& value);

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    Concrete& header(const HeaderType& header);
    Concrete& headers(const HeaderListType& headers);
    Concrete& headers(HeaderListType&& headers);
#endif

    Concrete& payload(const BufferType& value);
    Concrete& payload(BufferType&& value);

    Concrete& timestamp(std::chrono::milliseconds value);

    template <typename Clock, typename Duration = typename Clock::duration>
    Concrete& timestamp(std::chrono::time_point<Clock, Duration> value);

    Concrete& user_data(void* value);

    const std::string& topic() const;

    int partition() const;

    const BufferType& key() const;

    BufferType& key();

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    const HeaderListType& header_list() const;

    HeaderListType& header_list();
#endif

    const BufferType& payload() const;

    BufferType& payload();

    std::chrono::milliseconds timestamp() const;

    void* user_data() const;

    Message::InternalPtr internal() const; 

    Concrete& internal(Message::InternalPtr value);

protected:
    void construct_buffer(BufferType& lhs, const BufferType& rhs);

private:
    Concrete& get_concrete();
    std::string topic_;
    int partition_;
    BufferType key_;
    BufferType payload_;
    std::chrono::milliseconds timestamp_{0};
    void* user_data_;
    Message::InternalPtr internal_;
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    HeaderListType header_list_;
#endif
};

template <typename T, typename C>
BasicMessageBuilder<T, C>::BasicMessageBuilder(std::string topic)
: topic_(std::move(topic)), user_data_(nullptr) {}

template <typename T, typename C>
BasicMessageBuilder<T, C>::BasicMessageBuilder(const Message& message) 
: topic_(message.get_topic()), key_(Buffer(message.get_key().get_data(), message.get_key().get_size())),
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    header_list_(message.get_header_list() ? HeaderListType(rd_kafka_headers_copy(message.get_header_list().get_handle())) : HeaderListType()),
#endif
    payload_(Buffer(message.get_payload().get_data(), message.get_payload().get_size())),
    timestamp_(message.get_timestamp() ? message.get_timestamp().get().get_timestamp() : std::chrono::milliseconds(0)),
    user_data_(message.get_user_data()),
    internal_(message.internal())
{}

template <typename T, typename C>
template <typename U, typename V>
BasicMessageBuilder<T, C>::BasicMessageBuilder(const BasicMessageBuilder<U, V>& rhs)
: topic_(rhs.topic()), partition_(rhs.partition()),
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    header_list_(std::move(header_list_())),
#endif
    timestamp_(rhs.timestamp()),
    user_data_(rhs.user_data()),
    internal_(rhs.internal()) {
        get_concrete().construct_buffer(key_, std::move(rhs.key()));
        get_concrete().construct_buffer(payload_, std::move(rhs.payload()));
    }

template <typename T, typename C>
template <typename U, typename V>
BasicMessageBuilder<T, C>::BasicMessageBuilder(BasicMessageBuilder<U, V>&& rhs)
: topic_(rhs.topic()),
    partition_(rhs.partition()),
#if (RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION)
    header_list_(std::move(header_list())), //assume header ownership
#endif
    timestamp_(rhs.timestamp()),
    user_data_(rhs.user_data()),
    internal_(rhs.internal()) {
        get_concrete().construct_buffer(key_, std::move(rhs.key()));
        get_concrete().construct_buffer(payload_, std::move(rhs.payload()));
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::topic(std::string value) {
    topic_= std::move(value);
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::partition(int value) {
    partition_ = value;
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::key(const T& value) {
    get_concrete().construct_buffer(key_, value);
    return get_concrete();
}

template<typename T, typename C>
C& BasicMessageBuilder<T, C>::key(T&& value) {
    key_ = std::move(value);
    return get_concrete();
}

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
template <typename T, typename C>
C& BasicMessageBuilder<T, C>::header(const HeaderType& header) {
    if (!header_list_) {
        header_list_ = HeaderListType(5);
    }

    header_list_.add(header);
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::headers(const HeaderListType& headers) {
    header_list_ = headers;
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::headers(HeaderListType&& headers) {
    header_list_ = std::move(headers);
    return get_concrete();
}

#endif

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::payload(const T& value) {
    get_concrete().construct_buffer(payload_, value);
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::payload(T&& value) {
    payload_ = std::move(value);
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::timestamp(std::chrono::milliseconds value) {
    timestamp_ = value;
    return get_concrete();
}

template <typename T, typename C>
template <typename Clock, typename Duration>
C& BasicMessageBuilder<T, C>::timestamp(std::chrono::time_point<Clock, Duration> value) {
    timestamp_ = std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch());
    return get_concrete();
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::user_data(void* value) {
    user_data_ = value;
    return get_concrete();
}

template <typename T, typename C>
const std::string& BasicMessageBuilder<T, C>::topic() const {
    return topic_;
}

template <typename T, typename C>
int BasicMessageBuilder<T, C>::partition() const {
    return partition_;
}

template <typename T, typename C>
const T& BasicMessageBuilder<T, C>::key() const {
    return key_;
}

template <typename T, typename C>
T& BasicMessageBuilder<T, C>::key() {
    return key_;
}

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION

template <typename T, typename C>
const typename BasicMessageBuilder<T, C>::HeaderListType& BasicMessageBuilder<T, C>::header_list() const {
    return header_list_;
}

template <typename T, typename C>
typename BasicMessageBuilder<T, C>::HeaderListType& BasicMessageBuilder<T, C>::header_list() {
    return header_list_;
}

#endif

template <typename T, typename C>
const T& BasicMessageBuilder<T, C>::payload() const {
    return payload_;
}

template <typename T, typename C>
T& BasicMessageBuilder<T, C>::payload() {
    return payload_;
}

template <typename T, typename C>
std::chrono::milliseconds BasicMessageBuilder<T, C>::timestamp() const {
    return timestamp_;
}

template <typename T, typename C>
void* BasicMessageBuilder<T, C>::user_data() const {
    return user_data_;
}

template <typename T, typename C>
Message::InternalPtr BasicMessageBuilder<T, C>::internal() const {
    return internal_;
}

template <typename T, typename C>
C& BasicMessageBuilder<T, C>::internal(Message::InternalPtr value) {
    internal_ = value;
    return get_concrete();
}

template <typename T, typename C>
void BasicMessageBuilder<T, C>::construct_buffer(T& lhs, const T& rhs) {
    lhs = rhs;
}

template<typename T, typename C>
C& BasicMessageBuilder<T, C>::get_concrete() {
    return static_cast<C&>(*this);
}


class MessageBuilder : public BasicMessageBuilder<Buffer, MessageBuilder> {
public:
    using Base = BasicMessageBuilder<Buffer, MessageBuilder>;
    using BasicMessageBuilder<Buffer, MessageBuilder>::BasicMessageBuilder;

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    using HeaderType = Base::HeaderType;
    using HeaderListType = Base::HeaderListType;
#endif

    void construct_buffer(Buffer& lhs, const Buffer& rhs) {
        lhs = Buffer(rhs.get_data(), rhs.get_size());
    }

    template <typename T>
    void construct_buffer(Buffer& lhs, const T& rhs) {
        lhs = Buffer(std::forward<T>(rhs));
    }

    MessageBuilder clone() const {
        MessageBuilder builder(topic());
        builder.key(Buffer(key().get_data(), key().get_size())).
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
        headers(header_list()).
#endif
        payload(Buffer(payload().get_data(), payload().get_size())).
        timestamp(timestamp()).user_data(user_data()).
        user_data(user_data()).
        internal(internal());
        return builder;
    }
};

template <typename T>
class ConcreteMessageBuilder : public BasicMessageBuilder<T, ConcreteMessageBuilder<T>> {
public:
    using Base = BasicMessageBuilder<T, ConcreteMessageBuilder<T>>;
    using BasicMessageBuilder<T, ConcreteMessageBuilder<T>>::BasicMessageBuilder;
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    using HeaderType = typename Base::HeaderType;
    using HeaderListType = typename Base::HeaderListType;
#endif
};

} // namespace cppkafka

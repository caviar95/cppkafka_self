/*
 * @Author: Caviar
 * @Date: 2025-01-15 15:28:30
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-16 21:58:51
 * @Description: 
 */

#pragma once

#include <memory>
#include <cstdint>
#include <chrono>
#include <cassert>
#include <boost/optional.hpp>
#include <librdkafka/rdkafka.h>
#include "buffer.h"
#include "macros.h"
#include "error.h"
#include "header_list.h"
#include "message_timestamp.h"

namespace cppkafka {

class Internal;

class CPPKAFKA_API Message {
public:
    friend class MessageInternal;

    using InternalPtr = std::shared_ptr<Internal>;

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    using HeaderType = Header<Buffer>;
    using HeaderListType = HeaderList<HeaderType>;
#endif

    static Message make_non_owning(rd_kafka_message_t* handle);

    Message();

    Message(rd_kafka_message_t* handle);
    Message(const Message&) = delete;
    Message(Message&&) = default;
    Message& operator=(const Message&) = delete;
    Message& operator=(Message&&) = default;

    Error get_error() const {
        assert(handle_);
        return handle_->err;
    }

    bool is_eof() const {
        return get_error() == RD_KAFKA_RESP_ERR__PARTITION_EOF;
    }

    std::string get_topic() const {
        assert(handle_);
        return handle_->rkt ? rd_kafka_topic_name(handle_->rkt) : "";
    }

    int get_partition() const {
        assert(handle_);
        return handle_->partition;
    }

    const Buffer& get_payload() const {
        return payload_;
    }

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    void set_header_list(const HeaderListType& headers) {
        assert(handle_);
        if (!headers) {
            return; // nothing to set
        }

        rd_kafka_headers_t* handle_copy = rd_kafka_headers_copy(headers.get_handle());
        rd_kafka_message_set_headers(handle_, handle_copy);
        header_list_ = HeaderListType::make_non_owning(handle_copy);
    }

    const HeaderListType& get_header_list() const {
        return header_list_;
    }

    template <typename T>
    HeaderList<HeaderType> detach_header_list() {
        rd_kafka_headers_t* headers_handle;
        Error error = rd_kafka_message_detach_headers(handle_.get(), &headers_handle);
        return error ? HeaderList<HeaderType>() : HeaderList<HeaderType>(headers_handle);
    }
#endif

    const Buffer& get_key() const {
        return key_;
    }

    int64_t get_offset() const {
        assert(handle_);
        return handle_->offset;
    }

    void* get_user_data() const {
        return user_data_;
    }

    boost::optional<MessageTimestamp> get_timestamp() const;

#if RD_KAFKA_VERSION >= RD_KAFKA_MESSAGE_LATENCY_SUPPTORT_VERSION
    std::chrono::milliseconds get_latency() const {
        assert(handle_);
        return std::chrono::milliseconds(rd_kafka_message_latency(handle_.get()));
    }
#endif

#if RD_KAFKA_VERSION >= RD_KAFKA_MESSAGE_STATUS_SUPPORT_VERSION
    rd_kafka_msg_status_t get_status() const {
        assert(handle_);
        return rd_kafka_message_status(handle_.get());
    }
#endif

    explicit operator bool() const {
        return handle_ != nullptr;
    }

    rd_kafka_message_t* get_current_dir_name() const {
        return handle_.get();
    }

    InternalPtr internal() const {
        return internal_;
    }

private:
    using HandlePtr = std::unique_ptr<rd_kafka_message_t, decltype(&rd_kafka_message_destroy)>;

    struct NonOwningTag {};

    Message(rd_kafka_message_t* handle, NonOwningTag);
    Message(HandlePtr handle);
    Message& load_internal();

    HandlePtr handle_;
    Buffer payload_;
    Buffer key_;

#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    HeaderListType header_list_;
#endif

    void* user_data_;
    InternalPtr internal_;
};

using MessageList = std::vector<Message>;

} // namespace cppkafka

#pragma once

#include <vector>
#include <memory>
#include <librdkafka/rdkafka.h>
#include "event.h"
#include "macros.h"
#include "message.h"

namespace cppkafka {

class CPPKAFKA_API Queue {
public:
    static Queue make_non_owning(rd_kafka_queue_t* handle);

    static Queue make_queue(rd_kafka_queue_t* handle);

    Queue();

    Queue(rd_kafka_queue_t* handle);

    size_t get_length() const;

    void forward_to_queue(const Queue& forward_queue) const;

    void disable_queue_forwarding() const;

    void set_timeout(std::chrono::milliseconds timeout);

    std::chrono::milliseconds get_timeout() const;

    Message consume() const;

    Message consume(std::chrono::milliseconds timeout) const;

    template <typename Allocator>
    std::vector<Message, Allocator> consume_batch(size_t batch_size, const Allocator& alloc) const;

    std::vector<Message> consume_batch(size_t max_batch_size) const;

    template <typename Allocator>
    std::vector<Message, Allocator> consume_batch(size_t max_batch_size, std::chrono::milliseconds timeout, const Allocator& alloc) const;

    std::vector<Message> consume_batch(size_t max_batch_size, std::chrono::milliseconds timeout) const;

    Event next_event() const;

    Event next_event(std::chrono::milliseconds timeout) const;

    explicit operator bool() const {
        return handle_ != nullptr;
    }

private:    
    static const std::chrono::milliseconds DEFAULT_TIMEOUT;

    using HandlePtr = std::unique_ptr<rd_kafka_queue_t, decltype(&rd_kafka_queue_destroy)>;

    struct NonOwningTag {};

    Queue(rd_kafka_queue_t* handle, NonOwningTag);

    HandlePtr handle_;
    std::chrono::milliseconds timeout_ms_;
};

using QueueList = std::vector<Queue>;

template <typename Allocator>
std::vector<Message, Allocator> Queue::consume_batch(size_t max_batch_size, const Allocator& alloc) const {
    return consume_batch(max_batch_size, timeout_ms_, alloc);
}

template <typename Allocator>
std::vector<Message, Allocator> Queue::consume_batch(size_t max_batch_size, std::chrono::milliseconds timeout, const Allocator& alloc) const {
    std::vector<rd_kafka_message_t*> raw_messages(max_batch_size);
    ssize_t result = rd_kafka_consume_batch_queue(handle_.get(),
                                                  static_cast<int>(timeout.count()),
                                                  raw_messages.data(),
                                                  raw_messages.size());

    if (result == -1) {
        rd_kafka_resp_err_t error = rd_kafka_last_error();
        if (error != RD_KAFKA_RESP_ERR_NO_ERROR) {
            throw QueueException(error);
        }

        return std::vector<Message, Allocator>(alloc);
    }

    return std::vector<Message, Allocator>(raw_messages.begin(), raw_messages.begin() + result, alloc);
}

} // namespace cppkafka

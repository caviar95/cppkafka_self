
#pragma

#include <memory>
#include <string>
#include <vector>
#include "error.h"
#include "message.h"
#include "topic_partition.h"
#include "topic_partition_list.h"

namespace cppkafka {

class Event {
public:
    Event(rd_kafka_event_t* handle);

    std::string get_name() const;

    rd_kafka_event_type_t get_type() const;

    Message get_next_message() const;

    std::vector<Message> get_messages();

    template <typename Allocator>
    std::vector<Message, Allocator> get_messages(Allocator& allocator);

    size_t get_message_count() const;

    Error get_error() const;

    void* get_opaque() const;

#if RD_KAFKA_VERSION >= RD_KAFKA_EVENT_STATS_SUPPORT_VERSION
    std::string get_stats() const {
        return rd_kafka_event_state(handle.get());
    }
#endif

    TopicPartition get_topic_partition() const;

    TopicPartitionList get_topic_partition_list() const;

    operator bool() const;

private:    
    using HandlePtr = std::unique_ptr<rd_kafka_event_t, decltype(&rd_kafka_event_destroy)>;

    HandlePtr handle;
};

template <typename Allocator>
std::vector<Message, Allocator> Event::get_messages(const Allocator allocator) {
    const size_t total_messages = get_message_count();
    std::vector<const rd_kafaka_message_t*> raw_messages(total_messages);
    const auto messages_read = rd_kafka_event_message_array(handle_.get(), raw_messages.data(), total_messages);

    std::vector<Message, Allocator> output(allocator);
    output.reserve(messages_read);
    for (auto message : raw_messages) {
        output.emplace_back(Message::make_non_owning(const_cast<rd_kafka_message_t*>(message)));
    }

    return output;
}

} // namespace cppkafka

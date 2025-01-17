#pragma once

#include <memory>
#include "kafka_handle_base.h"
#include "configuration.h"
#include "buffer.h"
#include "topic.h"
#include "macros.h"
#include "message_builder.h"

namespace cppkafka {

class Topic;
class Buffer;
class TopicConfiguration;
class Message;

class CPPKAFKA_API Producer : public KafkaHandleBase {
public:
    using KafkaHandleBase::pause;

    enum class PayloadPolicy {
        PASSTHROUGH_PAYLOAD = 0, // rdkafka will not copy nor free the payload
        COPY_PAYLOAD = RD_KAFKA_MSG_F_COPY, // means RD_KAFKA_MSG_F_COPY
        FREE_PAYLOAD = RD_KAFKA_MSG_F_FREE, // means RD_KAFKA_MSG_F_FREE
        BLOCK_ON_FULL_QUEUE = RD_KAFKA_MSG_F_BLOCK // producer will block if queue is full
    };

    Producer(Configuration config);

    void set_payload_policy(PayloadPolicy policy);

    PayloadPolicy get_payload_policy() const;

    void produce(const MessageBuilder& builder);
    void produce(MessageBuilder&& builder);
    void produce(const Message& message);
    void produce(Message&& message);

    int poll();
    int poll(std::chrono::milliseconds timeout);
    
    void flush();
    void flush(std::chrono::milliseconds timeout);

private:
#if RD_KAFKA_VERSION >= RD_KAFKA_HEADERS_SUPPORT_VERSION
    void do_produce(const MessageBuilder& builder, MessageBuilder::HeaderListType&& headers);
    void do_produce(const Message& message, MessageBuilder::HeaderListType&& headers);
#else
    void do_produce(const MessageBuilder& builder);
    void do_produce(const Message& message);
#endif

    PayloadPolicy message_payload_policy_;
};

} // namesapce cppkafka

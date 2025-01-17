#pragma once

#include <memory>
#include "macros.h"

namespace cppkafka {

class Message;

class Internal {
public:
    virtual ~Internal() = default;
};

using InternalPtr = std::shared_ptr<Internal>;

class CPPKAFKA_API MessageInternal {
public:
    MessageInternal(void* user_data, InternalPtr internal);
    static std::unique_ptr<MessageInternal> load(Message& message);

    void* get_user_data() const;
    InternalPtr get_internal() const;

private:
    void* user_data_;
    InternalPtr internal_;
};

template <typename BuilderType>
class MessageInternalGuard {
    MessageInteralGuard(Builder& builder) 
    : builder_(builder), user_data_(builder.user_data()) {
        if (builder_.internal()) {
            ptr_.reset(new MessageInternal(user_data_, builder_.internal()));
            builder_.user_data(ptr_.get());
        }
    }

    ~MessageInternalGuard() {
        builder_.user_data(user_data_);
    }

    void release() {
        ptr_.release();
    }

private:
    BuilderType& builder_;
    std::unique_ptr<MessageInternal> ptr_;
    void* user_data_;
};

} // namespace cppkafka

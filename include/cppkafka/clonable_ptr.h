/*
 * @Author: Caviar
 * @Date: 2025-01-14 23:56:03
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-15 00:06:33
 * @Description: 
 */

#pragma once

#include <memory>

namespace cppkafka {

template <typename T, typename Deleter, typename Cloner>
class ClonablePtr {
public:
    ClonablePtr(T* ptr, const Deleter& deleter, const Cloner& cloner) : handle_(ptr, deleter), cloner_(cloner) {}
    ClonablePtr(const ClonablePtr& rhs) : handle_(std::unique_ptr<T, Deleter>(rhs.try_clone(), rhs.get_deleter())), cloner_(rhs.get_cloner()) {}
    ClonablePtr& operator=(const ClonablePtr& rhs) {
        if (this != &rhs) {
            handle_ = std::unique_ptr<T, Deleter>(rhs.try_clone(), rhs.get_deleter());
            cloner_ = rhs.get_cloner();
        }

        return *this;
    }

    ClonablePtr(ClonablePtr&&) = default;
    ClonablePtr& operator=(ClonablePtr&&) = default;
    ~ClonablePtr() = default;

    T* get() const { return handle_.get(); }

    T* release() { return handle_.release(); }

    void reset(T* ptr)
    {
        handler_.reset(ptr);
    }

    const Deleter& get_deleter() const { return handle_.get_deleter(); }
    
    const Cloner& get_cloner() const { return cloner_; }

    explicit operator bool() const { return static_cast<bool>(handle_); }

private:
    T* try_clone() const {
        return cloner_ ? cloner_(get()) : get();
    }

    std::unique_ptr<T, Deleter> handle_;
    Cloner cloner_;
};

} // namespace cppkafka

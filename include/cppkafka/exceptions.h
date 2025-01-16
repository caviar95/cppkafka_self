/*
 * @Author: Caviar
 * @Date: 2025-01-13 23:54:38
 * @LastEditors: Caviar
 * @LastEditTime: 2025-01-14 16:28:29
 * @Description: 
 * 这段C++代码定义了一个名为`cppkafka`的命名空间，其中包含了多个异常类，这些类都是用于处理在使用cppkafka库时可能遇到的错误情况。cppkafka是一个C++客户端库，用于与Apache Kafka进行交互。下面是对每个异常类的简要解释：

1. **`Exception` 类**：
   - 这是所有自定义异常的基类，继承自`std::exception`。
   - 它有一个构造函数，接受一个`std::string`类型的消息，用于描述异常的原因。
   - `what()`方法被重写，用于返回异常的描述信息。

2. **`ConfigException` 类**：
   - 继承自`Exception`类。
   - 用于表示配置相关的异常。
   - 构造函数接受配置名称和错误信息。

3. **`ConfigOptionNotFound` 类**：
   - 继承自`Exception`类。
   - 用于表示在配置中找不到指定的选项。
   - 构造函数接受配置名称。

4. **`InvalidConfigOptionType` 类**：
   - 继承自`Exception`类。
   - 用于表示配置选项的类型无效。
   - 构造函数接受配置名称和类型信息。

5. **`ElementNotFound` 类**：
   - 继承自`Exception`类。
   - 用于表示找不到指定的元素（可能是主题、分区等）。
   - 构造函数接受元素类型和名称。

6. **`ParseException` 类**：
   - 继承自`Exception`类。
   - 用于表示解析错误（可能是解析配置文件或消息时出错）。
   - 构造函数接受一个描述解析错误的消息。

7. **`UnexpectVersion` 类**：
   - 继承自`Exception`类。
   - 用于表示遇到了意外的版本。
   - 构造函数接受一个`uint32_t`类型的版本号。

8. **`HandleException` 类**：
   - 继承自`Exception`类。
   - 用于表示处理过程中发生的异常。
   - 构造函数接受一个`Error`类型的错误（这里`Error`可能是一个枚举或结构体，用于描述错误详情，但代码中没有给出定义）。
   - `get_error()`方法用于获取错误详情。

9. **`ConsumerException` 类**：
   - 继承自`Exception`类。
   - 用于表示消费者相关的异常。
   - 构造函数和`get_error()`方法与`HandleException`类似。

10. **`QueueException` 类**：
    - 继承自`Exception`类。
    - 用于表示队列相关的异常。
    - 构造函数和`get_error()`方法与`HandleException`类似。

11. **`ActionTerminatedException` 类**：
    - 继承自`Exception`类。
    - 用于表示某个操作被终止的异常。
    - 构造函数接受一个描述错误的消息。

`CPPKAFKA_API`宏可能用于控制这些类的导出/导入，以便在动态链接库（DLL）中使用。这个宏的具体定义没有在这段代码中给出，但它通常用于跨平台开发中，以确保类的正确导出和导入。

这些异常类为cppkafka库的使用者提供了一种结构化的方式来处理可能发生的错误，使得错误处理更加清晰和易于管理。
 */

#pragma once

#include <stdexcept>
#include <string>
#include <librdkafka/rdkafka.h>
#include "macros.h"
#include "error.h"


namespace cppkafka {

class CPPKAFKA_API Exception : public std::exception {
public:
    Exception(std::string message);

    const char* what() const noexcept;

private:
    std::string message_;
};

class CPPKAFKA_API ConfigException : public Exception {
public:
    ConfigException(const std::string &config_name, const std::string &error);
};

class CPPKAFKA_API ConfigOptionNotFound : public Exception {
public:
    ConfigOptionNotFound(const std::string &config_name);
};

class CPPKAFKA_API InvalidConfigOptionType : public Exception {
public:
    InvalidConfigOptionType(const std::string &config_name, const std::string &type);
};

class CPPKAFKA_API ElementNotFound : public Exception {
public:
    ElementNotFound(const std::string &element_type, const std::string &name);
};


class CPPKAFKA_API ParseException : public Exception {
public:
    ParseException(const std::string &message);
};

class CPPKAFKA_API UnexpectVersion : public Exception {
public:
    UnexpectVersion(uint32_t version);
};

class CPPKAFKA_API HandleException : public Exception {
public:
    HandleException(Error error);

    Error get_error() const;

private:
    Error error_{};
};

class CPPKAFKA_API ConsumerException : public Exception {
public:
    ConsumerException(Error error);

    Error get_error() const;

private:
    Error error_;
};

class CPPKAFKA_API QueueException : public Exception {
public:
    QueueException(Error error);

    Error get_error() const;

private:
    Error error_{};
};

class CPPKAFKA_API ActionTerminatedException : public Exception {
public:
    ActionTerminatedException(const std::string &error);
};

} // namespace kafka

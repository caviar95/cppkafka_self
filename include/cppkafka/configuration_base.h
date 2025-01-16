
#pragma once

#include <string>
#include <map>
#include <vector>

#include "exceptions.h"
#include "configuration_option.h"

namespace cppkafka {

/*
    ConfigurationBase是一个模板类，设计用于配置管理。
    它提供了一系列set和get方法，用于设置和获取配置选项的值。
    这个类通过模板参数Concrete来支持不同的具体配置类，实现了一种泛型配置管理框架。

    ConfigurationBase类提供了一个灵活的框架，用于管理配置选项。
    它通过模板和SFINAE技术实现了对多种数据类型的支持，同时保持了代码的简洁性和可扩展性。
    然而，代码中的convert方法目前只支持布尔型和整型的转换，如果需要支持更多类型，需要进一步扩展convert方法的实现。
 */
template <typename Concrete>
class ConfigurationBase {
private:
    template <typename T>
    struct Type2Type {};

public:
    /*
        这个方法允许用户通过字符串键名和一个布尔值来设置配置。
        内部，它调用proxy_set方法，将布尔值转换为字符串"true"或"false"。
     */
    Concrete& set(const std::string& name, bool value) {
        return proxy_set(name, value ? "true" : "false");
    }

    /*
        这是一个模板方法，使用SFINAE（Substitution Failure Is Not An Error）技术来限制T只能为整型。
        它允许用户通过字符串键名和一个整型值来设置配置。
        内部，它使用std::to_string将整型值转换为字符串，然后调用proxy_set方法。
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    Concrete& set(const std::string& name, T value) {
        return proxy_set(name, std::to_string(value));
    }

    /*
        这个方法允许用户通过一个ConfigurationOption对象的向量来批量设置配置。
        它遍历向量中的每个ConfigurationOption对象，调用proxy_set方法来设置每个选项。
     */
    Concrete& set(const std::vector<ConfigurationOption>& options) {
        for (const auto&& option : options) {
            proxy_set(option.get_key(), option.get_value());
        }

        return static_cast<Concrete&>(*this);
    }

    /*
        这个方法允许用户通过字符串键名来获取配置值，并将值转换为指定的类型T。
        它首先调用具体配置类的get方法获取字符串值，然后使用convert函数将字符串值转换为指定类型。
     */
    template <typename T>
    T get(const std::string& name) const {
        std::string value = static_cast<const Concrete&>(*this).get(name);
        return convert(value, Type2Type<T>());
    }

protected:
    /*
        这个方法用于解析一个字符串数组，将其转换为一个键值对映射。
        它假设数组中的元素成对出现，每对元素分别表示键和值。
     */
    static std::map<std::string, std::string> parse_dump(const char** values, size_t count) {
        std::map<std::string, std::string> output;
        for (size_t i = 0; i < count; i += 2) {
            output[values[i]] = values[i + 1];
        }

        return output;
    }

private:
    /*
        这个方法是一个辅助方法，用于将设置配置的操作代理给具体配置类。
        它通过静态类型转换将*this转换为Concrete类型的引用，然后调用其set方法。
     */
    Concrete& proxy_set(const std::string& name, const std::string& value) {
        return static_cast<Concrete&>(*this).set(name, value);
    }

    /*
        convert方法是一个重载的静态方法，用于将字符串值转换为指定类型。目前代码中只提供了布尔型和整型的转换实现。
        对于布尔型，它检查字符串值是否为"true"或"false"；对于整型，它使用std::stoi进行转换。
     */
    static std::string convert(const std::string& value, Type2Type<bool>) {
        if (value == "true") {
            return true;
        } else if (value == "false") {
            return false;
        } else {
            throw InvalidConfigOptionType(value, "bool");
        }
    }

    static int convert(const std::string& value, Type2Type<int>) {
        try {
            return std::stoi(value);
        } catch (std::invalid_argument&) {
            throw InvalidConfigOptionType(value, "int");
        }
    }
};

} // namespace cppkafka
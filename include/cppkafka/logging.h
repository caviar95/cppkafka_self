#pragma once

namespace cppkafka {

enum class LogLevel : int {
    LogEmerg = 0, // system is unusable
    LogAlert = 1, // action must be taken immediately
    LogCrit = 2, // critical conditions
    LogErr = 3, // error conditions
    LogWarning = 4, // warning conditions
    LogNotice = 5, // normal but significant condition
    LogInfo = 6, // informational
    LogDebug = 7, // debug-level messages
};

} // namepsace cppkafka

/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 12:56
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <memory>
#include <format>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include "../common/Singleton.h"

class LogManager {
public:
    void Init();
    [[nodiscard]] std::shared_ptr<spdlog::logger> GetLogger() { return m_pLogger; }

private:
    std::shared_ptr<spdlog::logger> m_pLogger;
};

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Singleton<LogManager>::GetInstance()->GetLogger(), std::format(__VA_ARGS__))
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(Singleton<LogManager>::GetInstance()->GetLogger(), std::format(__VA_ARGS__))
#define LOG_WARNING(...) SPDLOG_LOGGER_WARN(Singleton<LogManager>::GetInstance()->GetLogger(), std::format(__VA_ARGS__))
#define LOG_CRITICAL(...) \
SPDLOG_LOGGER_CRITICAL(Singleton<LogManager>::GetInstance()->GetLogger(), std::format(__VA_ARGS__)); \
std::exit(EXIT_FAILURE);

#define LOG_ASSERT(CONDITION) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0})", #CONDITION); }
#define LOG_ASSERT_INFO(CONDITION, INFO) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0}): {1}", #CONDITION, INFO); }

#endif //LOGMANAGER_H

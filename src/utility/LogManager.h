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
    void Debug(std::string info) const;
    void Info(std::string info) const;
    void Warning(std::string info) const;
    void Critical(std::string info) const;

private:
    std::shared_ptr<spdlog::logger> m_pLogger;
};

#define LOG_DEBUG(...) Singleton<LogManager>::GetInstance()->Debug(std::format(__VA_ARGS__))
#define LOG_INFO(...) Singleton<LogManager>::GetInstance()->Info(std::format(__VA_ARGS__))
#define LOG_WARNING(...) Singleton<LogManager>::GetInstance()->Warning(std::format(__VA_ARGS__))
#define LOG_CRITICAL(...) \
Singleton<LogManager>::GetInstance()->Critical(std::format(__VA_ARGS__)); \
std::exit(EXIT_FAILURE);

#define LOG_ASSERT(CONDITION) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0})", #CONDITION); }
#define LOG_ASSERT_INFO(CONDITION, INFO) if(!(CONDITION)){ LOG_CRITICAL("Assert failed({0}): {1}", #CONDITION, INFO); }

#endif //LOGMANAGER_H

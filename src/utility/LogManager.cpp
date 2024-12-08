/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 14:04
* @version: 1.0
* @description: 
********************************************************************************/

#include "LogManager.h"

void LogManager::Init() {
    m_pLogger = spdlog::stdout_color_mt<spdlog::async_factory>("async_logger");
    m_pLogger->set_level(spdlog::level::trace);
    m_pLogger->set_pattern("[%^%H:%M:%S:%e] [%P-%t] [%1!L] [%s:%#] - %v%$");
    this->Info("Log initialization successful");
}

void LogManager::Debug(std::string info) const {
    SPDLOG_LOGGER_DEBUG(m_pLogger, info);
}

void LogManager::Info(std::string info) const {
    SPDLOG_LOGGER_INFO(m_pLogger, info);
}

void LogManager::Warning(std::string info) const {
    SPDLOG_LOGGER_WARN(m_pLogger, info);
}

void LogManager::Critical(std::string info) const {
    SPDLOG_LOGGER_CRITICAL(m_pLogger, info);
}
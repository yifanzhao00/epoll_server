#ifndef E_S_LOGGER_H
#define E_S_LOGGER_H

#include <string>
#include "noncopyable.h"

#define LOG_INFO(logmsgformat, ...) \
    do \
    { \
        Logger& logger_single = Logger::instance(); \
        logger_single.setLogLevel(INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger_single.log(buf); \
    }while (0)

#define LOG_ERROR(logmsgformat, ...) \
    do \
    { \
        Logger& logger_single = Logger::instance(); \
        logger_single.setLogLevel(ERROR); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger_single.log(buf); \
    }while (0)

#define LOG_FATAL(logmsgformat, ...) \
    do \
    { \
        Logger& logger_single = Logger::instance(); \
        logger_single.setLogLevel(FATAL); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger_single.log(buf); \
        exit(-1); \
    }while (0)

#ifdef SERVERDEBUG
#define LOG_DEBUG(logmsgformat, ...) \
    do \
    { \
        Logger& logger_single = Logger::instance(); \
        logger_single.setLogLevel(DEBUG); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger_single.log(buf); \
    }while (0)
#else
    #define LOG_DEBUG(logmsgformat, ...) 
#endif





// four levelf the log INFO, ERROR, FATAL and DEBUG
enum Loglevel
{
    INFO,
    ERROR,
    FATAL,
    DEBUG,
};

// singleton class
class Logger : noncopyable
{
public:
    static Logger& instance();
    void setLogLevel(int level);
    void log(std::string msg);
private:
    int loglevel_;
}; 



#endif
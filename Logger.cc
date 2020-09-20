#include "Logger.h"
#include <iostream>

Logger& Logger::instance()
{
    static Logger Logger_single;
    return Logger_single; 
}

void Logger::setLogLevel(int level)
{
    level = loglevel_;
}

void Logger::log(std::string msg)
{
    switch(loglevel_)
    {
    case INFO:
        std::cout << "[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
        break;
    case FATAL:
        std::cout << "[FATAL]";
        break;
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break; 
    }
}
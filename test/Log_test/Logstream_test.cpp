#include "Logger.h"
int main()
{
    Logger::SET_LEVEL(LogLevel::DEBUG);


    LOG_DEBUG << "LOG_DEBUG";
    LOG_INFO << "LOG_INFO";
    LOG_WARN << "LOG_WARN";
    LOG_ERROR << "LOG_ERROR";
    LOG_FATAL << "LOG_FATAL";

    return 0;
}

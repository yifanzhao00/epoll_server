#include "TimeStamp.h"

TimeStamp::TimeStamp() : timerEpoch_(0){}

TimeStamp::TimeStamp(int64_t timerEpoch)
                    : timerEpoch_(timerEpoch)
                    {}


TimeStamp TimeStamp::now()
{
    return TimeStamp(time(NULL));
}

// extern struct tm *localtime (const time_t *__timer) __THROW;
std::string TimeStamp::toString() const
{
    char buf[128] = {0};
    tm* local_time = localtime(&timerEpoch_);
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec);
    return buf;
}

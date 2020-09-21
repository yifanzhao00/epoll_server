#ifndef E_S_TIMESTAMP_H
#define E_S_TIMESTAMP_H

#include <iostream>
#include <string>

#include "time.h"

class TimeStamp
{
public:
    TimeStamp();
    explicit TimeStamp(int64_t timerEpoch);
    static TimeStamp now();
    std::string toString() const;
private:
    int64_t timerEpoch_;
};










#endif
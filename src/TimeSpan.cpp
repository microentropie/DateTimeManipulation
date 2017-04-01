/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2016-12-18

Time difference handling class

Sources repository: https://github.com/microentropie/
*/

#include "TimeSpan.h"
#include "myTimeUtils.h"

#ifdef _MSC_VER
#ifndef uint32
typedef unsigned int uint32;
#endif //uint64
extern time_t system_mktime(uint32 year, uint32 mon, uint32 day, uint32 hour, uint32 min, uint32 sec);
#else //_MSC_VER
extern "C"
{
  //#ifndef uint64
  //  typedef unsigned long uint64;
  //#endif //uint64
#ifndef uint32
  typedef unsigned int uint32;
#endif //uint64

  extern time_t system_mktime(uint32 year, uint32 mon, uint32 day, uint32 hour, uint32 min, uint32 sec);
}
#endif //_MSC_VER


TimeSpan::TimeSpan()
{
  timeStamp = 0;
  //memset(humanReadableTime, sizeof(tm), 0);
  //valid_hrt = false;
}

TimeSpan::TimeSpan(int year, int month, int day, int hour, int minute, int second)
{
  this->timeStamp = system_mktime(year, month, day, hour, minute, second);
}

TimeSpan::TimeSpan(int hour, int minute, int second)
{
  this->timeStamp = system_mktime(0, 0, 0, hour, minute, second);
}

TimeSpan::TimeSpan(time_t &ts)
{
  this->timeStamp = ts;
}

time_t TimeSpan::Add(time_t &ts)
{
  timeStamp += ts;
  return timeStamp;
}

time_t TimeSpan::Subtract(time_t &ts)
{
  timeStamp -= ts;
  return timeStamp;
}

int TimeSpan::Days()
{
  // days + without fractional part
  // only the integer part of TotalDays()
  return (int)(timeStamp / SECS_PER_DAY);
}

int TimeSpan::Hours()
{
  return (timeStamp / SECS_PER_HOUR) % 24;
}

int TimeSpan::Minutes()
{
  return (timeStamp / SECS_PER_MIN) % 60;
}

int TimeSpan::Seconds()
{
  return timeStamp % 60;
}

double TimeSpan::TotalDays()
{
  // days + fractional part
  return (double)timeStamp / (double)SECS_PER_DAY;
}

double TimeSpan::TotalHours()
{
  return (double)timeStamp / (double)SECS_PER_HOUR;
}

double TimeSpan::TotalMinutes()
{
  return (double)timeStamp / (double)SECS_PER_MIN;
}

double TimeSpan::TotalSeconds()
{
  return (double)timeStamp;
}

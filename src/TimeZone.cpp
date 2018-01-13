/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2016-12-18

TimeZone and Daylight Savings Time handling

Sources repository: https://github.com/microentropie/
*/

#include <time.h>
 //#include <sntp.h>
#include "myTimeUtils.h"
#include "TimeZone.h"

static struct TimeAndDateInfo localInfo;
//static dstCriteria localTimeDst = dstCriteria::CET;
//static signed char localTimeZone = +1; // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)

void SetLocalization(float newTimeZone, dstCriteria newTimeDst,
  const char dateSeparator, short dateFormat,
  const char timeSeparator, short timeFormat)
{
  localInfo.timeZone = newTimeZone;
  localInfo.dst = newTimeDst;

  localInfo.dateSeparator = dateSeparator;
  localInfo.dateFormat = dateFormat;

  localInfo.timeSeparator = timeSeparator;
  localInfo.timeFormat = timeFormat;
}

void SetLocalization(struct TimeAndDateInfo &lcl)
{
  localInfo = lcl;
}

struct TimeAndDateInfo GetLocalization()
{
  return localInfo;
}

float GetTimeZone()
{
  return localInfo.timeZone;
}

dstCriteria GetDST()
{
  return localInfo.dst;
}

#ifdef _MSC_VER
//#ifndef uint64
//  typedef unsigned long uint64;
//#endif //uint64
extern struct tm *sntp_mktm_r(const time_t * tim_p, struct tm *res, int is_gmtime);

#ifndef uint32
typedef unsigned int uint32;
#endif //uint64

extern time_t system_mktime(uint32 year, uint32 mon, uint32 day, uint32 hour, uint32 min, uint32 sec);

#define snprintf sprintf_s
#else //_MSC_VER
#include <c_types.h>
extern "C"
{
  extern struct tm * ICACHE_FLASH_ATTR sntp_localtime(const time_t * tim_p);
  extern struct tm * ICACHE_FLASH_ATTR sntp_mktm_r(const time_t * tim_p, struct tm *res, int is_gmtime);
}
#endif //_MSC_VER


//bool IsDayLightSavingsTimeCET(time_t utcUnixTime);
//char *LocalTimeToString(char *TimeBuf, int szTimeBuf, bool printTzInfo)
//{
//  time_t utcUnixTime = time(NULL);
//  time_t unixTime = UtcToLocalTime(utcUnixTime, localInfo);
//  return Iso8601DateTime(TimeBuf, szTimeBuf, unixTime, printTzInfo, localInfo.timeZone, IsDayLightSavingsTime(utcUnixTime, localInfo.dst));
//}

//
// timezone support
//
// https://www.timeanddate.com/time/dst/2016.html
// http://hackaday.com/2012/07/16/automatic-daylight-savings-time-compensation-for-your-clock-projects/
// http://stackoverflow.com/questions/5590429/calculating-daylight-saving-time-from-only-date

bool IsDayLightSavingsTimeCET(time_t localUnixTime)
{
  // https://en.wikipedia.org/wiki/Summer_Time_in_Europe
  // European Summer Time begins at 01:00 UTC on the last Sunday in March and
  // ends at 01:00 UTC on the last Sunday in October each year

  tm res;
  // subtract 1 hour because DST starts and ends at 1:00 AM
  localUnixTime -= SECS_PER_HOUR;
  sntp_mktm_r(&localUnixTime, &res, 0);

  if (month(&res) < 3 || month(&res) > 10) return false; // no DST in Jan, Feb, Nov, Dec
  if (month(&res) > 3 && month(&res) < 10) return true; // yes DST in Apr, May, Jun, Jul, Aug, Sep
  if (month(&res) == 3)
  {
    if (day(&res) < 25) return false;
    if (day(&res) >= 25 && (day(&res) - 25 - weekday(&res)) >= 0) return true;
    return false;
  }
  if (month(&res) == 10)
  {
    if (day(&res) < 24) return true;
    if (day(&res) >= 24 && (day(&res) - 24 - weekday(&res)) >= 0) return false;
    return true;
  }
  return false; // should never fall here
  //return (month(&res) == 3 && (hour(&res) + 24 * day(&res)) >= (3 + 24 * (31 - (5 * year(&res) / 4 + 4) % 7))) ||
  //  (month(&res) == 10 && (hour(&res) + 24 * day(&res)) < (3 + 24 * (31 - (5 * year(&res) / 4 + 1) % 7)));
}

// returns the time offset in number of seconds (0 if no DST, 3600 if DST)
int DayLightSavingsTimeCET(time_t localUnixTime)
{
  return IsDayLightSavingsTimeCET(localUnixTime) ? 3600 : 0;
}

bool IsDayLightSavingsTimeUSA(time_t localUnixTime)
{
  tm res;
  // subtract 2 hours because DST starts and ends at 2:00 AM
  localUnixTime -= (SECS_PER_HOUR << 1);
  sntp_mktm_r(&localUnixTime, &res, 0);

  // Daylight saving time starts on the second Sunday in March and
  // ends on the first Sunday in November,
  // with the time changes taking place at 2:00 a.m. local time.
  // https://en.wikipedia.org/wiki/Daylight_saving_time_in_the_United_States

  if (month(&res) < 3 || month(&res) > 11) return false;
  if (month(&res) > 3 && month(&res) < 11) return true;
  if (month(&res) == 3)
  {
    if (day(&res) < 8) return false;
    if (day(&res) > 14) return true;
    if (day(&res) - 8 - weekday(&res) >= 0) return true;
    return false;
  }
  if (month(&res) == 11)
  {
    if (day(&res) > 7) return false;
    if (day(&res) - 1 - weekday(&res) >= 0) return false;
    return true;
  }
  return false; // should never fall here
  //  && day(&res) < 8 && weekday(&res) > 0)  return true;
  //if (month(&res) == 11 && day(&res) < 8 && weekday(&res) == 0 && hour(&res) < 2) return true;// DST ends 1st Sunday of November 2am
}

int DayLightSavingsTimeUSA(time_t localUnixTime)
{
  return IsDayLightSavingsTimeUSA(localUnixTime) ? 3600 : 0;
}

bool IsDayLightSavingsTime(time_t localUnixTime, dstCriteria localTimeDst)
{
  switch (localTimeDst)
  {
  case dstCriteria::CET:
    return IsDayLightSavingsTimeCET(localUnixTime);
  case dstCriteria::USA:
    return IsDayLightSavingsTimeUSA(localUnixTime);
  }
  return false;
}
int DayLightSavingsTimeOffset(time_t localUnixTime, dstCriteria localTimeDst)
{
  switch (localTimeDst)
  {
  case dstCriteria::CET:
    return DayLightSavingsTimeCET(localUnixTime);
  case dstCriteria::USA:
    return DayLightSavingsTimeUSA(localUnixTime);
  }
  return 0;
}

time_t GetLocalTimePrecalc(time_t utcUnixTime, float timeZone, int dstSeconds)
{
  return utcUnixTime + (time_t)(timeZone * SECS_PER_HOUR) + (time_t)dstSeconds;
}

time_t UtcToLocalTime(time_t utcUnixTime, struct TimeAndDateInfo &tzi)
{
  return GetLocalTimePrecalc(utcUnixTime, tzi.timeZone, DayLightSavingsTimeOffset(utcUnixTime, tzi.dst));
}

time_t LocalTimeToUtc(time_t &localUnixTime, struct TimeAndDateInfo &tzi)
{
  return localUnixTime - (time_t)(tzi.timeZone * SECS_PER_HOUR) - DayLightSavingsTimeOffset(localUnixTime, tzi.dst);
}

#ifndef _TIMEZONE_H_
#define _TIMEZONE_H_

#include <time.h>

enum dstCriteria
{
  noDST = 0,
  CET = 1,
  USA = 2,
};

int DstCriteriaStrings(dstCriteria crit, const char **briefDescription, const char **longDescription);
int DateFormatStrings(int fmt, const char **briefDescription, const char **longDescription);
int TimeFormatStrings(int fmt, const char **briefDescription, const char **longDescription);

struct TimeAndDateInfo
{
  dstCriteria dst;
  float timeZone;

  char dateSeparator;
  short dateFormat;
  char timeSeparator;
  short timeFormat;
};

extern void SetLocalization(float localTimeZone, dstCriteria localTimeDst,
  const char dateSeparator, short dateFormat,
  const char timeSeparator, short timeFormat);
extern void SetLocalization(struct TimeAndDateInfo &lcl);
extern struct TimeAndDateInfo GetLocalization();
extern float GetTimeZone();
extern dstCriteria GetDST();


extern time_t UtcToLocalTime(time_t utcUnixTime, struct TimeAndDateInfo &tzi);
extern time_t LocalTimeToUtc(time_t &localUnixTime, struct TimeAndDateInfo &tzi);

extern bool IsDayLightSavingsTime(time_t utcUnixTime, dstCriteria localTimeDst);
extern int DayLightSavingsTimeOffset(time_t utcUnixTime, dstCriteria localTimeDst);

//extern char *LocalTimeToString(char *TimeBuf, int szTimeBuf, bool printTzInfo);

#endif //_TIMEZONE_H_

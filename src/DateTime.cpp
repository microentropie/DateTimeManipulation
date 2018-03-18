/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2016-12-18

Library: Date and Time handling class

Sources repository: https://github.com/microentropie/
*/

#include <time.h>
#include <string.h>

#include "DateTime.h"
#include "myTimeUtils.h"
#include "TimeZone.h"

#include <stdlib.h>
#include <stdio.h>


DateTime::DateTime()
{
  unixTimeStamp = 0;
  memset(&tmCopy, 0, sizeof(struct tm));
  bTimeChanged = true;
}

DateTime::DateTime(const DateTime &dt)
{
  this->unixTimeStamp = dt.unixTimeStamp;
  memset(&tmCopy, 0, sizeof(struct tm));
  bTimeChanged = true;
}

//DateTime::DateTime(time_t &unixTimeStamp)
//{
//  this->unixTimeStamp = unixTimeStamp;
//  memset(&tmCopy, 0, sizeof(struct tm));
//  bTimeChanged = true;
//}

DateTime::DateTime(time_t unixTimeStamp)
{
  this->unixTimeStamp = unixTimeStamp;
  memset(&tmCopy, 0, sizeof(struct tm));
  bTimeChanged = true;
}

DateTime::DateTime(int year, int month, int day)
{
  //this->unixTimeStamp = system_mktime(year, month, day, 0, 0, 0);
  struct tm tmUtm;
  tmUtm.tm_isdst=false;
  tmUtm.tm_sec=0;
  tmUtm.tm_min=0;
  tmUtm.tm_hour=0;
  tmUtm.tm_mday=day;
  tmUtm.tm_mon=month-1;
  tmUtm.tm_year=year - 1900;
  this->unixTimeStamp = mktime(&tmUtm);
  bTimeChanged = true;
}
DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
{
  //this->unixTimeStamp = system_mktime(year, month, day, hour, minute, second);
  struct tm tmUtm;
  tmUtm.tm_isdst=false;
  tmUtm.tm_sec=second;
  tmUtm.tm_min=minute;
  tmUtm.tm_hour=hour;
  tmUtm.tm_mday=day;
  tmUtm.tm_mon=month-1;
  tmUtm.tm_year=year - 1900;
  this->unixTimeStamp = mktime(&tmUtm);
  bTimeChanged = true;
}

struct tm *DateTime::GetTm()
{
  if (bTimeChanged)
  {
    tmCopy = *gmtime(&unixTimeStamp);
    //sntp_mktm_r(&unixTimeStamp, &tmCopy, 0);
    bTimeChanged = false;
  }
  return &tmCopy;
}

/*static*/ time_t DateTime::Now()
{
  return time(NULL);
}

/*static*/ time_t DateTime::Today()
{
  return Date(Now());
}

/*static*/ time_t DateTime::Date(time_t utm)
{
  struct tm tmUtm;
  //sntp_mktm_r(&utm, &tmUtm, 0);
  //return system_mktime(year(&tmUtm), month(&tmUtm), day(&tmUtm), 0, 0, 0);
  tmUtm = *gmtime(&utm);
  tmUtm.tm_sec=0;
  tmUtm.tm_min=0;
  tmUtm.tm_hour=0;
  return mktime(&tmUtm);
}

time_t DateTime::Date()
{
  return Date(unixTimeStamp);
}

TimeSpan *DateTime::TimeOfDay()
{
  return new TimeSpan(hour(GetTm()), minute(GetTm()), second(GetTm()));
}

int DateTime::Year()
{
  return year(GetTm());
}

int DateTime::Month()
{
  return month(GetTm());
}

int DateTime::Day()
{
  return day(GetTm());
}

int DateTime::DayOfWeek()
{
  return weekday(GetTm());
}

int DateTime::Hour()
{
  return hour(GetTm());
}

int DateTime::Minute()
{
  return minute(GetTm());
}

int DateTime::Second()
{
  return second(GetTm());
}


// operations
DateTime *DateTime::Add(TimeSpan value)
{
  this->unixTimeStamp += (time_t)value;
  bTimeChanged = value != 0;
  return this;
}

//DateTime DateTime::AddYears(int value);
//DateTime DateTime::AddMonths(int value);
DateTime *DateTime::AddDays(int value)
{
  this->unixTimeStamp += value * SECS_PER_DAY;
  bTimeChanged = value != 0;
  return this;
}

DateTime *DateTime::AddHours(int value)
{
  this->unixTimeStamp += value * SECS_PER_HOUR;
  bTimeChanged = value != 0;
  return this;
}

DateTime *DateTime::AddMinutes(int value)
{
  this->unixTimeStamp += value * SECS_PER_MIN;
  bTimeChanged = value != 0;
  return this;
}

DateTime *DateTime::AddSeconds(int value)
{
  this->unixTimeStamp += value;
  bTimeChanged = value != 0;
  return this;
}

/*static*/ int DateTime::Compare(DateTime &dt1, DateTime &dt2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return dt1.unixTimeStamp - dt2.unixTimeStamp;
  if (dt1.unixTimeStamp == dt2.unixTimeStamp)
    return 0;
  return dt1.unixTimeStamp > dt2.unixTimeStamp ? 1 : -1;
}

/*static*/ int DateTime::Compare(DateTime &dt1, DateTime *pDt2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return dt1.unixTimeStamp - pDt2->unixTimeStamp;
  if (dt1.unixTimeStamp == pDt2->unixTimeStamp)
    return 0;
  return dt1.unixTimeStamp > pDt2->unixTimeStamp ? 1 : -1;
}

/*static*/ int DateTime::Compare(DateTime *pDt1, DateTime &dt2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return pDt1->unixTimeStamp - dt2.unixTimeStamp;
  if (pDt1->unixTimeStamp == dt2.unixTimeStamp)
    return 0;
  return pDt1->unixTimeStamp > dt2.unixTimeStamp ? 1 : -1;
}

/*static*/ int DateTime::Compare(DateTime *pDt1, DateTime *pDt2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return pDt1->unixTimeStamp - pDt2->unixTimeStamp;
  if (pDt1->unixTimeStamp == pDt2->unixTimeStamp)
    return 0;
  return pDt1->unixTimeStamp > pDt2->unixTimeStamp ? 1 : -1;
}

/*static*/ int DateTime::Compare(DateTime *pDt1, time_t &tm2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return pDt1->unixTimeStamp - pDt2->unixTimeStamp;
  if (pDt1->unixTimeStamp == tm2)
    return 0;
  return pDt1->unixTimeStamp > tm2 ? 1 : -1;
}

/*static*/ int DateTime::Compare(time_t &tm1, DateTime *pDt2)
{
  // time_t (64 bit) is bigger than int (32 bit)
  // cannot simply:
  // return pDt1->unixTimeStamp - pDt2->unixTimeStamp;
  if (tm1 == pDt2->unixTimeStamp)
    return 0;
  return tm1 > pDt2->unixTimeStamp ? 1 : -1;
}

/*static*/ time_t DateTime::LocalToUtc(time_t &tm, struct TimeAndDateInfo &tzi)
{
  return LocalTimeToUtc(tm, tzi);

  return tm;
}
/*static*/ DateTime DateTime::LocalToUtc(DateTime &dt, struct TimeAndDateInfo &tzi)
{
  time_t tmUtc = LocalTimeToUtc(dt.unixTimeStamp, tzi);
  return *new DateTime(tmUtc);
}

/*static*/ time_t DateTime::UtcToLocal(time_t &tm, struct TimeAndDateInfo &tzi)
{
  return UtcToLocalTime(tm, tzi);

  return tm;
}
/*static*/ DateTime DateTime::UtcToLocal(DateTime &dt, struct TimeAndDateInfo &tzi)
{
  time_t tmLocal = UtcToLocalTime(dt.unixTimeStamp, tzi);
  return *new DateTime(tmLocal);
}

#if defined(SYSTEM_TIME_ADVANCED_FN) || defined(_MSC_VER)
/*static*/ char *DateTime::printfTime(char *TimeBuf, int szTimeBuf, struct tm *pTm, const char *formatString)
{
  if (!formatString)
    return asctime(pTm);
  strftime(TimeBuf, szTimeBuf, formatString, pTm);
  return TimeBuf;
}

/*static*/ char *DateTime::ToString(char *TimeBuf, int szTimeBuf, const char *formatString)
{
  return DateTime::printfTime(TimeBuf, szTimeBuf, GetTm(), formatString);
}
#endif

// separator character
//   if '\0' or invalid, ':' will be used
// Format: Example:     Description:
//   16    14:55:02     HH:mm:SS (ISO 8601) time format
//   17    02:55:02 pm  imperial time format
//   18     2:55:02 pm  imperial time format (space as 1st char if hour not 12)
//    0    14:55        HH:mm 24h time format
//    1    02:55 pm     imperial time format without seconds
//    2     2:55 pm     imperial time format (space as 1st char if hour not 12) without seconds
char *DateTime::TimeToString(char *TimeBuf, int szTimeBuf, struct TimeAndDateInfo &lcl)
{
  int separator = ':';
  if (lcl.timeSeparator >= 32)
    separator = lcl.timeSeparator;

  char ampm = '\0';
  int h;
  if ((lcl.timeFormat & 0xf) == 1 || (lcl.timeFormat & 0xf) == 2)
  {
    ampm = 'a';
    h = hour(GetTm());
    if (h > 11) ampm = 'p';
    if (h > 12)
      h -= 12;
    else if (h == 0)
      h = 12;
  }
  switch (lcl.timeFormat)
  {
  default:
  case TIMEPRINT_HHmmSS:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%02d", hour(GetTm()), separator, minute(GetTm()), separator, second(GetTm()));
    break;
  case TIMEPRINT_HHmm:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d", hour(GetTm()), separator, minute(GetTm()));
    break;
  case TIMEPRINT_SpaceAMPMSS:
    snprintf(TimeBuf, szTimeBuf, "%2d%c%02d%c%02d %cm", h, separator, minute(GetTm()), separator, second(GetTm()), ampm);
    break;
  case TIMEPRINT_AMPMSS:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%02d %cm", h, separator, minute(GetTm()), separator, second(GetTm()), ampm);
    break;
  case TIMEPRINT_SpaceAMPM:
    snprintf(TimeBuf, szTimeBuf, "%2d%c%02d %cm", h, separator, minute(GetTm()), ampm);
    break;
  case TIMEPRINT_AMPM:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d %cm", h, separator, minute(GetTm()), ampm);
    break;
  }
  return TimeBuf;
}

/*static*/ char *DateTime::LocalTimeToString(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, struct TimeAndDateInfo &lcl)
{
  time_t localUnixTime = DateTime::UtcToLocal(utcUnixTime, lcl);
  DateTime *pDt = new DateTime(localUnixTime);
  pDt->TimeToString(TimeBuf, szTimeBuf, lcl);
  delete pDt;
  return TimeBuf;
}

/*static*/ char *DateTime::LocalTimeToString(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, TimePrintOption_e tpo)
{
  struct TimeAndDateInfo lcl = GetLocalization();
  if (tpo == TimePrintOption_e::SecondsYes)
    lcl.timeFormat |= TIMEPRINT_SECONDS;
  else
    lcl.timeFormat &= ~TIMEPRINT_SECONDS;
  return DateTime::LocalTimeToString(TimeBuf, szTimeBuf, utcUnixTime, lcl);
}

/*static*/ char *DateTime::LocalTimeToString(char *TimeBuf, int szTimeBuf, TimePrintOption_e tpo)
{
  return DateTime::LocalTimeToString(TimeBuf, szTimeBuf, time(NULL), tpo);
}

// separator character
//   if '\0' or invalid, '/' will be used
// Format: Example:     Description:
//   64    2001/08/23   YYYY-MM-DD (ISO 8601) format
//   65    23/08/2001   DD-MM-YYYY format
//   66    08/23/2001   MM-DD-YYYY format

//   80    2001/8/23    YYYY-M-D   format
//   81    23/8/2001    D-M-YYYY   format
//   82    8/23/2001    M-D-YYYY   format

//   32    01/08/23     YY-MM-DD   format
//   33    23/08/01     DD-MM-YY   format
//   34    08/23/01     MM-DD-YY   format

//   48    01/8/23      YY-M-D     format
//   49    23/8/01      D-M-YY     format
//   50    8/23/01      M-D-YY     format

//    0    08/23        MM-DD      format
//    1    23/08        DD-MM      format
//    2    08/23        MM-DD      format

//   16    8/23         M-D        format
//   17    23/8         D-M        format
//   18    8/23         M-D        format
char *DateTime::DateToString(char *TimeBuf, int szTimeBuf, struct TimeAndDateInfo &lcl)
{
  int separator = '/';
  if (lcl.timeSeparator >= 32)
    separator = lcl.dateSeparator;

  switch (lcl.dateFormat)
  {
    // YEAR FULL
  default:
  case DATEPRINT_YYYYMMDD:
    snprintf(TimeBuf, szTimeBuf, "%04d%c%02d%c%02d", year(GetTm()), separator, month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DDMMYYYY:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%04d", day(GetTm()), separator, month(GetTm()), separator, year(GetTm()));
    break;
  case DATEPRINT_MMDDYYYY:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%04d", month(GetTm()), separator, day(GetTm()), separator, year(GetTm()));
    break;

  case DATEPRINT_YYYYMD:
    snprintf(TimeBuf, szTimeBuf, "%04d%c%d%c%d", year(GetTm()), separator, month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DMYYYY:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d%c%04d", day(GetTm()), separator, month(GetTm()), separator, year(GetTm()));
    break;
  case DATEPRINT_MDYYYY:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d%c%04d", month(GetTm()), separator, day(GetTm()), separator, year(GetTm()));
    break;

    // YEAR SHORT
  case DATEPRINT_YYMMDD:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%02d", year(GetTm()) % 100, separator, month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DDMMYY:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%02d", day(GetTm()), separator, month(GetTm()), separator, year(GetTm()) % 100);
    break;
  case DATEPRINT_MMDDYY:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d%c%02d", month(GetTm()), separator, day(GetTm()), separator, year(GetTm()) % 100);
    break;

  case DATEPRINT_YYMD:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%d%c%d", year(GetTm()) % 100, separator, month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DMYY:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d%c%02d", day(GetTm()), separator, month(GetTm()), separator, year(GetTm()) % 100);
    break;
  case DATEPRINT_MDYY:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d%c%02d", month(GetTm()), separator, day(GetTm()), separator, year(GetTm()) % 100);
    break;

    // YEAR NO
  case DATEPRINT_MMDD:
  case DATEPRINT_MMDDY | DATEPRINT_YEAR_NO:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d", month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DDMM:
    snprintf(TimeBuf, szTimeBuf, "%02d%c%02d", day(GetTm()), separator, month(GetTm()));
    break;

  case DATEPRINT_MD:
  case DATEPRINT_MMDDY | DATEPRINT_YEAR_NO | DATEPRINT_SHORT_MD:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d", month(GetTm()), separator, day(GetTm()));
    break;
  case DATEPRINT_DM:
    snprintf(TimeBuf, szTimeBuf, "%d%c%d", day(GetTm()), separator, month(GetTm()));
    break;
  }
  return TimeBuf;
}

/*static*/ char *DateTime::LocalDateToString(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, struct TimeAndDateInfo &lcl)
{
  time_t localUnixTime = DateTime::UtcToLocal(utcUnixTime, lcl);
  DateTime *pDt = new DateTime(localUnixTime);
  pDt->DateToString(TimeBuf, szTimeBuf, lcl);
  delete pDt;
  return TimeBuf;
}

/*static*/ char *DateTime::LocalDateToString(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, DatePrintOption_e dpo)
{
  struct TimeAndDateInfo lcl = GetLocalization();
  lcl.dateFormat &= ~DATEPRINT_YEAR_MASK;
  switch (dpo)
  {
  default:
  case DatePrintOption_e::YearFull:
    lcl.dateFormat |= DATEPRINT_YEAR_FULL;
    break;
  case DatePrintOption_e::YearShort:
    lcl.dateFormat |= DATEPRINT_YEAR_SHORT;
    break;
  case DatePrintOption_e::YearNo:
    //lcl.dateFormat |= DATEPRINT_YEAR_NO;
    break;
  }
  return DateTime::LocalDateToString(TimeBuf, szTimeBuf, utcUnixTime, lcl);
}

/*static*/ char *DateTime::LocalDateToString(char *TimeBuf, int szTimeBuf, DatePrintOption_e dpo)
{
  return DateTime::LocalDateToString(TimeBuf, szTimeBuf, time(NULL), dpo);
}

/*static*/ char *DateTime::Iso8601DateTime(char *TimeBuf, int szTimeBuf, time_t unixTime, bool printTzInfo, float timeZone, bool isDst)
{
  if (szTimeBuf < 27)
    return NULL;
  // yyyy/MM/dd hh:mm:ss +12 dst
  //           11111111112222222222
  // 012345678901234567890123456789

  struct TimeAndDateInfo lcl = GetLocalization();
  lcl.dateSeparator = '/';
  lcl.dateFormat = DATEPRINT_YYYYMMDD;
  lcl.timeSeparator = ':';
  lcl.timeFormat = TIMEPRINT_HHmmSS;

  DateTime *pDt = new DateTime(unixTime);
  pDt->DateToString(TimeBuf, szTimeBuf, lcl);
  int l = strlen(TimeBuf);
  TimeBuf[l++] = ' ';
  pDt->TimeToString(&TimeBuf[l], szTimeBuf - l, lcl);
  delete pDt;

  if (printTzInfo)
  {
    if (timeZone < -24.0)
    {
      // unixTime is UTC
      l = strlen(TimeBuf);
      snprintf(&TimeBuf[l], szTimeBuf - l, " UTC");
    }
    else
    {
      // local time
      l = strlen(TimeBuf);
      // assume no decimal or max 2 decimals
      timeZone += 0.005f;
      int integerPart = (int)timeZone;
      int decimalPart = (int)((timeZone - (float)integerPart) * 100);
      if (decimalPart == 0)
        snprintf(&TimeBuf[l], szTimeBuf - l, " (%+d", integerPart);
      else
        snprintf(&TimeBuf[l], szTimeBuf - l, " (%+d.%02d", integerPart, decimalPart);
      if (isDst)
      {
        l = strlen(TimeBuf);
        snprintf(&TimeBuf[l], szTimeBuf - l, " +dst");
      }
      l = strlen(TimeBuf);
      snprintf(&TimeBuf[l], szTimeBuf - l, ")");
    }
  }
  return TimeBuf;
}

/*static*/ char *DateTime::LocalIso8601DateTime(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, bool printTzInfo)
{
  struct TimeAndDateInfo tzi = GetLocalization();
  time_t localUnixTime = DateTime::UtcToLocal(utcUnixTime, tzi);
  return DateTime::Iso8601DateTime(TimeBuf, szTimeBuf, localUnixTime, printTzInfo,
    tzi.timeZone,
    IsDayLightSavingsTime(utcUnixTime, tzi.dst));
}

/*static*/ char *DateTime::UtcIso8601DateTime(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, bool printTzInfo)
{
  return DateTime::Iso8601DateTime(TimeBuf, szTimeBuf, utcUnixTime, printTzInfo, -99.0, false);
}

// time is parsed as is, TZ and DST are ignored
bool DateTime::TryParseTime(const char *strTime, char chTimeSeparator, time_t &value)
{
  // hh:mm[:ss] [am|pm]
  int hour = 0;
  int min = 0;
  int sec = 0;
  char *p = (char *)strTime;
  char *sep = strchr(p, chTimeSeparator);
  hour = atoi(p);
  if (sep)
  {
    p = ++sep;
    sep = strchr(p, chTimeSeparator);
    min = atoi(p);
    if (min < 0 || min >= 60)
      return false;
  }
  if (sep)
  {
    p = ++sep;
    sec = atoi(p);
    if (sec < 0 || sec >= 60)
      return false;
  }
  sep = strchr(p, 'a');
  if (!sep)
    sep = strchr(p, 'p');
  if (!sep)
    sep = strchr(p, 'A');
  if (!sep)
    sep = strchr(p, 'P');
  if (sep)
  {
    // must convert to 24 hour format
    bool pm = *sep == 'p' || *sep == 'P';
    if (pm)
    {
      if (hour < 12) // 12 pm = 12
        hour += 12;
    }
    else
    {
      if (hour == 12) // 12 am = 00
        hour = 0;
    }
  }
  if (hour < 0 || hour >= 24)
    return false;

  value = hour * SECS_PER_HOUR + min * SECS_PER_MIN + sec;
  return true;
}

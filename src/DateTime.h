#ifndef _DateTime_h_
#define _DateTime_h_

#include <time.h>
#include "TimeSpan.h"
#include "TimeZone.h"

enum TimePrintOption_e
{
  SecondsYes,
  SecondsNo,
};

enum DatePrintOption_e
{
  YearFull,
  YearShort,
  YearNo,
};

class DateTime
{
private:
  time_t unixTimeStamp;
  struct tm tmCopy;
  bool bTimeChanged;

protected:
  struct tm *GetTm();

public:
  DateTime();
  DateTime(const DateTime &dt);
  operator time_t() const { return unixTimeStamp; }
  //DateTime(time_t &unixTimeStamp);
  DateTime(time_t unixTimeStamp);
  DateTime(int year, int month, int day);
  DateTime(int year, int month, int day, int hour, int minute, int second);

  static time_t Now();
  static time_t Today();
  static time_t Date(time_t utm);

  time_t Date();
  TimeSpan *TimeOfDay();

  int Year();
  int Month();
  int Day();
  int DayOfWeek();
  int Hour();
  int Minute();
  int Second();

  // operations
  DateTime *Add(TimeSpan value);
  //DateTime *AddYears(int value);
  //DateTime *AddMonths(int value);
  DateTime *AddDays(int value);
  DateTime *AddHours(int value);
  DateTime *AddMinutes(int value);
  DateTime *AddSeconds(int value);

  static int Compare(DateTime &dt1, DateTime &dt2);
  static int Compare(DateTime &dt1, DateTime *pDt2);
  static int Compare(DateTime *pDt1, DateTime &dt2);
  static int Compare(DateTime *pDt1, DateTime *pDt2);
  static int Compare(DateTime *pDt1, time_t &dt2);
  static int Compare(time_t &dt1, DateTime *pDt2);

  static time_t LocalToUtc(time_t &tm, struct TimeAndDateInfo &tzi);
  static DateTime LocalToUtc(DateTime &dt, struct TimeAndDateInfo &tzi);
  static time_t UtcToLocal(time_t &tm, struct TimeAndDateInfo &tzi);
  static DateTime UtcToLocal(DateTime &dt, struct TimeAndDateInfo &tzi);

#if defined(SYSTEM_TIME_ADVANCED_FN) || defined(_MSC_VER)
  // formatString uses strftime() syntax
  char *ToString(char *TimeBuf, int szTimeBuf, const char *formatString);
  //static char *ToString(char *TimeBuf, int szTimeBuf, time_t &tm, const char *formatString);
  // formatString uses strftime() syntax
  static char *printfTime(char *TimeBuf, int szTimeBuf, struct tm *pTm, const char *formatString);
#endif
  char *TimeToString(char *TimeBuf, int szTimeBuf, struct TimeAndDateInfo &lcl);
  static char *LocalTimeToString(char *TimeBuf, int szTimeBuf, time_t unixTime, struct TimeAndDateInfo &lcl);
  static char *LocalTimeToString(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, TimePrintOption_e tpo);
  static char *LocalTimeToString(char *TimeBuf, int szTimeBuf, TimePrintOption_e tpo);
  char *DateToString(char *TimeBuf, int szTimeBuf, struct TimeAndDateInfo &lcl);
  static char *LocalDateToString(char *TimeBuf, int szTimeBuf, time_t unixTime, struct TimeAndDateInfo &lcl);
  static char *LocalDateToString(char *TimeBuf, int szTimeBuf, time_t unixTime, DatePrintOption_e dpo);
  static char *LocalDateToString(char *TimeBuf, int szTimeBuf, DatePrintOption_e dpo);

  static char *Iso8601DateTime(char *TimeBuf, int szTimeBuf, time_t unixTime, bool printTzInfo, float timeZone, bool isDst);
  static char *LocalIso8601DateTime(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, bool printTzInfo);
  static char *UtcIso8601DateTime(char *TimeBuf, int szTimeBuf, time_t utcUnixTime, bool printTzInfo);

  static bool TryParseTime(const char *strTime, char chTimeSeparator, time_t &value);
};

// internal use, subject to changes
#define DATEPRINT_YMMDD 0
#define DATEPRINT_DDMMY 1
#define DATEPRINT_MMDDY 2

#define DATEPRINT_SHORT_MD 16

#define DATEPRINT_YEAR_NO     0
#define DATEPRINT_YEAR_SHORT 32
#define DATEPRINT_YEAR_FULL  64
#define DATEPRINT_YEAR_MASK  (DATEPRINT_YEAR_FULL | DATEPRINT_YEAR_SHORT)

// DateToString Format Constants:

// YEAR NO:
#define DATEPRINT_MMDD (DATEPRINT_YMMDD | DATEPRINT_YEAR_NO)
#define DATEPRINT_DDMM (DATEPRINT_DDMMY | DATEPRINT_YEAR_NO)
//#define DATEPRINT_MMDD (DATEPRINT_MMDDY | DATEPRINT_YEAR_NO)

#define DATEPRINT_MD (DATEPRINT_YMMDD | DATEPRINT_YEAR_NO | DATEPRINT_SHORT_MD)
#define DATEPRINT_DM (DATEPRINT_DDMMY | DATEPRINT_YEAR_NO | DATEPRINT_SHORT_MD)
//#define DATEPRINT_MD (DATEPRINT_MMDDY | DATEPRINT_YEAR_NO | DATEPRINT_SHORT_MD)

// YEAR SHORT:
#define DATEPRINT_YYMMDD (DATEPRINT_YMMDD | DATEPRINT_YEAR_SHORT)
#define DATEPRINT_DDMMYY (DATEPRINT_DDMMY | DATEPRINT_YEAR_SHORT)
#define DATEPRINT_MMDDYY (DATEPRINT_MMDDY | DATEPRINT_YEAR_SHORT)

#define DATEPRINT_YYMD (DATEPRINT_YMMDD | DATEPRINT_YEAR_SHORT | DATEPRINT_SHORT_MD)
#define DATEPRINT_DMYY (DATEPRINT_DDMMY | DATEPRINT_YEAR_SHORT | DATEPRINT_SHORT_MD)
#define DATEPRINT_MDYY (DATEPRINT_MMDDY | DATEPRINT_YEAR_SHORT | DATEPRINT_SHORT_MD)

// YEAR FULL:
#define DATEPRINT_YYYYMMDD (DATEPRINT_YMMDD | DATEPRINT_YEAR_FULL)
#define DATEPRINT_DDMMYYYY (DATEPRINT_DDMMY | DATEPRINT_YEAR_FULL)
#define DATEPRINT_MMDDYYYY (DATEPRINT_MMDDY | DATEPRINT_YEAR_FULL)

#define DATEPRINT_YYYYMD (DATEPRINT_YMMDD | DATEPRINT_YEAR_FULL | DATEPRINT_SHORT_MD)
#define DATEPRINT_DMYYYY (DATEPRINT_DDMMY | DATEPRINT_YEAR_FULL | DATEPRINT_SHORT_MD)
#define DATEPRINT_MDYYYY (DATEPRINT_MMDDY | DATEPRINT_YEAR_FULL | DATEPRINT_SHORT_MD)

// TimeToString Format Constants:
#define TIMEPRINT_HHmm      0
#define TIMEPRINT_AMPM      1
#define TIMEPRINT_SpaceAMPM 2

#define TIMEPRINT_SECONDS  16

#define TIMEPRINT_HHmmSS      (TIMEPRINT_HHmm | TIMEPRINT_SECONDS)
#define TIMEPRINT_AMPMSS      (TIMEPRINT_AMPM | TIMEPRINT_SECONDS)
#define TIMEPRINT_SpaceAMPMSS (TIMEPRINT_SpaceAMPM | TIMEPRINT_SECONDS)


#endif //_DateTime_h_

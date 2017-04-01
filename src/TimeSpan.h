#ifndef _TimeSpan_h_
#define _TimeSpan_h_

#include <time.h>

class TimeSpan
{
private:
  time_t timeStamp;
  //tm humanReadableTime;
  //bool valid_hrt;
public:
  TimeSpan();
  TimeSpan(int year, int month, int day, int hour, int minute, int second);
  TimeSpan(int hour, int minute, int second);
  TimeSpan(time_t &ts);
  time_t Add(time_t &ts);
  time_t Subtract(time_t &ts);
  operator time_t() const { return timeStamp; }

  int Days();
  int Hours();
  int Minutes();
  int Seconds();
  double TotalDays();
  double TotalHours();
  double TotalMinutes();
  double TotalSeconds();
};

#endif //_TimeSpan_h_

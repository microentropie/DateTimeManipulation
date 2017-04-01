/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2016-12-18

Using DateTime Libary

Sources repository: https://github.com/microentropie/
*/

#include <DateTime.h>

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("Date example");
  Serial.println("https://github.com/microentropie/DateTimeManipulation");
}

void loop()
{
  char Buf[100];

  
  DateTime myTime = DateTime::Now(); 
  // If neither a RTC nor an NTP clock is present, time will be 1970/01/01 00:00:00

  DateTime::LocalTimeToString(Buf, sizeof(Buf), myTime, TimePrintOption_e::SecondsNo); 
  Serial.print("Initial   time is: ");
  Serial.print(Buf);
  Serial.println();

  myTime.AddSeconds(3 * 60 + 1); // add 3 minutes and 1 second

  myTime.AddMinutes(1); // further add 1 minute
  
  DateTime::LocalTimeToString(Buf, sizeof(Buf), myTime, TimePrintOption_e::SecondsNo); 
  Serial.print("Resulting time is: ");
  Serial.print(Buf);
  Serial.println();
 
  delay(30000);
}
/*
 * output from this sketch
Date example
https://github.com/microentropie/DateTimeManipulation
Initial time is: 00:00
Resulting time is: 00:04
*/
 
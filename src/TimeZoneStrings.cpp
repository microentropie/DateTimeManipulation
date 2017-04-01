/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2016-12-18

TimeZone strings for the configuration interface

Sources repository: https://github.com/microentropie/
*/

#include "TimeZone.h"
#include "DateTime.h"

int DstCriteriaStrings(dstCriteria crit, const char **briefDescription, const char **longDescription)
{
  if (briefDescription == NULL || longDescription == NULL)
    return -1;
  switch (crit)
  {
  case noDST:
    *briefDescription = "noDST";
    *longDescription = "no Daylight Savings Time";
    break;
  case CET:
    *briefDescription = "CET";
    *longDescription = "most Center European countries";
    break;
  case USA:
    *briefDescription = "USA";
    *longDescription = "most USA places";
    break;
  default:
    *briefDescription = NULL;
    *longDescription = NULL;
    crit = (dstCriteria)-1; // return an invalid value, existing enums are all >= 0)
    break;
  }
  return crit;
};



// separator character
//   if '\0' or invalid, '/' will be used
// Format: Example:     Description:
//   32    2001/08/23   YYYY-MM-DD (ISO 8601) format
//   33    23/08/2001   DD-MM-YYYY format
//   34    08/23/2001   MM-DD-YYYY format

//   48    2001/8/23    YYYY-M-D   format
//   49    23/8/2001    D-M-YYYY   format
//   50    8/23/2001    M-D-YYYY   format

//    0    01/08/23     YY-MM-DD   format
//    1    23/08/01     DD-MM-YY   format
//    2    08/23/01     MM-DD-YY   format

//   16    01/8/23      YY-M-D     format
//   17    23/8/01      D-M-YY     format
//   18    8/23/01      M-D-YY     format
int DateFormatStrings(int fmt, const char **briefDescription, const char **longDescription)
{
  if (briefDescription == NULL || longDescription == NULL)
    return -1;
  switch (fmt)
  {
    // YEAR FULL
  case DATEPRINT_YYYYMMDD:
    *briefDescription = "YYYY-MM-DD";
    *longDescription = "(ISO 8601) Ex.: 2001/08/23";
    break;
  case DATEPRINT_DDMMYYYY:
    *briefDescription = "DD-MM-YYYY";
    *longDescription = "Ex.: 23/08/2001";
    break;
  case DATEPRINT_MMDDYYYY:
    *briefDescription = "MM-DD-YYYY";
    *longDescription = "Ex.: 08/23/2001";
    break;

  case DATEPRINT_YYYYMD:
    *briefDescription = "YYYY-M-D";
    *longDescription = "Ex.: 2001/8/23";
    break;
  case DATEPRINT_DMYYYY:
    *briefDescription = "D-M-YYYY";
    *longDescription = "Ex.: 23/8/2001";
    break;
  case DATEPRINT_MDYYYY:
    *briefDescription = "M-D-YYYY";
    *longDescription = "Ex.: 8/23/2001";
    break;

    // YEAR SHORT
  case DATEPRINT_YYMMDD:
    *briefDescription = "YY-MM-DD";
    *longDescription = "Ex.: 01/08/23";
    break;
  case DATEPRINT_DDMMYY:
    *briefDescription = "DD-MM-YY";
    *longDescription = "Ex.: 23/08/01";
    break;
  case DATEPRINT_MMDDYY:
    *briefDescription = "MM-DD-YY";
    *longDescription = "Ex.: 08/23/01";
    break;

  case DATEPRINT_YYMD:
    *briefDescription = "YY-M-D";
    *longDescription = "Ex.: 01/8/23";
    break;
  case DATEPRINT_DMYY:
    *briefDescription = "D-M-YY";
    *longDescription = "Ex.: 23/8/01";
    break;
  case DATEPRINT_MDYY:
    *briefDescription = "M-D-YY";
    *longDescription = "Ex.: 8/23/01";
    break;

    // YEAR NO
  case DATEPRINT_MMDD:
    *briefDescription = "MM-DD";
    *longDescription = "Ex.: 08/23";
    break;
  case DATEPRINT_DDMM:
    *briefDescription = "DD-MM";
    *longDescription = "Ex.: 23/08";
    break;

  case DATEPRINT_MD:
    *briefDescription = "M-D";
    *longDescription = "Ex.: 8/23";
    break;
  case DATEPRINT_DM:
    *briefDescription = "D-M";
    *longDescription = "Ex.: 23/8";
    break;

  default:
    *briefDescription = NULL;
    *longDescription = NULL;
    fmt = -1; // return an invalid value, valid values are all >= 0)
    break;
  }

  return fmt;
}

//   16    14:55:02     HH:mm:SS (ISO 8601) time format
//   17    02:55:02 pm  imperial time format
//   18     2:55:02 pm  imperial time format (space as 1st char if hour not 12)
//    0    14:55        HH:mm 24h time format
//    1    02:55 pm     imperial time format without seconds
//    2     2:55 pm     imperial time format (space as 1st char if hour not 12) without seconds
int TimeFormatStrings(int fmt, const char **briefDescription, const char **longDescription)
{
  switch (fmt)
  {
  case TIMEPRINT_HHmmSS:
    *briefDescription = "HH:mm:SS";
    *longDescription = "(ISO 8601) Ex.: 14:55:02";
    break;
  case TIMEPRINT_AMPMSS:
    *briefDescription = "HH:mm:SS ?m";
    *longDescription = "Ex.: 02:55:02 pm";
    break;
  case TIMEPRINT_SpaceAMPMSS:
    *briefDescription = " H:mm:SS ?m";
    *longDescription = "Ex.:  2:55:02 pm";
    break;

  case TIMEPRINT_HHmm:
    *briefDescription = "HH:mm";
    *longDescription = "Ex.: 14:55";
    break;
  case TIMEPRINT_AMPM:
    *briefDescription = "HH:mm ?m";
    *longDescription = "Ex.: 02:55 pm";
    break;
  case TIMEPRINT_SpaceAMPM:
    *briefDescription = " H:mm ?m";
    *longDescription = "Ex.:  2:55 pm";
    break;

  default:
    *briefDescription = NULL;
    *longDescription = NULL;
    fmt = -1; // return an invalid value, valid values are all >= 0)
    break;
  }

  return fmt;
}

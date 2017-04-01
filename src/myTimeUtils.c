/*
Time library extension.

Copyright(C) Stefano Di Paolo 2016
Copyright(c) Michael Margolis 2009 - 2014

This library is free software; you can redistribute it and / or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include <time.h>

// the hour for the given time in 12 hour format
int hourFormat12(struct tm *pTm)
{
  if (pTm->tm_hour == 0)
    return 12; // 12 midnight
  else if (pTm->tm_hour > 12)
    return pTm->tm_hour - 12;
  else
    return pTm->tm_hour;
}

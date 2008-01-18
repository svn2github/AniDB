// Copyright (C) 2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <windows.h>
#include <iostream>
#include <iomanip>

struct reference_point
{
  FILETIME file_time;
  LARGE_INTEGER counter;
};

void simplistic_synchronize(reference_point& ref_point)
{
  FILETIME      ft0 = { 0, 0 },
                ft1 = { 0, 0 };
  LARGE_INTEGER li;

  //
  // Spin waiting for a change in system time. Get the matching
  // performance counter value for that time.
  //
  ::GetSystemTimeAsFileTime(&ft0);
  do
  {
    ::GetSystemTimeAsFileTime(&ft1);
    ::QueryPerformanceCounter(&li);
  }
  while ((ft0.dwHighDateTime == ft1.dwHighDateTime) &&
         (ft0.dwLowDateTime == ft1.dwLowDateTime));

  ref_point.file_time = ft1;
  ref_point.counter = li;
}

void get_time(LARGE_INTEGER frequency, const reference_point& 
    reference, FILETIME& current_time)
{
  LARGE_INTEGER li;

  ::QueryPerformanceCounter(&li);

  //
  // Calculate performance counter ticks elapsed
  //
  LARGE_INTEGER ticks_elapsed;
  
  ticks_elapsed.QuadPart = li.QuadPart - 
      reference.counter.QuadPart;

  //
  // Translate to 100-nanoseconds intervals (FILETIME 
  // resolution) and add to
  // reference FILETIME to get current FILETIME.
  //
  ULARGE_INTEGER filetime_ticks,
                 filetime_ref_as_ul;

  filetime_ticks.QuadPart = 
      (ULONGLONG)((((double)ticks_elapsed.QuadPart/(double)
      frequency.QuadPart)*10000000.0)+0.5);
  filetime_ref_as_ul.HighPart = reference.file_time.dwHighDateTime;
  filetime_ref_as_ul.LowPart = reference.file_time.dwLowDateTime;
  filetime_ref_as_ul.QuadPart += filetime_ticks.QuadPart;

  //
  // Copy to result
  //
  current_time.dwHighDateTime = filetime_ref_as_ul.HighPart;
  current_time.dwLowDateTime = filetime_ref_as_ul.LowPart;
}

#include "utils.h"
int getms()
{
  reference_point ref_point;
  LARGE_INTEGER   frequency;
  FILETIME        file_time;
  SYSTEMTIME      system_time;

  ::QueryPerformanceFrequency(&frequency);
  simplistic_synchronize(ref_point);

  get_time(frequency, ref_point, file_time);
  ::FileTimeToSystemTime(&file_time, &system_time);

  return system_time.wMilliseconds;
}
/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c)2000-2002 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "Win32Timer.h"

namespace renderer {
//-------------------------------------------------------------------------
void Win32Timer::Reset() {
  Timer::Reset();
  QueryPerformanceFrequency(&frequency_);
  QueryPerformanceCounter(&start_time_);
}
//-------------------------------------------------------------------------
unsigned long Win32Timer::GetMilliseconds() {
  LARGE_INTEGER cur_time;
  LONGLONG new_ticks;

  QueryPerformanceCounter(&cur_time);

  new_ticks = (cur_time.QuadPart - start_time_.QuadPart);
  // Scale by 1000 in order to get millisecond precision
  new_ticks *= 1000;
  new_ticks /= frequency_.QuadPart;

  return (unsigned long)new_ticks;
}
}
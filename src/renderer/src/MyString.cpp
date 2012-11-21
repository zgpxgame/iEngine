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
#include "MyString.h"
#include "StringVector.h"

namespace renderer {

//-----------------------------------------------------------------------
void StringUtil::trim(String& str, bool left, bool right) {
  size_t lspaces, rspaces, len = str.length(), i;

  lspaces = rspaces = 0;

  if( left ) {
    // Find spaces / tabs on the left
    for( i = 0;
         i < len && ( str.at(i) == ' ' || str.at(i) == '\t' || str.at(i) == '\r');
         ++lspaces, ++i );
  }

  if( right && lspaces < len ) {
    // Find spaces / tabs on the right
    for( i = len - 1;
         i >= 0 && ( str.at(i) == ' ' || str.at(i) == '\t' || str.at(i) == '\r');
         rspaces++, i-- );
  }

  str = str.substr(lspaces, len-lspaces-rspaces);
}

//-----------------------------------------------------------------------
std::vector<String> StringUtil::split(const String& str, const String& delims, unsigned int maxSplits) {
  // static unsigned dl;
  std::vector<String> ret;
  unsigned int numSplits = 0;

  // Use STL methods
  size_t start, pos;
  start = 0;
  do {
    pos = str.find_first_of(delims, start);
    if (pos == start) {
      // Do nothing
      start = pos + 1;
    } else if (pos == String::npos || (maxSplits && numSplits == maxSplits)) {
      // Copy the rest of the string
      ret.push_back( str.substr(start) );
    } else {
      // Copy up to delimiter
      ret.push_back( str.substr(start, pos - start) );
      start = pos + 1;
    }
    // parse up to next real data
    start = str.find_first_not_of(delims, start);
    ++numSplits;

  } while (pos != String::npos);



  return ret;
}

//-----------------------------------------------------------------------
String StringUtil::toLowerCase(String& str) {
  std::transform(
    str.begin(),
    str.end(),
    str.begin(),
    static_cast<int(*)(int)>(::tolower) );

  return str;
}

//-----------------------------------------------------------------------
String StringUtil::toUpperCase(String& str) {
  std::transform(
    str.begin(),
    str.end(),
    str.begin(),
    static_cast<int(*)(int)>(::toupper) );

  return str;
}
}


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
#ifndef _String_H__
#define _String_H__

#include "Prerequisites.h"

// If we're using the GCC 3.1 C++ Std lib
#if defined( GCC_3_1 )

#include <ext/hash_map>
namespace __gnu_cxx {
template <> struct hash< String > {
  size_t operator()( const String& str ) const {
    register size_t ret = 0;
    for( String::const_iterator it = str.begin(); it != str.end(); ++it )
      ret = 5 * ret + *it;

    return ret;
  }
};
}

// If we're using plain vanilla VC7 Std lib
#elif !defined( _STLP_HASH_FUN_H )

#	if _DEFINE_DEPRECATED_HASH_CLASSES
namespace std
#	else
namespace stdext
#	endif
{
template<> size_t hash_compare< String, std::less< String > >::operator ()( const String& str ) const {
  register size_t ret = 0;
  for( String::const_iterator it = str.begin(); it != str.end(); ++it )
    ret = 5 * ret + *it;

  return ret;
}
}

#endif

namespace renderer {


#ifdef GCC_3_1
typedef ::__gnu_cxx::hash< String > _StringHash;
#elif !defined( _STLP_HASH_FUN_H )
#	if _DEFINE_DEPRECATED_HASH_CLASSES
typedef std::hash_compare< String, std::less< String > > _StringHash;
#	else
typedef stdext::hash_compare< String, std::less< String > > _StringHash;
#	endif
#else
typedef std::hash< String > _StringHash;
#endif

} // namespace Ogre

#endif // _String_H__

#ifndef _ZipArchiveFactory_H__
#define _ZipArchiveFactory_H__

#include "Prerequisites.h"

#include "ArchiveFactory.h"
#include "Zip.h"

namespace renderer {
/** Specialisation of ArchiveFactory for Zip files. */

class ZipArchiveFactory : public ArchiveFactory {
public:
  virtual ~ZipArchiveFactory();

  String getArchiveType(void);
  ArchiveEx *createObj( const String& name );
  String getType();
};

}

#endif

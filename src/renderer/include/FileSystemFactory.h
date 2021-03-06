#ifndef _FileSystemFactory_H__
#define _FileSystemFactory_H__

#include "ArchiveFactory.h"

namespace renderer {
/** Specialisation of ArchiveFactory for folders. */

class FileSystemFactory : public ArchiveFactory {
public:
  virtual ~FileSystemFactory();

  String getArchiveType(void);
  ArchiveEx* createArchive(const String& name);

  ArchiveEx *createObj( const String& name );
  String getType();
};

}

#endif

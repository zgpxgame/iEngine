#include "FileSystemFactory.h"
#include "FileSystem.h"

namespace renderer {

FileSystemFactory::~FileSystemFactory() {
}

//-----------------------------------------------------------------------
String FileSystemFactory::getArchiveType(void) {
  return "FileSystem";
}

//-----------------------------------------------------------------------
ArchiveEx* FileSystemFactory::createArchive(const String& name) {
  return new FileSystem(name);
}

String FileSystemFactory::getType() {
  return "FileSystem";
}

ArchiveEx *FileSystemFactory::createObj( const String& name) {
  return new FileSystem(name);
}

}

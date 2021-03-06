#ifndef __FileSystem__
#define __FileSystem__

#include "Prerequisites.h"
#include "ArchiveEx.h"

namespace renderer {

class FileSystem : public ArchiveEx {
public:
  FileSystem();
  FileSystem( const String& name );
  ~FileSystem();

  void load();
  void unload();

  bool fileOpen( const String& strFile, FILE** ppFile ) const;
  bool fileRead( const String& strFile, DataChunk** ppChunk ) const;

  bool fileSave( FILE* pFile, const String& strPath, bool bOverwrite = false );
  bool fileWrite( const DataChunk& refChunk, const String& strPath, bool bOverwrite = false );

  bool fileDele( const String& strFile );
  bool fileMove( const String& strSrc, const String& strDest, bool bOverwrite );

  bool fileInfo( const String& strFile, FileInfo** ppInfo ) const;
  bool fileCopy( const String& strSrc, const String& strDest, bool bOverwrite );

  bool fileTest( const String& strFile ) const;

  std::vector<String> dirGetFiles( const String& strDir ) const;
  std::vector<String> dirGetSubs( const String& strDir ) const;

  bool dirDele( const String& strDir, bool bRecursive );
  bool dirMove( const String& strSrc, const String& strDest, bool bOverwrite );

  bool dirInfo( const String& strDir, FileInfo** ppInfo ) const;
  bool dirCopy( const String& strSrc, const String& strDest, bool bOverwrite );

  bool dirTest( const String& strDir ) const;

  std::vector<String> getAllNamesLike( const String& strStartPath, const String& strPattern, bool bRecursive=true );

  bool _allowFileCaching() const {
    return true;
  }

private:
  void setPath() const;
  void unsetPath() const;
  void recursDeleDir( bool bRecursive );

  String mstrBasePath;
  mutable char mszTempPath[261];
};

#define retunset( a ) { unsetPath(); return a; }

}

#endif

#ifndef __FILEINDEX_
#define __FILEINDEX_

#include"file_utils.hpp"

template<typename D, typename P>
struct FileIndex{
  D dictionary;
  P postingList;

  FileIndex() = default;
  ~FileIndex() = default;
};


#endif

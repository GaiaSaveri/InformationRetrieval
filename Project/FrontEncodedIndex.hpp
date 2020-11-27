#ifndef __FRONTENCODEDINDEX_
#define __FRONTENCODEDINDEX_

#include<iostream>
#include<fstream>
#include<vector>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<utility>

#include"B+Tree/linkedlist.hpp"
#include"CompressedDictionary.hpp"


struct FrontEncodedIndex{
  CompressedDictionary d;
  std::string postName;

}

#endif

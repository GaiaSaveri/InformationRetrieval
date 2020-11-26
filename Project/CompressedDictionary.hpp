#ifndef __COMPRESSEDDICTIONARY_
#define __COMPRESSEDDICTIONARY_

#include"dictionary_compression.hpp"

struct CompressedDictionary{
  /** name of the file containing the uncompressed dictionary */
  std::string dictName;
  /** name of the file containing the compressed dictionary */
  std::string compName;
  /** block size */
  int k;
  /** last block size */
  int lastBlock;
  /** pointers to starting byte of each block */
  std::vector<int> offsets;
  /** pointer to the beginning of compressed dictionary */
  unsigned char* cdptr;

  /** default constructor */
  CompressedDictionary(){
    dictName = "dictionary.txt";
    compName = "compressed_dictionary.txt";
    k = 4;
    compressDictionary();
    cdptr = fileToDisk<unsigned char>(compName);
  }

  /** read a block of uncompressed terms */
  void readBlockUncompressed(char* &dptr, int k, std::vector<std::string>& termBlock);
  /** front coding of the block */
  void frontCodingBlock(int& currentOffset, std::vector<std::string>& block);
  /** compress the whole dictionary */
  void compressDictionary();

  /** read first term in a block **/
  std::string readFirst(unsigned char* &cdptr);
  /** find the block theoritically containing a term by binary searching the compressed dictionary */
  int findBlock(std::string& term);
  /** read a block of compressed terms */
  void readBlockCompressed(unsigned char* &cdptr, int k, std::vector<std::string>& termBlock);
  /** find a term inside the compressed dictionary */
  int findTerm(std::string& term, std::pair<int,int>& pair);

  /** destructor */
  ~CompressedDictionary() = default;
};

#include"cd_methods.hpp"

#endif

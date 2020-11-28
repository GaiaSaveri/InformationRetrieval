#ifndef __COMPRESSEDDICTIONARY_
#define __COMPRESSEDDICTIONARY_

#include"../file_utils.hpp"
#include"Dictionary.hpp"

struct CompressedDictionary : public Dictionary{
  /** name of the file containing the uncompressed dictionary */
  //std::string dictName;
  /** name of the file containing the compressed dictionary */
  std::string compName;
  /** block size */
  int k;
  /** last block size */
  int lastBlock;
  /** number of bytes before the starting byte of each block (in the compressed dictionary) */
  std::vector<int> offsets;
  /** pointer to the beginning of compressed dictionary */
  unsigned char* cdptr;

  /** default constructor */
  CompressedDictionary() : Dictionary{} {
    compName = "files/compressed_dictionary.txt";
    k = 4;
    compressDictionary(); //MAGARI METTERE UN IF SE IL FILE E' GIA' PRESENTE
    cdptr = fileToDisk<unsigned char>(compName);
  }

  /** read a block of uncompressed terms */
  void readBlockUncompressed(char* &dptr, int k, std::vector<std::string>& termBlock);
  /** front coding of the block */
  void frontCodingBlock(int& currentOffset, std::vector<std::string>& block, std::ofstream& file);
  /** compress the whole dictionary */
  void compressDictionary();

  /** read first term in a block **/
  std::string readFirst(unsigned char* &cdptr);
  /** find the block theoritically containing a term by binary searching the compressed dictionary */
  int findBlock(std::string& term);
  /** read a block of compressed terms */
  void readBlockCompressed(unsigned char* &cdptr, int k, std::vector<std::string>& termBlock);
  /** find a term inside the compressed dictionary */
  int findTerm(std::string& term, int& index) override;

  /** destructor */
  ~CompressedDictionary() = default;
};

#include"cd_methods.hpp"

#endif

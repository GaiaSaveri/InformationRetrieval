/**
 *\file CompressedDictionary.hpp
 *\author Gaia Saveri
 *\brief Class describing a compressed dictionary. It is inherited from Dictionary.
 */

#ifndef __COMPRESSEDDICTIONARY_
#define __COMPRESSEDDICTIONARY_

#include"../Indexes/file_utils.hpp"
#include"Dictionary.hpp"

struct CompressedDictionary : public Dictionary{
  /** Name of the file containing the compressed dictionary */
  std::string compName;
  /** Block size */
  int k;
  /** Last block size (if term%k!=0, then last block may have less than k terms) */
  int lastBlock;
  /** Number of bytes before the starting byte of each block (in the compressed dictionary) */
  std::vector<int> offsets;
  /** Pointer to the beginning of compressed dictionary file. */
  unsigned char* cdptr;
  /**
   *\brief Default constructor.
   *
   * If the file containing the compressed dictionary already exists (as well as the file
   * containing the offsets), then those files are read.
   * Otherwise they are created from scratch (and saved).
   */
  CompressedDictionary() : Dictionary{} {
    compName = "../files/compressed_dictionary.txt";
    std::string offFile = "../files/compressed_dict_offsets";
    k = 8;
    if(exists(compName) && exists(offFile)){ //compress dictionary already exists
      fileToVector(offFile, offsets);
      lastBlock = (terms%k == 0)? k : terms%k;
    }
    else{
      compressDictionary();
      vectorToFile(offFile, offsets);
    }
    cdptr = fileToDisk<unsigned char>(compName);
  }

  /**
   *\brief Read a block of uncompressed terms.
   *\param dptr Pointer to the beginning of the uncompressed dictionary file.
   *\param k Number of terms to read (either k or lastBlock).
   *\param termBlock Vector of strings in which the read terms will be stored.
   *
   *Starting from the beginning of the uncompressed dictionary, this function will read
   *all terms in the dictionary, block by block.
   */
  void readBlockUncompressed(char* &dptr, int k, std::vector<std::string>& termBlock);
  /**
   *\brief Front coding of a block of terms.
   *\param currentOffset Number of bytes occupied by the current block of terms
   *(needed to populate the vector offsets).
   *\param block Block of terms we are going to compress.
   *\param file File in which we will write the compressed dictionary.
   */
  void frontCodingBlock(int& currentOffset, std::vector<std::string>& block, std::ofstream& file);
  /**
   *\brief Compress the whole dictionary.
   *
   *Starting from the beginning of the file where the uncompressed dictionary is saved,
   *this function reads a block of terms, compress it and save all the required informations on
   *another file.
   */
  void compressDictionary();

  /**
   *\brief Read first term in a block.
   *\param ptr Pointer to the beginning of the block in the compressed dictionary.
   *\return std::string String in which we store the first term of the block we are reading.
   *
   *This is an auxiliary function that helps in identifying the block containing a certain term.
   */
  std::string readFirst(unsigned char* &cdptr);
  /**
   *\brief Find the block theoritically containing a term by binary searching the compressed dictionary
   *\param term Term we want to find inside the compressed dictionary.
   *\return int Index of the block (eventually) containing the block
   *(we may later discover that the term is not in the dictionary).
   */
  int findBlock(std::string& term);
  /**
   *\brief Read a block of compressed terms.
   *\param ptr Pointer to the beginning of the block in the compressed dictionary file.
   *\param k Number of terms contained in the current block of terms.
   *\param termBlock Vector of strings in which the terms contained in the block will be stored.
   */
  void readBlockCompressed(unsigned char* &cdptr, int k, std::vector<std::string>& termBlock);
  /**
   *\brief Find a term inside the compressed dictionary.
   *\param term Term that we are looking for.
   *\param index Location in the offsets vector of the starting byte of the term.
   *\return int It is set to 1 if the term has been found, -1 otherwise.
   */
  int findTerm(std::string& term, int& index) override;

  /**
   *\brief Default destructor for the compressed dictionary.
   */
  ~CompressedDictionary() = default;
};

#include"cd_methods.hpp"

#endif

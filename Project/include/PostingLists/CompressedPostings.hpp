/**
 *\file CompressedPostings.hpp
 *\author Gaia Saveri
 *\brief Class describing the compressed posting lists. It is inherited from postings.
 */

#ifndef __COMPRESSEDPOSTINGS_
#define __COMPRESSEDPOSTINGS_

#include"../Indexes/file_utils.hpp"
#include"Postings.hpp"

struct CompressedPostings : public Postings{
  /** Name of the file containing the compressed posting lists */
  std::string postCompName;
  /** Number of bytes before the first byte of each posting list in the compressed file */
  std::vector<int> compPostingOffsets;
  /** Pointer to the beginning of the compressed posting lists file */
  unsigned char* cplptr;

  /**
   *\brief Default constructor.
   *
   * If the file containing the compressed posting lists already exists (as well as the file
   * containing the offsets), then those files are read.
   * Otherwise they are created from scratch (and saved).
   */
  CompressedPostings() : Postings{}{
    postCompName = "../files/compressed_posting_list.txt";
    std::string offFile = "../files/compressed_post_offsets";
    if(exists(postCompName) && exists(offFile)){
      //file is present, only need to set the offsets
      fileToVector(offFile, compPostingOffsets);
    }
    else {
      //file not present, need to compress
      compressPostings();
      vectorToFile(offFile, compPostingOffsets);
    }
    cplptr = fileToDisk<unsigned char>(postCompName);
  }
  /**
   *\brief Function for computing the gaps in the posting list entries.
   *\param l Linked List containing the posting list we are interested in for computing the gaps.
   *\param gaps Vector of integers representing the gaps between postings.
   *
   *This function produces the gaps between postings that will be later compressed.
   */
  void computeGaps(List<int>& l, std::vector<int>& gaps);
  /**
   *\brief Compress an (unsigned) int using variable byte coding technique.
   *\param coded Vector of unsigned int containing the "encoded version" of each
   *byte of the number we want to compress.
   *\param number Number (unsigned integer) we are going to compress.
   */
  void VBencoder(std::vector<unsigned char>& coded, int number);
  /**
   *\brief Compress the whole posting lists file, using variable length encoding.
   */
  void compressPostings();

  /**
   *\brief Function for uncompressing an integer coded via variable length encoding.
   *\param ptr Pointer to the starting byte of the number we are going to uncompress
   *in the compressed posting lists file.
   */
  int VBdecoder(unsigned char* &ptr);
  /**
   *\brief Find the i-th posting list in the compressed file.
   *\param i Index of the line containing the posting list.
   *\param postings Linked List in which we will store the posting list we read.
   *
   *For further details, please refer to the same function in the Postings.hpp file.
   */
  void findPostingList(int i, List<int>& postings) override;
  /**
   *\brief Deafult destructor.
   */
  ~CompressedPostings() = default;

};

#include"cpl_methods.hpp"

#endif

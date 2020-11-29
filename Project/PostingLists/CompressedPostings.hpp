#ifndef __COMPRESSEDPOSTINGS_
#define __COMPRESSEDPOSTINGS_

#include"../file_utils.hpp"
#include"Postings.hpp"

struct CompressedPostings : public Postings{
  /** name of the file containing the compressed posting lists */
  std::string postCompName;
  /** number of bytes before the first byte of each block in the compressed file */
  std::vector<int> compPostingOffsets;
  /** pointer to the beginning of the compressed posting lists file */
  unsigned char* cplptr;

  /** default constructor */
  CompressedPostings() : Postings{}{
    postCompName = "files/compressed_posting_list.txt";
    std::string offFile = "files/compressed_post_offsets";
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

  /** compute the gaps in the posting list entries */
  void computeGaps(List<int>& l, std::vector<int>& gaps);
  /** compress an (unsigned) int using variable byte encoding */
  void VBencoder(std::vector<unsigned char>& coded, int number);
  /** compress the whole posting lists file */
  void compressPostings();

  /** uncompress an integer from VB code */
  int VBdecoder(unsigned char* &ptr);
  /** find the i-th posting list in the compressed file */
  void findPostingList(int i, List<int>& postings) override;

  /** deafult destructor */
  ~CompressedPostings() = default;

};

#include"cpl_methods.hpp"

#endif

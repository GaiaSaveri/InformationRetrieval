#ifndef __COMPRESSEDPOSTINGS_
#define __COMPRESSEDPOSTINGS_

#include"dictionary_compression.hpp"

struct CompressedPostings{
  /** name of the file containing the uncompressed posting lists */
  std::string postName; //QUESTO SARA' NEL NON COMPRESSED
  /** name of the file containing the compressed posting lists */
  std::string postCompName;
  /** number of bytes before the first byte of each block in the uncompressed file */
  std::vector<int> postOffsets; //QUESTO SARA' NEL NON COMPRESSED
  /** number of bytes before the first byte of each block in the compressed file */
  std::vector<int> compPostingOffsets;
  /** pointer to the beginning of the uncompressed posting lists file */
  char* plptr; //QUESTO SARA' NEL NON COMPRESSED
  /** pointer to the beginning of the compressed posting lists file */
  unsigned char* cplptr;

  /** default constructor */
  CompressedPostings(){
    postName = "posting_lists.txt";
    postCompName = "compressed_posting_list.txt";
    plptr = fileToDisk<char>(postName); //QUESTO SARA' NEL NON COMPRESSED
    setOffsets();//QUESTO SARA' NEL NON COMPRESSED
    compressPostings();
    cplptr = fileToDisk<unsigned char>(postCompName);
  }

  //offsets to the beginning of each line in the uncompressed file
  void setOffsets(); //QUESTO SARA' NEL NON COMPRESSED

  /** read a posting list from the uncompressed file */
  void readPostingUncompressed(char* &ptr, List<int>& postings);
  /** compute the gaps in the posting list entries */
  void computeGaps(List<int>& l, std::vector<int>& gaps);
  /** compress an (unsigned) int using variable byte encoding */
  void VBencoder(std::vector<unsigned char>& coded, int number);
  /** compress the whole posting lists file */
  void compressPostings();

  /** uncompress an integer from VB code */
  int VBdecoder(unsigned char* &ptr);
  /** find the i-th posting list in the compressed file */
  void findPostingList(int i, List<int>& postings);


};

#include"cpl_methods.hpp"

#endif

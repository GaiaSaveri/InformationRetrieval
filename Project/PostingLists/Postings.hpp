#ifndef __POSTINGS_
#define __POSTINGS_

#include"../FileIndex.hpp"

struct Postings{
  /** name of the file containing the uncompressed posting lists */
  std::string postName;
  /** pointer to the beginning of the uncompressed posting lists file */
  char* plptr;
  /** number of bytes before the first byte of each block in the uncompressed file */
  std::vector<int> postOffsets;

  Postings(){
    postName = "files/posting_lists.txt";
    plptr = fileToDisk<char>(postName);
    setOffsets();
  }

  /** set offsets to the beginning of each line in the uncompressed file */
  void setOffsets();

  /** read a posting list from the uncompressed file */
  void readPostingUncompressed(char* &ptr, List<int>& postings);
  /** find posting list stored in the i-th line of the file */
  virtual void findPostingList(int i, List<int>& postings);

  ~Postings() = default;

};

#include"p_methods.hpp"

#endif

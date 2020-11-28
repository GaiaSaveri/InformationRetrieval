#ifndef __DICTIONARY_
#define __DICTIONARY_

#include"../FileIndex.hpp"

struct Dictionary{
  /** name of the file containing the uncompressed dictionary */
  std::string dictName;
  /** pointer to the beginning of the file */
  char* dptr;
  /** number of terms in the dictionary */
  int terms;
  /** offsets in bytes to the beginning of each term in the file */
  std::vector<int> dicOffsets;

  Dictionary() {
    dictName = "files/dictionary.txt";
    dptr = fileToDisk<char>(dictName);
    terms = countLinesFile(dictName);
    setOffsets();
  }

  /** compute the offsets from the beginning of the file */
  void setOffsets();
  /** read a term, starting from dptr */
  void readTerm(std::string& term, char* &dptr);

  /** find a term by binary-searching the file */
  virtual int findTerm(std::string& term, int& index);

  ~Dictionary() = default;
};

#include"d_methods.hpp"

#endif

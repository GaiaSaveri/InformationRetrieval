/**
 *\file Dictionary.hpp
 *\author Gaia Saveri
 *\brief Class describing an uncompressed dictionary. The compressed dictionary will be inherited from this class.
 */

#ifndef __DICTIONARY_
#define __DICTIONARY_

#include"../FileIndex.hpp"
#include"../TreeIndex.hpp"
#include"../file_utils.hpp"

/**
struct TermNotFound{
  std::string message;
  TermNotFound(const std::string& m) : message{m} {}
};
*/

struct Dictionary{
  /** Name of the file containing the uncompressed dictionary */
  std::string dictName;
  /** Pointer to the beginning of the file on disk */
  char* dptr;
  /** Number of terms in the dictionary */
  int terms;
  /** Offsets in bytes to the beginning of each term in the file (the one pointed to by dptr) */
  std::vector<int> dicOffsets;
  /**
   *\brief Default constructor.
   *
   *If the file containing all the terms in the dictionary is already present, the dictionary
   *is constructed by reading that file.
   *If the file is not present, then it is built via the B+Tree
   *(this will produce also the file containing the posting lists).
   */
  Dictionary() {
    dictName = "files/dictionary.txt";
    if(!exists(dictName)){
      //build dictionary (and posting lists) using B+Tree
      TreeIndex index{};
    }
    dptr = fileToDisk<char>(dictName);
    terms = countLinesFile(dictName);
    setOffsets();
    std::string offFile = "files/dict_offsets.txt";
    vectorToFile(offFile, dicOffsets);
  }
  /**
   *\brief Compute the offset for each term from the beginning of the file pointed to by dptr.
   */
  void setOffsets();
  /**
   *\brief Read a term, starting from a pointer to a file (i.e. the dictionary file) on disk.
   *\param Term string in which the term will be stored.
   *\param ptr Pointer to the starting byte of the term we are going to read.
   */
  void readTerm(std::string& term, char* &ptr);
  /**
   *\brief Find a term by binary-searching the file.
   *\param term Term we are going to (eventually read).
   *\param index Index (in dicOffsets vector) of the starting byte of the term.
   *\return int It is 1 if the term has been found, -1 otherwise.
   */
  virtual int findTerm(std::string& term, int& index);
  /**
   *\brief Destructor for the dictionary.
   */
  ~Dictionary() = default;
};

#include"d_methods.hpp"

#endif

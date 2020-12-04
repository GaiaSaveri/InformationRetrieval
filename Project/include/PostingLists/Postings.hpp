/**
 *\file Postings.hpp
 *\author Gaia Saveri
 *\brief Class describing the uncompressed posting lists. The compressed version
 * of the posting lists will be inherited from this class.
 */

#ifndef __POSTINGS_
#define __POSTINGS_

#include"../Indexes/FileIndex.hpp"

struct Postings{
  /** Name of the file containing the uncompressed posting lists */
  std::string postName;
  /** Pointer to the beginning of the uncompressed posting lists file */
  char* plptr;
  /** Number of bytes before the first byte of each posting list in the uncompressed file */
  std::vector<int> postOffsets;
  /**
   *\brief Deafault constructor.
   *
   *We index under the hypothesis that if the file containing the dictionary is present,
   *then also the one containg the posting lists is. Since in the index (see FileIndex.hpp)
   *we build the dictionary before building the posting lists, if the file containing the dictionary
   *is not present, it is built via the B+tree, which produces the file containing the posting lists as well.
   */
  Postings(){
    postName = "files/posting_lists.txt";
    plptr = fileToDisk<char>(postName);
    setOffsets();
    std::string offFile = "files/post_offsets.txt";
    vectorToFile(offFile, postOffsets);
  }
  /**
   *\brief Set offsets to the beginning of each line (i.e. of each posting list) in the uncompressed file.
   */
  void setOffsets();
  /**
   *\brief Read a posting list from the uncompressed file.
   *\param ptr Pointer to the beginning of the posting list we are going to read
   *in the file containg the posting lists.
   *\param postings Linked List in which we will store the posting list.
   */
  void readPostingUncompressed(char* &ptr, List<int>& postings);
  /**
   *\brief Function for finding posting list stored in the i-th line of the file.
   *\param i Line of the posting lists file at which the posting list we are interested in is located.
   *\param postings Linked List in which we will store the read posting list.
   *
   *This function is coupled with the function findTerm of the dictionary.
   *The assumption is that a term and its posting list are stored in the same line of the respective files.
   *This is true in our case because we produce both the dictionary file and the posting lists file
   *using the B+tree. So we retrieve the line number via the findTerm function of the Dictionary class,
   *and we read the posting list starting in that line (thanks to the postOffsets vector).
   */
  virtual void findPostingList(int i, List<int>& postings);
  /**
   *\brief Default destructor.
   */
  ~Postings() = default;

};

#include"p_methods.hpp"

#endif

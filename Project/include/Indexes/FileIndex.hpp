/**
 *\file FileIndex.hpp
 *\author Gaia Saveri
 *\brief Class describing an inverted index built on top of a file containing
 *the (compressed) dictionary and a file containing the (compressed) posting lists.
 */

#ifndef __FILEINDEX_
#define __FILEINDEX_

#include"../B+Tree/linkedlist.hpp"
#include"../B+Tree/BPTree.hpp"

/**
 *\tparam D Type of dictionary (either Dictionary or CompressedDictionary).
 *\tparam P Type of the posting lists (either Postings or CompressedPostings).
 */
template<typename D, typename P>
struct FileIndex{
  /** Collection of all the terms inside the inverted index. */
  D dictionary;
  /** Posting lists associated to each of the terms in the dictionary. */
  P postingList;
  /**
   *\brief Default constructor.
   *
   *The dictionary is built before the posting lists, indeed in the default constructor
   *of the class Dictionary, we check if the file containg all the terms is already saved.
   *In case it isn't, it is built via the B+Tree along with the file containg the posting lists.
   *For what concerns the compressed dictionary, it is a subclass of Dictionary, so the same
   *considerations apply.
   */
  FileIndex() : dictionary{}, postingList{} {}
  /**
   *\brief Function for retrieving the posting list associated to a term.
   *\param term Term of which we want to get the associated posting list.
   *\param postings Linked List in which we store the posting list we are retrieving.
   *
   *The term is first searched in the dictionary. If found, we store the number of
   *the line in which it appears. Then, using the fact the line number in which the term
   *is stored in the dictionary file is the same line number in which its associated
   *posting list is stored in the posting lists file (true because we create those files
   * saving the B+tree's leaves), we retrieved the desired posting list.
   */
  void getPostingList(std::string& term, List<int>& postings){
    try{
      int index;
      int found = dictionary.findTerm(term, index);
      if(found!=-1) postingList.findPostingList(index, postings);
    } catch (const TermNotFound& m) {
      std::cerr<<m.message<<std::endl;
    }
  }
  /**
   *\brief Default destructor.
   */
  ~FileIndex() = default;
};

#endif

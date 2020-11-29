#ifndef __FILEINDEX_
#define __FILEINDEX_

#include"B+Tree/linkedlist.hpp"

template<typename D, typename P>
struct FileIndex{
  D dictionary;
  P postingList;

  FileIndex() : dictionary{}, postingList{} {}

  void getPostingList(std::string& term, List<int>& postings){
    int index;
    int found = dictionary.findTerm(term, index);
    if(found!=-1){
      postingList.findPostingList(index, postings);
    }
    else std::cout<<"term not found"<<std::endl;
  }

  ~FileIndex() = default;
};


#endif

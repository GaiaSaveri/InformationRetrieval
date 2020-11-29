#ifndef __IRSYSTEM_
#define __IRSYSTEM_

#include"file_utils.hpp"
#include"TreeIndex.hpp"

template<typename I>
class IRSystem{

public:
  using LinkedList = List<int>;
  template<typename IR> friend class QueryParser;

private:
  /** name of the directory where all the document are */
  std::string corpus;
  /** number of documents in the corpus */
  int maxDocID;
  /** inverted index */
  I invertedIndex;

  /** generate a list having the form [0, 1, ..., maxDocID-1], used in NOT queries */
  void generateAllList(LinkedList& all){
    //populate all
    for(int i=0; i<=this->maxDocID; i++){
      all.insert(i, method::push_back);
    }
  }

public:

  /** default constructor */
  IRSystem() : invertedIndex{} {
    corpus = "data/small/";
    maxDocID = countFiles(corpus) - 1;
  }

  /** get total number of documents in the corpus (-1) */
  int getMaxDocID(){
    return this->maxDocID;
  }

  /** answer a boolean query */
  LinkedList answer_query(std::string& query);

  /** default constructor */
  ~IRSystem() = default;
};

#include"QueryParser.hpp"

template<typename I>
typename IRSystem<I>::LinkedList IRSystem<I>::answer_query(std::string& query){
  QueryParser<I> qp{query};
  return qp.answer(*this);
}

#endif

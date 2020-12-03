/**
 *\file IRSystem.hpp
 *\author Gaia Saveri
 *\brief Class describing a Boolean Information Retrieval System.
 */

#ifndef __IRSYSTEM_
#define __IRSYSTEM_

#include"file_utils.hpp"
#include"TreeIndex.hpp"
/**
 *\tparam I Type on the inverted index (either FileIndex or TreeIndex).
 */
template<typename I>
class IRSystem{

public:
  using LinkedList = List<int>;
  //need to instantiate a query parser in ordert to answer queries.
  template<typename IR> friend class QueryParser;

private:
  /** Name of the directory where all the documents are. */
  std::string corpus;
  /** Number of documents in the corpus (actually total docs-1, for indexing purposes). */
  int maxDocID;
  /** Inverted index. */
  I invertedIndex;

public:

  /**
   *\brief Default constructor.
   */
  IRSystem() : invertedIndex{} {
    corpus = "data/documents/";
    maxDocID = countFiles(corpus) - 1;
  }
  /**
   *\brief Function used to get the total number of documents in the corpus.
   *\return int Greatest DocID.
   */
  int getMaxDocID(){
    return this->maxDocID;
  }
  /**
   *\brief Function for answering a boolean query.
   *\param query Boolean query we aim to answer.
   *\return LinkedList List of the DocIDs retrieved.
   *
   *We use the class QueryParser to analyze and answer to the query.
   */
  LinkedList answer_query(std::string& query);
  /**
   *\brief Default constructor.
   */
  ~IRSystem() = default;
};

#include"QueryParser.hpp"

template<typename I>
typename IRSystem<I>::LinkedList IRSystem<I>::answer_query(std::string& query){
  QueryParser<I> qp{query};
  return qp.answer(*this);
}

#endif

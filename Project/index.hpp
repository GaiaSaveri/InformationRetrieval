#ifndef __INDEX_
#define __INDEX_

#include<utility> //std::pair

#include"B+Tree/BPTree.hpp"
#include"document_utils.hpp"

struct Index{

  using LinkedList = List<int>;

  /** the index is a B+tree*/
  BPTree<std::string, int> index;
  /** number of documents contained in the index */
  int maxDocID;

  struct Document{
   /**
    *A document is composed by a DocID and a bag of words of terms.
    *DocID is an integer.
    *The set of terms is a vector of strings.
    */
    std::pair<int, std::vector<std::string>> doc;
    /** default constructor */
    Document() = default;
    /** custom custructor */
    Document(int DocID, std::vector<std::string>& bagOfWords){
      doc.first = DocID;
      doc.second = std::move(bagOfWords);
    }
    /** custom constructor */
    Document(int DocID, std::string& filename){
      doc.first = DocID;
      std::vector<std::string> bagOfWords;
      documentPreprocessing(bagOfWords, filename);
      doc.second = std::move(bagOfWords);
    }

    /** default destructor */
    ~Document() = default;
  };

  /** default constructor */
  Index() = default;

  /** custom constructor */
  Index(Document& d){
    for(int i=0; i<d.doc.second.size(); i++){
      index.insert(d.doc.second.at(i), d.doc.first);
    }
    maxDocID = 1;
  }

  /** custom constructor */
  Index(std::vector<std::string>& filenames){
    readFolder(filenames);
    for(int i=0; i<3; i++){
      Document d{i, filenames.at(i)};
      for(int j=0; j<d.doc.second.size(); j++)
        index.insert(d.doc.second.at(j), i);
      maxDocID = i; //DA RIMUOVERE
    }
    //maxDocID = filenames.size();
  }
  /** custom constructor */
  Index(std::string& dictionary, std::string& posting_lists){
    std::ifstream dict(dictionary);
    std::ifstream post(posting_lists);
    std::string term;
    std::string line;
    std::vector<int> posting;
    while(std::getline(dict, term) && std::getline(post, line)){
      std::istringstream is(line);
      posting = std::vector<int>(std::istream_iterator<int>(is), std::istream_iterator<int>());
      for(auto i : posting){
        index.insert(term, i);
        if(i>maxDocID)
          maxDocID = i;
      }
    }
  }

  int getMaxDocID(){
    return this->maxDocID;
  }

  void printInvertedIndex(){
    index.printLeaves();
  }

  void getPostingList(std::string& term, List<int>& postings){
    index.searchValues(term, postings);
  }

  void printDictionary(){
    index.printLevels();
  }

  void saveIndex(){
    index.writeOnFile();
  }

  void generateAllList(LinkedList& all){
    //populate all
    for(int i=0; i<=this->maxDocID; i++){
      all.insert(i, method::push_back);
    }
  }
 /**
  void andQuery(std::string& term1, std::string& term2, LinkedList& result){
    LinkedList list1{};
    LinkedList list2{};
    //list of posintgs associated to term1
    getPostingList(term1, list1);
    //list of postings associated to term2
    getPostingList(term2, list2);
    //intersection
    list1.intersection(list2, result);
  }

  void orQuery(std::string& term1, std::string& term2, LinkedList& result){
    LinkedList list1{};
    LinkedList list2{};
    //list of posintgs associated to term1
    getPostingList(term1, list1);
    //list of postings associated to term2
    getPostingList(term2, list2);
    //intersection
    list1.union_list(list2, result);
  }
  */

  void notQuery(std::string& term1, LinkedList& result){
    LinkedList all{}; //list containing all docID
    LinkedList list1{};
    //list of posintgs associated to term1
    getPostingList(term1, list1);
    //populate all
    generateAllList(all);
    //difference between all and list1
    all.difference(list1, result);
  }
/*
  void answerQueryTwoTerms(std::string& term1, std::string& term2, std::string& op, LinkedList& result){
    if(op.compare("AND") == 0){
      andQuery(term1, term2, result);
    }
    else if(op.compare("OR") == 0){
      orQuery(term1, term2, result);
    }
    else{ //"AND NOT" or "OR NOT"
      LinkedList list2{};
      notQuery(term2, list2);
      LinkedList list1{};
      getPostingList(term1, list1);
      if(op.compare("ANDNOT") == 0){
        list1.intersection(list2, result);
      }
      else if(op.compare("ORNOT") == 0){
        list1.union_list(list2, result);
      }
    }
  }
*/
  void answerQuery(LinkedList& list1, LinkedList& list2, std::string& op, LinkedList& result){
    if(op.compare("AND") == 0){
      list1.intersection(list2, result);
    }
    else if(op.compare("OR") == 0){
      list1.union_list(list2, result);
    }
    else { //"AND NOT" or "OR NOT"
      LinkedList all{};
      generateAllList(all);
      LinkedList diff{};
      all.difference(list2, diff);
      if(op.compare("ANDNOT") == 0){
        list1.intersection(diff, result);
      }
      else if(op.compare("ORNOT") == 0){
        list1.union_list(diff, result);
      }
    }
  }

  /** default destructor */
  ~Index() = default;
};

#endif

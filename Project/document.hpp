#ifndef __DOCUMENT_
#define __DOCUMENT_

#include<utility> //std::pair

#include"B+Tree/BPTree.hpp"
#include"document_utils.hpp"

struct Index{

  /** the index is a B+tree*/
  BPTree<std::string, int> index;

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
  }

  /** custom constructor */
  Index(std::vector<std::string>& filenames){
    readFolder(filenames);
    for(int i=0; i<filenames.size(); i++){
      Document d{i, filenames.at(i)};
      for(int j=0; j<d.doc.second.size(); j++)
        index.insert(d.doc.second.at(j), i);
    }
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

  /** default destructor */
  ~Index() = default;
};

#endif

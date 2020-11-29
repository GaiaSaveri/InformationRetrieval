#ifndef __TREEINDEX_
#define __TREEINDEX_

#include<utility> //std::pair

#include"B+Tree/BPTree.hpp"
#include"document_utils.hpp"

struct TreeIndex{

  using LinkedList = List<int>;

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
  TreeIndex() = default;

  /** custom constructor */
  TreeIndex(Document& d){
    for(int i=0; i<d.doc.second.size(); i++){
      index.insert(d.doc.second.at(i), d.doc.first);
    }
    maxDocID = 1;
  }

  /** custom constructor */
  TreeIndex(std::vector<std::string>& filenames){
    readFolder(filenames);
    //std::cout<<filenames.size()<<std::endl;
    for(int i=0; i<filenames.size(); i++){
      Document d{i, filenames.at(i)};
      for(int j=0; j<d.doc.second.size(); j++)
        index.insert(d.doc.second.at(j), i);
    }
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
      }
    }
  }

  void getPostingList(std::string& term, List<int>& postings){
    index.searchValues(term, postings);
  }

#ifdef DEBUG
  void printInvertedIndex(){
    index.printLeaves();
  }

  void printDictionary(){
    index.printLevels();
  }
#endif

  void saveIndex(){
    index.writeOnFile();
  }

  /** default destructor */
  ~Index() = default;
};

#endif

#ifndef __TREEINDEX_
#define __TREEINDEX_

#include"Document.hpp"
#include"B+Tree/BPTree.hpp"
#include"file_utils.hpp"


struct TreeIndex{

  using LinkedList = List<int>;

  /** the index is a B+tree*/
  BPTree<std::string, int> index;

  /** default constructor */
  TreeIndex(){
    std::string dictionary = "files/dictionary.txt";
    std::string posting_lists = "files/posting_lists.txt";
    if(exists(dictionary) && exists(posting_lists)){
      //dictionary and posting lists are already saved
      //assuming that we have both or none
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
        } }
      }
      else { //need to built the tree from scratch
        std::vector<std::string> filenames;
        std::string dirname = "data/small/";
        buildFromScratch(filenames, dirname);
        saveIndex();
      }
  }

  void buildFromScratch(std::vector<std::string>& filenames, std::string& dirname){
    readFolder(filenames, dirname);
    for(int i=0; i<filenames.size(); i++){
      Document d{i, filenames.at(i)};
      for(int j=0; j<d.doc.second.size(); j++)
        index.insert(d.doc.second.at(j), i);
    }
  }

  void getPostingList(std::string& term, List<int>& postings){
    index.searchValues(term, postings);
  }

//#ifdef DEBUG
  void printInvertedIndex(){
    index.printLeaves();
  }

  void printDictionary(){
    index.printLevels();
  }
//#endif

  void saveIndex(){
    index.writeOnFile();
  }
  /** default destructor */
  ~TreeIndex() = default;
};

#endif

/**
 *\file TreeIndex.hpp
 *\author Gaia Saveri
 *|brief Class describing an inverted index built using a B+Tree.
 */

#ifndef __TREEINDEX_
#define __TREEINDEX_

#include"Document.hpp"
#include"../B+Tree/BPTree.hpp"
#include"file_utils.hpp"

struct TreeIndex{

  using LinkedList = List<int>;

  /** The index is a B+tree, where keys are strings (terms), and values are integers (DocIDs). */
  BPTree<std::string, int> index;
  /**
   *\brief Default constructor.
   *
   *If the files containing the dictionary and the posting lists are already present,
   *it inserts each term with its associated posting list in the tree.
   *Otherwise the index is built from scratch, starting from the corpus.
   */
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
        std::string dirname = "data/documents/";
        buildFromScratch(filenames, dirname);
        saveIndex();
      }
  }
  /**
   *\brief Function to built a B+tree containing terms and posting lists starting from the corpus.
   *\param filenames Names of the files used as documents.
   *\param dirname Name of the directory containing all the documents of our collection.
   *
   *The directory is first scanned to get all the filenames. Then for each file a document is
   *instanciated and all the words (tokens) present in the file are preprocessed and
   *inserted in the tree (i.e. in the index), as well as the DocId associated to the document
   *we are scanning.
   */
  void buildFromScratch(std::vector<std::string>& filenames, std::string& dirname){
    readFolder(filenames, dirname);
    for(size_t i=0; i<filenames.size(); i++){
      Document d{static_cast<int>(i), filenames.at(i)};
      for(size_t j=0; j<d.doc.second.size(); j++)
        index.insert(d.doc.second.at(j), i);
    }
  }
  /**
   *\brief Retrieve the posting list associated to a term.
   *\param term Term of which we want to get the associated posting list.
   *\param postings Linked List containing the postings we are looking for.
   */
  void getPostingList(std::string& term, List<int>& postings){
    try {
      index.searchValues(term, postings);
    }
    catch(const TermNotFound& m){
      std::cerr<<m.message<<std::endl;
    }
  }

#ifdef DEBUG
  /**
   *\brief Function to print each term along with its associated posting list, in alphabetical order.
   */
  void printInvertedIndex(){
    index.printLeaves();
  }
  /**
   *\brief Function to print all the terms as nodes of the B+tree, to see the structure of the tree itself.
   */
  void printDictionary(){
    index.printLevels();
  }
#endif
  /**
   *\brief Function to save the dictionary and the posting lists on two separated files.
   */
  void saveIndex(){
    index.writeOnFile();
  }
  /**
   *\brief Default destructor.
   */
  ~TreeIndex() = default;
};

#endif

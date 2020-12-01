/**
 *\file Document.hpp
 *\author Gaia Saveri
 *\brief Class describing a document as composed by a DocID and a bag of words of terms.
 */

#ifndef __DOCUMENT_
#define __DOCUMENT_

#include<utility>

#include"document_utils.hpp"

struct Document{
  /** The first component of the term represents the DocID, the second the bag of word sof terms. */
  std::pair<int, std::vector<std::string>> doc;
  /**
   *\brief Default constructor.
   */
  Document() = default;
  /**
   *\brief Custom custructor.
   *\param DocID Unique integer number associated to a term.
   *\param bagOfWords Vector of strings, each representing a term in the document.
   */
  Document(int DocID, std::vector<std::string>& bagOfWords){
    doc.first = DocID;
    doc.second = std::move(bagOfWords);
  }
  /**
   *\brief Custom constructor.
   *\param DocID Unique integer number associated to a term.
   *\param filename Name of the file from which we are reading our document.
   *
   *The file is preprocessed in such a way that it is split in tokens,
   *tokens are further normalized, stop words and duplicates are removed.
   */
  Document(int DocID, std::string& filename){
    doc.first = DocID;
    std::vector<std::string> bagOfWords;
    documentPreprocessing(bagOfWords, filename);
    doc.second = std::move(bagOfWords);
  }
  /**
   *\brief Function that reads a file word by word (i.e. splitting on spaces).
   *\param words Vector of strings in which we will store the words read in the file.
   *\param filename Name of the file we are going to read.
   */
  void readFile(std::vector<std::string>& words, std::string& filename);
  /**
   *\brief Function for removing stop words.
   *\param words Vector of terms from which we want to remove stopwords.
   *\param stopwords Vector of stopwords.
   *\param fileStopWords Name of the file containing the list of stopwords.
   *
   *The file containig the stop words is provided along with the dataset we are using.
   */
  void removeStopWords(std::vector<std::string>& words, std::vector<std::string>& stopwords, std::string& fileStopWords);
  /**
   *\brief Function for removing duplicates words.
   *\param words Vector of terms from which we want to extract unique terms.
   *
   *The duplicates are removed by inserting all elements of the vector in a set, which
   *as a data structure does not allows duplicates (then copying them back to a vector).
   *Doing this we also alphabetically order the words.
   */
  void removeDuplicates(std::vector<std::string>& words);
  /**
   *\brief Preprocessing for a single document.
   *\param words Words contained in the document under analysis.
   *\param filename Name of the file containg the document we want to parse.
   *
   *Inside this function, the file is read into tokens, the tokens are normalized,
   *stop words are removed as well as duplicates and stemming is applied.
   */
  void documentPreprocessing(std::vector<std::string>& words, std::string& filename);
  /**
   *\brief Default destructor.
   */
  ~Document() = default;
};

#include"document_methods.hpp"

#endif

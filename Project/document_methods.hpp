#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
#include<cctype> //?
#include<dirent.h> //?

#include"document_utils.hpp" //normalize, stemming

//read a file word by word
void Document::readFile(std::vector<std::string>& words, std::string& filename){
  std::ifstream file;
  file.open(filename);
  std::string word;
  //>> separates on whitespace
  while(file>>word){
    words.push_back(word);
  }
}

//remove stop words
void Document::removeStopWords(std::vector<std::string>& words, std::vector<std::string>& stopwords, std::string& fileStopWords){
  //read all the stopwords inside a vector called stopwords
  readFile(stopwords, fileStopWords);
  //loop across all stopwords
  for(int i=0; i<stopwords.size(); i++){
      words.erase(std::remove(words.begin(), words.end(), stopwords.at(i)), words.end());
    }
}

//remove duplicates
void Document::removeDuplicates(std::vector<std::string>& words){
  //insert everything in a set
  //(as a side effect words are also ordered alphabetically)
  std::set<std::string> w(words.begin(), words.end());
  //then go back to vector
  words.assign(w.begin(), w.end());
}

//preprocessing for a single document
void Document::documentPreprocessing(std::vector<std::string>& words, std::string& filename){
  readFile(words, filename);
  normalize(words);
  std::string fileStopWords = "data/stopwords";
  std::vector<std::string> stopwords;
  removeStopWords(words, stopwords, fileStopWords);
  stemming(words);
  removeDuplicates(words);
}

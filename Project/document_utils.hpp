#ifndef __DOCUMENT_UTILS_
#define __DOCUMENT_UTILS_

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
#include<cctype>

#include"stemming/porter2_stemmer.h"

//read a file word by word
void readFile(std::vector<std::string>& words, std::string& filename){
  std::ifstream file;
  file.open(filename);
  std::string word;
  //>> separates on whitespace
  while(file>>word){
    words.push_back(word);
  }
}

//normalize words
void normalize(std::vector<std::string>& words){
  //words separated by - are considered two words
  //e.g. "director-general" = "director", "general"
  std::string delimiter_1 = "-";
  size_t pos_1 = 0;
  std::string tmp;
  //in words with an apostroph we delete the apostroph and everything that follows
  //e.g. "it's" = "it"
  std::string delimiter_2 = "'";
  size_t pos_2 = 0;
  for(int i=0; i<words.size(); i++){
    //detect delimiter "-" and split if necessary
    while((pos_1 = words.at(i).find(delimiter_1)) != std::string::npos){
      tmp = words.at(i).substr(0,pos_1);
      words.push_back(tmp);
      words.at(i).erase(0, pos_1+delimiter_1.length());
    }
    pos_1 = 0;
    //detect apostroph
    while((pos_2 = words.at(i).find(delimiter_2)) != std::string::npos){
      words.at(i) = words.at(i).substr(0,pos_2);
    }
    //remove every non-alphabetical character (punctuation, numbers, etc.. )
    words.at(i).erase(std::remove_if(words.at(i).begin(), words.at(i).end(),
      [](char c){return !isalpha(c);}), words.at(i).end());
    //to lower case
    std::transform(words.at(i).begin(), words.at(i).end(), words.at(i).begin(),
      [](char c){return std::tolower(c);});
  }
  //remove empty strings
  words.erase(std::remove_if(words.begin(), words.end(),
      [](std::string s){return s.size()==0;}), words.end());
}

//remove stop words
void removeStopWords(std::vector<std::string>& words, std::vector<std::string>& stopwords, std::string& fileStopWords){
  //read all the stopwords inside a vector called stopwords
  readFile(stopwords, fileStopWords);
  //loop across all stopwords
  for(int i=0; i<stopwords.size(); i++){
      words.erase(std::remove(words.begin(), words.end(), stopwords.at(i)), words.end());
    }
}

//stemming, from https://bitbucket.org/smassung/porter2_stemmer/wiki/Home
void stemming(std::vector<std::string>& words){
  for(int i=0; i<words.size(); i++){
    Porter2Stemmer::stem(words.at(i));
  }
}

//remove duplicates
void removeDuplicates(std::vector<std::string>& words){
  //insert everything in a set
  //(as a side effect words are also ordered alphabetically)
  std::set<std::string> w(words.begin(), words.end());
  //then go back to vector
  words.assign(w.begin(), w.end());
}


//preprocessing for a single document
void documentPreprocessing(std::vector<std::string>& words, std::string& filename){
  readFile(words, filename);
  normalize(words);
  std::string fileStopWords = "data/stopwords";
  std::vector<std::string> stopwords;
  removeStopWords(words, stopwords, fileStopWords);
  stemming(words);
  removeDuplicates(words);
}

#endif

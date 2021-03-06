#ifndef __DOCUMENT_UTILS_
#define __DOCUMENT_UTILS_

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cctype>
#include<dirent.h>

#include"stemming/porter2_stemmer.h"

//normalize words
void normalize(std::vector<std::string>& words){
  //words separated by - are considered two words
  //e.g. "director-general" = "director", "general"
  std::string delimiter_1 = "-";
  size_t pos_1 = 0;
  std::string tmp;
  std::string delimiter_2 = "'";
  size_t pos_2 = 0;
  for(size_t i=0; i<words.size(); i++){
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

//stemming, from https://bitbucket.org/smassung/porter2_stemmer/wiki/Home
void stemming(std::vector<std::string>& words){
  for(size_t i=0; i<words.size(); i++){
    Porter2Stemmer::stem(words.at(i));
  }
}

//read all filenames of a folder
void readFolder(std::vector<std::string>& filenames, std::string& dirname){
  DIR *dir;
  struct dirent *ent;
  std::string tmp;
  if((dir=opendir(dirname.c_str()))!=nullptr){
    while((ent = readdir(dir))!=nullptr){
        filenames.push_back(ent->d_name);
    }
    closedir(dir);
  }
  else{
    std::cout<<"not opened"<<std::endl;
  }
  filenames.erase(std::remove_if(filenames.begin(), filenames.end(),
      [](std::string s){return (s=="." || s == "..");}), filenames.end());
  //full path name to the document
  std::transform(filenames.begin(), filenames.end(), filenames.begin(),
    [dirname](std::string s){return s.insert(0,dirname);});
}

#endif

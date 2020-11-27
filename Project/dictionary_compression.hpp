#ifndef __DICTIONARY_COMPRESSION_
#define __DICTIONARY_COMPRESSION_

#include<iostream>
#include<fstream>
#include<vector>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<cmath>
#include<utility>

#include"B+Tree/linkedlist.hpp"

//---------------------------------------FILE UTILS-----------------------------------//

//get size of the file (in bytes) containing the dict
int getFileSize(std::string& filename){
  struct stat buf;
  int s = stat(filename.c_str(), &buf);
  return s == 0? buf.st_size : -1;
}

//mmap file to disk
template<class T>
T* fileToDisk(std::string& filename){
  int size = getFileSize(filename);
  int file = open(filename.c_str(), O_RDONLY, 0);
  //void* fp = mmap(NULL, size, PROT_READ, MAP_SHARED, file, 0);
  T* fp = (T*)mmap(NULL, size, PROT_READ, MAP_SHARED, file, 0);
  if(fp == MAP_FAILED) std::cout<<"mmap failed"<<std::endl;
  //T* ptr = reinterpret_cast<T*>(mmap(NULL, size, PROT_READ, MAP_FILE | MAP_SHARED, file, 0));
  return fp;
}

//get total number of terms
int countLinesFile(std::string& filename){
  int lines = 0;
  std::string line;
  std::ifstream file(filename);
  while(std::getline(file, line)) ++lines;
  return lines;
}

//---------------------------------------------------------------------------------------//

//---------------------------CONNECT COMPRESSED DICT TO LISTS-------------------------------//
/**
void setPostingOffsets(char* &plptr, std::vector<std::vector<int>>& postingOffsets, std::vector<int>& offsets, int k, int lastBlock){
  //adjust dimensions of postings offsets
  postingOffsets.resize(offsets.size());
  for(int i=0; i<postingOffsets.size(); i++){
    postingOffsets.at(i).resize(k);
    if(k!=lastBlock && i==postingOffsets.size()-1) postingOffsets.at(i).resize(lastBlock);
  }
  auto ptr = plptr;
  for(int i=0; i<postingOffsets.size(); i++){
    for(int j=0; j<postingOffsets.at(i).size(); j++){
      int k = 0;
      while(ptr[k]!='\n'){
        k++;
      }
    if(j==postingOffsets.at(i).size()-1){
      if(i!=postingOffsets.size()-1)
        postingOffsets.at(i+1).at(0) = k+1;
    }
    else postingOffsets.at(i).at(j+1) = k+1;
    ptr = &ptr[k+1];
  }
  }
  //prefix sum offset
  int cumulative = 0;
  for(int i=0; i<postingOffsets.size(); i++){
    postingOffsets.at(i).at(0) += cumulative;
    for(int j=1; j<postingOffsets.at(i).size(); j++){
      postingOffsets.at(i).at(j) += postingOffsets.at(i).at(j-1);
    }
    cumulative = postingOffsets.at(i).back();
  }
}


void readPosting(char* &ptr, List<int>& postings){
  int i = 0;
  while(ptr[i]!='\n'){
    if(ptr[i]!=' ') {
      postings.insert(ptr[i]-'0', method::push_back);
    }
    i++;
  }
}

void findPostings(std::string& term, unsigned char* &cdptr, std::vector<int>& offsets,
                  int k, int lastBlock, char* &plptr, std::vector<std::vector<int>>& postingOffsets,
                  List<int>& postings){
  //binary search throughout all the blocks
  int b = findBlock(term, cdptr, offsets);
  std::pair<int, int> pair;
  //linear scan the theoretically right block
  int t = findTerm(term, cdptr, offsets, pair, k, lastBlock);
  if(t==-1) std::cout<<"term not found"<<std::endl;
  //read the posting list file at right position
  else { //term is in dictionary
    char* postOffset = plptr + postingOffsets.at(pair.first).at(pair.second);
    readPosting(postOffset, postings);
  }
}
*/
#endif

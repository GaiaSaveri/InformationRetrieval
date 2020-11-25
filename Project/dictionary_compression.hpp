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

//read a block of uncompressed terms
void readBlockUncompressed(char* &dptr, int k, std::vector<std::string>& termBlock){
  for(int i=0; i<k; i++){
    std::string term = "";
    //read a term
    int j = 0;
    while(dptr[j]!='\n'){
      term.push_back(dptr[j]);
      j++;
    }
    //adjust ptr to file on disk
    dptr = &dptr[term.size()+1];
    termBlock.push_back(term);
  }
}

//read a block of compressed terms
void readBlockCompressed(unsigned char* &cdptr, int k, std::vector<std::string>& termBlock){
  unsigned char length; //length of the term we are reading
  unsigned char prefix; //length of the prefix
  std::string currentTerm = "";
  char* ptrCurrentTerm; //ptr to the beginning of the current term on disk
  //read one term at a time
  for(int i=0; i<k; i++){
    if(i==0){ //reading the first term of the block --> no prefix
      length = *cdptr; //current position on disk
      prefix = 0;
    }
    else{
      //move ptr to file on disk
      cdptr = reinterpret_cast<unsigned char*>(ptrCurrentTerm);
      length = *cdptr; //first byte is the length of the term
      cdptr++;
      prefix = *cdptr; //second byte is the length of the prefix
      //actual prefix
      currentTerm = termBlock.at(i-1).substr(0, prefix);
    }
    //read suffix
    cdptr++;
    ptrCurrentTerm = reinterpret_cast<char*>(cdptr);
    for(int j=0; j<length-prefix; j++){
      currentTerm.push_back(*ptrCurrentTerm);
      ptrCurrentTerm++;
    }
    termBlock.push_back(currentTerm);
    currentTerm.clear();
  }
  //update pointer to the compressed dictionary file
  cdptr = reinterpret_cast<unsigned char*>(ptrCurrentTerm);
}

//front coding of the block
void frontCodingBlock(int& currentOffset, std::vector<std::string>& block, std::string& filename){
  std::ofstream file (filename, std::ios::binary | std::ofstream::app);
  //write first term as it is, along with its length
  unsigned char length = block.at(0).length();
  file.write((const char*)&length, sizeof(unsigned char));
  file << block.at(0);
  //the length of a string is the number of actual bytes it occupies
  currentOffset = 1 + block.at(0).length();
  //write all other terms with length and length of the prefix
  for(int i=1; i<block.size(); i++){
    unsigned char prefix = 0; //length of the current prefix
    std::string suffix = ""; //suffix of the current term
    //compute prefix
    while((block.at(i-1).at(prefix)==block.at(i).at(prefix)) && prefix<block.at(i-1).size()-1){
      //it can happen that a whole word is prefix for the sequent
      //e.g. cent - central
      prefix++;
    }
    length = prefix;
    //compute suffix
    while(length<block.at(i).size()){
      suffix.push_back(block.at(i).at(length));
      length++;
    }
    //write length of the current term
    file.write((const char*)&length, sizeof(unsigned char));
    //write length of the prefix of the current term
    file.write((const char*)&prefix, sizeof(unsigned char));
    //write suffix
    file<<suffix;
    currentOffset += 2 + suffix.length();
  }
}

//compress the whole dictionary
void compressDictionary(std::string& dictName, std::string& compName, int k, std::vector<int>& offsets, int& lastBlock){
  //compute number of terms
  int terms = countLinesFile(dictName);
  //compute number of blocks
  int blocks = ceil(terms/k);
  offsets.resize(blocks);
  offsets.at(0)=0;
  //mmap dictionary
  char* dptr = fileToDisk<char>(dictName);
  std::vector<std::string> termBlock;
  //read and compress every block
  for(int i=0; i<blocks; i++){
    int currentOffset = 0;
    if(i!=blocks-1){ //we have exactly k terms per block
      readBlockUncompressed(dptr, k, termBlock);
      frontCodingBlock(currentOffset, termBlock, compName);
      offsets.at(i+1) = currentOffset;
    }
    else{ //last block may have < k terms
      int n = terms%k;
      if(n==0){
        lastBlock = k;
        readBlockUncompressed(dptr, k, termBlock);
      }
      else {
        lastBlock = n;
        readBlockUncompressed(dptr, n, termBlock);
      }
      frontCodingBlock(currentOffset, termBlock, compName);
    }
    termBlock.clear();
  }
  //prefix sum of the offsets
  for(int i=1; i<offsets.size(); i++){
    offsets.at(i) += offsets.at(i-1);
  }
}

//read first term in a block
std::string readFirst(unsigned char* &cdptr){
  //read length
  unsigned char length = *cdptr;
  cdptr++;
  std::string currentTerm = "";
  char* ptrCurrentTerm = reinterpret_cast<char*>(cdptr);
  for(int i=0; i<length; i++){
    currentTerm.push_back(*ptrCurrentTerm);
    ptrCurrentTerm++;
  }
  return currentTerm;
}

//find the block theoritically containing a term by binary searching the compressed dictionary
int findBlock(std::string& term, unsigned char* &cdptr, std::vector<int>& offsets){
  int beginning = 0;
  int end = offsets.size()-1;
  while(beginning<=end){
    int mid = (beginning+end)/2;
    unsigned char* midPtr = cdptr + offsets.at(mid);
    std::string midTerm = readFirst(midPtr);
    if(mid!=offsets.size()-1){ //if I can check mid+1
      unsigned char* midPtrOne = cdptr + offsets.at(mid+1);
      std::string midTermOne = readFirst(midPtrOne);
      if(term>=midTermOne) beginning = mid + 1;
      else if(term<midTerm) end = mid - 1;
      else return mid;
    }
    else{ //cannot check mid+1
      if(term<midTerm) end = mid-1;
      else return mid;
    }
  }
  return -1; //unsuccessful binary search
}

//find a term inside the compressed dictionary
int findTerm(std::string& term, unsigned char* &cdptr, std::vector<int>& offsets, std::pair<int,int>& pair, int k, int lastBlock){
  int b = findBlock(term, cdptr, offsets);
  if(b==-1) return b; //unsuccessful binary search
  else { //term present
    unsigned char* blockBegin = cdptr + offsets.at(b);
    std::vector<std::string> termBlock;
    if(b==offsets.size()-1){ //last block may not have k terms
      readBlockCompressed(blockBegin, lastBlock, termBlock);
    }
    else readBlockCompressed(blockBegin, k, termBlock);
    //linear scanning the block
    int i = 0;
    bool found = 0;
    while(!found && i<termBlock.size()){
      if(termBlock.at(i) == term) found = 1;
      else i++;
    }
    if(!found) return -1; //term not found
    else{
      pair = std::make_pair(b, i);
      return 1;
    }
  }
}


#endif

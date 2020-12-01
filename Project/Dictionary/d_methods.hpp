#include<iostream>
#include<fstream>
#include<vector>
#include<sys/mman.h>
#include<cmath>
#include<utility>

void Dictionary::setOffsets(){
  auto ptr = dptr;
  dicOffsets.resize(terms);
  for(int i=0; i<terms-1; i++){
    int j = 0;
    while(ptr[j]!='\n'){
      j++;
    }
    dicOffsets.at(i+1) = j + 1;
    ptr = &ptr[j+1];
  }
  //prefix sum
  for(int i=1; i<dicOffsets.size(); i++)
    dicOffsets.at(i) += dicOffsets.at(i-1);
}

void Dictionary::readTerm(std::string& term, char* &ptr){
  int j = 0;
  while(ptr[j]!='\n'){
    term.push_back(ptr[j]);
    j++;
  }
}

int Dictionary::findTerm(std::string& term, int& index){
  int beginning = 0;
  int end = dicOffsets.size() - 1;
  while(beginning<=end){
    int mid = (beginning + end)/2;
    auto ptr = dptr + dicOffsets.at(mid);
    std::string midTerm = "";
    readTerm(midTerm, ptr);
    if(term>midTerm) beginning = mid + 1;
    else if(term<midTerm) end = mid - 1;
    else{ //terms are equal
      index = mid;
      return 1; //term found
    }
  }
  return -1; //term not found
}

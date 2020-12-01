#include<iostream>
#include<fstream>
#include<vector>
#include<sys/mman.h>
#include<cmath>
#include<utility>

//offsets to the beginning of each line in the uncompressed file
void Postings::setOffsets(){
  int l = countLinesFile(postName);
  auto ptr = plptr;
  postOffsets.resize(l);
  postOffsets.at(0) = 0;
  for(int i=0; i<postOffsets.size()-1; i++){
    int k = 0;
    while(ptr[k]!='\n'){
      k++;
    }
    postOffsets.at(i+1) = k+1;
    ptr = &ptr[k+1];
  }
  //prefix sum of the offsets
  for(int i=1; i<postOffsets.size(); i++)
    postOffsets.at(i) += postOffsets.at(i-1);
}

//read a posting list from the uncompressed file
void Postings::readPostingUncompressed(char* &ptr, List<int>& postings){
  int i = 0;
  std::string s = "";
  while(ptr[i]!='\n'){
    s.clear();
    while(ptr[i]!=' '){
      s.append(std::to_string(ptr[i]-'0'));
      i++;
    }
    postings.insert(std::stoi(s), method::push_back);
    i++;
  }
}

//read a posting list locted at i-th position in the file
void Postings::findPostingList(int i, List<int>& postings){
  auto ptr = plptr + postOffsets.at(i);
  readPostingUncompressed(ptr, postings);
}

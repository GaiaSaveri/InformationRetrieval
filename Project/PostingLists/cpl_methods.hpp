#include<iostream>
#include<fstream>
#include<vector>
#include<sys/mman.h>
#include<cmath>
#include<utility>

//--------------------------- COMPRESSION METHODS ----------------------------//

/**
//read a posting list from the uncompressed file
void CompressedPostings::readPostingUncompressed(char* &ptr, List<int>& postings){
  int i = 0;
  while(ptr[i]!='\n'){
    if(ptr[i]!=' ') {
      postings.insert(ptr[i]-'0', method::push_back);
    }
    i++;
  }
}
*/

//compute the gaps in the posting list entries
void CompressedPostings::computeGaps(List<int>& l, std::vector<int>& gaps){
  //reverse list in a vector
  for(auto x : l) gaps.push_back(x);
  //compute gaps
  for(int i=gaps.size()-1; i>0; i--){
    gaps.at(i) -= gaps.at(i-1);
  }
}

//compress an (unsigned) int using variable byte encoding
void CompressedPostings::VBencoder(std::vector<unsigned char>& coded, int number){
  while(number > 0x7f){ //>127-->need more than one byte
    //take only the least significant byte
    auto x = (number & 0x7f); //0x7f = 1111111 = 127
    //add continuation bit
    x |= 0x80; //0x80 = 10000000 = 128
    //insert byte in the vector
    coded.push_back(x);
    //advance
    number >>= 7;
  }
  //insert last byte with continuation bit 0
  coded.push_back(number & 0x7f);
}

//compress the whole file
void CompressedPostings::compressPostings(){
  //std::ofstream file(filename, std::ios::binary | std::ofstream::app);
  std::ofstream file(postCompName, std::ios::binary | std::ofstream::app);
  int n = postOffsets.size(); //total number of posting lists
  //one more than total number of postings
  compPostingOffsets.resize(n+1);
  //read compress and write each posting list
  for(int i=0; i<n; i++){
    //set corrent pointer to file
    auto ptr = plptr + postOffsets.at(i);
    //read into list
    List<int> l{};
    Postings::readPostingUncompressed(ptr, l);
    //compute gaps
    std::vector<int> gaps;
    computeGaps(l, gaps);
    int currentOffset = 0;
    //encode gaps
    for(int j=0; j<gaps.size(); j++){
      std::vector<unsigned char> coded;
      VBencoder(coded, gaps.at(j));
      currentOffset += coded.size(); //byte needed for the i-th gap
      for(int k=0; k<coded.size(); k++)
        file.write((char*)&coded.at(k), sizeof(unsigned char));
    }
    //number of bytes occupied by the i-th posting list
    compPostingOffsets.at(i+1) = currentOffset;
  }
  //prefix sum of compPostingOffsets
  for(int i=1; i<compPostingOffsets.size(); i++){
    compPostingOffsets.at(i) += compPostingOffsets.at(i-1);
  }
  file.close();
}

//------------------------------- FIND METHODS -------------------------------//

//reconstruct int from VB encoding
int CompressedPostings::VBdecoder(unsigned char* &ptr){
  int shift = 0;
  int number = 0;
  int currentByte = *ptr;
  //read first byte
  number |= (currentByte & 0x7f);
  shift += 7;
  ptr++;
  //read other bytes (if any)
  while(currentByte & 0x80){ //as long as the continuation byte is 1
    currentByte = *ptr; //advance pointer
    //read the part we are interested in
    number |= (currentByte & 0x7f) << shift;
    shift += 7;
    ptr++;
  }
  return number;
}

//read the i-th posting
void CompressedPostings::findPostingList(int i, List<int>& postings){
  //set pointer to beginning of i-th list
  auto begin = cplptr + compPostingOffsets.at(i);
  //number of bytes of the i-th list
  int n = compPostingOffsets.at(i+1) - compPostingOffsets.at(i);
  auto end = begin + n;
  //start reading
  std::vector<int> gaps;
  while(begin!=end){
    int current = VBdecoder(begin);
    gaps.push_back(current);
  }
  postings.insert(gaps.at(0), method::push_back);
  for(int i=1; i<gaps.size(); i++){
    gaps.at(i) += gaps.at(i-1);
    postings.insert(gaps.at(i), method::push_back);
  }
}

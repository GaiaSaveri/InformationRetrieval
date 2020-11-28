#include<iostream>
#include<fstream>
#include<vector>
#include<sys/mman.h>
#include<cmath>
#include<utility>

//--------------------------- COMPRESSION METHODS ----------------------------//

//read a block of uncompressed terms
void CompressedDictionary::readBlockUncompressed(char* &dptr, int k, std::vector<std::string>& termBlock){
  for(int i=0; i<k; i++){
    std::string term = "";
    Dictionary::readTerm(term, dptr);
    //adjust ptr to file on disk
    dptr = &dptr[term.size()+1];
    termBlock.push_back(term);
  }
}

//compress a block of terms using front coding
void CompressedDictionary::frontCodingBlock(int& currentOffset, std::vector<std::string>& block, std::ofstream& file){
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
void CompressedDictionary::compressDictionary(){
  std::ofstream file (compName, std::ios::binary | std::ofstream::app);
  //compute number of blocks
  int blocks = terms/k + ((terms%k==0)? 0 : 1);
  offsets.resize(blocks);
  offsets.at(0)=0;
  std::vector<std::string> termBlock;
  //read and compress every block
  for(int i=0; i<blocks; i++){
    int currentOffset = 0;
    if(i!=blocks-1){ //we have exactly k terms per block
      readBlockUncompressed(dptr, k, termBlock);
      frontCodingBlock(currentOffset, termBlock, file);
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
      frontCodingBlock(currentOffset, termBlock, file);
    }
    termBlock.clear();
  }
  //prefix sum of the offsets
  for(int i=1; i<offsets.size(); i++){
    offsets.at(i) += offsets.at(i-1);
  }
  file.close();
}

//------------------------------- FIND METHODS -------------------------------//

//read first term of a block
std::string CompressedDictionary::readFirst(unsigned char* &cdptr){
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

//find the block that should contain the term
int CompressedDictionary::findBlock(std::string& term){
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

//read a block of compressed terms
void CompressedDictionary::readBlockCompressed(unsigned char* &cdptr, int k, std::vector<std::string>& termBlock){
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

//find the term (if present) inside the compressed dictionary
int CompressedDictionary::findTerm(std::string& term, int& index){
  int b = findBlock(term);
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
      index = b*termBlock.size() + i;
      return 1;
    }
  }
}

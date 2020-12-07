#include<chrono>

#include"Indexes/FileIndex.hpp"
#include"Indexes/TreeIndex.hpp"
#include"Dictionary/CompressedDictionary.hpp"
#include"PostingLists/CompressedPostings.hpp"
#include"IR/IRSystem.hpp"

template<typename T>
void ask_and_answer(T& ir){
  std::string query;
  std::cout<<"Please insert your boolean query:"<<std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  getline(std::cin, query);
  std::cout<<"Answering to query..."<<std::endl;
  auto begin = std::chrono::high_resolution_clock::now();
  List<int> l = ir.answer_query(query);
  auto end = std::chrono::high_resolution_clock::now();
  auto total = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
  std::cout<<"Query answered in "<<total<<" microseconds"<<std::endl;
}


int main(){
  int n;
  std::cout<<"Please choose the index you wish to test: "<<std::endl;
  std::cout<<"\t1. Tree index (dictionary is a B+Tree in memory)"<<std::endl;
  std::cout<<"\t2. File index (dictionary is a file on disk)"<<std::endl;
  std::cout<<"Please type 1 or 2: "<<std::endl;
  std::cin >> n;
  if(n == 1){
    std::cout<<"Building the inverted index..."<<std::endl;
    IRSystem<TreeIndex> ir{};
    std::cout<<"Index built"<<std::endl;
    ask_and_answer<IRSystem<TreeIndex>>(ir);
  }
  else if(n == 2){
    int m;
    int w;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"Please choose the kind of dictionary: "<<std::endl;
    std::cout<<"\t1. Non-compressed Dictionary"<<std::endl;
    std::cout<<"\t2. Compressed Dictionary"<<std::endl;
    std::cout<<"Please type 1 or 2: "<<std::endl;
    std::cin >> m;
    std::cout<<"Please choose the kind of posting lists: "<<std::endl;
    std::cout<<"\t1. Non-compressed Posting Lists"<<std::endl;
    std::cout<<"\t2. Compressed Posting Lists"<<std::endl;
    std::cout<<"Please type 1 or 2: "<<std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin >> w;
    if(m == 1){
      if(w == 1){
        std::cout<<"Building the inverted index..."<<std::endl;
        IRSystem<FileIndex<Dictionary, Postings>> ir{};
        std::cout<<"Index built"<<std::endl;
        ask_and_answer<IRSystem<FileIndex<Dictionary, Postings>>>(ir);
      }
      else if(w == 2){
        std::cout<<"Building the inverted index..."<<std::endl;
        IRSystem<FileIndex<Dictionary, CompressedPostings>> ir{};
        std::cout<<"Index built"<<std::endl;
        ask_and_answer<IRSystem<FileIndex<Dictionary, CompressedPostings>>>(ir);
      }
      else {
        std::cout<<"Wrong input, exiting..."<<std::endl;
        return -1;
      }
    }
    else if(m == 2){
      if(w == 1){
        std::cout<<"Building the inverted index..."<<std::endl;
        IRSystem<FileIndex<CompressedDictionary, Postings>> ir{};
        std::cout<<"Index built"<<std::endl;
        ask_and_answer<IRSystem<FileIndex<CompressedDictionary, Postings>>>(ir);
      }
      else if(w == 2){
        std::cout<<"Building the inverted index..."<<std::endl;
        IRSystem<FileIndex<CompressedDictionary, CompressedPostings>> ir{};
        std::cout<<"Index built"<<std::endl;
        ask_and_answer<IRSystem<FileIndex<CompressedDictionary, CompressedPostings>>>(ir);
      }
      else {
        std::cout<<"Wrong input, exiting..."<<std::endl;
        return -1;
      }
    }
    else {
      std::cout<<"Wrong input, exiting..."<<std::endl;
      return -1;
    }
    }
    else {
      std::cout<<"Wrong input, exiting..."<<std::endl;
      return -1;
    }
    return 0;
}

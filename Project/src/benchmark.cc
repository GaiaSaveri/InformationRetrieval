#include<chrono>

#include"Indexes/FileIndex.hpp"
#include"Indexes/TreeIndex.hpp"
#include"Dictionary/CompressedDictionary.hpp"
#include"PostingLists/CompressedPostings.hpp"
#include"IR/IRSystem.hpp"
#include"Indexes/file_utils.hpp"
#include"IR/QueryParser.hpp"

//reported here for convenience
void readTerm(std::string& term, char* &dptr){
  int j = 0;
  while(dptr[j]!='\n'){
    term.push_back(dptr[j]);
    j++;
  }
}

int main(){
  //std::ifstream file("../../files/dictionary.txt");
  //std::string line;
  //while(std::getline(file, line)){
    //std::cout<<line<<std::endl;
  //}
  //std::string file = "files/dictionary.txt";
  //std::cout<<exists(file)<<std::endl;
  IRSystem<FileIndex<CompressedDictionary, CompressedPostings>> ir{};

  /** testing time to retrieve terms and posting lists */

  std::string dicName = "files/dictionary.txt";
  char* dptr = fileToDisk<char>(dicName);
  std::vector<int> off;
  std::string offName = "files/dict_offsets.txt";
  fileToVector(offName, off);

  std::ofstream file1; //time to retrieve terms
  std::ofstream file2; //time to retrieve postings

  FileIndex<CompressedDictionary, Postings> index1{};

  //5 runs for each test, to average results
  for(int i=1; i<=5; i++){
    //file1.open("test/compress_24_dict_" + std::to_string(i)+".txt");
    //file2.open("test/compress_post_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      double total2 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        std::cout<<term<<std::endl;
        int index;
        auto begin1 = std::chrono::high_resolution_clock::now();
        index1.dictionary.findTerm(term, index);
        auto end1 = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count();
        List<int> p;
        auto begin2 = std::chrono::high_resolution_clock::now();
        index1.postingList.findPostingList(index, p);
        auto end2 = std::chrono::high_resolution_clock::now();
        total2 += std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count();
      }
      //file1 << total1/n << "\n";
      //file2 << total2/n << "\n";
    }
    //file1.close();
    //file2.close();
  }

/**
  FileIndex<Dictionary, Postings> index2{};
  for(int i=1; i<=5; i++){
    file1.open("test/dict_" + std::to_string(i)+".txt");
    file2.open("test/post_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      double total2 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        int index;
        auto begin1 = std::chrono::high_resolution_clock::now();
        index2.dictionary.findTerm(term, index);
        auto end1 = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count();
        List<int> p;
        auto begin2 = std::chrono::high_resolution_clock::now();
        index2.postingList.findPostingList(index, p);
        auto end2 = std::chrono::high_resolution_clock::now();
        total2 += std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count();
      }
      file1 << total1/n << "\n";
      file2 << total2/n << "\n";
    }
    file1.close();
    file2.close();
  }


  TreeIndex index3{};
  //5 runs for each test, to average results
  for(int i=1; i<=5; i++){
    file1.open("test/tree_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        List<int> p;
        auto begin = std::chrono::high_resolution_clock::now();
        index3.getPostingList(term, p);
        auto end = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
      }
      file1 << total1/n << "\n";
    }
    file1.close();
  }
*/
}

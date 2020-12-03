#include<chrono>

#include"../include/Indexes/FileIndex.hpp"
#include"../include/Indexes/TreeIndex.hpp"
#include"../include/Dictionary/CompressedDictionary.hpp"
#include"../include/PostingLists/CompressedPostings.hpp"
#include"../include/IR/IRSystem.hpp"
#include"../include/Indexes/file_utils.hpp"
#include"../include/IR/QueryParser.hpp"



int main(){

  IRSystem<FileIndex<Dictionary, Postings>> ir1{};
  IRSystem<FileIndex<CompressedDictionary, Postings>> ir2{};
  IRSystem<FileIndex<Dictionary, CompressedPostings>> ir3{};
  IRSystem<FileIndex<CompressedDictionary, CompressedPostings>> ir4{};
  IRSystem<TreeIndex> ir5{};

  /* correctness test for simple query */

  std::ifstream file("../data/simple_queries.txt");
  std::string line;
  int i = 1;
  double total1 = 0;
  std::vector<int> v1;
  double total2 = 0;
  std::vector<int> v2;
  double total3 = 0;
  std::vector<int> v3;
  double total4 = 0;
  std::vector<int> v4;
  double total5 = 0;
  std::vector<int> v5;

  while(std::getline(file, line)){
    auto begin1 = std::chrono::high_resolution_clock::now();
    List<int> l1 = ir1.answer_query(line);
    auto end1 = std::chrono::high_resolution_clock::now();
    total1 += std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count();
    for(auto x : l1) v1.push_back(x);
    auto begin2 = std::chrono::high_resolution_clock::now();
    List<int> l2 = ir2.answer_query(line);
    auto end2 = std::chrono::high_resolution_clock::now();
    total2 += std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count();
    for(auto x : l2) v2.push_back(x);
    auto begin3 = std::chrono::high_resolution_clock::now();
    List<int> l3 = ir3.answer_query(line);
    auto end3 = std::chrono::high_resolution_clock::now();
    total3 += std::chrono::duration_cast<std::chrono::microseconds>(end3-begin3).count();
    for(auto x : l3) v3.push_back(x);
    auto begin4 = std::chrono::high_resolution_clock::now();
    List<int> l4 = ir4.answer_query(line);
    auto end4 = std::chrono::high_resolution_clock::now();
    total4 += std::chrono::duration_cast<std::chrono::microseconds>(end4-begin4).count();
    for(auto x : l4) v4.push_back(x);
    auto begin5 = std::chrono::high_resolution_clock::now();
    List<int> l5 = ir5.answer_query(line);
    auto end5 = std::chrono::high_resolution_clock::now();
    total5 += std::chrono::duration_cast<std::chrono::microseconds>(end5-begin5).count();
    for(auto x : l5) v5.push_back(x);
    v1.clear();
    v2.clear();
    v3.clear();
    v4.clear();
    v5.clear();
    if(v1==v2 && v2==v3 && v3==v4 && v4==v5){
      std::cout<<"IR systems agree on query: "<<line<<std::endl;
    }
    else {
      std::cout<<"Discrepancy"<<std::endl;
      i = 0;
    }
  }
  std::cout<<"\n"<<i<<std::endl;
  std::cout<<"\n"<<std::endl;
  std::cout<<total1/(double)1000<<std::endl;
  std::cout<<total2/(double)1000<<std::endl;
  std::cout<<total3/(double)1000<<std::endl;
  std::cout<<total4/(double)1000<<std::endl;
  std::cout<<total5/(double)1000<<std::endl;

  /** correctness test for complex queries */
/**
  std::ifstream file1("../data/complex_queries.txt");
  i = 1;
  while(std::getline(file1, line)){
    List<int> l1 = ir1.answer_query(line);
    for(auto x : l1) v1.push_back(x);
    List<int> l2 = ir2.answer_query(line);
    for(auto x : l2) v2.push_back(x);
    List<int> l3 = ir3.answer_query(line);
    for(auto x : l3) v3.push_back(x);
    List<int> l4 = ir4.answer_query(line);
    for(auto x : l4) v4.push_back(x);
    List<int> l5 = ir5.answer_query(line);
    for(auto x : l5) v5.push_back(x);
    v1.clear();
    v2.clear();
    v3.clear();
    v4.clear();
    v5.clear();
    if(v1==v2 && v2==v3 && v3==v4 && v4==v5){
      std::cout<<"IR systems agree on query: "<<"\n"<<line<<std::endl;
    }
    else {
      std::cout<<"Discrepancy"<<std::endl;
      i = 0;
    }
  }

  std::cout<<"\n"<<i<<std::endl;
*/
}

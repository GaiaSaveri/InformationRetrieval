#include<chrono>

#include"FileIndex.hpp"
#include"TreeIndex.hpp"
#include"Dictionary/CompressedDictionary.hpp"
#include"PostingLists/CompressedPostings.hpp"
#include"IRSystem.hpp"
#include"file_utils.hpp"
#include"QueryParser.hpp"


int main(){

  IRSystem<FileIndex<CompressedDictionary, Postings>> ir1{};
  //IRSystem<FileIndex<CompressedDictionary, Postings>> ir2{};
  //IRSystem<FileIndex<Dictionary, CompressedPostings>> ir3{};
  //IRSystem<FileIndex<CompressedDictionary, CompressedPostings>> ir4{};
  //IRSystem<TreeIndex> ir5;

  /* correctness test for simple query */

  //std::ifstream file("data/simple_queries.txt");
  //std::string line;
  //int i = 1;
  //while(std::getline(file, line)){
  std::string query = "ciao";

    std::cout<<query<<std::endl;
    List<int> l1 = ir1.answer_query(query);
    //std::cout<<l1<<std::endl;

    /**
    std::vector<int> v1;
    l1.listToVector(v1);
    List<int> l2 = ir2.answer_query(line);
    std::vector<int> v2;
    l1.listToVector(v2);
    List<int> l3 = ir3.answer_query(line);
    std::vector<int> v3;
    l1.listToVector(v3);
    List<int> l4 = ir4.answer_query(line);
    std::vector<int> v4;
    l1.listToVector(v4);
    List<int> l5 = ir5.answer_query(line);
    std::vector<int> v5;
    l1.listToVector(v5);
    if(v1==v2 && v2==v3 && v3==v4 && v4==v5){
      std::cout<<"All IR systems agree on query: "<<line<<std::endl;
    }
    else {
      std::cout<<"discrepancy"<<std::endl;
      i = 0;
    }
    */
  //}




  /** answer simple queries */

}

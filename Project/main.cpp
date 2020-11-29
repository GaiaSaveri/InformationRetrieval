#include"FileIndex.hpp"
#include"TreeIndex.hpp"
#include"Dictionary/CompressedDictionary.hpp"
#include"PostingLists/CompressedPostings.hpp"
#include"IRSystem.hpp"
#include"file_utils.hpp"
#include"QueryParser.hpp"

int main(){
  /**
  FileIndex<CompressedDictionary, CompressedPostings> index{};
  std::string term = "acceler";
  List<int> postings;
  index.getPostingList(term, postings);
  std::cout<<postings<<std::endl;
  */
  //std::string dirname = "data/small/";
  //int i = countFiles(dirname);
  //std::cout<<i<<std::endl;
  IRSystem<TreeIndex> ir{};
  //List<int> p;
  //std::string q = "maintain";
  //ir.invertedIndex.getPostingList(q, p);
  //std::cout<<p<<std::endl;
  std::string query = "pct OR (maintain AND hold)";
  List<int> result = ir.answer_query(query);
  std::cout<<result<<std::endl;
}

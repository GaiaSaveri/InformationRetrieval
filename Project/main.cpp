#include"FileIndex.hpp"
#include"CompressedDictionary.hpp"
#include"CompressedPostings.hpp"

int main(){
  //FileIndex<CompressedDictionary, CompressedPostings> index{};
  //Dictionary d{};
  //std::string term = "virtual";
  //int index;
  //int found = d.findTerm(term, index);
  //std::cout<<"found? "<<found<<std::endl;
  //std::cout<<index<<std::endl;
  //for(int i=0; i<10; i++) std::cout<<d.dicOffsets.at(i)<<std::endl;
  Postings p{};
  List<int> post;
  p.findPostingList(582, post);
  std::cout<<post<<std::endl;
}

#include"document.hpp"

int main(){

  //std::vector<std::string> filenames;

  //Index i{filenames};
  //i.printInvertedIndex();
  //List<int> postings;
  //std::string term = "see";
  //i.getPostingList(term, postings);
  //std::cout<<postings<<std::endl;
  //i.printDictionary();

  BPTree<int, int> btree{};
  for(int i=0; i<100; i++){
    btree.insert(i,i);
    btree.insert(i,i+1);
    btree.insert(i,i+2);
    btree.insert(i, i+3);
  }

  btree.printLeaves();

}

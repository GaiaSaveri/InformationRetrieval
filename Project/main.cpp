#include"document.hpp"

#include<stdlib.h>
#include<time.h>

int main(){


  //std::vector<std::string> filenames;

  //Index i{filenames};
  //i.printInvertedIndex();
  //List<int> postings;
  //std::string term = "year";
  //i.getPostingList(term, postings);
  //std::cout<<postings<<std::endl;
  //i.printDictionary();


  srand(time(NULL));
  BPTree<int, int> btree{};
  for(int i=0; i<100; i++){
    btree.insert(rand()% 100 + 1,i);
    //btree.insert(i,i+1);
    //btree.insert(i,i+2);
    //btree.insert(i, i+3);
  }
  //btree.insert(101, 2);
  btree.printLeaves();


/**
  //List<int> p;
  //p.isEmpty();
  for(int i=0; i<10; i++){
    List<int> tmp;
    //stmp.isEmpty();
    btree.searchValues(i, tmp);
    //p.push_back(tmp);
    tmp.isEmpty();
  }
  //std::cout<<p.size()<<std::endl;
*/
}

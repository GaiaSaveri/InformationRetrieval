#include"BPTree.hpp"

int main(){
  /** testing basic implementation of B+Tree */

  BPTree<int, int> btree; //default constructor
  node<int, int> * found = nullptr;
  for(int i=0; i<1000; i++){
    btree.insert(i,i);
  }


  for(int i=0; i<1000; i++){
    std::cout<<i<<" ";
    found = btree.searchLeaf(i);
  }

//btree.insert(25,25);
//found = btree.searchLeaf(18);

//found = btree.searchLeaf(18);

  //btree.printLeaves();
  //btree.printLevels();


  btree.clear();
}

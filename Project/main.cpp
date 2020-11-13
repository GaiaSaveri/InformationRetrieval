#include"BPTree.hpp"

int main(){
  /** testing basic implementation of B+Tree */

  BPTree<int, int> btree; //default constructor

  for(int i=0; i<100; i++){
    btree.insert(i,i);
  }

  btree.printLeaves();
  //std::cout<<"uscito stampa";


  //btree.clear();
  return 0;
}

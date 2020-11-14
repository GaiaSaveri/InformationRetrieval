#include"BPTree.hpp"

int main(){

node<int,int>* found;

  BPTree<int, int> btree{};
  for(int i=0; i<20; i++){
    btree.insert(i,i);
  }

  for(int i=0; i<10; i++){
    btree.insert(i,i+1);
  }


  for(int i=0; i<50; i++){
    found = btree.searchLeaf(i);
  }

  btree.printLeaves();

/**
  std::vector<List<int>> v;
  List<int> l;
  l.insert(3, method::push_back);
  v.push_back(l);
  List<int> ll;
  ll.insert(4, method::push_back);
  List<int> l1;
  l1.insert(5, method::push_back);
  List<int> l2;
  l2.insert(6, method::push_back);

  v.push_back(ll);
  v.push_back(l1);
  v.push_back(l2);

  for(int i=0; i<v.size(); i++){
    std::cout<<v.at(i)<<std::endl;
  }

  std::vector<List<int>> u;

  for(int i=0; i<v.size(); i++){
    //u.at(i)=List<int>(v.at(i));
    u.push_back(v.at(i));
  }

  for(int i=0; i<u.size(); i++){
    std::cout<<u.at(i)<<std::endl;
  }

*/



}

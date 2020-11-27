#include"CompressedPostings.hpp"


int main(){
  //------------------ TESTING CLASS --------------//
  remove("compressed_posting_list.txt");
  CompressedPostings p{};
  List<int> pos;
  p.findPostingList(582, pos);
  std::cout<<pos<<std::endl;

}

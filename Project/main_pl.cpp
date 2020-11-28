#include"PostingLists/CompressedPostings.hpp"


int main(){
  //------------------ TESTING CLASS --------------//
  remove("files/compressed_posting_list.txt");
  CompressedPostings p{};
  List<int> pos;
  p.findPostingList(377, pos);
  std::cout<<pos<<std::endl;

}

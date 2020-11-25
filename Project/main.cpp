#include"dictionary_compression.hpp"
#include"B+Tree/linkedlist.hpp"

int main(){
  std::string dictName = "dictionary.txt";
  std::string compName = "compressed_dictionary.txt";
  remove("compressed_dictionary.txt");
  //starting byte of each block
  std::vector<int> offsets;
  int lastBlock;
  int k = 4;
  compressDictionary(dictName, compName, 4, offsets, lastBlock);
  //ptr to the beginning of compressed dictionary
  unsigned char* cdptr = fileToDisk<unsigned char>(compName);
  std::string term = "pct";
  //int b = findBlock(term, cdptr, offsets);
  //std::cout<<b<<std::endl;
  //std::pair<int, int> pair;
  //int t = findTerm(term, cdptr, offsets, pair, 4, lastBlock);
  //std::cout<<"found? "<< t <<std::endl;
  //std::cout<<pair.first<<" "<<pair.second<<std::endl;
  //line is ((first*k)+second)+1
  //-----------------QUI HO COMPRESSO E TROVATO IL TERM-----------------------//

  std::string postName = "posting_lists.txt";
  char* plptr = fileToDisk<char>(postName);
  std::vector<std::vector<int>> postingOffsets;
  setPostingOffsets(plptr, postingOffsets, offsets, k, lastBlock);

  //std::cout<<postingOffsets.at(9).back()<<std::endl;
  //for(auto x : postingOffsets.at(1)) std::cout<<x<<std::endl;
  //plptr = fileToDisk<char>(postName);
  List<int> postings;
  findPostings(term, cdptr, offsets, k, lastBlock, plptr, postingOffsets, postings);
  std::cout<<postings<<std::endl;
  //std::cout<<plptr[110]<<std::endl;






  /**
  for(int i=0; i<offsets.size(); i++){
    std::vector<std::string> termBlock;
    readBlockCompressed(cdptr, 4, termBlock);
    for(auto x : termBlock) std::cout<<x<<std::endl;
    termBlock.clear();
  }
  */
}

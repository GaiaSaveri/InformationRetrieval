#include<chrono>

#include"FileIndex.hpp"
#include"TreeIndex.hpp"
#include"Dictionary/CompressedDictionary.hpp"
#include"PostingLists/CompressedPostings.hpp"
#include"IRSystem.hpp"
#include"file_utils.hpp"
#include"QueryParser.hpp"

/**
template<class T>
void test(std::ofstream& file, T& B, std::vector<V>& v)
{
  for(int nsearch=50; nsearch<N; nsearch+=50)
    {
      auto begin =std::chrono::high_resolution_clock::now();
      for (int i=0u; i<nsearch; ++i)
	       B.find(v[i]);
      auto end = std::chrono::high_resolution_clock::now();
      auto total = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
      file << total/double(nsearch) << "\n";
    }
}
*/

void readTerm(std::string& term, char* &dptr){
  int j = 0;
  while(dptr[j]!='\n'){
    term.push_back(dptr[j]);
    j++;
  }
}

int main(){
  //std::cout<<s<<std::endl;
  //int n = std::stoi(s);
  //std::cout<<n<<std::endl;
    //ints.push_back(y);
    //i++;
  //}
  //for(auto z:ints) std::cout<<z<<std::endl;
    //if(ptr[i]!=' ') {
    //ints.push_back(ptr[3]-'0');
    //ptr += sizeof(int);
  //}

  //}
  //IRSystem<FileIndex<CompressedDictionary, CompressedPostings>> ir{};
  //std::string query = "pct OR (maintain AND hold)";
  //List<int> result = ir.answer_query(query);
  //std::cout<<result<<std::endl;

  /** testing time to retrieve terms and posting lists */

  std::string dicName = "files/dictionary.txt";
  char* dptr = fileToDisk<char>(dicName);
  std::vector<int> off;
  std::string offName = "files/dict_offsets.txt";
  //double total = 5;
  //auto begin =std::chrono::high_resolution_clock::now();
  fileToVector(offName, off);
  //auto end = std::chrono::high_resolution_clock::now();
  //total = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
  //std::ofstream file;
  //file.open("boh.txt");
  //file << total << "\n";

  std::ofstream file1; //time to retrieve terms
  std::ofstream file2; //time to retrieve postings
  FileIndex<CompressedDictionary, Postings> index1{};

  //3 runs for each test, to average results

  for(int i=1; i<=5; i++){
    file1.open("test/compress_dict_" + std::to_string(i)+".txt");
    file2.open("test/compress_post_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      double total2 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        int index;
        auto begin1 = std::chrono::high_resolution_clock::now();
        index1.dictionary.findTerm(term, index);
        auto end1 = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count();
        List<int> p;
        auto begin2 = std::chrono::high_resolution_clock::now();
        index1.postingList.findPostingList(index, p);
        auto end2 = std::chrono::high_resolution_clock::now();
        total2 += std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count();
      }
      file1 << total1/n << "\n";
      file2 << total2/n << "\n";
    }
    file1.close();
    file2.close();
  }
/**

  FileIndex<Dictionary, Postings> index2{};
  for(int i=1; i<=5; i++){
    file1.open("test/dict_" + std::to_string(i)+".txt");
    file2.open("test/post_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      double total2 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        int index;
        auto begin1 = std::chrono::high_resolution_clock::now();
        index2.dictionary.findTerm(term, index);
        auto end1 = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count();
        List<int> p;
        auto begin2 = std::chrono::high_resolution_clock::now();
        index2.postingList.findPostingList(index, p);
        auto end2 = std::chrono::high_resolution_clock::now();
        total2 += std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count();
      }
      file1 << total1/n << "\n";
      file2 << total2/n << "\n";
    }
    file1.close();
    file2.close();
  }
*/

  TreeIndex index3{};
  //5 runs for each test, to average results
  for(int i=1; i<=5; i++){
    file1.open("test/Tree_" + std::to_string(i)+".txt");
    //search n terms at a time
    for(int n=50; n<10000; n+=50){
      double total1 = 0;
      for(int j=0; j<n; j++){
        auto ptr = dptr + off.at(j);
        std::string term;
        readTerm(term, ptr);
        List<int> p;
        auto begin = std::chrono::high_resolution_clock::now();
        index3.getPostingList(term, p);
        auto end = std::chrono::high_resolution_clock::now();
        total1 += std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
      }
      file1 << total1/n << "\n";
    }
    file1.close();
  }
}

#include"query_parser.hpp"
//#include<sys/mman.h>
//#include<sys/types.h>
//#include<fcntl.h>
//#include<unistd.h>

#include<cstdlib>
#include<ctime>

int main(){

  /**
  std::vector<std::string> filenames;

  Index i{filenames};

  i.printInvertedIndex();
  */
  std::string dictionary = "dictionary.txt";
  std::string posting_lists = "posting_lists.txt";
  Index ii{dictionary, posting_lists};
  //ii.printInvertedIndex();
  //i.saveIndex();

  std::string query = "(maintain AND hold) OR (pct AND year)";

   List<int> r = answer(query, ii);
   std::cout<<"Query result is: "<<r<<std::endl;



  //for(auto x : tokens) std::cout<<x<<" ";
  //std::cout<<std::endl;
  //for(auto y : currentQuery) std::cout<<y<<" ";
  //std::cout<<std::endl;


  //the first subexpression is surely a simple query


  //no parenthesis case-->find terms and operators

   //List<int> result;
   //answerSimpleQuery(tokens, result, ii);
   //std::cout<<result<<std::endl;
   //----------------------ok no parentheses--------------------//
  }

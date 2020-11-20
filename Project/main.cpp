#include"index.hpp"
//#include<sys/mman.h>
//#include<sys/types.h>
//#include<fcntl.h>
//#include<unistd.h>

#include<cstdlib>
#include<ctime>

int main(){


  std::vector<std::string> filenames;

  Index i{filenames};

  i.printInvertedIndex();



}

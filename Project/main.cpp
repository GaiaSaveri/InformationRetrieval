#include"document_utils.hpp"
//#include<string>
#include<dirent.h>

int main(){
  DIR *dir;
  struct dirent *ent;
  if((dir=opendir("data/small/"))!=nullptr){
    while((ent = readdir(dir))!=nullptr){
      std::cout<<ent->d_name<<std::endl;
    }
    closedir(dir);
  }
  else{
    std::cout<<"not opened"<<std::endl;
  }
}

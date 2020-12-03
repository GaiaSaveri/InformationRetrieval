#ifndef __FILE_UTILS_
#define __FILE_UTILS_

#include<iostream>
#include<fstream>
#include<vector>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<cmath>
#include<cstring>
#include<utility>

//get size of the file (in bytes) containing the dict
int getFileSize(std::string& filename){
  struct stat buf;
  int s = stat(filename.c_str(), &buf);
  return s == 0? buf.st_size : -1;
}

//check if a file exists (assuming exists <-> accessible)
bool exists(std::string& filename){
  std::ifstream file(filename);
  return file.good();
}

//reverse content of a vector into a file
void vectorToFile(std::string& filename, std::vector<int>& v){
  std::ofstream file(filename);
  for(int i=0; i<v.size(); i++){
    file << v.at(i) << "\n";
  }
  file.close();
}

//reverse content of a file into a vector
void fileToVector(std::string& filename, std::vector<int>& v){
  std::ifstream file(filename);
  int value;
  while(file>>value)
    v.push_back(value);
  file.close();
}

//mmap file to disk
template<class T>
T* fileToDisk(std::string& filename){
  int size = getFileSize(filename);
  int file = open(filename.c_str(), O_RDONLY, 0);
  T* fp = (T*)mmap(NULL, size, PROT_READ, MAP_SHARED, file, 0);
  if(fp == MAP_FAILED) std::cout<<"mmap failed"<<std::endl;
  //T* ptr = reinterpret_cast<T*>(mmap(NULL, size, PROT_READ, MAP_FILE | MAP_SHARED, file, 0));
  return fp;
}

//get total number of terms
int countLinesFile(std::string& filename){
  int lines = 0;
  std::string line;
  std::ifstream file(filename);
  while(std::getline(file, line)) ++lines;
  return lines;
}

//count total number of files in a directory
int countFiles(std::string& dirname){
  DIR *dp;
  int i = 0;
  struct dirent *d;
  dp = opendir(dirname.c_str());
  if(dp!=nullptr){
    while(d = readdir(dp)){
      if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0)
        i++;
    }
    (void)closedir(dp);
  }
  else std::cout<<"directory not opened"<<std::endl;
  return i;
}

#endif

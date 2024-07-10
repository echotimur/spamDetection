#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

namespace own{


template <typename T>

class LogFile{

  private:

    std::string way;
    T process;
    int startTime;
    int endTime;
    std::string func;

  public:

    LogFile(std::string way, T process, int startTime, int endTime, std::string func) : way{way}, process{process}, startTime{startTime}, endTime{endTime}, func{func}
    {
    }

    void logger(){

      std::fstream logfile(way, std::ios::app);
      auto current_time = std::chrono::system_clock::now();
      int t = (endTime - startTime) / CLOCKS_PER_SEC;

      std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);

      logfile<<'|'<<ctime(&current_time_t)<<"|Execution time (sec): "<<t<<std::endl<<"|In function: "<<func<<std::endl<<"|The result of execute: "<<process<<std::endl<<"----------------"<<std::endl<<std::endl;

      logfile.close();
    }

};


void split(std::string &str, char delim, std::vector<std::string> &v){
  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != std::string::npos){
    end = str.find(delim, start);
    v.push_back(str.substr(start, end - start));
  }

}


std::string charConvertToString(char* a, int size){

  int i;
  std::string s = "";
  for (i = 0; i < size; i++){
    s = s + a[i];
  }

  return s;

}


}

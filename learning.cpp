#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const char specSymbol[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '+', '-', '=', '<', '>', '/', '?', ';', ':', '|', '[', ']', '{', '}', '`', '~', ',', '.', '\\'};
const string predlogi[] = {
  "без", "между", "под", "в", "на", "по", "вокруг", "о", "про", "до", "об", "с", "для", "около", "из-за", "за", "от", "из-под", "к", "перед", "и", "не",
  "Без", "Между", "Под", "В", "На", "По", "Вокруг", "О", "Про", "До", "Об", "С", "Для", "Около", "Из-за", "За", "От", "Из-под", "К", "Перед", "И", "Не"
};


string convertToString(char* a, int size){

  int i;
  string s = "";
  for (i = 0; i < size; i++){
    s = s + a[i];
  }

  return s;

}



string punctuation_marks(string checkingWord){

  int length = checkingWord.length();

  char *checkArray1 = new char[length+1];

  strcpy(checkArray1, checkingWord.c_str());

  for(int i=0; i<29; i++){
    if(checkingWord.find(specSymbol[i]) != string::npos){
      for(int i=length-1; i<=length; i++){
        checkArray1[i] = checkArray1[i+1];
      }
      --length;
    } else continue;
  }

  checkingWord = convertToString(checkArray1, length);

  delete [] checkArray1;

  return checkingWord;
}



void model_make(){

  fstream study("../study.txt", ios::in);
  fstream model("../model.txt", ios::app);

  if(!study.is_open() && !model.is_open()){
    cout<<"Error";
    exit(1);
  }

  string transit = "";
  vector<string> allWords;
  map<string, int> sortWord;

  while(!study.eof()){
    study>>transit;
    transit = punctuation_marks(transit);
    allWords.push_back(transit);
    transit = "";
  }

  int rewriteWordCount = 0;

  for(int i=0; i<allWords.size()-1; i++){
    for(int j=0; j<allWords.size()-1; j++){
      if(allWords[i]==allWords[j]){
        rewriteWordCount++;
      }
    }

    for(int k=0; k<allWords.size(); ++k){
      if(rewriteWordCount > 1){
        sortWord.emplace(allWords[i], rewriteWordCount);
      } else continue;
    }

    rewriteWordCount = 0;

  }

  bool skip = false;

  for (const auto& [word, count] : sortWord) {
    for(int i=0; i<44; i++){
      if(word == predlogi[i]) skip = true;
    }
    if(skip == false)
      model << word << ": " << count << "\n";
    skip = false;
  }

  model.close();
  study.close();

}



void study_func(string fileName[], int numFile){


  fstream study("../study.txt", ios::app);

  for(int i=0; i<numFile; i++){
    fstream file(fileName[i]);

    string buffer = "";

    while(!file.eof()){
      file>>buffer;
      study<<buffer<<" ";
      buffer = "";
    }

    file.close();

  }

  study.close();
}



int main(){

  remove("../study.txt");

  remove("../model.txt");
  
  int numberOfFile = 0;

  string path = "../resource/";

  for(const auto & entry : fs::directory_iterator(path))
    numberOfFile++;

  string *arrFileName = new string[numberOfFile]; //ошибка ? (заменить на динамический массив) : (оставить как есть)

  int i = 0;
  for(const auto & entry : fs::directory_iterator(path)){
    arrFileName[i] = entry.path();
    while(i!=numberOfFile){
      i++;
      break;
    }
  }

  study_func(arrFileName, numberOfFile);
  model_make();

  delete [] arrFileName;
  return 0;
}

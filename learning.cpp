#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const char specSymbol[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '+', '-', '=', '<', '>', '/', '?', ';', ':', '|', '[', ']', '{', '}', '`', '~', ',', '.', '\\'};
const int sizeSpecSymbol = sizeof(specSymbol) / sizeof(char);
// const auti sizeSpecSymbol = size(specSymbol);
const string predlogi[] = {
  "без", "между", "под", "в", "на", "по", "вокруг", "о", "про", "до", "об", "с", "для", "около", "из-за", "за", "от", "из-под", "к", "перед", "и", "не", "а", "во", "из",
  "как", "со",
  "Без", "Между", "Под", "В", "На", "По", "Вокруг", "О", "Про", "До", "Об", "С", "Для", "Около", "Из-за", "За", "От", "Из-под", "К", "Перед", "И", "Не", "А", "Во", "Из",
  "Как", "Со"
};
const int sizePredlogi = sizeof(predlogi) / sizeof(string);
// const auto sizePredlogi = size(predlogi);



void split(string &str, char delim, vector<string> &v){
  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != string::npos){
    end = str.find(delim, start);
    v.push_back(str.substr(start, end - start));
  }
}



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

  for(int i=0; i<sizeSpecSymbol; i++){
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
  fstream prom("../prom.txt", ios::app);
  fstream model("../model.txt", ios::app);

  if(!study.is_open() || !model.is_open() || !prom.is_open()){
    cout<<"Error";
    exit(1);
  }

  int wordCount = 0;
  int rewriteWordCount = 0;
  string transit = "";
  vector<string> allWords;
  map<string, int> sortWord;

  while(!study.eof()){

    if(wordCount == 999){

      cout<<"+1000 words processed"<<endl;

      study>>transit;
      transit = punctuation_marks(transit);
      allWords.push_back(transit);
      transit = "";

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
        for(int i=0; i<sizePredlogi; i++){
          if(word == predlogi[i]) skip = true;
        }
        if(skip == false)
          prom << word << " " << count << "\n";
        skip = false;
      }

      allWords.clear();
      sortWord.clear();
      wordCount = 0;
      continue;
    }

    study>>transit;
    transit = punctuation_marks(transit);
    allWords.push_back(transit);
    wordCount++;
    transit = "";
  }

  if(allWords.size() > 1){
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
      for(int i=0; i<sizePredlogi; i++){
        if(word == predlogi[i]) skip = true;
      }
      if(skip == false)
        prom << word << " " << count << "\n";
      skip = false;
    }

  }

  allWords.clear();
  sortWord.clear();
  prom.close();

  prom.open("../prom.txt", ios::in);
  int num = 0;
  int num1 = 0;
  bool add = true;
  wordCount = 0;
  string transit2 = "";
  vector<string> a;
  vector<string> b;

  fstream prom1("../prom1.txt", ios::app);

  if(!prom1.is_open()){
    cout<<"Error";
    exit(1);
  }

  while(getline(prom, transit)){
    prom1<<transit<<"\n";
  }
  transit = "";
  prom.close();
  prom1.close();

  prom.open("../prom.txt", ios::in);
  while(getline(prom, transit)){

    if(num1 == 999) break;

    split(transit, ' ', a);

    prom1.open("../prom1.txt", ios::in);
    while(getline(prom1, transit2)){

      if(num < 999){
        num++;
        continue;
      }

      split(transit2, ' ', b);
      if(a[0] == b[0]){

        add = false;

        wordCount = stoi(a[1]) + stoi(b[1]);
        a[1] = to_string(wordCount);

        for(const auto &word : a){
          model<<word<<' ';
        }
        model<<"\n";

        b.clear();
        wordCount = 0;
        num = 0;
      }

      num1++;

    }

    if(add == true){
      for(const auto &word : a){
        model<<word<<' ';
      }
      model<<"\n";
    }

    a.clear();

    prom1.close();
  }

  model.close();
  prom.close();
  study.close();

}



void study_func(string fileName[], int numFile){

  fstream study("../study.txt", ios::app);

  long int wordCount = 0;

  for(int i=0; i<numFile; i++){
    fstream file(fileName[i]);

    string buffer = "";

    while(!file.eof()){
      file>>buffer;
      study<<buffer<<" ";
      wordCount++;
      buffer = "";
    }

    file.close();

  }

  cout<<"Total number of words: "<<wordCount<<endl;

  study.close();

}



int main(){

  remove("../study.txt");

  remove("../prom.txt");
  remove("../prom1.txt");

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

  cout<<"Beginnig studing..."<<endl;

  study_func(arrFileName, numberOfFile);

  cout<<"Starting make model-file"<<endl;

  model_make();

  cout<<"End!"<<endl;

  delete [] arrFileName;

  return 0;

}

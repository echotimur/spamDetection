#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "lib.h"

namespace fs = std::filesystem;
using namespace std;

const char specSymbol[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '+', '-', '=', '<', '>', '/', '?', ';', ':', '|', '[', ']', '{', '}', '`', '~', ',', '.', '\\'};
const auto sizeSpecSymbol = size(specSymbol);

const string predlogi[] = {
  "без", "между", "под", "в", "на", "по", "вокруг", "о", "про", "до", "об", "с", "для", "около", "из-за", "за", "от", "из-под", "к", "перед", "и", "не", "а", "во", "из",
  "как", "со",
  "Без", "Между", "Под", "В", "На", "По", "Вокруг", "О", "Про", "До", "Об", "С", "Для", "Около", "Из-за", "За", "От", "Из-под", "К", "Перед", "И", "Не", "А", "Во", "Из",
  "Как", "Со"
};
const auto sizePredlogi = size(predlogi);





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

  checkingWord = own::charConvertToString(checkArray1, length);

  delete [] checkArray1;

  return checkingWord;
}



void selection_create(){

  fstream model("../temporary_files/model_derivative.txt", ios::in);

  string buffer = "";

  vector <string> a;
  map <int, string> toSelect;

  while(getline(model, buffer)){
    own::split(buffer, ' ', a);
    toSelect.emplace(stoi(a[1]), a[0]);
    a.clear();
  }

  fstream selection("../selection.txt", ios::app);

  selection<<"Нere is the general information about the original files:"<<endl;

  for(const auto& [count, word] : toSelect)
    selection<<count<<": "<<word<<endl;

  toSelect.clear();

}



void model_make(){

  fstream study("../temporary_files/study.txt", ios::in);
  fstream prom("../temporary_files/prom.txt", ios::app);
  fstream model("../model.txt", ios::app);
  fstream model_derivative("../temporary_files/model_derivative.txt", ios::app);

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
    // в этом цикле мы добавляем 1000 слов и делаем предварительный анализ
    if(wordCount == 999){

      cout<<"+1000 words processed"<<endl<<"---"<<endl;

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
  // а здесь мы добиваем остаток, если условно слов 1002
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

  prom.open("../temporary_files/prom.txt", ios::in);
  int num = 0;
  int num1 = 0;
  bool add = true;
  wordCount = 0;
  string transit2 = "";
  vector<string> a;
  vector<string> b;

  fstream prom1("../temporary_files/prom1.txt", ios::app);

  if(!prom1.is_open()){
    cout<<"Error";
    exit(1);
  }
  // создаём дубликат файла prom (просто копируем содержимое из prom), он нам понадобится чуть ниже для окончательной сортировки
  while(getline(prom, transit)){
    prom1<<transit<<"\n";
  }
  transit = "";
  // необходимо закрыть эти текстовые потоки, а потом снова открыть, чтобы указатель считывания переместился с конца файла в начало
  // (да, есть другие реализации, но такой способ первым пришёл в голову, наличие двух transit-ов обусловлено тем же самым):
  prom.close();
  prom1.close();

  const auto &request1 = "CREATE TABLE words(\n    id INT NOT NULL KEY AUTO_INCREMENT,\n    word TEXT,\n    count INT\n);\n\n";
  model<<request1;
  prom.open("../temporary_files/prom.txt", ios::in);
  while(getline(prom, transit)){

    if(num1 == 999) break;

    own::split(transit, ' ', a);

    prom1.open("../temporary_files/prom1.txt", ios::in);
    while(getline(prom1, transit2)){

      if(num < 999){
        num++;
        continue;
      }

      own::split(transit2, ' ', b);
      if(a[0] == b[0]){

        add = false;

        wordCount = stoi(a[1]) + stoi(b[1]);
        a[1] = to_string(wordCount);

      const auto &word1 = a[0];
      const auto &word2 = a[1];
      const auto &request2 = "INSERT INTO words(`word`, `count`) VALUES('" + a[0] + "', " + a[1] + ");\n";
      model<<request2;

      for(const auto &word : a){
        model_derivative<<word<<' ';
      }
      model_derivative<<"\n";

        b.clear();
        wordCount = 0;
        num = 0;
      }

      num1++;

    }

    if(add == true){

      const auto &word1 = a[0];
      const auto &word2 = a[1];
      const auto &request2 = "INSERT INTO words(`word`, `count`) VALUES('" + a[0] + "', " + a[1] + ");\n";
      model<<request2;

      for(const auto &word : a){
        model_derivative<<word<<' ';
      }
      model_derivative<<"\n";

    }

    a.clear();

    prom1.close();
  }

  model.close();
  prom.close();
  study.close();

}



void study_func(string fileName[], int numFile){
  // просто собираем все исходные текстовые в один...
  fstream study("../temporary_files/study.txt", ios::app);

  long long int wordCount = 0;

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

  cout<<"Total number of words: "<<wordCount<<endl<<"----------------"<<endl;

  study.close();

}



int main(){
  // очищаем все документы, перед началом. (Важно!:сделать контроль версий model.txt)
  int start = clock();
  remove("../temporary_files/study.txt");

  remove("../temporary_files/prom.txt");
  remove("../temporary_files/prom1.txt");
  remove("../temporary_files/model_derivative.txt");

  remove("../model.txt");
  remove("../selection.txt");

  int numberOfFile = 0;

  string path = "../resource/";

  for(const auto & entry : fs::directory_iterator(path))
    numberOfFile++;

  string *arrFileName = new string[numberOfFile];

  int i = 0;
  for(const auto & entry : fs::directory_iterator(path)){

    arrFileName[i] = entry.path();

    while(i!=numberOfFile){
      i++;
      break;
    }
  }

  cout<<endl<<"================"<<endl<<"Beginnig studing..."<<endl<<"----------------"<<endl;

  study_func(arrFileName, numberOfFile);

  cout<<"Starting make model-file"<<endl<<"----------------"<<endl;

  model_make();

  cout<<"Create selection file"<<endl<<"----------------"<<endl;

  selection_create();

  cout<<"End!"<<endl<<"================"<<endl<<endl;

  delete [] arrFileName;

  int end = clock();

  string func = __FUNCTION__;

  own::LogFile obj{"SUCCESS", start, end, func};
  obj.logger();

  return 0;

}

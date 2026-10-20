#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;
int main(){
  bool isChar;
  int charCount;
  char chars[26] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
  };
  int randomNum;
  string keyCode[10];
  srand(time(0));
  for(int sl = 0; sl < 10; sl++){
    randomNum = rand() %1;
    if(randomNum == 0){
      isChar = false;
    }
    else{
      isChar = true;
    }
    isChar = randomNum;
    if(isChar){
      randomNum = rand() %27;
      keyCode[sl] = string(1, chars[randomNum]);
      cout << "Debug " << sl << ":" << randomNum << ':' << chars[randomNum] << ": Debug Char" << endl;
    }
    else{
      randomNum = rand() %10;
      keyCode[sl] = to_string(randomNum);
      cout << "Debug " << sl << ":" << randomNum << " Debug Num" << endl;
    }
  }
  cout << keyCode[0];
}
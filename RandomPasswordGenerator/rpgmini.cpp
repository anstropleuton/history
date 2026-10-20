#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;
bool yn(string input){
  if(input == "y")
  return true;
  else return false;
}
int integer(string input){
  ostringstream convert;
  for(int l = 0; l < input.length(); l++){
    switch(input[l]){
      case '0': convert << 0; break;
      case '1': convert << 1; break;
      case '2': convert << 2; break;
      case '3': convert << 3; break;
      case '4': convert << 4; break;
      case '5': convert << 5; break;
      case '6': convert << 6; break;
      case '7': convert << 7; break;
      case '8': convert << 8; break;
      case '9': convert << 9; break;
    }
  }
  stringstream value(convert.str());
  int output = 0;
  value >> output;
  return output;
}
int main(){
  int length, count, random, seed;
  string character, userInput;
  bool upper, lower, number = true, useTime;
  ostringstream characterSetup;
  cout << "Length                 :";
  cin >> userInput;
  length = integer(userInput);
  if(length == 0) length = 1;
  cout << "Count                  :";
  cin >> userInput;
  count = integer(userInput);
  if(count == 0) count = 1;
  cout << "Use time as seed (y/n) :";
  cin >> userInput;
  useTime = yn(userInput);
  if(useTime == true){
    seed = time(0);
    srand(seed);
  }
  else{
    cout << "Seed :";
    cin >> userInput;
    seed = integer(userInput);
    srand(seed);
  }
  cout << "Upper characters (y/n) :";
  cin >> userInput;
  upper = yn(userInput);
  cout << "Lower characters (y/n) :";
  cin >> userInput;
  lower = yn(userInput);
  if(upper == true || lower == true){
    cout << "Numbers          (y/n) :";
    cin >> userInput;
    number = yn(userInput);
  }
  for(int i = 0; i < 62; i++){
    if(i < 10 && number == true){
      characterSetup << char(i + 48);
    }
    if(i < 36 && i > 10 && upper == true){
      characterSetup << char(i + 55);
    }
    if(i > 36 && lower == true){
      characterSetup << char(i + 61);
    }
  }
  character = characterSetup.str();
  for(int b = 0; b < count; b++){
    for(int a = 0; a < length; a++){
      random = rand() % character.length();
      cout << character[random];
    }
    cout << endl;
  }
}
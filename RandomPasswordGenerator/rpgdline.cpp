#include <iostream>
#include <sstream>
using namespace std;
string ftrPswCountLetter(int input){
  ostringstream convert;
  string output;
  int value[10000];
  int count = 0;
  while(input){
    value[count] = input % 26;
    input = input / 26;
    count++;
  }
  for(int z = 0; z < count - 1; z++){
    if(value[z] <= 0){
      value[z] += 26;
      value[z + 1] = value[z + 1] - 1;
    }
  }
  for(int z = count; z >= 0; z--){
    if(value[z] > 0) convert << char('A' + value[z] - 1);
  }
  output = convert.str();
  return output;
}
int main(){
  for(int i = 0; i < 106; i++){
    cout << ftrPswCountLetter(i) << endl;
  }
  return 0;
}
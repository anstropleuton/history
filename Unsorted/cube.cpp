#include <iostream>
using namespace std;
int main(){
  restart:{
    int height, length;
    cout << "Length :";
    cin >> length;
    cout << "Height :";
    cin >> height;
    if(height < 2){
      height = 2;
    }
    if(length < 2){
      length = 2;
    }
    length--;
    length--;
    height--;
    height--;
    for(int k = -2; k < 7; k++){
      cout << '=';
    }
    cout << endl << '+';
    for(int i = 0; i < length; i++){
      cout << '-';
    }
    cout << '+' << endl;
    for(int a = 0; a < height; a++){
      cout << '|';
      for(int b = 0; b < length; b++){
        cout << '#';
      }
      cout << '|' << endl;
    }
    cout << '+';
    for(int j = 0; j < length; j++){
      cout << '-';
    }
    cout << '+' << endl;
    for(int l = -2; l < 7; l++){
      cout << '=';
    }
    cout << endl;
  }
  goto restart;
}

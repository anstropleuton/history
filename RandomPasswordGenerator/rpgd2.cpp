#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;
int intExtract(string input){
  ostringstream convert;
  int inputLength = input.length(), output = 0;
  register int z;
  bool negativeNumberDetect[inputLength], negativeNumberSkip[inputLength];
  for(z = 0; z < inputLength; z++){
    negativeNumberDetect[z] = false;
    negativeNumberSkip[z] = false;
  }
  for(z = 0; z < inputLength; z++){
    switch(input[z]){
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
      case '-': negativeNumberDetect[z] = true; break;
      default: negativeNumberSkip[z] = true; break;
    }
  }
  stringstream value(convert.str());
  value >> output;
  for(z = 0; z < inputLength; z++){
    if(!negativeNumberSkip[z])
    if(negativeNumberDetect[z]) output = output * (-1);
    else z = inputLength;
  }
  return output;
}
void ftrLineGenerate(bool line, bool graph, int length, int data[3], string dot, string pattern){
  register int z;
  if(line){
    if(graph){
      for(z = 0; z < length; z++){
        if(z == data[0] || z == data[1] || z == data[2]){
          cout << dot;
          z += dot.length() - 1;
        }
        else cout << pattern;
      }
    }
    else for(z = 0; z < length; z++){
      cout << pattern;
    }
  }
}
string ftrPswCountLetter(int input){
  ostringstream convert;
  string output;
  int value[10000], count = 0;
  register int z;
  while(input){
    value[count] = input % 26;
    input = input / 26;
    count++;
  }
  for(z = 0; z < count - 1; z++){
    if(value[z] <= 0){
      value[z] += 26;
      value[z + 1] = value[z + 1] - 1;
    }
  }
  for(z = count; z >= 0; z--){
    if(value[z] > 0) convert << char('A' + value[z] - 1);
  }
  output = convert.str();
  return output;
}
int main(){
  string pswCharacter, settingData = "help", ftrInput = "n", tempUserInput = "n", tempConsoleOutput = "n", ftrLineDrawPattern = "-", ftrPswCountPre = "(", ftrPswCountNext = ")", ftrGraphLineCharacter = "|", ftrGraphLineDot = "+", ftrPswCountString;
  int pswLength = 1, pswCount = 1, randomNumber = 0, generatorSeed = 0, ftrLineDrawValue = 0, ftrGraphLineCheckValue[3], ftrPswCountLength;
  register int z = 0;
  bool ftrLineDraw = true, ftrPswCount = true, ftrPswCountNumber = false, ftrGraphLine = true, autoAdjust = true, setupUpper = false, setupLower = true, setupNumber = true;
  ostringstream characterSetup, intLength;
  string diag[13];
  //cout << "Password Length :";
  pswLengthAsking:{
    //cin >> tempUserInput;
    tempUserInput = "20";
    if(tempUserInput != "help")
      pswLength = intExtract(tempUserInput);
    else{
      cout << diag[0];
      goto pswLengthAsking;
    }
    pswLength = intExtract(tempUserInput);
    if(pswLength <= 0){
      cout << "Invalid Number, There has to be more than 0 characters. Try again :";
      goto pswLengthAsking;
    }
  }
  //cout << "Passwords Count :";
  pswCountAsking:{
    //cin >> tempUserInput;
    tempUserInput = "1024";
    if(tempUserInput != "help")
      pswCount = intExtract(tempUserInput);
    else{
      cout << diag[1];
      goto pswCountAsking;
    }
    if(pswCount <= 0){
      cout << "Invalid Number, There cannot be less than 1 passwords. Try again :";
      goto pswCountAsking;
    }
  }
  if(ftrPswCount){
      ftrLineDrawValue = (pswLength + ftrPswCountPre.length() + ftrPswCountNext.length() + 1);
  }
  else{
    ftrLineDrawValue = pswLength;
  }
  if(ftrGraphLine){
    ftrLineDrawValue += (ftrGraphLineDot.length() * 2);
    if(ftrPswCount){
      ftrLineDrawValue += ftrGraphLineDot.length();
      ftrGraphLineCheckValue[0] = 0;
      ftrGraphLineCheckValue[1] = (ftrPswCountPre.length() + ftrPswCountNext.length() + 2);
      ftrGraphLineCheckValue[2] = (ftrGraphLineCheckValue[1] + pswLength + 1);
    }
    else{
      ftrGraphLineCheckValue[0] = 0;
      ftrGraphLineCheckValue[1] = -1;
      ftrGraphLineCheckValue[2] = pswLength;
    }
  }
  else{
    ftrGraphLineCheckValue[0] = -1;
    ftrGraphLineCheckValue[1] = -1;
    ftrGraphLineCheckValue[2] = -1;
  }
  generatorSeed = time(0);
  for(z = 0; z < 62; z++){
    if(z < 10 && setupNumber == true){
      characterSetup << char(z + 48);
    }
    if(z < 36 && z > 10 && setupUpper == true){
      characterSetup << char(z + 55);
    }
    if(z > 36 && setupLower == true){
      characterSetup << char(z + 61);
    }
  }
  srand(generatorSeed);
  pswCharacter = characterSetup.str();
  cout << endl;
  ftrLineGenerate(ftrLineDraw, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern);
  cout << endl;
  for(int a = 0; a < pswCount; a++){
    if(ftrPswCount){
      if(ftrGraphLine){
        cout << ftrGraphLineCharacter;
      }
      cout << ftrPswCountPre;
      if(ftrPswCountNumber){
        cout << (a + 1);
        intLength.str("");
        intLength.clear();
        intLength << (a + 2);
        ftrPswCountLength = intLength.str().length() - 1;
      }
      else{
        ftrPswCountString = ftrPswCountLetter((a + 1));
        cout << ftrPswCountString;
        ftrPswCountString = ftrPswCountLetter((a + 2));
        ftrPswCountLength = ftrPswCountString.length() - 1;
      }
      cout << ftrPswCountNext;
      if(ftrGraphLine){
        cout << ftrGraphLineCharacter;
      }
    }
    else if(ftrGraphLine){
      cout << ftrGraphLineCharacter;
    }
    for(int b = 0; b < pswLength; b++){
      randomNumber = rand() % pswCharacter.length();
      cout << pswCharacter[randomNumber];
    }
    ftrGraphLineCheckValue[1] += ftrPswCountLength;
    ftrGraphLineCheckValue[2] += ftrPswCountLength;
    ftrLineDrawValue += ftrPswCountLength;
    if(ftrGraphLine){
      cout << ftrGraphLineCharacter << endl;
      ftrLineGenerate(ftrLineDraw, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern);
    }
    ftrGraphLineCheckValue[1] -= ftrPswCountLength;
    ftrGraphLineCheckValue[2] -= ftrPswCountLength;
    ftrLineDrawValue -= ftrPswCountLength;
    cout << endl;
  }
  cout << endl << "Seed :" << generatorSeed << endl;
}
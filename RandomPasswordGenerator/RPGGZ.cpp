#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
using namespace std;
int intExtract(string input){
  stringstream convert;
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
  convert >> output;
  for(z = 0; z < inputLength; z++){
    if(!negativeNumberSkip[z])
      if(negativeNumberDetect[z]) output = output * (-1);
      else z = inputLength;
  }
  return output;
}
void ftrLineGenerate(bool line, bool graph, int length, int data[3], string dot, string pattern, ostringstream &output){
  register int z;
  if(line){
    if(graph){
      for(z = 0; z < length; z++){
        if(z == data[0] || z == data[1] || z == data[2]){
          output << dot;
          z += dot.length() - 1;
        }
        else output << pattern;
      }
    }
    else for(z = 0; z < length; z++) cout << pattern;
    output << '\n';
  }
}
string ftrPswCounterLetter(int input){
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
int intConvert(bool data, char input){
  static string integer;
  int output = 0;
  if(data){
    output = intExtract(integer);
    integer = "";
    return output;
  }
  else integer += input;
}
bool settingBool(string input, char skip, int& counter){
  bool output = false;
  counter++;
  if(input[(counter - 1)] != skip){if(input[(counter - 1)]){output = true;}}
  return output;
}
string settingString(string input, char skip, char prev, char next, int& counter){
  string output;
  ostringstream convert;
  if(input[counter] != skip){
    if(input[counter] == prev){
      while(input[(counter + 1)] != next){
        counter++;
        convert << input[counter];
      }
      output = convert.str();
      convert.str("");
      counter++;
    }
  }
  counter++;
  return output;
}
int settingInt(string input, char skip, char prev, char next, int& counter){
  int output = 0;
  if(input[counter] != skip){
    if(input[counter] == prev){
      while(input[(counter + 1)] != next){
        counter++;
        intConvert(false, input[counter]);
      }
      output += intConvert(true, 0);
      counter++;
    }
  }
  counter++;
  return output;
}
int main(){
  string pswCharacter, settingData = "help", ftrInput = "n", tempUserInput = "n", tempConsoleOutput = "n", ftrLineDrawPattern = "-", ftrPswCounterPrev = "[", ftrPswCounterNext = "]", ftrGraphLineCharacter = "|", ftrGraphLineDot = "+", ftrPswCounterString;
  char strPrev = '<', strNext = '>', strSkip = '!';
  int pswLength = 1, pswCount = 1, randomNumber = 0, generatorSeed = time(0), ftrGraphLineCheckValue[3], ftrPswCounterLength, settingCounter = 3;
  register int z = 0;
  bool ftrLineDraw = false, ftrPswCounter = false, ftrPswCounterNumber = true, ftrGraphLine = false, autoAdjustSetting = true, setupUpper = false, setupLower = true, setupNumber = true, settingDataSetup = false;
  int ftrLineDrawValue = pswCount;
  ostringstream characterSetup, intLength, output;
  string diag[] = {
    "How long you want your password to be (1+) :",
    "Invalid Number, There has to be more than 0 characters. Try again (1+) :",
    "How many passwords you want to generate (1+) :",
    "Invalid Number, There cannot be less than 1 passwords. Try again (1+) :",
    "Line Draw is a feature that draws lines made of your choice at the start of generating password and at the end of generated passwords. This is just a decorational feature! (y/n/help) :",
    "This is the feature that adds a count value of passwords generated. e.g.\n(1):<password>\n(2):<password>\n... Etc. This feature is also customizable! (y/n/help) :",
    "This feature draws lines vertically to make the passwords organized like a excel sPrevad sheet. But it requires 2 characters to use. One for the vertical line and another for the dot used to connect horisontal line. The horisontal line is Line Pattern if you entered before. This feature is also for decoration (y/n/help) :",
    "The graph chatracter used to make Graph e.g. '|' :",
    "This feature uses characters from Prevvious features, \"Line Draw\" and the character is used from \"Line Draw Pattern\" and it seems like you did not used that feature, enter the character for the program to use to make graph e.g. '-' :",
    "The Dot characer used to connect vertical line and horizontal line e.g. '+' :",
    "Adjusts all the settings automatically so it's stable for the generator (Warning, If you don't use this, chances are the program will be unstable or might Crash) (y/n/help) :",
    "ftrLineDrawValue is the feature that draws line using characters. This value determines how many characters to use (<number>) :",
    "ftrGraphLineCheckValue is the setting that is used to check if the matching conditions are met for the ftrGraphLineDot to ube used (<number>) :",
    "Use lower cased characters as password random letters (y/n/help) :",
    "Use upper cased characters as password random letters (y/n/help) :",
    "Use numbers as password random numbers (y/n/help) :",
    "Seed determines how your password is gonna look. The last letter \"Seed:<number>\" is the seed for current password. You can share seed but better not because others can generate exact same passwords just like yours. If you found something cool and you aren't using this program for password then feel free to share seed as well as password length and count as that's necessare to generate exact same passwords. The 't' means to use the system time as the seed for random passwords to generate (<seed>/t/help) :",
    "Change settings that is used to generate password (Warning! Inserting a 'Space' character might crash the program) You can also input all the settings at once if you like and the program will detect them and use them accordingly (the first character you type as data is going to be the start of a string value and the next is end of the string then you can use them while entering string data. e.g. you entered \"<>\" then whenever you use '<' the program will detect that and use at start of the string and then it will search for '>' and then it will conclude the value and settes up the next and the third character will be skip character) (y/n/help/<data>) :",
    "Restart all of the code (Doesn't resets the values and settings unless selected 'r' which briengs everything to default) The 'd' sends all the information the code has used and all the settings values and stuffs for debug purposes, The 's' sends the input debug used by settingData. The 'o' to output the passwords into a file which will be created in '/storage/emulated/0/' or 'C:/'. You can also read contents from the file my typing 'i' if you don't know what exists in it (y/n/d/r/s/i/o/help) :",
    "Some optional features to enable to make your passwords look a little bit fancier! (y/n/help) :"
  };
  restartPassword:
  cout << "Password Length :";
  pswLengthAsking:
  cin >> tempUserInput;
  if(tempUserInput != "help") pswLength = intExtract(tempUserInput);
  else{
    cout << diag[0];
    goto pswLengthAsking;
  }
  if(pswLength <= 0){
    cout << diag[1];
    goto pswLengthAsking;
  }
  cout << "Passwords Count :";
  pswCountAsking:
  cin >> tempUserInput;
  if(tempUserInput != "help") pswCount = intExtract(tempUserInput);
  else{
    cout << diag[2];
    goto pswCountAsking;
  }
  if(pswCount <= 0){
    cout << diag[3];
    goto pswCountAsking;
  }  ftrLineDrawValue = pswCount;
  cout << "Change Settings (y/n/help) :";
  settingDataAsking:
  cin >> settingData;
  if(settingData == "y"){
    cout << "Enable features (y/n/help) :";
    ftrEnableAsking:
    cin >> tempUserInput;
    if(tempUserInput == "y"){
      cout << "Line Draw (y/n/help) :";
      ftrLineDrawAsking:
      cin >> tempUserInput;
      if(tempUserInput == "y"){
        ftrLineDrawPatternAsking:
        cout << "Line Pattern e.g. '-' :";
        cin >> ftrLineDrawPattern;
        cout << "Line pattern might look like this :" << endl;
        for(z = 0; z < 20; z++) cout << ftrLineDrawPattern;
        cout << endl << "Confirm (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput != "y"){
          cout << "Try Again :" << endl;
          goto ftrLineDrawPatternAsking;
        }
        else if(tempUserInput == "help"){
          cout << diag[4];
          goto ftrLineDrawAsking;
        }
        else ftrLineDraw = true;
      }
      else if(tempUserInput == "help"){
        cout << diag[4];
        goto ftrLineDrawAsking;
      }
      cout << "Password Counter (y/n/help) :";
      ftrPswCounterAsking:
      cin >> tempUserInput;
      if(tempUserInput == "y"){
        ftrPswCounterConfigAsking:
        cout << "Use Numbers/Letters (n/l) :";
        cin >> tempUserInput;
        if(tempUserInput == "l"){
          ftrPswCounterNumber = false;
          tempConsoleOutput = "Letter";
        }
        else{
          ftrPswCounterNumber = true;
          tempConsoleOutput = "Number";
        }
        cout << "Before " << tempConsoleOutput <<" e.g. '[' :";
        cin >> ftrPswCounterPrev;
        cout << "After " << tempConsoleOutput << " e.g. ']' :";
        cin >> ftrPswCounterNext;
        if(ftrPswCounterNumber) tempConsoleOutput = "20";
        else tempConsoleOutput = "AB";
        cout << "Password Count might look like this :" << endl << ftrPswCounterPrev << tempConsoleOutput << ftrPswCounterNext << "a1b2c3x7y8z9" << endl << "Confirm (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput != "y"){
          cout << "Try Again :" << endl;
          goto ftrPswCounterConfigAsking;
        }
        else if(tempUserInput == "help"){
          cout << diag[5];
          goto ftrPswCounterAsking;
        }
        else ftrPswCounter = true;
      }
      else if(tempUserInput == "help"){
        cout << diag[5];
        goto ftrPswCounterAsking;
      }
      else ftrPswCounter = false;
      cout << "Graph Line (y/n/help) :";
      ftrGraphLineAsking:
      cin >> tempUserInput;
      if(tempUserInput == "y"){
        ftrGraphLineCharacterAsking:
        cout << "Graph Character e.g. '|' :";
        cin >> tempUserInput;
        if(tempUserInput == "help"){
          cout << diag[7];
          goto ftrGraphLineCharacterAsking;
        }
        else ftrGraphLineCharacter = tempUserInput;
        if(!ftrLineDraw){
          cout << "Horizontal Line e.g. '-' :";
          ftrGraphHorizontalAsking:
          cin >> tempUserInput;
          if(tempUserInput == "help"){
            cout << diag[8];
            goto ftrGraphHorizontalAsking;
          }
          else ftrLineDrawPattern = tempUserInput;
        }
        cout << "Dot Character e.g. '+' :";
        ftrGraphLineDotAsking:
        cin >> tempUserInput;
        if(tempUserInput == "help"){
          cout << diag[9];
          goto ftrGraphLineDotAsking;
        }
        else ftrGraphLineDot = tempUserInput;
        cout << ftrGraphLineDot;
        for(z = 0; z < 12; z++) cout << ftrLineDrawPattern;
        cout << ftrGraphLineDot << endl << ftrGraphLineCharacter << "a1b2c3x7y8z9" << ftrGraphLineCharacter << endl << ftrGraphLineDot;
        for(z = 0; z < 12; z++) cout << ftrLineDrawPattern;
        cout << ftrGraphLineDot << endl << "Confirm (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput != "y"){
          cout << "Try Again :";
          goto ftrGraphLineCharacterAsking;
        }
        else if(tempUserInput == "help"){
          cout << diag[6];
          goto ftrGraphLineAsking;
        }
        else ftrGraphLine = true;
      }
      else if(tempUserInput == "help"){
        cout << diag[6];
        goto ftrGraphLineAsking;
      }
      else ftrGraphLine = false;
    }
    else if(tempUserInput == "help"){
      cout << diag[19];
      goto ftrEnableAsking;
    }
    cout << "Upper characters (y/n/help) :";
    setupUpperAsking:
    cin >> tempUserInput;
    if(tempUserInput == "y") setupUpper = true;
    else if(tempUserInput == "help"){
      cout << diag[13];
      goto setupUpperAsking;
    }
    else setupLower = false;
    cout << "Lower characters (y/n/help) :";
    setupLowerAsking:
    cin >> tempUserInput;
    if(tempUserInput == "y") setupLower = true;
    else if(tempUserInput == "help"){
      cout << diag[14];
      goto setupLowerAsking;
    }
    else setupLower = false;
    if(setupUpper || setupLower){
      cout << "Numbers (y/n/help) :";
      setupNumberAsking:
      cin >> tempUserInput;
      if(tempUserInput == "y") setupNumber = true;
      else if(tempUserInput == "help"){
        cout << diag[15];
        goto setupNumberAsking;
      }
      else setupNumber = false;
    }
    cout << "Generator Seed (<seed>/t/help) :";
    generatorSeedAsking:
    cin >> tempUserInput;
    if(tempUserInput == "help"){
      cout << diag[16];
      goto generatorSeedAsking;
    }
    else if(tempUserInput == "t") generatorSeed = time(0);
    else generatorSeed = intExtract(tempUserInput);
    cout << "Auto Adjust (very recommended) (y/n/help) :";
    autoAdjustSettingAsking:
    cin >> tempUserInput;
    if(tempUserInput == "y"){
      autoAdjustSetting = true;
    }
    else if(tempUserInput == "help"){
      cout << diag[10];
      goto autoAdjustSettingAsking;
    }
    else{
      if(ftrLineDraw){
        cout << "ftrLineDrawValue = ";
        ftrLineDrawValueAsking:
        cin >> tempUserInput;
        if(tempUserInput == "help") cout << diag[11];
        else ftrLineDrawValue = intExtract(tempUserInput);
      }
      if(ftrGraphLine){
        cout << "ftrGraphLineCheckValue[0] = ";
        ftrGraphLineCheckValueZeroAsking:
        cin >> tempUserInput;
        if(tempUserInput == "help"){
          cout << diag[12];
          goto ftrGraphLineCheckValueZeroAsking;
        }
        else ftrGraphLineCheckValue[0] = intExtract(tempUserInput);
        cout << "ftrGraphLineCheckValue[1] = ";
        ftrGraphLineCheckValueOneAsking:
        cin >> tempUserInput;
        if(tempUserInput == "help"){
          cout << diag[12];
          goto ftrGraphLineCheckValueOneAsking;
        }
        else ftrGraphLineCheckValue[1] = intExtract(tempUserInput);
        cout << "ftrGraphLineCheckValue[2] = ";
        ftrGraphLineCheckValueTwoAsking:
        cin >> tempUserInput;
        if(tempUserInput == "help"){
          cout << diag[12];
          goto ftrGraphLineCheckValueTwoAsking;
        }
        else ftrGraphLineCheckValue[2] = intExtract(tempUserInput);
      }
    }
  }
  else if(settingData == "help"){
    cout << diag[17];
    goto settingDataAsking;
  }
  else if(settingData != "n"){
    settingDataSetup = true;
    goto settingDataSet;
    settingDataFinish:
    cout << "Configure Success!" << endl;
  }
  if(autoAdjustSetting){
    if(ftrPswCounter) ftrLineDrawValue = (pswLength + ftrPswCounterPrev.length() + ftrPswCounterNext.length() + 1);
    else ftrLineDrawValue = pswLength;
    if(ftrGraphLine){
      ftrLineDrawValue += (ftrGraphLineDot.length() * 2);
      if(ftrPswCounter){
        ftrLineDrawValue += ftrGraphLineDot.length();
        ftrGraphLineCheckValue[0] = 0;
        ftrGraphLineCheckValue[1] = (ftrPswCounterPrev.length() + ftrPswCounterNext.length() + 2);
        ftrGraphLineCheckValue[2] = (ftrGraphLineCheckValue[1] + pswLength + 1);
      }
      else{
        ftrGraphLineCheckValue[0] = 0;
        ftrGraphLineCheckValue[1] = -1;
        ftrGraphLineCheckValue[2] = (pswLength + 1);
      }
    }
    else{
      ftrGraphLineCheckValue[0] = -1;
      ftrGraphLineCheckValue[1] = -1;
      ftrGraphLineCheckValue[2] = -1;
    }
  }
  for(z = 0; z < 62; z++){
    if(z < 10 && setupNumber) characterSetup << char(z + 48);
    if(z < 36 && z > 9 && setupUpper) characterSetup << char(z + 55);
    if(z > 35 && setupLower) characterSetup << char(z + 61);
  }
  pswCharacter = characterSetup.str();
  srand(generatorSeed);
  cout << endl;
  if(ftrGraphLine && !ftrLineDraw) ftrLineGenerate(true, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern, output);
  else ftrLineGenerate(ftrLineDraw, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern, output);
  for(int a = 0; a < pswCount; a++){
    if(ftrPswCounter){
      if(ftrGraphLine) output << ftrGraphLineCharacter;
      output << ftrPswCounterPrev;
      if(ftrPswCounterNumber){
        output << (a + 1);
        intLength.str("");
        intLength.clear();
        intLength << (a + 2);
        ftrPswCounterLength = intLength.str().length() - 1;
      }
      else{
        ftrPswCounterString = ftrPswCounterLetter((a + 1));
        output << ftrPswCounterString;
        ftrPswCounterString = ftrPswCounterLetter((a + 2));
        ftrPswCounterLength = ftrPswCounterString.length() - 1;
      }
      output << ftrPswCounterNext;
      if(ftrGraphLine) output << ftrGraphLineCharacter;
    }
    else if(ftrGraphLine) output << ftrGraphLineCharacter;
    for(int b = 0; b < pswLength; b++){
      randomNumber = rand() % pswCharacter.length();
      output << pswCharacter[randomNumber];
    }
    if(ftrGraphLine) output << ftrGraphLineCharacter;
    if(ftrPswCounter){
      ftrGraphLineCheckValue[1] += ftrPswCounterLength;
      ftrGraphLineCheckValue[2] += ftrPswCounterLength;
      ftrLineDrawValue += ftrPswCounterLength;
    }
    output << '\n';
    if(ftrLineDraw || ftrGraphLine){
      if(a + 1 == pswCount) ftrLineGenerate(true, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern, output);
      else ftrLineGenerate(ftrLineDraw, ftrGraphLine, ftrLineDrawValue, ftrGraphLineCheckValue, ftrGraphLineDot, ftrLineDrawPattern, output);
    }
    if(ftrPswCounter){
      ftrGraphLineCheckValue[1] -= ftrPswCounterLength;
      ftrGraphLineCheckValue[2] -= ftrPswCounterLength;
      ftrLineDrawValue -= ftrPswCounterLength;
    }
  }
  cout << output.str() << endl << "Seed :" << generatorSeed << endl << "Restart (y/n/d/r/s/i/o/help) :";
  restartAsking:
  cin >> tempUserInput;
  if(tempUserInput == "y"){
    goto restartPassword;
  }
  else if(tempUserInput == "help"){
    cout << diag[18];
    goto restartAsking;
  }
  else if(tempUserInput == "d"){
    cout << "register int z : " << z << endl;
    cout << "int pswLength : " << pswLength << endl;
    cout << "int pswCount : " << pswCount << endl;
    cout << "int randomNumber : " << randomNumber << endl;
    cout << "int generatorSeed : " << generatorSeed << endl;
    cout << "int ftrGraphLineCheckValue[0] : " << ftrGraphLineCheckValue[0] << endl;
    cout << "int ftrGraphLineCheckValue[1] : " << ftrGraphLineCheckValue[1] << endl;
    cout << "int ftrGraphLineCheckValue[2] : " << ftrGraphLineCheckValue[2] << endl;
    cout << "int ftrPswCounterLength : " << ftrPswCounterLength << endl;
    cout << "bool ftrLineDraw : " << ftrLineDraw << endl;
    cout << "bool ftrPswCounter : " << ftrPswCounter << endl;
    cout << "bool ftrPswCounterNumber : " << ftrPswCounterNumber << endl;
    cout << "bool ftrGraphLine : " << ftrGraphLine << endl;
    cout << "bool autoAdjustSetting : " << autoAdjustSetting << endl;
    cout << "bool setupUpper : " << setupUpper << endl;
    cout << "bool setupLower : " << setupLower << endl;
    cout << "bool setupNumber : " << setupNumber << endl;
    cout << "bool settingDataSetup : " << settingDataSetup << endl;
    cout << "string pswCharacter : " << pswCharacter << endl;
    cout << "string settingData : " << settingData << endl;
    cout << "string ftrInput : " << ftrInput << endl;
    cout << "string tempUserInput : " << tempUserInput << endl;
    cout << "string tempConsoleOutput : " << tempConsoleOutput << endl;
    cout << "string ftrLineDrawPattern : " << ftrLineDrawPattern << endl;
    cout << "string ftrPswCounterPrev : " << ftrPswCounterPrev << endl;
    cout << "string ftrPswCounterNext : " << ftrPswCounterNext << endl;
    cout << "string ftrGraphLineCharacter : " << ftrGraphLineCharacter << endl;
    cout << "string ftrGraphLineDot : " << ftrGraphLineDot << endl;
    cout << "string ftrPswCounterString : " << ftrPswCounterString << endl;
    cout << "ostringstream characterSetup : " << characterSetup.str() << endl;
    cout << "ostringstream intLength : " << intLength.str() << endl;
    cout << "Restart (y/n/d/r/s/i/o/help) :";
    goto restartAsking;
  }
  else if(tempUserInput == "r"){
    z = 0;
    pswLength = 0;
    pswCount = 0;
    randomNumber = 0;
    generatorSeed = time(0);
    ftrGraphLineCheckValue[0] = 0;
    ftrGraphLineCheckValue[1] = 0;
    ftrGraphLineCheckValue[2] = 0;
    ftrPswCounterLength = 0;
    ftrLineDraw = false;
    ftrPswCounter = false;
    ftrPswCounterNumber = true;
    ftrGraphLine = false;
    autoAdjustSetting = true;
    setupUpper = false;
    setupLower = true;
    setupNumber = true;
    settingDataSetup = false;
    characterSetup.str("");
    intLength.str("");
    output.str("");
    pswCharacter.clear();
    settingData = "help";
    ftrInput = "n";
    tempUserInput = "n";
    tempConsoleOutput = "n";
    ftrLineDrawPattern = "-";
    ftrPswCounterPrev = "[";
    ftrPswCounterNext = "]";
    ftrGraphLineCharacter = "|";
    ftrGraphLineDot = "+";
    ftrPswCounterString.clear();
    strPrev = '<';
    strNext = '>';
    strSkip = '!';
    system("clear");
    goto restartPassword;
  }
  else if(tempUserInput == "s"){
    cout << "char strPrev : " << strPrev << endl;
    cout << "char strNext : " << strNext << endl;
    cout << "char strSkip : " << strSkip << endl;
    cout << "bool ftrLineDraw : " << ftrLineDraw << endl;
    cout << "string ftrLineDrawPattern : " << ftrLineDrawPattern << endl;
    cout << "bool ftrPswCounter : " << ftrPswCounter << endl;
    cout << "bool ftrPswCounterNumber : " << ftrPswCounterNumber << endl;
    cout << "string ftrPswCounterPre : " << ftrPswCounterPrev << endl;
    cout << "string ftrPswCounterNext  : " << ftrPswCounterNext << endl;
    cout << "bool ftrGraphLin : " << ftrGraphLine << endl;
    cout << "string ftrGraphLineCharacter : " << ftrGraphLineCharacter << endl;
    cout << "string ftrGraphLineDo : " << ftrGraphLineDot << endl;
    cout << "bool setupUpper : " << setupUpper << endl;
    cout << "bool setupLowe : " << setupLower << endl;
    cout << "bool setupNumber : " << setupNumber << endl;
    cout << "bool autoAdjustSetting : " << autoAdjustSetting << endl;
    cout << "int ftrLineDrawValue : " << ftrLineDrawValue << endl;
    cout << "int ftrGraphLineCheckValue[0] : " << ftrGraphLineCheckValue[0] << endl;
    cout << "int ftrGraphLineCheckValue[1] : " << ftrGraphLineCheckValue[1] << endl;
    cout << "int ftrGraphLineCheckValue[2] : " << ftrGraphLineCheckValue[2] << endl;
    cout << "Data Used : " << settingData << endl;
    cout << "Restart (y/n/d/r/s/i/o/help) :";
    goto restartAsking;
  }
  else if(tempUserInput == "o"){
    ofstream ofile;
    cout << "Warning! The contents in the file will be replaced by passwords excluding seed. Are you sure? use 'b' to backup the original for once (y/n/b) :";
    cin >> tempUserInput;
    if(tempUserInput == "b"){
      ifstream bfile;
      bfile.open("/storage/emulated/0/GeneratedPasswords.txt");
      if(bfile.fail()){
        bfile.open("C:/GeneratedPasswords.txt");
        if(bfile.fail()){
          cout << "Failed to backup a file." << endl << "Restart (y/n/d/r/s/i/o/help) :";
          bfile.close();
          goto restartAsking;
        }
      }
      ofstream kfile;
      kfile.open("/storage/emulated/0/Backup_GeneratedPasswords.txt");
      if(kfile.fail()){
        kfile.open("C:/Backup_GeneratedPasswords.txt");
        if(kfile.fail()){
          cout << "Failed to create a file." << endl << "Restart (y/n/d/r/s/i/o/help) :";
          kfile.close();
          goto restartAsking;
        }
      }
      while(!bfile.eof()){
        tempConsoleOutput.clear();
        bfile >> tempConsoleOutput;
        kfile << tempConsoleOutput << endl;
      }
      cout << "Successfully Backuped!" << endl;
      bfile.close();
      kfile.close();
    }
    else if(tempUserInput != "y"){
      cout << "Restart (y/n/d/r/s/i/o/help) :";
      goto restartAsking;
    }
    ofile.open("/storage/emulated/0/GeneratedPasswords.txt");
    if(ofile.fail()){
      ofile.open("C:/GeneratedPasswords.txt");
      if(ofile.fail()){
        cout << "Failed to create a file." << endl << "Restart (y/n/d/r/s/i/o/help) :";
        ofile.close();
        goto restartAsking;
      }
    }
    ofile << output.str();
    cout << "Passwords Saved!" << endl << "Restart (y/n/d/r/s/i/o/help) :";
    ofile.close();
    goto restartAsking;
  }
  else if(tempUserInput == "i"){
    ifstream ifile;
    cout << "Reading Passwords..." << endl << endl;
    ifile.open("/storage/emulated/0/GeneratedPasswords.txt");
    if(ifile.fail()){
      ifile.open("C:/GeneratedPasswords.txt");
      if(ifile.fail()){
        cout << "Failed to open a file." << endl << "Restart (y/n/d/r/s/i/o/help) :";
        ifile.close();
        goto restartAsking;
      }
    }
    z = 0;
    while(!ifile.eof()){
      tempConsoleOutput.clear();
      ifile >> tempConsoleOutput;
      cout << tempConsoleOutput << endl;
      z++;
    }
    ifile.close();
    z--;
    cout << z << " Passwords Found!" << endl << "Restart (y/n/d/r/s/i/o/help) :";
    goto restartAsking;
  }
  else exit(1);
  settingDataSet:
  if(settingDataSetup){
    strPrev = settingData[0];
    strNext = settingData[1];
    strSkip = settingData[2];
    ftrLineDraw = settingBool(settingData, strSkip, settingCounter);
    ftrLineDrawPattern = settingString(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrPswCounter = settingBool(settingData, strSkip, settingCounter);
    ftrPswCounterNumber = settingBool(settingData, strSkip, settingCounter);
    ftrPswCounterPrev = settingString(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrPswCounterNext = settingString(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrGraphLine = settingBool(settingData, strSkip, settingCounter);
    ftrGraphLineCharacter = settingString(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrGraphLineDot = settingString(settingData, strSkip, strPrev, strNext, settingCounter);
    setupUpper = settingBool(settingData, strSkip, settingCounter);
    setupLower = settingBool(settingData, strSkip, settingCounter);
    setupNumber = settingBool(settingData, strSkip, settingCounter);
    autoAdjustSetting = settingBool(settingData, strSkip, settingCounter);
    ftrLineDrawValue = settingInt(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrGraphLineCheckValue[0] = settingInt(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrGraphLineCheckValue[1] = settingInt(settingData, strSkip, strPrev, strNext, settingCounter);
    ftrGraphLineCheckValue[2] = settingInt(settingData, strSkip, strPrev, strNext, settingCounter);
    settingDataSetup = false;
    goto settingDataFinish;
  }
}
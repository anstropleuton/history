#include <iostream>
#include <sstream>
using namespace std;
int counter(int input){
  static int count;
  count += input;
  return count;
}
using namespace std;
bool settingBool(string input, char skip){
  bool output;
  cout << endl << endl << "sBol...";
  if(input[counter(0)] != skip){
    cout << endl << "Bool : " << input[counter(0)] << " : " << counter(0);
    if(input[counter(0)] == 'y') output = true;
    else output = false;
  }
  counter(1);
  return output;
}
string settingString(string setting, char skip, char prev, char next){
  register int z;
  ostringstream convert;
  string output;
  char input;
  input = setting[counter(0)];
  cout << endl << endl << "sStr... : " << setting[counter(0)] << " : " << input << " --- " << prev << " ||| " << next << " +++ " << skip;
  if(0 == 0){
    for(z = 0; z < 100; z++){
      input = setting[(counter(0) + z + 1)];
      cout << endl << "sStr For..." << setting[counter(0)] << " : " << input << " --- " << prev << " ||| " << next << " +++ " << skip;
      if(input != next){
        output += input;
        cout << endl << "sStr : " << z << " : " << counter(0) << " : " << input << output << endl;
      }
      else{
        output = convert.str();
        counter((output.length() + 2));
        cout << endl << "sIntComplete : " << z << " : " << counter(0) << " : " << input << output << endl;
      }
    }
  }
  return output;
}
int settingInt(string setting, char skip, char prev, char next){
  register int z;
  ostringstream len;
  int output = 0;
  char input;
  input = setting[counter(0)];
  cout << endl << endl << "sInt..." << setting[counter(0)] << " : " << input << " --- " << prev << " ||| " << next << " +++ " << skip;
  if(input == prev){
    for(z = 0; z < -1; z++){
      input = setting[(counter(0) + z + 1)];
      cout << endl << "sInt For..." << setting[counter(0)] << " : " << input << " --- " << prev << " ||| " << next << " +++ " << skip;
      if(input != next){
        output += input;
        cout << endl << "sInt : " << z << " : " << counter(0) << " : " << input << " : " << output << endl;
      }
      else{
        len << output;
        counter((len.str().length() + 2));
        cout << endl << "sIntComplete : " << z << " : " << counter(0) << " : " << input << " : " << output << endl;
        z = -1;
      }
    }
  }
  return output;
}
int main(){
  string pswCharacter, settingData = "help", ftrInput = "n", tempUserInput = "n", tempConsoleOutput = "n", ftrLineDrawPattern = "-", ftrPswCounterprev = "[", ftrPswCounterNext = "]", ftrGraphLineCharacter = "|", ftrGraphLineDot = "+", ftrPswCounterString;
  char strprev = '<', strNext = '>', strSkip = '!';
  int pswLength = 1, pswCount = 1, randomNumber = 0, generatorSeed = time(0), ftrGraphLineCheckValue[3], ftrPswCounterLength;
  register int z = 0;
  bool ftrLineDraw = false, ftrPswCounter = false, ftrPswCounterNumber = true, ftrGraphLine = false, autoAdjustSetting = true, setupUpper = false, setupLower = true, setupNumber = true, settingDataSetup = false;
  int ftrLineDrawValue = pswCount;
  ostringstream characterSetup, intLength;
  string diag[] = {
    "0 How long you want your password to be (1+) :",
    "1 Invalid Number, There has to be more than 0 characters. Try again (1+) :",
    "2 How many passwords you want to generate (1+) :",
    "3 Invalid Number, There cannot be less than 1 passwords. Try again (1+) :",
    "4 Line Draw is a feature that draws lines made of your choice at the start of generating password and at the end of generated passwords. This is just a decorational feature! (y/n/help) :",
    "5 This is the feature that adds a count value of passwords generated. e.g.\n(1):<password>\n(2):<password>\n... Etc. This feature is also customizable! (y/n/help) :",
    "6 This feature draws lines vertically to make the passwords organized like a excel sprevad sheet. But it requires 2 characters to use. One for the vertical line and another for the dot used to connect horisontal line. The horisontal line is Line Pattern if you entered before. This feature is also for decoration (y/n/help) :",
    "7 The graph chatracter used to make Graph e.g. '|' :",
    "8 This feature uses characters from prevvious features, \"Line Draw\" and the character is used from \"Line Draw Pattern\" and it seems like you did not used that feature, enter the character for the program to use to make graph e.g. '-' :",
    "9 The Dot characer used to connect vertical line and horizontal line e.g. '+' :",
    "10 Adjusts all the settings automatically so it's stable for the generator (Warning, If you don't use this, chances are the program will be unstable or might Crash) (y/n/help) :",
    "11 ftrLineDrawValue is the feature that draws line using characters. This value determines how many characters to use (<number>) :",
    "12 ftrGraphLineCheckValue is the setting that is used to check if the matching conditions are met for the ftrGraphLineDot to ube used (<number>) :",
    "13 Use lower cased characters as password random letters (y/n/help) :",
    "14 Use upper cased characters as password random letters (y/n/help) :",
    "15 Use numbers as password random numbers (y/n/help) :",
    "16 Seed determines how your password is gonna look. The last letter \"Seed:<number>\" is the seed for current password. You can share seed but better not because others can generate exact same passwords just like yours. If you found something cool and you aren't using this program for password then feel free to share seed as well as password length and count as that's necessare to generate exact same passwords. The 't' means to use the system time as the seed for random passwords to generate (y/t/help) :",
    "17 Change settings that is used to generate password (Warning! Inserting a 'Space' character might crash the program) You can also input all the settings at once if you like and the program will detect them and use them accordingly (the first character you type as data is going to be the start of a string value and the next is end of the string then you can use them while entering string data. e.g. you entered \"<>\" then whenever you use '<' the program will detect that and use at start of the string and then it will search for '>' and then it will conclude the value and settes up the next and the third character will be skip character) (y/n/help/<data>) :",
    "18 Restart all of the code (Doesn't resets the values and settings unless selected 'r' which briengs everything to default) The 'd' sends all the information the code has used and all the settings values and stuffs for debug purposes (y/n/help/d/r) :",
    "19 Some optional features to enable to make your passwords look a little bit fancier! (y/n/help) :"
  };
  settingData = "<>!y<->yy<[><]>y<|><+>nyyy!!!!";
  settingDataSetup = true;
  settingDataSet:
  if(settingDataSetup){
    counter(3);
    strprev = settingData[0];
    strNext = settingData[1];
    strSkip = settingData[2];
    ftrLineDraw = settingBool(settingData, strSkip);
    ftrLineDrawPattern = settingString(settingData, strSkip, strprev, strNext);
    ftrPswCounter = settingBool(settingData, strSkip);
    ftrPswCounterNumber = settingBool(settingData, strSkip);
    ftrPswCounterprev = settingString(settingData, strSkip, strprev, strNext);
    ftrPswCounterNext = settingString(settingData, strSkip, strprev, strNext);
    ftrGraphLine = settingBool(settingData, strSkip);
    ftrGraphLineCharacter = settingString(settingData, strSkip, strprev, strNext);
    ftrGraphLineDot = settingString(settingData, strSkip, strprev, strNext);
    setupUpper = settingBool(settingData, strSkip);
    setupLower = settingBool(settingData, strSkip);
    setupNumber = settingBool(settingData, strSkip);
    autoAdjustSetting = settingBool(settingData, strSkip);
    ftrLineDrawValue = settingInt(settingData, strSkip, strprev, strNext);
    ftrGraphLineCheckValue[0] = settingInt(settingData, strSkip, strprev, strNext);
    ftrGraphLineCheckValue[1] = settingInt(settingData, strSkip, strprev, strNext);
    ftrGraphLineCheckValue[2] = settingInt(settingData, strSkip, strprev, strNext);
  }
  cout << "\n\n\n\n\nchar strprev : " << strprev << endl;
  cout << "char strNext : " << strNext << endl;
  cout << "char strSkip : " << strSkip << endl;
  cout << "bool ftrLineDraw : " << ftrLineDraw << endl;
  cout << "string ftrLineDrawPattern : " << ftrLineDrawPattern << endl;
  cout << "bool ftrPswCounter : " << ftrPswCounter << endl;
  cout << "bool ftrPswCounterNumber : " << ftrPswCounterNumber << endl;
  cout << "string ftrPswCounterprev : " << ftrPswCounterprev << endl;
  cout << "string ftrPswCounterNext : " << ftrPswCounterNext << endl;
  cout << "bool ftrGraphLine : " << ftrGraphLine << endl;
  cout << "string ftrGraphLineCharacter : " << ftrGraphLineCharacter << endl;
  cout << "string ftrGraphLineDot : " << ftrGraphLineDot << endl;
  cout << "bool setupUpper : " << setupUpper << endl;
  cout << "bool setupLower : " << setupLower << endl;
  cout << "bool setupNumber : " << setupNumber << endl;
  cout << "bool autoAdjustSetting : " << autoAdjustSetting << endl;
  cout << "int ftrLineDrawValue : " << ftrLineDrawValue << endl;
  cout << "int ftrGraphLineCheckValue[0] : " << ftrGraphLineCheckValue[0] << endl;
  cout << "int ftrGraphLineCheckValue[1] : " << ftrGraphLineCheckValue[1] << endl;
  cout << "int ftrGraphLineCheckValue[2] : " << ftrGraphLineCheckValue[2] << endl;
}
//<>!y<->yy<[><]>y<|><+>nyyy!!!!
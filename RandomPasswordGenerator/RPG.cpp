#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <sstream>
#include <string>
using namespace std;
int main(){
  string randomCharacterList = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", lineCharacter = "-", skipLineRender, customLineValue, characterChangeInput, characterListEditSelector, randomizerSeed, settings, passwordCountDisplayInput, randomizerSeedInput, lineDrawBottomString, removeCharacterPreset,  characterRemovePresetSelector, passwordStyleInput, passwordStylePick, passwordDashType, passwordPatternOverlapType, showExistingCharacters, settingData, convertedSettings, convertedNextSettings;
  int a, b, c, characterSelector, passwordCharacterLength, passwordCount, lineDrawValue, randomCharacterListLength = randomCharacterList.length(), pl, passwordCountLineDraw = 0, lineDrawValueBottom, bottomLine, topLine, lineLengthBottomCount = 0, passwordPatternCount, characterSettingData = 0, convertedSettingsInt;
  bool breaker = false;
  randomizerSeed = (time(0));
  srand(randomizerSeed);
  ostringstream convertLineLengthBottom, convertRandomCharacterList, convertCharacterChangeData, convertSettingData;
  cout << "How many passwords: ";
  cin >> passwordCount;
  cout << "Password Length: ";
  cin >> passwordCharacterLength;
  lineDrawValue = passwordCharacterLength;
  cout << "Change Settings? (y/n): ";
  cin >> settings;
  if(settings == "data"){
    cout << "Enter Data: ";
    cin >> settingData;
    convertSettingData << settingData[0];
    convertSettingData.str("");
    convertedSettings.clear();
    convertedSettings = convertSettingData.str();
    skipLineRender = convertedSettings;
    convertSettingData.str("");
    convertedSettings.clear();
    convertSettingData << settingData[1];
    convertedSettings = convertSettingData.str();
    if(convertedSettings != "n"){
      stringstream convertSettingInt(convertedSettings);
      convertSettingInt >> convertedSettingsInt;
      lineDrawValue = convertedSettingsInt;
      cout << "1 Checked . " << convertedSettingsInt << endl;
    }
    convertSettingData.str("");
    convertedSettings.clear();
    convertSettingData << settingData[2];
    convertedSettings = convertSettingData.str();
    if(convertedSettings != "n"){
      lineCharacter = convertedSettings;
      cout << "2 Checked . " << convertedSettings << endl;
    }
    convertSettingData.str("");
    convertedSettings.clear();
    convertSettingData << settingData[3];
    convertedSettings = convertSettingData.str();
    if(convertedSettings == "n"){
      characterListEditSelector = convertedSettings;
      cout << "3 Checked . " << convertedSettings << endl;
    }
    else{
      convertSettingData.str("");
      convertedSettings.clear();
      convertSettingData << settingData[4];
      convertedSettings = convertSettingData.str();
      if(convertedSettings == "<"){
        cout << "4 Checking . " << convertedSettings << endl;
        characterSettingData = 1;
        for(c = 0; c < characterSettingData; c++){
          cout << ":4.loop." << c << endl;
          convertSettingData.str("");
          convertedNextSettings.clear();
          convertSettingData << settingData[(c + 5)];
          convertedNextSettings = convertSettingData.str();
          if(convertedNextSettings != ">"){
            cout << ":4.if" << endl;
            convertCharacterChangeData << convertedNextSettings;
            cout << ":debug.sD." << convertedNextSettings << endl;
            characterSettingData++;
            cout << ":4.char." << characterSettingData << endl << "-----" << endl;
          }
        }
        characterSettingData--;
        cout << "c: " << c << endl;
        cout << "cSd: " << characterSettingData << endl;
        cout << "4 Checked" << endl;
      }
      if(characterListEditSelector == "a"){
        characterChangeInput = convertCharacterChangeData.str();
        randomCharacterList = (randomCharacterList + characterChangeInput);
        randomCharacterListLength = randomCharacterList.length();
      }
      else if(characterListEditSelector == "e"){
        randomCharacterList = convertCharacterChangeData.str();
        randomCharacterListLength = randomCharacterList.length();
      }
      else if(characterListEditSelector == "r"){
        characterChangeInput = convertCharacterChangeData.str();
        for(a = 0, b = 0; a < randomCharacterList.length(); a++){
          while(b < characterChangeInput.length()){
            if(characterChangeInput[b] == randomCharacterList[a] && !breaker){
              breaker = true;
            }
            b++;
          }
          if(!breaker){
            convertRandomCharacterList << randomCharacterList[a];
          }
          b = 0;
          breaker = false;
        }
        randomCharacterList = convertRandomCharacterList.str();
        randomCharacterListLength = randomCharacterList.length();
      }
      else{
        randomCharacterListLength = randomCharacterList.length();
      }
    }
    convertSettingData.str("");
    convertedSettings.clear();
    convertSettingData << settingData[characterSettingData + 6];
    convertedSettings = convertSettingData.str();
    passwordCountDisplayInput = convertedSettings;
    cout << "Debug passwordCountDisplayInput: " << passwordCountDisplayInput << endl;
    if(passwordCountDisplayInput == "y"){
      passwordCountLineDraw = 5;
      cout << "\"Y\" Check" << endl;
    }
    else{
      cout << "\"N\" Check" << endl;
      passwordCountLineDraw = 0;
    }
    convertSettingData.str("");
    convertedSettings.clear();
    convertSettingData << settingData[characterSettingData + 7];
    convertedSettings = convertSettingData.str();
  }
  else{
    if(settings == "y"){
      cout << "Skip Line Draw (y/n): ";
      cin >> skipLineRender;
      if(skipLineRender != "y"){
        cout << "Custom Line Length (y/n): ";
        cin >> customLineValue;
        if(customLineValue == "y"){
          cout << "Length: ";
          cin >> lineDrawValue;
        }
        cout << "Custom Line Character: ";
        cin >> lineCharacter;
      }
      cout << "Characters Edit (e), Add (a), Remove (r), None (n): ";
      cin >> characterListEditSelector;
      if(characterListEditSelector == "a"){
        cout << "Characters (e.g. \"-=[]\\;',./'\"): ";
        cin >> characterChangeInput;
        randomCharacterList = (randomCharacterList + characterChangeInput);
        randomCharacterListLength = randomCharacterList.length();
      }
      else if(characterListEditSelector == "e"){
        cout << "Characters: ";
        cin >> randomCharacterList;
        randomCharacterListLength = randomCharacterList.length();
      }
      else if(characterListEditSelector == "r"){
        cout << "Use Presets? (y/n): ";
        cin >> removeCharacterPreset;
        if(removeCharacterPreset == "y"){
          cout << "CAPS only (a), small only (b), Numbers only (c), Letters only (d): ";
          cin >> characterRemovePresetSelector;
          if(characterRemovePresetSelector == "a"){
            characterChangeInput = "abcdefghijklmnopqrstuvwxyz";
          }
          else if(characterRemovePresetSelector == "b"){
             characterChangeInput = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
          }
          else if(characterRemovePresetSelector == "c"){
            characterChangeInput = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";;
          }
          else if(characterRemovePresetSelector == "d"){
            characterChangeInput = "0123456789";
          }
          else{
            cout << "Invalid, Select your own choices" << endl;
          }
        }
        else{
          cout << "Show the existing list? (y/n): ";
          cin >> showExistingCharacters;
          if(showExistingCharacters == "y"){
            cout << randomCharacterList << endl;
          }
          cout << "Remove stuffs from list: ";
          cin >> characterChangeInput;
        }
        for(a = 0, b = 0; a < randomCharacterList.length(); a++){
          while(b < characterChangeInput.length()){
            if(characterChangeInput[b] == randomCharacterList[a] && !breaker){
              breaker = true;
            }
            b++;
          }
          if(!breaker){
            convertRandomCharacterList << randomCharacterList[a];
          }
          b = 0;
          breaker = false;
        }
        randomCharacterList = convertRandomCharacterList.str();
        randomCharacterListLength = randomCharacterList.length();
      }
      else{
        randomCharacterListLength = randomCharacterList.length();
      }
      cout << "Show Password Count Number (y/n): ";
      cin >> passwordCountDisplayInput;
      if(passwordCountDisplayInput == "y"){
        passwordCountLineDraw = 5;
      }
      else{
        passwordCountLineDraw = 0;
      }
      cout << "Custom Seed (y/n): ";
      cin >> randomizerSeedInput;
      if(randomizerSeedInput == "y"){
        cout << "Seed: ";
        cin >> randomizerSeed;
        srand(randomizerSeed);
      }
      cout << "Configured!" << endl;
    }
  }
  cout << "Style (y/n): ";
  cin >> passwordStyleInput;
  if(passwordStyleInput == "y"){
    cout << "Pattern (a), Algorythm (b): ";
    cin >> passwordStylePick;
    if(passwordStylePick == "a"){
      cout << "Pattern Count: ";
      cin >> passwordPatternCount;
    }
  }
  int passwordDividerStart[passwordPatternCount], passwordDivisionValue[passwordPatternCount];
  string divisionCharacter[passwordPatternCount];
  bool passwordDivider[passwordPatternCount][passwordCharacterLength], passwordDivisionComplete[passwordPatternCount];
  if(passwordStyleInput == "y"){
    if(passwordStylePick == "a"){
      for(a = 0; a < passwordPatternCount; a++){
        cout << "(" << a << "): Minimum Division Number: ";
        cin >> passwordDividerStart[a];
        if(passwordDividerStart[a] >= passwordCharacterLength){
          passwordDividerStart[a] = (passwordCharacterLength / 2);
          cout << "(" << (a + 1) << "): Must be half as Password Length or less, Setting it to " << passwordDividerStart[a] << endl;
        }
        else if(passwordDividerStart[a] < 1){
          passwordDividerStart[a] = 1;
          cout << "(" << (a + 1) << "): Must be 1 or more, Setting it to 1";
        }
        cout << "(" << (a + 1) << "): Division Symbol: ";
        cin >> divisionCharacter[a];
        passwordDivisionComplete[a] = false;
        for(b = 0; b < passwordDividerStart[a]; b++){
          passwordDivider[a][b] = false;
        }
        passwordDivider[a][0] = false;
        for(b = passwordDividerStart[a]; !passwordDivisionComplete[a] ; b++){
          if((passwordCharacterLength % b) == 0 && !passwordDivisionComplete[a]){
            passwordDivisionValue[a] = b;
            passwordDivisionComplete[a] = true;
          }
          if(b == passwordCharacterLength){
            passwordDivisionValue[a] = passwordCharacterLength;
            passwordDivisionComplete[a] = true;
          }
        }
        for(b = passwordDividerStart[a]; b < passwordCharacterLength; b++){
          if(b % passwordDivisionValue[a] == 0){
            passwordDivider[a][(b - 1)] = true;
          }
          else{
            passwordDivider[a][(b - 1)] = false;
            passwordDivider[a][b] = false;
            passwordDivider[a][(b + 1)] = false;
          }
        }
      }
    }
  }
  if(passwordCountDisplayInput == "y"){
    topLine = (lineDrawValue + passwordCountLineDraw + 1);
  }
  else{
    topLine = lineDrawValue;
  }
  convertLineLengthBottom << lineLengthBottomCount;
  lineDrawBottomString = convertLineLengthBottom.str();
  lineDrawValueBottom = lineDrawBottomString.length();
  if(passwordCountDisplayInput == "y"){
    bottomLine = (lineDrawValue + passwordCountLineDraw + lineDrawValueBottom);
  }
  else{
    bottomLine = lineDrawValue;
  }
  if(skipLineRender != "y"){
    if(passwordStylePick == "a"){
      for(c = 0; c < passwordPatternCount; c++){
        topLine = topLine + (passwordCharacterLength / passwordDivisionValue[c]) - 1;
      }
    }
    for(c = 0; c < topLine; c++){
      cout << lineCharacter;
      lineLengthBottomCount++;
    }
    cout << endl;
  }
  for(c = 1; c <= passwordCount; c++){
    if(passwordCountDisplayInput == "y"){
      cout << '(' << c << ") : ";
    }
    if(passwordStyleInput != "y"){
      for(c = 0; c < passwordCharacterLength; c++){
        characterSelector = rand() %randomCharacterListLength;
        cout << randomCharacterList[characterSelector];
      }
    }
    else{
      if(passwordStylePick == "a"){
        for(a = 0; a < passwordCharacterLength; a++){
          characterSelector = rand() %randomCharacterListLength;
          cout << randomCharacterList[characterSelector];
          for(b = 0; b < passwordPatternCount; b++){
            if(passwordDivider[b][a] == true){
              cout << divisionCharacter[b];
            }
          }
        }
      }
    }
    cout << endl;
  }
  if(skipLineRender != "y"){
    if(passwordStylePick == "a"){
      for(c = 0; c < passwordPatternCount; c++){
        bottomLine = bottomLine + (passwordCharacterLength / passwordDivisionValue[c]) - 1;
      }
    }
    for(c = 0; c < bottomLine; c++){
      cout << lineCharacter;
    }
    cout << endl; 
  }
  cout << "Seed: " << randomizerSeed << endl;
}
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
using namespace std;
int intExtract(string input){
    stringstream convert;
    int output = 0, z;
    bool negative = false;
    for(z = 0; z < input.length(); z++){
        if(input[z] >= '0' && input[z] <= '9') convert << input[z];
        if(input[z] == '-') negative = true;
    }
    convert >> output;
    if(negative) output = output * -1;
    return output;
}
void ftrLineGenerate(bool line, bool graph, int length, int data[3], char dot, char pattern, stringstream &output){
    int z;
    if(line){
        if(graph){
            for(z = 0; z < length; z++){
                if(z == data[0] || z == data[1] || z == data[2]) output << dot;
                else output << pattern;
            }
        }
        else for(z = 0; z < length; z++) cout << pattern;
        output << '\n';
    }
}
string ftrPswCounterLetter(int input){
    stringstream convert;
    string output;
    int value[2147483647], count = 0, z;
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
bool settingBool(string input, char skip, int& counter){
    bool output = false;
    counter++;
    if(input[(counter - 1)] != skip){if(input[(counter - 1)]){output = true;}}
    return output;
}
string settingString(string input, char skip, char prev, char next, int& counter){
    string output;
    stringstream convert;
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
char settingChar(string input, char skip, char prev, char next, int& counter){
    char output;
    stringstream convert;
    if(input[counter] != skip){
        counter++;
        output = input[counter];
        counter++;
    }
    counter++;
    return output;
}
int settingInt(string input, char skip, char prev, char next, int& counter){
    int output;
    stringstream convert;
    if(input[counter] != skip){
        if(input[counter] == prev){
            while(input[(counter + 1)] != next){
                counter++;
                convert << input[counter];
            }
            convert >> output;
            counter++;
        }
    }
    counter++;
    return output;
}
int main(){
    string pswCharacter, settingData = "help", tempUserInput = "n", consoleCache, ftrPswCounterString, windows = "C:/Generated_Passwords.txt", android = "/storage/emulated/0/Generated_Passwords.txt", backupWindows, backupAndroid;
    char strPrev = '<', strNext = '>', strSkip = '!', ftrLineDrawPattern = '-', ftrPswCounterPrev = '[', ftrPswCounterNext = ']', ftrGraphLineCharacter = '|', ftrGraphLineDot = '+';
    int pswLength = 1, pswCount = 1, randomNumber = 0, generatorSeed = time(0), ftrGraphLineCheckValue[3], ftrPswCounterLength, settingCounter = 3, z;
    bool ftrLineDraw = false, ftrPswCounter = false, ftrPswCounterNumber = true, ftrGraphLine = false, autoAdjustSetting = true, setupUpper = false, setupLower = true, setupNumber = true, settingDataSetup = false, fileBackuped = false, fileFailed = false;
    int ftrLineDrawValue = pswCount;
    stringstream characterSetup, intLength, output, backup, fileBackupString;
    string diag[] = {
        "How long you want your password to be?\n",
        "Invalid Number, There has to be more than 0 characters. Try again.\n",
        "How many passwords you want to generate?\n",
        "Invalid Number, There cannot be less than 1 passwords. Try again.\n",
        "Line Draw is a feature that draws lines made of your choice at the start of generating password and at the end of generated passwords. This is just a decoration feature!\n",
        "This is the feature that adds a count value of passwords generated. e.g.\n(1):<password>\n(2):<password>\n... Etc. This feature is also customization!\n",
        "This feature draws lines vertically to make the passwords organized like a excel spread sheet. But it requires 2 characters to use. One for the vertical line and another for the dot used to connect horizontal line. The horizontal line is Line Pattern if you entered before. This feature is also for decoration!\n",
        "The graph character used to make Graph e.g. '|'.\n",
        "This feature uses characters from Previous features, \"Line Draw\" and the character is used from \"Line Draw Pattern\" and it seems like you did not used that feature, enter the character for the program to use to make graph e.g. '-'.\n",
        "The Dot character used to connect vertical line and horizontal line e.g. '+'.\n",
        "Adjusts all the settings automatically so it's stable for the generator (Warning, If you don't use this, chances are the program will be unstable or might Crash).\n",
        "ftrLineDrawValue is the feature that draws line using characters. This value determines how many characters to use.\n",
        "ftrGraphLineCheckValue is the setting that is used to check if the matching conditions are met for the ftrGraphLineDot to be used.\n",
        "Use lower cased characters as password random letters?\n",
        "Use upper cased characters as password random letters?\n",
        "Use numbers as password random numbers?\n",
        "Seed determines how your password is gonna look. The last letter \"Seed:<number>\" is the seed for current password. You can share seed but better not because others can generate exact same passwords just like yours. If you found something cool and you aren't using this program for password then feel free to share seed as well as password length and count as that's necessary to generate exact same passwords. The 't' means to use the system time as the seed for random passwords to generate.\n",
        "Change settings that is used to generate password (Warning! Inserting a 'Space' character might crash the program) You can also input all the settings at once if you like and the program will detect them and use them accordingly (the first character you type as data is going to be the start of a string value and the next is end of the string then you can use them while entering string data. e.g. you entered \"<>\" then whenever you use '<' the program will detect that and uses it at start of the string and then it will search for '>' and then it will conclude the value and sets up the next and the third character will be skip character. Then what's left is all data, type just 'o' to get order).\n",
        "Restart all of the code (Doesn't resets the values and settings unless selected 'r' which brings everything to default) The 'd' sends all the information the code has used and all the settings values and stuffs for debug purposes, The 's' sends the input debug used by settingData. The 'o' to output the passwords into a file which will be created in '/storage/emulated/0/' or 'C:/'. You can also read contents from the file my typing 'i' if you don't know what exists in it!\n",
        "Some optional features to enable to make your passwords look a little bit fancier!\n",
        "Orders of the datas used in settings (fill in the rest with skip character)\n\nstrPrev;\nstrNext;\nstrSkip;\nftrLineDraw;\nftrLineDrawPattern;\nftrPswCounter;\nftrPswCounterNumber;\nftrPswCounterPrev;\nftrPswCounterNext;\nftrGraphLine;\nftrGraphLineCharacter;\nftrGraphLineDot;\nsetupUpper;\nsetupLower;\nsetupNumber;\nautoAdjustSetting;\nftrLineDrawValue;\nftrGraphLineCheckValue[0];\nftrGraphLineCheckValue[1];\nftrGraphLineCheckValue[2];\n\n",
        "This character is used before the password Numbers or Letters are shown.\n",
        "This character is used after the password Numbers or Letters are shown.\n"
    };
    cout << "Note that you can use \"help\" anywhere in the input in the program to get a brief help about it! And you can use \"back\" if you want to go back to previous conversation!" << endl;
    restartPassword:
    output.str("");
    pswLengthAsking:
    cout << "Password Length (1+):";
    cin >> tempUserInput;
    if(tempUserInput == "help"){
        cout << diag[0];
        goto pswLengthAsking;
    }
    else if(tempUserInput == "back"){
        cout << "There is no step back." << endl;
        goto pswLengthAsking;
    }
    else pswLength = intExtract(tempUserInput);
    if(pswLength <= 0){
        cout << diag[1];
        goto pswLengthAsking;
    }
    pswCountAsking:
    cout << "Passwords Count (1+):";
    cin >> tempUserInput;
    if(tempUserInput == "help"){
        cout << diag[2];
        goto pswCountAsking;
    }
    else if(tempUserInput == "back") goto pswLengthAsking;
    else pswCount = intExtract(tempUserInput);
    if(pswCount <= 0){
        cout << diag[3];
        goto pswCountAsking;
    }    ftrLineDrawValue = pswCount;
    settingDataAsking:
    cout << "Change Settings (y/n/o/<data>) :";
    cin >> settingData;
    if(settingData == "y"){
        ftrEnableAsking:
        cout << "Enable features (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput == "y"){
            ftrLineDrawAsking:
            cout << "Line Draw (y/n) :";
            cin >> tempUserInput;
            if(tempUserInput == "y"){
                ftrLineDrawPatternAsking:
                cout << "Line Pattern e.g. '-' :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[4];
                    goto ftrLineDrawPatternAsking;
                }
                else if(tempUserInput == "back") goto ftrLineDrawAsking;
                else ftrLineDrawPattern = tempUserInput[0];
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
            else if(tempUserInput == "back") goto ftrEnableAsking;
            else if(tempUserInput == "help"){
                cout << diag[4];
                goto ftrLineDrawAsking;
            }
            else ftrLineDraw = false;
            ftrPswCounterAsking:
            cout << "Password Counter (y/n) :";
            cin >> tempUserInput;
            if(tempUserInput == "y"){
                ftrPswCounterConfigAsking:
                cout << "Use Numbers/Letters (n/l) :";
                cin >> tempUserInput;
                if(tempUserInput == "l"){
                    ftrPswCounterNumber = false;
                    consoleCache = "Letter";
                    cout << "Configured to use Letters!" << endl;
                }
                else{
                    ftrPswCounterNumber = true;
                    consoleCache = "Number";
                    cout << "Configured to use Numbers!" << endl;
                }
                ftrPswCounterPrevAsking:
                cout << "Before " << consoleCache <<" e.g. '[' :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[21];
                    goto ftrPswCounterPrevAsking;
                }
                else if(tempUserInput == "back") goto ftrPswCounterConfigAsking;
                else if(tempUserInput == "none") ftrPswCounterPrev = '\0';
                else ftrPswCounterPrev = tempUserInput[0];
                ftrPswCounterNextAsking:
                cout << "After " << consoleCache << " e.g. ']' :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[22];
                    goto ftrPswCounterNextAsking;
                }
                else if(tempUserInput == "back") goto ftrPswCounterPrevAsking;
                else if(tempUserInput == "none") ftrPswCounterNext = '\0';
                else ftrPswCounterNext = tempUserInput[0];
                if(ftrPswCounterNumber) consoleCache = "20";
                else consoleCache = "AB";
                cout << "Password Count might look like this :" << endl << ftrPswCounterPrev << consoleCache << ftrPswCounterNext << "a1b2c3x7y8z9" << endl << "Confirm (y/n) :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[5];
                    goto ftrPswCounterAsking;
                }
                else if(tempUserInput == "back") goto ftrPswCounterNextAsking;
                else if(tempUserInput != "y"){
                    cout << "Try Again." << endl;
                    goto ftrPswCounterConfigAsking;
                }
                else ftrPswCounter = true;
            }
            else if(tempUserInput == "back") goto ftrLineDrawAsking;
            else if(tempUserInput == "help"){
                cout << diag[5];
                goto ftrPswCounterAsking;
            }
            else ftrPswCounter = false;
            ftrGraphLineAsking:
            cout << "Graph Line (y/n) :";
            cin >> tempUserInput;
            if(tempUserInput == "y"){
                ftrGraphLineCharacterAsking:
                cout << "Graph Character e.g. '|' :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[7];
                    goto ftrGraphLineCharacterAsking;
                }
                else if(tempUserInput == "back") goto ftrGraphLineAsking;
                else ftrGraphLineCharacter = tempUserInput[0];
                if(!ftrLineDraw){
                    ftrGraphHorizontalAsking:
                    cout << "Horizontal Line e.g. '-' :";
                    cin >> tempUserInput;
                    if(tempUserInput == "help"){
                        cout << diag[8];
                        goto ftrGraphHorizontalAsking;
                    }
                    else if(tempUserInput == "back") goto ftrGraphLineCharacterAsking;
                    else ftrLineDrawPattern = tempUserInput[0];
                }
                else cout << "Using Line Draw Character as Horizontal Line." << endl;
                ftrGraphLineDotAsking:
                cout << "Dot Character e.g. '+' :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[9];
                    goto ftrGraphLineDotAsking;
                }
                else if(tempUserInput == "back") goto ftrGraphLineCharacterAsking;
                else ftrGraphLineDot = tempUserInput[0];
                cout << ftrGraphLineDot;
                for(z = 0; z < 12; z++) cout << ftrLineDrawPattern;
                cout << ftrGraphLineDot << endl << ftrGraphLineCharacter << "a1b2c3x7y8z9" << ftrGraphLineCharacter << endl << ftrGraphLineDot;
                for(z = 0; z < 12; z++) cout << ftrLineDrawPattern;
                cout << ftrGraphLineDot << endl << "Confirm (y/n) :";
                cin >> tempUserInput;
                if(tempUserInput == "help"){
                    cout << diag[6];
                    goto ftrGraphLineAsking;
                }
                else if(tempUserInput == "back") goto ftrGraphLineDotAsking;
                else if(tempUserInput != "y"){
                    cout << "Try Again :";
                    goto ftrGraphLineCharacterAsking;
                }
                else ftrGraphLine = true;
            }
            else if(tempUserInput == "back") goto ftrPswCounterAsking;
            else if(tempUserInput == "help"){
                cout << diag[6];
                goto ftrGraphLineAsking;
            }
            else ftrGraphLine = false;
        }
        else if(tempUserInput == "back") goto settingDataAsking;
        else if(tempUserInput == "help"){
            cout << diag[19];
            goto ftrEnableAsking;
        }
        setupUpperAsking:
        cout << "Upper characters (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput == "y") setupUpper = true;
        else if(tempUserInput == "back") goto ftrEnableAsking;
        else if(tempUserInput == "help"){
            cout << diag[13];
            goto setupUpperAsking;
        }
        else setupLower = false;
        setupLowerAsking:
        cout << "Lower characters (y/n) :";
        cin >> tempUserInput;
        if(tempUserInput == "y") setupLower = true;
        else if(tempUserInput == "help"){
            cout << diag[14];
            goto setupLowerAsking;
        }
        else setupLower = false;
        if(setupUpper || setupLower){
            setupNumberAsking:
            cout << "Numbers (y/n) :";
            cin >> tempUserInput;
            if(tempUserInput == "y") setupNumber = true;
            else if(tempUserInput == "help"){
                cout << diag[15];
                goto setupNumberAsking;
            }
            else setupNumber = false;
        }
        generatorSeedAsking:
        cout << "Generator Seed (<seed>/t) :";
        cin >> tempUserInput;
        if(tempUserInput == "help"){
            cout << diag[16];
            goto generatorSeedAsking;
        }
        else if(tempUserInput == "t") generatorSeed = time(0);
        else generatorSeed = intExtract(tempUserInput);
        autoAdjustSettingAsking:
        cout << "Auto Adjust (very recommended) (y/n) :";
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
    else if(settingData == "back") goto pswCountAsking;
    else if(settingData == "help"){
        cout << diag[17];
        goto settingDataAsking;
    }
    else if(settingData == "o"){
        cout << diag[20];
        goto settingDataAsking;
    }
    else if(settingData != "n"){
        settingDataSetup = true;
        goto settingDataSet;
        settingDataFinish:
        cout << "Configure Success!" << endl;
    }
    if(autoAdjustSetting){
        if(ftrPswCounter){
            ftrLineDrawValue = pswLength + 1;
            if(ftrPswCounterNext != '\0') ftrLineDrawValue++;
            if(ftrPswCounterPrev != '\0') ftrLineDrawValue++;
        }
        else ftrLineDrawValue = pswLength;
        if(ftrGraphLine){
            ftrLineDrawValue += 2;
            if(ftrPswCounter){
                ftrLineDrawValue += 1;
                ftrGraphLineCheckValue[0] = 0;
                ftrGraphLineCheckValue[1] = 2;
                if(ftrPswCounterNext != '\0') ftrGraphLineCheckValue[1]++;
                if(ftrPswCounterPrev != '\0') ftrGraphLineCheckValue[1]++;
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
    cout << output.str() << endl << "Seed :" << generatorSeed << endl;
    restartAsking:
    cout << "Restart (y/n/d/r/s/i/o) :";
    cin >> tempUserInput;
    if(tempUserInput == "y") goto restartPassword;
    else if(tempUserInput == "help"){
        cout << diag[18];
        goto restartAsking;
    }
    if(tempUserInput == "back"){
        cout << "Cannot change settings after passwords has beed generated." << endl;
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
        cout << "string tempUserInput : " << tempUserInput << endl;
        cout << "string consoleCache : " << consoleCache << endl;
        cout << "string ftrLineDrawPattern : " << ftrLineDrawPattern << endl;
        cout << "string ftrPswCounterPrev : " << ftrPswCounterPrev << endl;
        cout << "string ftrPswCounterNext : " << ftrPswCounterNext << endl;
        cout << "string ftrGraphLineCharacter : " << ftrGraphLineCharacter << endl;
        cout << "string ftrGraphLineDot : " << ftrGraphLineDot << endl;
        cout << "string ftrPswCounterString : " << ftrPswCounterString << endl;
        cout << "stringstream characterSetup : " << characterSetup.str() << endl;
        cout << "stringstream intLength : " << intLength.str() << endl;
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
        pswCharacter.clear();
        settingData = "help";
        tempUserInput = "n";
        consoleCache = "n";
        ftrLineDrawPattern = '-';
        ftrPswCounterPrev = '[';
        ftrPswCounterNext = ']';
        ftrGraphLineCharacter = '|';
        ftrGraphLineDot = '+';
        strPrev = '<';
        strNext = '>';
        strSkip = '!';
        fileBackuped = false;
        fileFailed = false;
        ftrPswCounterString.clear();
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
        cout << "string ftrPswCounterNext    : " << ftrPswCounterNext << endl;
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
        goto restartAsking;
    }
    else if(tempUserInput == "i"){
        fstream viewFile("/storage/emulated/0/Generated_Passwords.txt");
        if(viewFile.fail()){
            viewFile.open("C:/Generated_Passwords.txt");
            if(viewFile.fail()){
                cout << "Failed to view a file." << endl;
                viewFile.close();
                goto restartAsking;
            }
        }
        cout << "Reading Passwords..." << endl << endl;
        for(z = 0;getline(viewFile, consoleCache); z++) cout << consoleCache << endl;
        viewFile.close();
        cout << endl << z << " Lines Found" << endl;
        goto restartAsking;
    }
    else if(tempUserInput == "o"){
        ofstream ofile, backupFile;
        ifstream ifile;
        ifile.open(android.c_str());
        if(ifile.fail()){
            ifile.open(windows.c_str());
            if(ifile.fail()){
                ifile.close();
                ofile.open(android.c_str());
                if(ofile.fail()){ofile.open(windows.c_str()); if(ofile.fail()){cout << "Failed to create a file" << endl; ofile.close(); goto restartAsking;}}
                ofile << output.str();
                ofile.close();
            }
        }
        else{
            fileBackupString << time(0);
            fileBackupString >> consoleCache;
            fileBackupString.str("");
            backupWindows = ("C:/" + consoleCache + "_Backup_Generated_Passwords.txt");
            backupAndroid = ("/storage/emulated/0/" + consoleCache + "_Backup_Generated_Passwords.txt");
            cout << "\"Generated_Passwords.txt\" found, Making a backup" << endl;
            backupFile.open(backupAndroid.c_str());
            if(backupFile.fail()){ofile.open(backupWindows.c_str()); if(backupFile.fail()){cout << "Failed to backup a file" << endl; ifile.close(); backupFile.close(); goto restartAsking;}}
            while(getline(ifile, consoleCache)) backupFile << consoleCache << endl;
            ifile.close();
            backupFile.close();
            cout << "Backuped file." << endl;
            ofile.open(android.c_str());
            if(ofile.fail()){ofile.open(windows.c_str()); if(ofile.fail()){cout << "Failed to store passwords in a file" << endl; goto restartAsking;}}
            consoleCache.clear();
            ofile << output.str();
            ofile.close();
        }
        cout << "Successfully Outputted Passwords!" << endl << "Location of the file is either at \"" << android << "\" if you are using Android or \"" << windows << "\" if you are using Windows." << endl;
        goto restartAsking;
    }
    else exit(1);
    settingDataSet:
    if(settingDataSetup){
        strPrev = settingData[0];
        strNext = settingData[1];
        strSkip = settingData[2];
        ftrLineDraw = settingBool(settingData, strSkip, settingCounter);
        ftrLineDrawPattern = settingChar(settingData, strSkip, strPrev, strNext, settingCounter);
        ftrPswCounter = settingBool(settingData, strSkip, settingCounter);
        ftrPswCounterNumber = settingBool(settingData, strSkip, settingCounter);
        ftrPswCounterPrev = settingChar(settingData, strSkip, strPrev, strNext, settingCounter);
        ftrPswCounterNext = settingChar(settingData, strSkip, strPrev, strNext, settingCounter);
        ftrGraphLine = settingBool(settingData, strSkip, settingCounter);
        ftrGraphLineCharacter = settingChar(settingData, strSkip, strPrev, strNext, settingCounter);
        ftrGraphLineDot = settingChar(settingData, strSkip, strPrev, strNext, settingCounter);
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
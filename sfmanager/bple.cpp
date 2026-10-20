#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;
bool yn(string input){if(input == "y") return true; else return false;}
int main(){
  string fileName = "Level_Sandbox_06", temp;
  stringstream cnv;
  register int z, a, b;
  int sector = 0, lineCount, partType[65536], partSkin[65536], partPosX[65536], partPosY[65536], partProp[65536], partFlip[65536], resX = 0, resY = 0, posX, posY;
  restart:
  cout << "File Name :";
  //cin >> fileName;
  temp = "/storage/emulated/0/Android/data/com.INNOVATION.U/files/contraptionsB/" + fileName;
  ifstream ifile(temp.c_str());
  if(ifile.fail()){ifile.close(); cout << "Open Failed" << endl; goto restart;}
  if(ifile.bad()){ifile.close(); cout << "Bad File" << endl; goto restart;}
  for(z=0;!ifile.eof();z++) getline(ifile, temp);
  lineCount = z;
  lineCount--;
  ifile.clear();
  ifile.seekg(0);
  temp.clear();
  for(z=0;!ifile.eof();z++){
    getline(ifile, temp);
    cout << temp << endl;
    for(int i=0;i<temp.length();i++){
      if(temp[i] == ','){
        sector++;
        switch(sector){
          case 1: cnv >> partType[z]; break;
          case 2: cnv >> partSkin[z]; break;
          case 3: cnv >> partPosX[z]; break;
          case 4: cnv >> partPosY[z]; break;
          case 5: cnv >> partProp[z]; break;
        }
        cnv.str("");
        cnv.clear();
      }
      else cnv << temp[i];
      if(i == temp.length() - 1){
        cnv >> partFlip[z];
        sector = 0;
        cnv.str("");
        cnv.clear();
      }
    }
  }
  ifile.close();
  
  for(z=0;z<lineCount;z++){
    if(resX < partPosX[z]) resX = partPosX[z];
    if(resY < partPosY[z]) resY = partPosY[z];
  }
  
  cnv.clear();
  cnv.str("");
  cout << "Debug : " << resX << " : " << resY << endl;
  for(a=0;a<resY;a++){
    for(b=0;b<resX;b++){
      if(posX == b && posY == a){
        cout << '-';
        posY = partPosY[a];
        posX = partPosX[b];
      }
      else cout << '+';
    }
    cout << endl;
  }
  cout << cnv.str();
  cnv.clear();
  cnv.str("");
  cin >> temp;
  
  goto restart;
}
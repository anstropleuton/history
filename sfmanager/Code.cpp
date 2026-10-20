#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
int intExtract( string input ){
    stringstream convert;
    int output = 0, z;
    bool negative = false;
    for( z = 0; z < input.length(); z++ ){
        if( input[ z ] >= '0' && input[ z ] <= '9' ) convert << input[ z ];
        if( input[ z ] == '-' ) negative = true;
    }
    convert >> output;
    if( negative ) output = output * -1;
    return output;
};
class saveFile{
    public:
    int sector, lines;
    vector<int> PartType,
                PartSkin,
                PartPosX,
                PartPosY,
                PartProp,
                PartFlip;
    string temp;
    stringstream convert, contents;
    ifstream ifile;
    saveFile( string DIRECTORY ){
        ifile.open( DIRECTORY.c_str() );
        if( ifile.fail() ){
            cout << "Error while reading file..." << endl;
            system( "pause" );
            exit( 1 );
        }
        while( !ifile.eof() ){
            getline( ifile, temp );
            lines++;
        }
        ifile.clear();
        ifile.seekg( 0 );
        for( lines = 0; getline( ifile, temp ); lines++ ){
            contents << temp << endl;
            temp = temp + ",";
            sector = 0;
            for( int i = 0; i < temp.length(); i++ ){
                if( temp[ i ] == ',' ){
                    switch( sector ){
                        case 0: PartType.push_back( intExtract( convert.str() )); break;
                        case 1: PartSkin.push_back( intExtract( convert.str() )); break;
                        case 2: PartPosX.push_back( intExtract( convert.str() )); break;
                        case 3: PartPosY.push_back( intExtract( convert.str() )); break;
                        case 4: PartProp.push_back( intExtract( convert.str() )); break;
                        case 5: PartFlip.push_back( intExtract( convert.str() )); break;
                    }
                    convert.str("");
                    convert.clear();
                    sector++;
                }
                else convert << temp[ i ];
            }
            convert.str("");
        }
        ifile.close();
    }
};
int main(){
    saveFile saveOne( "/storage/emulated/0/Android/data/com.INNOVATION.U.BPLE/files/contraptionsB/Level_Sandbox_06_2.bak" );
    for( int i = 0; i < saveOne.lines; i++ ){
        cout << saveOne.PartType.at( i ) << ','
             << saveOne.PartSkin.at( i ) << ','
             << saveOne.PartPosX.at( i ) << ','
             << saveOne.PartPosY.at( i ) << ','
             << saveOne.PartProp.at( i ) << ','
             << saveOne.PartFlip.at( i ) << '\n';
    }
    system( "pause" );
}
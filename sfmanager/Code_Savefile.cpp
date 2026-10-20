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
class levelFile{
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
    ofstream ofile;
    void loadFile( string DIRECTORY ){
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
            convert.clear();
        }
        ifile.close();
    }
    void saveFile( string DIRECTORY ){
        ofile.open( DIRECTORY.c_str() );
        if( ofile.fail() ){
            cout << "Error while writing file...";
            system( "pause" );
            exit( 1 );
        }
        for( int i = 0; i < lines; i++ ){
            ofile << PartType[ i ] << ','
                  << PartSkin[ i ] << ','
                  << PartPosX[ i ] << ','
                  << PartPosY[ i ] << ','
                  << PartProp[ i ] << ','
                  << PartFlip[ i ] << '\n';
        }
        ofile.close();
    }
    void fileContents(){ cout << contents.str(); }
    /*void fileRender(){
        vector<int> Parts = {
            "Err", " @ ", "+++", " K ", " O ", "",
            "", " V ", "|V|", " v ", "^v^", "<B>",
            " - ", " | ", " T ", " > ", "=> ", " ->",
            "=->", "<=>", "(=)", "[=]", "(T)", "[T]",
            "~~~", "TNT", "---", " B ", "B B", "BBB",
            " H ", "H H", "HHH", "--W", "T--", "WW-",
            "-TT", "[P]", "{V}", "=G=", "{P}", "{K}",
            "[%]", "G-K", "-L-", "+L+", "JET", "-+-"
        }, Props = {
            "Err", " @ ", "+0+", "+1+", "+2+", "+3+",
            " K ", " O ", " - "
        };

    }*/
};
int main(){
    levelFile fileOne;
    fileOne.loadFile( "/storage/emulated/0/Android/data/com.INNOVATION.U.BPLE/files/contraptionsB/Level_Sandbox_06_2" );
    // fileOne.fileContents();
    // cout << endl;
    for( int i = 0; i < fileOne.lines; i++ ){
        cout << fileOne.PartType[ i ] << ','
             << fileOne.PartSkin[ i ] << ','
             << fileOne.PartPosX[ i ] << ','
             << fileOne.PartPosY[ i ] << ','
             << fileOne.PartProp[ i ] << ','
             << fileOne.PartFlip[ i ] << " :::: " << ( i + 1 ) << '\n';
    }
    system( "pause" );
}
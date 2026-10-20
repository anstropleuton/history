// Compile Command: g++ -std=c++11 savefile.cpp -o savefile.exe -Wall -mwindows -static -Wno-missing-braces -I . -I inclib/ -L . -L inclib/ -lraylib -lopengl32 -lgdi32 -lwinmm
#include <iostream>
#include <base.cpp>
using namespace std;
int main(){
	cout << "Generating the savefile in the working directory" << endl
		 << "of the executable..." << endl;
	ofstream ofile( "Level_Sandbox_06_1" );
	if( ofile.fail() ){
		ofile.close();
		return 1;
	}
	for( int Alpha = 0; Alpha < 48; Alpha++ ){
		for( int Beta = 0; Beta < 133; Beta++ ){
			ofile << Alpha << "," << Beta << "," << Alpha * 2 << "," << Beta * 2 << ",0,0" << endl;
		}
	}
	ofile.close();
	cout << "Done!, 1. Copy the generated \"Level_Sandbox_06_1\" -" << endl
		 << "- file and paste it in the BPLE Savefile directory." << endl
		 << "2. Open the level in BPLE and just press the Save button." << endl
		 << "3. Go to menu of the BPLE ( Unload the level file )." << endl
		 << "4. Copy the savefile \"Level_Sandbox_06_1\" file and -" << endl
		 << "- paste it in the working directory of the executable" << endl
		 << "Press Enter to continue...";
	getchar();
	cout << "Making the list of skins from the savefile in the" << endl
		 << "working directory of the executable..." << endl;
	Main World;
	World.LoadFile( "Level_Sandbox_06_1" );
	World.SaveFile( "Level_Sandbox_06_1_Backup" );
	int TempPosX = World.PartPosX.at( 0 ),
		TempPosY = World.PartPosY.at( 0 );
	vector< int > SkinCount;
	int Skins = 0;
	for( int Index = 1; Index < World.Lines; Index++ ){
		if( TempPosY == World.PartPosY.at( Index ) ){
			Skins++;
		}
		else{
			SkinCount.push_back( Skins );
			Skins = 0;
		}
		TempPosY = World.PartPosY.at( Index );
	}
	cout << "Done!, Modifying the savefile in the working directory" << endl
		 << "of the executable..." << endl;
	for( int Index = 0; Index < World.Lines * 2; Index += 2 ){
		World.PartType.insert( World.PartType.begin() + Index, 1 );
		World.PartSkin.insert( World.PartSkin.begin() + Index, 0 );
		World.PartPosX.insert( World.PartPosX.begin() + Index, World.PartPosX.at( Index ) );
		World.PartPosY.insert( World.PartPosY.begin() + Index, World.PartPosY.at( Index ) );
		World.PartProp.insert( World.PartProp.begin() + Index, 0 );
		World.PartFlip.insert( World.PartFlip.begin() + Index, 0 );
	}
	World.Lines = World.Lines * 2;
	World.SaveFile( "Level_Sandbox_06_1" );
	cout << "Done!, 1. Copy the generated \"Level_Sandbox_06_1\" -" << endl
		 << "- file and paste it in the BPLE Savefile directory." << endl
		 << "2. Open the level in BPLE and just press the Save button." << endl
		 << "3. Go to menu of the BPLE ( Unload the level file )." << endl
		 << "4. Copy the savefile \"Level_Sandbox_06_1\" file and -" << endl
		 << "- paste it in the working directory of the executable" << endl
		 << "Press Enter to continue...";
	getchar();
	cout << "Making the list of frames from the savefile in the" << endl
		 << "working directory of the executable..." << endl;
	World.Clear();
	World.LoadFile( "Level_Sandbox_06_1" );
	vector< bool > IsFrame;
	int SkippedParts = 0;
	for( int Index = 1; Index < World.Lines; Index++ ){
		TempPosX = World.PartPosX.at( Index - 1 ),
		TempPosY = World.PartPosY.at( Index - 1 );
		if( TempPosX == World.PartPosX.at( Index ) && TempPosY == World.PartPosY.at( Index ) ){
			IsFrame.push_back( true );
			Index++;
			SkippedParts++;
		}
		else IsFrame.push_back( false );
	}
	cout << "Done!, Here is the list:" << endl;
	for( int Index = 0; Index < World.Lines - SkippedParts; Index++ ){
		cout << "Index: " << Index << " Frame: " << IsFrame.at( Index ) << endl;
	}
}
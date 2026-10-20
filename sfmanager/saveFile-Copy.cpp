// Compile Command: g++ savefile.cpp -o savefile.exe -static -I .

// Included basic input and output stream
#include <iostream>

// Included standard lib to pause the system
#include <cstdlib>

// This is for "Main" class to load files
#include <base.cpp>

// I'm tired of using std::
using namespace std;

// Generates new line and inputs tabs
string Line( int TabsCountInput = 1 ){
	stringstream String;
	static int TabsCount = TabsCountInput;
	String << endl;
	for( int Tabs = 0; Tabs < TabsCount; Tabs++ ) String << "\t";
	return String.str();
}

// main()
int main(){

	// Filename of the generation file
	string FileName, TempCin;

	// Variables to limit the part type and skins generation
	int AlphaLimit = 48, BetaLimit = 133;

	cout << "How many part types are there in the game? ( '48' in 2022.1 ) : ";
	cin >> AlphaLimit;

	cout << "How many maximum skins are there in the game? ( '133' in 2022.1 ) : ";
	cin >> BetaLimit;

	cout << "Name of the file to generate ( No Spaces ) : Level_Sandbox_";
	cin >> FileName;

	FileName = "Level_Sandbox_" + FileName;

	// "working directory of the executable"
	// meaning the place where the built exe is
	cout << "Generating the savefile in the working directory" << endl
		 << "of the executable..." << endl;
	
	ofstream ofile( FileName.c_str() );
	if( ofile.fail() ){

		// Either you need to change the place where
		// the exe is or change the permission of the
		// folder or run exe as administrator
		cout << "Failed to generate file..." << endl;

		ofile.close();

		cin >> TempCin;

		return 1;
	}

	// Generates all parts
	for( int Alpha = 0; Alpha < AlphaLimit; Alpha++ ){
		for( int Beta = 0; Beta < BetaLimit; Beta++ ){
			ofile << Alpha << "," << Beta << "," << Alpha * 2 << "," << Beta * 2 << ",0,0" << endl;
		}
	}

	ofile.close();

	// This is because there is ALL the generated parts but we only need the ones which are available in-game
	cout << "Done!, 1. Copy the generated \"" << FileName << "\" -" << endl
		 << "- file and paste it in the BPLE Savefile directory." << endl
		 << "2. Open the level in BPLE and just press the Save button." << endl
		 << "3. Go to menu of the BPLE ( Unload the level file )." << endl
		 << "4. Copy the savefile \"" << FileName << "\" file and -" << endl
		 << "- paste it in the working directory of the executable" << endl;
	
	cin >> TempCin;

	cout << "Making the list of skins from the savefile in the" << endl
		 << "working directory of the executable..." << endl;
	
	// Main class from base.cpp
	Main World;
	World.LoadFile( FileName.c_str() );

	int TempPosX, TempPosY, Temp = 0;

	// Stores the skin count data for all the parts
	vector< int > SkinCount;

	// Decodes Skin Count
	// Remember we start with index
	// of 1 so we can access previous
	// part without crashing the program
	for( int Index = 1; Index < World.Lines; Index++ ){

		// Temporary Variable for previous PosX of the part
		TempPosX = World.PartPosX.at( Index - 1 );

		// Checks if the old part is in the same X-Axis of the current one
		// This is because of the way we generated the file, all the parts
		// with same skin is in the same X-Axis
		if( TempPosX == World.PartPosX.at( Index ) ) Temp++;

		// If false then it will add one more skin to the list so that the
		// last old part is in the list of skins and then it stores the data
		else{
			Temp++;
			SkinCount.push_back( Temp );
			Temp = 0;
		}
	}

	// The final remaining is also added and stored
	Temp++;
	SkinCount.push_back( Temp );
	Temp = 0;

	cout << "Done!, Modifying the savefile in the working directory" << endl
		 << "of the executable..." << endl;

	// Copies the existing parts and addes it above the line of the
	// existing part because the game replaces newer line-parts
	// with the old one. Now there will be pig sitting in all the
	// available frames
	for( int Index = 0; Index < World.Lines * 2; Index += 2 ){

		World.PartType.insert( World.PartType.begin() + Index, 1 );
		World.PartSkin.insert( World.PartSkin.begin() + Index, 0 );
		World.PartPosX.insert( World.PartPosX.begin() + Index, World.PartPosX.at( Index ) );
		World.PartPosY.insert( World.PartPosY.begin() + Index, World.PartPosY.at( Index ) );
		World.PartProp.insert( World.PartProp.begin() + Index, 0 );
		World.PartFlip.insert( World.PartFlip.begin() + Index, 0 );
	}

	// Because we duplicated parts, we need to duplicate lines count
	// too to save all the parts
	World.Lines = World.Lines * 2;
	World.SaveFile( FileName.c_str() );

	// Now this is to eliminate the extra pigs that are sitting in
	// non-frame blocks as the pigs will be replaced with next line
	// of the savefile by the game
	cout << "Done!, 1. Copy the generated \"" << FileName << "\" -" << endl
		 << "- file and paste it in the BPLE Savefile directory." << endl
		 << "2. Open the level in BPLE and just press the Save button." << endl
		 << "3. Go to menu of the BPLE ( Unload the level file )." << endl
		 << "4. Copy the savefile \"" << FileName << "\" file and -" << endl
		 << "- paste it in the working directory of the executable" << endl;

	cin >> TempCin;

	cout << "Making the list of frames from the savefile in the" << endl
		 << "working directory of the executable..." << endl;

	// Clears the world and loads game-generated file
	World.LoadFile( FileName.c_str() );

	// Stores weather or not an object is a frame or non-frame
	vector< bool > IsFrame;

	// Decodes the Frame bool
	// Remember we start with index
	// of 1 so we can access previous
	// part without crashing the program
	for( int Index = 1; Index < World.Lines; Index++ ){

		// Again, Comparing the previous part with current
		// This is because if there is pig in the previous
		// part then the position x and y of the pig will
		// be the same as the current one ( frame )
		TempPosX = World.PartPosX.at( Index - 1 ),
		TempPosY = World.PartPosY.at( Index - 1 );

		// Checks wether it's in same position x, y of the current one
		if( TempPosX == World.PartPosX.at( Index ) && TempPosY == World.PartPosY.at( Index ) ){

			IsFrame.push_back( true );

			// This is to skip the comparing the frame with the
			// next part because remember we duplicated the parts
			// so this will help skip the duplicated pigs
			Index++;
		}
		else IsFrame.push_back( false );
	}

	// Finally one part was skipped so we manually add it as non-frame
	IsFrame.push_back( false );

	cout << "Done!, Successfully generated the list," << endl << "Displaying the list..." << endl;

	Temp = 0; 

	// Display the list
	for( int Index = 0; Index < SkinCount.size(); Index++ ){

		// Count of skins per part
		int Count = 0;

		cout << endl << "Part Index: " << Index << endl;

		// Display skins per part
		while( Count < SkinCount.at( Index ) ){
			cout << "    Skin Index: " << Count << " : Is it a Frame ? : ";

			// Check if it is a frame
			if( IsFrame.at( Temp ) ) cout << "Yeah ++++" << endl;
								else cout << "Nope ----" << endl;
			Count++;

			// Index of vector IsFrame
			Temp++;
		}
	}

	cout << "List Completed." << endl << "Generating the Vector file of the data..." << endl;

	// The ammount of tabs to use before putting the
	// actual data so that you don't have to manually do it
	int TabsCount = 0;

	Temp = 0;

	// This is to skip the first comma or braces in the
	// file and do it next every time
	bool SkipFirst = true;
	bool SkipFirstBig = true;

	cout << "Taps Count at the start: ";
	cin >> TabsCount;

	// Set the static tab count of the function
	// === I don't think it will work for   ===
	// === any inputs other than 1          ===
	Line( TabsCount );

	// Output file name
	ofstream VectorFile( "Vector_IsFrame.cpp" );

	// Included header so that it won't give me error just to check it out
	VectorFile << "#include <vector>" << Line() << "std::vector< std::vector< bool > > IsFrame = {";

	for( int Index = 0; Index < SkinCount.size(); Index++ ){

		// Big Skip is for Part Type
		if( !SkipFirstBig ) VectorFile << " },";
		else SkipFirstBig = false;

		VectorFile << Line() << "\t{ ";

		// Count of skins per part
		int Count = 0;

		// Save skins frame bool per part
		while( Count < SkinCount.at( Index ) ){

			// Non-Big Skip is for Skins
			if( !SkipFirst ) VectorFile << ", ";
			else SkipFirst = false;

			// Save the actual data
			if( IsFrame.at( Temp ) ) VectorFile << "true";
			else VectorFile << "false";
			Count++;

			// Index of vector IsFrame
			Temp++;
		}
		// Reset the skin skip
		SkipFirst = true;
	}

	// Finishing up
	VectorFile << " }" << Line() << "};";

	// Done!
	cout << "Successfully Generated \"Vector_IsFrame.cpp\" in the working directory of the executable!" << endl;
	
	// Pause the console
	cin >> TempCin;

	// That's it!
}

// This took me a lot of brain
// and a lot of tials and errors
// and finally it worked!

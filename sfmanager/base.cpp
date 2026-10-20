#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#if defined( USING_RAYLIB )
#include <raylib.h>
#endif
int intExtract( std::string Input ){
	std::stringstream Convert;
	int Output = 0, z;
	bool Negative = false;
	for( z = 0; z < Input.length(); z++ ){
		if( Input[ z ] >= '0' && Input[ z ] <= '9' ) Convert << Input[ z ];
		if( Input[ z ] == '-' ) Negative = true;
	}
	Convert >> Output;
	if( Negative ) Output = Output * -1;
	return Output;
}
#if defined( USING_RAYLIB )
bool StaticBounds( Rectangle Bounds = { 0 } ){
	static std::vector< Rectangle > Collutions;
	if( Bounds.x == 0 && Bounds.y == 0 && Bounds.width == 0 && Bounds.height == 0 )
		for( int Index = 0; Index < Collutions.size(); Index++ )
			if( CheckCollisionPointRec( GetMousePosition(), Collutions.at( Index ) ) )
				return true;
			else return false;
	else for( int Index = 0; Index < Collutions.size(); Index++ )
		if(
			Collutions.at( Index ).x != Bounds.x &&
			Collutions.at( Index ).y != Bounds.y &&
			Collutions.at( Index ).width != Bounds.width &&
			Collutions.at( Index ).height != Bounds.height
		) Collutions.push_back( Bounds );
}
int Slider( Rectangle Box, int Min = 0, int Max = 100, int Size = 20 ){
	static int Value = 0;
	if( CheckCollisionPointRec( GetMousePosition(), Rectangle{ Box.x, Box.y, Box.width, Box.height / 2 } ) ) Value++;
	if( CheckCollisionPointRec( GetMousePosition(), Rectangle{ Box.x, Box.y + Box.height / 2, Box.width, Box.height / 2 } ) ) Value--;
	if( Value < Min ) Value = Min;
	if( Value > Max ) Value = Max;
	DrawText( "^", Box.x + Size / 2, Box.y, Size, Color{ 100, 100, 255, 255 } );
	DrawText( TextFormat( "%i", Value ), Box.x, Box.y, Size, Color{ 100, 100, 255, 255 } );
	DrawText( "v", Box.x + Size / 2, Box.height - Size / 2, Size, Color{ 100, 100, 255, 255 } );
	return Value;
}
bool Toggle( Rectangle Box, std::string Display, int Size = 20 ){
	static int State = 0;
	static bool Active = false;
	StaticBounds( Box );
	if( CheckCollisionPointRec( GetMousePosition(), Box ) ){
		if( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) Active = !Active;
		if( Active ){
			if( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) State = 5;
			else State = 3;
		}
		else{
			if( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) State = 4;
			else State = 2;
		}
	}
	else if( Active ) State = 1;
	else State = 0;
	switch( State ){
		case 0: {
			DrawRectangleRec( Box, Color{ 100, 175, 255, 255 } );
		} break;
		case 1: {
			DrawRectangleRec( Box, Color{ 0, 100, 255, 255 } );
		} break;
		case 2: {
			DrawRectangleRec( Box, Color{ 100, 150, 255, 255 } );
		} break;
		case 3: {
			DrawRectangleRec( Box, Color{ 0, 125, 255, 255 } );
		} break;
		case 4: {
			DrawRectangleRec( Box, Color{ 100, 125, 255, 255 } );
		} break;
		case 5: {
			DrawRectangleRec( Box, Color{ 0, 150, 255, 255 } );
		} break;
		default: {
			DrawRectangleRec( Box, Color{ 127, 127, 127, 255 } );
		} break;
	}
	DrawText( Display.c_str(), Box.x, Box.y + Size / 2, Size, Color{ 100, 100, 255, 255 } );
	return Active;
}
#endif
class Main{
	public:
	unsigned int Sector, Lines;
	std::vector< int > PartType,
					   PartSkin,
					   PartPosX,
					   PartPosY,
					   PartProp,
					   PartFlip;
	std::vector< std::vector< bool > > IsFrame = {
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  },
		{  }
	};
	std::string Temp;
	std::stringstream Convert;
	std::ifstream Ifile;
	std::ofstream Ofile;
	enum Rotation{
		Up = 0,
		Left = 270,
		Down = 180,
		Right = 90,
		UpLeft = 315,
		DownLeft = 255,
		DownRight = 135,
		UpRight = 45
	};
	void LoadFile( std::string Directory ){
		Clear();
		Ifile.open( Directory.c_str() );
		if( Ifile.fail() ){
			Ifile.close();
			return;
		}
		for( Lines = 0; std::getline( Ifile, Temp ); Lines++ ){
			Temp = Temp + ",";
			Sector = 0;
			for( int Index = 0; Index < Temp.length(); Index++ ){
				if( Temp[ Index ] == ',' ){
					switch( Sector ){
						case 0: PartType.push_back( intExtract( Convert.str() )); break;
						case 1: PartSkin.push_back( intExtract( Convert.str() )); break;
						case 2: PartPosX.push_back( intExtract( Convert.str() )); break;
						case 3: PartPosY.push_back( intExtract( Convert.str() )); break;
						case 4: PartProp.push_back( intExtract( Convert.str() )); break;
						case 5: PartFlip.push_back( intExtract( Convert.str() )); break;
					}
					Convert.str("");
					Convert.clear();
					Sector++;
				}
				else Convert << Temp[ Index ];
			}
			Convert.str("");
			Convert.clear();
		}
		Ifile.close();
	}
	void SaveFile( std::string Directory ){
		Ofile.open( Directory.c_str() );
		if( Ofile.fail() ){
			Ofile.close();
			return;
		}
		for( int Index = 0; Index < Lines; Index++ ){
			Ofile << PartType[ Index ] << ','
				  << PartSkin[ Index ] << ','
				  << PartPosX[ Index ] << ','
				  << PartPosY[ Index ] << ','
				  << PartProp[ Index ] << ','
				  << PartFlip[ Index ] << '\n';
		}
		Ofile.close();
	}
#if defined( USING_RAYLIB )
	Image MainTexture = LoadImage( "Files/Textures.png" );
	Image CurrentImage = LoadImage( "Files/Test.png" );
	Texture CurrentTexture = LoadTextureFromImage( CurrentImage );
	std::vector< std::vector< std::vector< Texture > > > PartTexture = {
		// Part Type
		{
			// Part Skin
			{
				//Part Prop
			}
		}
	};
	void DrawTest(){
		for( int Index = 0; Index < Lines; Index++ ){
			DrawTexturePro( CurrentTexture, Rectangle{ 0.0f, 0.0f, ( float ) CurrentTexture.width, ( float ) CurrentTexture.height }, Rectangle{ ( PartPosX.at( Index ) * 10.0f ) + 5.0f, ( PartPosY.at( Index ) * -10.0f ) + 5.0f, 11.0f, 11.0f }, Vector2{ 5.5f, 5.5f }, 0, WHITE );
		}
	}
	void DrawPart( int Index ){
		CurrentTexture = PartTexture.at( PartType.at( Index ) ).at( PartSkin.at( Index ) ).at( PartProp.at( Index ) );
		DrawTextureEx( CurrentTexture, Vector2{ PartPosX.at( Index ) * 10.0f, PartPosY.at( Index ) * -10.0f }, 0.0f, 1.0f, WHITE );
	}
	void DrawAll(){
		for( int Index = 0; Index < Lines; Index++ ){
			Texture CurrentTexture = PartTexture.at( PartType.at( Index ) ).at( PartSkin.at( Index ) ).at( PartProp.at( Index ) );
			DrawTextureEx( CurrentTexture, Vector2{ PartPosX.at( Index ) * 10.0f, PartPosY.at( Index ) * -10.0f }, 0.0f, 1.0f, WHITE );
		}
	}
	void DeletePointPart( Vector2 Position ){
		for( int Index = 0; Index < Lines; Index++ ){
			if( PartPosX.at( Index ) == ( int ) Position.x && PartPosY.at( Index ) == ( int ) Position.y ){

			}
		}
	}
#endif
	void InsertPart( int Type, int Skin, int PosX, int PosY, int Prop, int Flip ){
		PartType.push_back( Type );
		PartSkin.push_back( Skin );
		PartPosX.push_back( PosX );
		PartPosY.push_back( PosY );
		PartProp.push_back( Prop );
		PartFlip.push_back( Flip );
		Lines++;
	}
	void DeleteFindPart( int Type, int Skin, int PosX, int PosY, int Prop, int Flip ){}
	void DeletePart(){
		PartType.clear();
		PartSkin.clear();
		PartPosX.clear();
		PartPosY.clear();
		PartProp.clear();
		PartFlip.clear();
		Lines = 0;
	}
	void Clear(){
		DeletePart();
		Sector = 0;
#if defined( USING_RAYLIB )

#endif
	}
};
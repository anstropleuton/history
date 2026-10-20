#define RAYGUI_IMPLEMENTATION
#include <vector>
#include <string>
#include <time.h>
#include <raylib.h>
#include <raygui.h>
using namespace std;
int GridAlpha = 20,
	GridBeta = 20,
	MinesCount = 50,
	DeadAlpha = -1,
	DeadBeta = -1,
	FirstAlpha = -1,
	FirstBeta = -1,
	FlagsCount = 0,
	RemainingMines = 0,
	FlagDisplayNumber_0 = 0,
	FlagDisplayNumber_1 = 0,
	FlagDisplayNumber_2 = 0,
	TimerDisplayNumber_0 = 0,
	TimerDisplayNumber_1 = 0,
	TimerDisplayNumber_2 = 0,
	BlockSize = 20,
	Spacing = 0,
	Offset = 10,
	Timer = 0,
	TempGridAlpha = 20,
	TempGridBeta = 20,
	TempMinesCount = 50,
	TempBlockSize = 20,
	TempSpacing = 0,
	TempOffset = 10;
bool IWon = false,
	 ILoss = false,
	 Double_Click_Active = false,
	 FirstClick = true,
	 SelectionMode = false,
	 EditMode_1 = false,
	 EditMode_2 = false,
	 EditMode_3 = false,
	 EditMode_4 = false,
	 EditMode_5 = false,
	 EditMode_6 = false;
Texture Close,
		Open,
		SelectionClose,
		SelectionOpen,
		Mine,
		Exploded,
		Smily,
		Dead,
		Glasses,
		Flag,
		Mark,
		Num_1,
		Num_2,
		Num_3,
		Num_4,
		Num_5,
		Num_6,
		Num_7,
		Num_8,
		Display_0,
		Display_1,
		Display_2,
		Display_3,
		Display_4,
		Display_5,
		Display_6,
		Display_7,
		Display_8,
		Display_9;
Rectangle Close_Source,
		  Open_Source,
		  SelectionClose_Source,
		  SelectionOpen_Source,
		  Mine_Source,
		  Exploded_Source,
		  Smily_Source,
		  Dead_Source,
		  Glasses_Source,
		  Flag_Source,
		  Mark_Source,
		  Num_1_Source,
		  Num_2_Source,
		  Num_3_Source,
		  Num_4_Source,
		  Num_5_Source,
		  Num_6_Source,
		  Num_7_Source,
		  Num_8_Source,
		  DeadBlock,
		  Display_0_Source,
		  Display_1_Source,
		  Display_2_Source,
		  Display_3_Source,
		  Display_4_Source,
		  Display_5_Source,
		  Display_6_Source,
		  Display_7_Source,
		  Display_8_Source,
		  Display_9_Source,
		  RestartButton;
vector< vector< bool > > IsOpen,
						 IsMine,
						 HasFlag;
vector< vector< int > > MineNumber;
float Double_Click_Timer = 0.0f,
	  WindowAlpha,
	  WindowBeta,
	  SelectionAlpha = -1.0f,
	  SelectionBeta = -1.0f;
void InitValues( bool Random );
void InitSettings();
void DnitValues();
void InitTextures();
void DnitTextures();
bool EditModes();
void OpenRest( int Alpha, int Beta );
void OpenFlag( int Alpha, int Beta );
void DrawNumber( int Alpha, int Beta, Rectangle Block, Vector2 BlockOrigin );
void DrawBorders();
void DrawDisplay();
int main(){
	InitValues( false );
	SetConfigFlags( FLAG_WINDOW_RESIZABLE );
	InitWindow(
		GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f,
		GridBeta * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f + BlockSize * 2.0f + 86.0f,
		"MineSweeper"
	);
	SetTargetFPS( 60 );
	WindowAlpha = GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f,
	WindowBeta = GridBeta * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f + BlockSize * 2.0f + 86.0f;
	InitTextures();
	RestartButton = ( Rectangle ){ GetScreenWidth() / 2.0f - BlockSize + Offset / 2.0f, Offset, BlockSize * 2.0f - Offset, BlockSize * 2.0f - Offset };
	while( !WindowShouldClose() ){
		BeginDrawing();
		ClearBackground( ( Color ){ 191, 191, 191, 255 } ); {
			if( GetScreenWidth() != WindowAlpha ){
				BlockSize = ( ( GetScreenWidth() + Spacing - Offset * 2.0f ) / GridAlpha ) - Spacing;
				TempBlockSize = BlockSize;
				while( GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f < 260 ){
					BlockSize++;
					TempBlockSize = BlockSize;
				}
				SetWindowSize(
					GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f,
					GridBeta * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f + BlockSize * 2.0f + 86.0f
				);
				WindowAlpha = GetScreenWidth();
				RestartButton = ( Rectangle ){
					GetScreenWidth() / 2.0f - BlockSize + Offset / 2.0f,
					Offset,
					BlockSize * 2.0f - Offset,
					BlockSize * 2.0f - Offset
				};
			}
			if( GetScreenHeight() != WindowBeta ){
				BlockSize = ( ( GetScreenHeight() + Spacing - Offset * 2.0f - BlockSize * 2.0f - 86.0f ) / GridBeta ) - Spacing;
				TempBlockSize = BlockSize;
				while( GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f < 260 ){
					BlockSize++;
					TempBlockSize = BlockSize;
				}
				SetWindowSize(
					GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f,
					GridBeta * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f + BlockSize * 2.0f + 86.0f
				);
				WindowBeta = GetScreenHeight();
				RestartButton = ( Rectangle ){
					GetScreenWidth() / 2.0f - BlockSize + Offset / 2.0f,
					Offset,
					BlockSize * 2.0f - Offset,
					BlockSize * 2.0f - Offset
				};
			}
			if(
				(
					CheckCollisionPointRec( GetMousePosition(), RestartButton ) &&
					IsMouseButtonPressed( MOUSE_LEFT_BUTTON )
				) ||
				(
					IsKeyPressed( KEY_BACKSPACE ) &&
					!EditModes()
				)
			){
				if( !Double_Click_Active ){
					Double_Click_Timer = GetFrameTime() * 16.0f;
					InitSettings();
				}
				else{
					Double_Click_Active = false;
					DnitValues();
					DnitTextures();
					InitSettings();
					InitValues( false );
					InitTextures();
				}
			}
			if( Double_Click_Timer > 0.0f ){
				Double_Click_Timer -= GetFrameTime();
				Double_Click_Active = true;
			}
			else Double_Click_Active = false;
			DrawTexturePro( Smily, Smily_Source, RestartButton, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE );
			if( IWon ) DrawTexturePro( Glasses, Glasses_Source, RestartButton, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE );
			if( ILoss ) DrawTexturePro( Dead, Dead_Source, RestartButton, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE );
			DeadBlock = ( Rectangle ){
				Offset + ( DeadAlpha ) * ( BlockSize + Spacing ),
				Offset + ( DeadBeta ) * ( BlockSize + Spacing ) + BlockSize * 2.0f,
				BlockSize,
				BlockSize
			};
			DrawDisplay();
			DrawBorders();
			int CorrectFlags = 0, CorrectOpens = 0;
			for( int Alpha = 0; Alpha < GridAlpha; Alpha++ ){
				for( int Beta = 0; Beta < GridBeta; Beta++ ){
					if( IsMine.at( Alpha ).at( Beta ) && HasFlag.at( Alpha ).at( Beta ) ) CorrectFlags++;
					if( !IsMine.at( Alpha ).at( Beta ) && IsOpen.at( Alpha ).at( Beta ) ) CorrectOpens++;
					if( CorrectFlags == MinesCount && CorrectOpens == GridAlpha * GridBeta - MinesCount ){
						IWon = true;
						IsOpen.at( Alpha ).at( Beta ) = true;
					}
					Rectangle Block = ( Rectangle ){
						Offset + ( Alpha ) * ( BlockSize + Spacing ),
						Offset + ( Beta ) * ( BlockSize + Spacing ) + BlockSize * 2.0f,
						BlockSize,
						BlockSize
					};
					Vector2 BlockOrigin = { 0.0f, 0.0f };
					DrawTexturePro( Open, Open_Source, Block, BlockOrigin, 0.0f, WHITE );
					if( Alpha == ( int ) SelectionAlpha && Beta == ( int ) SelectionBeta )
						DrawTexturePro( SelectionOpen, SelectionOpen_Source, Block, BlockOrigin, 0.0f, WHITE );
					if(
						CheckCollisionPointRec( GetMousePosition(), Block ) &&
						IsMouseButtonPressed( MOUSE_BUTTON_MIDDLE )
					){
						if( Alpha == ( int ) SelectionAlpha && Beta == ( int ) SelectionBeta ) SelectionMode = !SelectionMode;
						else SelectionMode = true;
						SelectionAlpha = Alpha;
						SelectionBeta = Beta;
						if( EditModes() ){
							SelectionMode = false;
							SelectionAlpha = -1.0f;
							SelectionBeta = -1.0f;
						}
					}
					if( IsOpen.at( Alpha ).at( Beta ) ){
						DrawNumber( Alpha, Beta, Block, BlockOrigin );
						if(
							CheckCollisionPointRec( GetMousePosition(), Block ) &&
							IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) &&
							!( ILoss || IWon ) &&
							MineNumber.at( Alpha ).at( Beta ) != 0
						) OpenFlag( Alpha, Beta );
					}
					else{
						DrawTexturePro( Close, Close_Source, Block, BlockOrigin, 0.0f, WHITE );
						if( Alpha == ( int ) SelectionAlpha && Beta == ( int ) SelectionBeta )
							DrawTexturePro( SelectionClose, SelectionClose_Source, Block, BlockOrigin, 0.0f, WHITE );
						if( ( ILoss || IWon ) && !HasFlag.at( Alpha ).at( Beta ) ){
							if( IsMine.at( Alpha ).at( Beta ) )
								DrawTexturePro( Mine, Mine_Source, Block, BlockOrigin, 0.0f, WHITE );
						}
						if( HasFlag.at( Alpha ).at( Beta ) ){
							if( ( IWon || ILoss ) ){
								if( IsMine.at( Alpha ).at( Beta ) )
									DrawTexturePro( Flag, Flag_Source, Block, BlockOrigin, 0.0f, WHITE );
								else DrawTexturePro( Mark, Mark_Source, Block, BlockOrigin, 0.0f, WHITE );
							}
							else DrawTexturePro( Flag, Flag_Source, Block, BlockOrigin, 0.0f, WHITE );
						}
						if( CheckCollisionPointRec( GetMousePosition(), Block ) ){
							if( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) && !( ILoss || IWon ) ){
								if( FirstClick ){
									IsOpen.at( Alpha ).at( Beta ) = true;
									Timer = time( 0 );
									FirstAlpha = Alpha;
									FirstBeta = Beta;
									FirstClick = false;
									InitValues( true );
									OpenRest( Alpha, Beta );
								}
								else if( !HasFlag.at( Alpha ).at( Beta ) ){
									if( IsMine.at( Alpha ).at( Beta ) ){
										ILoss = true;
										DeadAlpha = Alpha;
										DeadBeta = Beta;
									}
									else{
										IsOpen.at( Alpha ).at( Beta ) = true;
										OpenRest( Alpha, Beta );
									}
								}
							}
							if( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) && !( ILoss || IWon ) ){
								HasFlag.at( Alpha ).at( Beta ) = !HasFlag.at( Alpha ).at( Beta );
								if( HasFlag.at( Alpha ).at( Beta ) ) FlagsCount++;
								else FlagsCount--;
							}
						}
					}
				}
			}
			if( ILoss )
				DrawTexturePro(
					Exploded,
					Exploded_Source,
					DeadBlock,
					( Vector2 ){
						0.0f, 0.0f
					},
					0.0f,
					WHITE
				);
			if( GuiValueBox( ( Rectangle ){ 100, GetScreenHeight() - 81, 36, 20 }, "Grid Size X", &TempGridAlpha, 1, 1000, EditMode_1 ) )
				EditMode_1 = !EditMode_1;
			if( GuiValueBox( ( Rectangle ){ 100, GetScreenHeight() - 59, 36, 20 }, "Grid Size Y", &TempGridBeta, 1, 1000, EditMode_2 ) )
				EditMode_2 = !EditMode_2;
			if( GuiValueBox( ( Rectangle ){ 100, GetScreenHeight() - 37, 36, 20 }, "Block Spacing", &TempSpacing, 0, 1000, EditMode_3 ) )
				EditMode_3 = !EditMode_3;
			if( GuiValueBox( ( Rectangle ){ 206, GetScreenHeight() - 81, 36, 20 }, "Mines Count", &TempMinesCount, 0, 1000, EditMode_4 ) )
				EditMode_4 = !EditMode_4;
			if( GuiValueBox( ( Rectangle ){ 206, GetScreenHeight() - 59, 36, 20 }, "Block Size", &TempBlockSize, 1, 1000, EditMode_5 ) )
				EditMode_5 = !EditMode_5;
			if( GuiValueBox( ( Rectangle ){ 206, GetScreenHeight() - 37, 36, 20 }, "Offset", &TempOffset, 0, 1000, EditMode_6 ) )
				EditMode_6 = !EditMode_6;
			if( IsKeyPressed( KEY_TAB ) ){
				if( EditMode_1 ){
					EditMode_1 = false;
					EditMode_2 = true;
				}
				else if( EditMode_2 ){
					EditMode_2 = false;
					EditMode_3 = true;
				}
				else if( EditMode_3 ){
					EditMode_3 = false;
					EditMode_4 = true;
				}
				else if( EditMode_4 ){
					EditMode_4 = false;
					EditMode_5 = true;
				}
				else if( EditMode_5 ){
					EditMode_5 = false;
					EditMode_6 = true;
				}
				else if( EditMode_6 ){
					EditMode_6 = false;
					EditMode_1 = true;
				}
				else EditMode_1 = true;
			}
			if( EditModes() ) SelectionMode = false;
			if( IsKeyDown( KEY_LEFT ) && !( ILoss || IWon ) ){
				SelectionAlpha -= GetFrameTime() * 10.0f;
				if( !SelectionMode ) SelectionMode = true;
			}
			if( IsKeyDown( KEY_RIGHT ) && !( ILoss || IWon ) ){
				SelectionAlpha += GetFrameTime() * 10.0f;
				if( !SelectionMode ) SelectionMode = true;
			}
			if( IsKeyDown( KEY_UP ) && !( ILoss || IWon ) ){
				SelectionBeta -= GetFrameTime() * 10.0f;
				if( !SelectionMode ) SelectionMode = true;
			}
			if( IsKeyDown( KEY_DOWN ) && !( ILoss || IWon ) ){
				SelectionBeta += GetFrameTime() * 10.0f;
				if( !SelectionMode ) SelectionMode = true;
			}
			if( SelectionMode ){
				if( SelectionAlpha < 0.0f )
					SelectionAlpha = 0.0f;
				if( SelectionBeta < 0.0f )
					SelectionBeta = 0.0f;
				if( SelectionAlpha > GridAlpha - 1.0f )
					SelectionAlpha = GridAlpha - 1.0f;
				if( SelectionBeta > GridBeta - 1.0f )
					SelectionBeta = GridBeta - 1.0f;
			}
			else{
				SelectionAlpha = -1.0f;
				SelectionBeta = -1.0f;
			}
			if( IsKeyPressed( KEY_SPACE ) && !EditModes() && SelectionMode && !( ILoss || IWon ) ){
				if( IsOpen.at( SelectionAlpha ).at( SelectionBeta ) ){
					if( !( ILoss || IWon ) && MineNumber.at( SelectionAlpha ).at( SelectionBeta ) != 0 )
						OpenFlag( SelectionAlpha, SelectionBeta );
				}
				else{
					if( FirstClick ){
						IsOpen.at( SelectionAlpha ).at( SelectionBeta ) = true;
						Timer = time( 0 );
						FirstAlpha = SelectionAlpha;
						FirstBeta = SelectionBeta;
						FirstClick = false;
						InitValues( true );
						OpenRest( SelectionAlpha, SelectionBeta );
					}
					else if( !HasFlag.at( SelectionAlpha ).at( SelectionBeta ) ){
						if( IsMine.at( SelectionAlpha ).at( SelectionBeta ) ){
							ILoss = true;
							DeadAlpha = SelectionAlpha;
							DeadBeta = SelectionBeta;
						}
						else{
							IsOpen.at( SelectionAlpha ).at( SelectionBeta ) = true;
							OpenRest( SelectionAlpha, SelectionBeta );
						}
					}
				}
			}
			if( IsKeyPressed( KEY_ENTER ) && !EditModes() && SelectionMode && !( ILoss || IWon ) ){
				HasFlag.at( SelectionAlpha ).at( SelectionBeta ) = !HasFlag.at( SelectionAlpha ).at( SelectionBeta );
				if( HasFlag.at( SelectionAlpha ).at( SelectionBeta ) ) FlagsCount++;
				else FlagsCount--;
			}
		} EndDrawing();
	}
	DnitTextures();
	DnitValues();
	CloseWindow();
}
void InitValues( bool Random ){
	SetRandomSeed( time( 0 ) );
	if( !Random ){
		for( int Alpha = 0; Alpha < GridAlpha; Alpha++ ){
			vector< bool > TempFillBool;
			for( int Beta = 0; Beta < GridBeta; Beta++ )
				TempFillBool.push_back( false );
			IsOpen.push_back( TempFillBool );
			IsMine.push_back( TempFillBool );
			HasFlag.push_back( TempFillBool );
			vector< int > TempFillInt;
			for( int Beta = 0; Beta < GridBeta; Beta++ )
				TempFillInt.push_back( 0 );
			MineNumber.push_back( TempFillInt );
		}
	}
	else{
		for( int Index = 0; Index < MinesCount; Index++ ){
			int TempRandomAlpha = GetRandomValue( 0, GridAlpha - 1 ),
				TempRandomBeta = GetRandomValue( 0, GridBeta - 1 );
			if( TempRandomAlpha == FirstAlpha && TempRandomBeta == FirstBeta ){
				Index--;
				continue;
			}
			if( !IsMine.at( TempRandomAlpha ).at( TempRandomBeta ) )
				IsMine.at( TempRandomAlpha ).at( TempRandomBeta ) = true;
			else Index--;
		}
		for( int Alpha = 0; Alpha < GridAlpha; Alpha++ ){
			for( int Beta = 0; Beta < GridBeta; Beta++ ){
				int Value = 0;
				if( IsMine.at( Alpha ).at( Beta ) )
					continue;
				if( Alpha != 0 && Beta != 0 )
					if( IsMine.at( Alpha - 1 ).at( Beta - 1 ) )
						Value++;
				if( Beta != 0 )
					if( IsMine.at( Alpha ).at( Beta - 1 ) )
						Value++;
				if( Alpha != GridAlpha - 1 && Beta != 0 )
					if( IsMine.at( Alpha + 1 ).at( Beta - 1 ) )
						Value++;
				if( Alpha != 0 )
					if( IsMine.at( Alpha - 1 ).at( Beta ) )
						Value++;
				if( Alpha != GridAlpha - 1 )
					if( IsMine.at( Alpha + 1 ).at( Beta ) )
						Value++;
				if( Alpha != 0 && Beta != GridBeta - 1 )
					if( IsMine.at( Alpha - 1 ).at( Beta + 1 ) )
						Value++;
				if( Beta != GridBeta - 1 )
					if( IsMine.at( Alpha ).at( Beta + 1 ) )
						Value++;
				if( Alpha != GridAlpha - 1 && Beta != GridBeta - 1 )
					if( IsMine.at( Alpha + 1 ).at( Beta + 1 ) )
						Value++;
				MineNumber.at( Alpha ).at( Beta ) = Value;
			}
		}
	}
}
void InitSettings(){
	GridAlpha = TempGridAlpha;
	GridBeta = TempGridBeta;
	Spacing = TempSpacing;
	MinesCount = TempMinesCount;
	BlockSize = TempBlockSize;
	Offset = TempOffset;
	if( MinesCount >= GridAlpha * GridBeta ){
		MinesCount = GridAlpha * GridBeta - 1;
		TempMinesCount = GridAlpha * GridBeta - 1;
	}
	while( GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f < 260 ){
		BlockSize++;
		TempBlockSize = BlockSize;
	}
	WindowAlpha = GridAlpha * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f,
	WindowBeta = GridBeta * ( BlockSize + Spacing ) - Spacing + Offset * 2.0f + BlockSize * 2.0f + 86.0f;
	SetWindowSize( WindowAlpha, WindowBeta );
	RestartButton = ( Rectangle ){
		GetScreenWidth() / 2.0f - BlockSize + Offset / 2.0f,
		Offset,
		BlockSize * 2.0f - Offset,
		BlockSize * 2.0f - Offset
	};
}
void DnitValues(){
	IsOpen.clear();
	IsMine.clear();
	HasFlag.clear();
	MineNumber.clear();
	GridAlpha = 20;
	GridBeta = 20;
	MinesCount = 50;
	DeadAlpha = -1;
	DeadBeta = -1;
	FirstAlpha = -1;
	FirstBeta = -1;
	FlagsCount = 0;
	RemainingMines = 0;
	FlagDisplayNumber_0 = 0;
	FlagDisplayNumber_1 = 0;
	FlagDisplayNumber_2 = 0;
	TimerDisplayNumber_0 = 0;
	TimerDisplayNumber_1 = 0;
	TimerDisplayNumber_2 = 0;
	BlockSize = 20;
	Spacing = 0;
	Offset = 10;
	Timer = 0;
	IWon = false;
	ILoss = false;
	Double_Click_Active = false;
	FirstClick = true;
	EditMode_1 = false;
	EditMode_2 = false;
	EditMode_3 = false;
	EditMode_4 = false;
	EditMode_5 = false;
	EditMode_6 = false;
	Double_Click_Timer = 0;
}
void InitTextures(){
	Close = LoadTexture( "Textures/Block_0.png" );
	Open = LoadTexture( "Textures/Block_1.png" );
	SelectionClose = LoadTexture( "Textures/Block_2.png" );
	SelectionOpen = LoadTexture( "Textures/Block_3.png" );
	Mine = LoadTexture( "Textures/Mine_0.png" );
	Exploded = LoadTexture( "Textures/Mine_1.png" );
	Smily = LoadTexture( "Textures/Face_0.png" );
	Dead = LoadTexture( "Textures/Face_1.png" );
	Glasses = LoadTexture( "Textures/Face_2.png" );
	Flag = LoadTexture( "Textures/Flag_0.png" );
	Mark = LoadTexture( "Textures/Flag_1.png" );
	Num_1 = LoadTexture( "Textures/1.png" );
	Num_2 = LoadTexture( "Textures/2.png" );
	Num_3 = LoadTexture( "Textures/3.png" );
	Num_4 = LoadTexture( "Textures/4.png" );
	Num_5 = LoadTexture( "Textures/5.png" );
	Num_6 = LoadTexture( "Textures/6.png" );
	Num_7 = LoadTexture( "Textures/7.png" );
	Num_8 = LoadTexture( "Textures/8.png" );
	Display_0 = LoadTexture( "Textures/Num_0.png" );
	Display_1 = LoadTexture( "Textures/Num_1.png" );
	Display_2 = LoadTexture( "Textures/Num_2.png" );
	Display_3 = LoadTexture( "Textures/Num_3.png" );
	Display_4 = LoadTexture( "Textures/Num_4.png" );
	Display_5 = LoadTexture( "Textures/Num_5.png" );
	Display_6 = LoadTexture( "Textures/Num_6.png" );
	Display_7 = LoadTexture( "Textures/Num_7.png" );
	Display_8 = LoadTexture( "Textures/Num_8.png" );
	Display_9 = LoadTexture( "Textures/Num_9.png" );
	Close_Source = ( Rectangle ){ 0.0f, 0.0f, Close.width, Close.height };
	Open_Source = ( Rectangle ){ 0.0f, 0.0f, Open.width, Open.height };
	SelectionClose_Source = ( Rectangle ){ 0.0f, 0.0f, SelectionClose.width, SelectionClose.height };
	SelectionOpen_Source = ( Rectangle ){ 0.0f, 0.0f, SelectionOpen.width, SelectionOpen.height };
	Mine_Source = ( Rectangle ){ 0.0f, 0.0f, Mine.width, Mine.height };
	Exploded_Source = ( Rectangle ){ 0.0f, 0.0f, Exploded.width, Exploded.height };
	Smily_Source = ( Rectangle ){ 0.0f, 0.0f, Smily.width, Smily.height };
	Dead_Source = ( Rectangle ){ 0.0f, 0.0f, Dead.width, Dead.height };
	Glasses_Source = ( Rectangle ){ 0.0f, 0.0f, Glasses.width, Glasses.height };
	Flag_Source = ( Rectangle ){ 0.0f, 0.0f, Flag.width, Flag.height };
	Mark_Source = ( Rectangle ){ 0.0f, 0.0f, Mark.width, Mark.height };
	Num_1_Source = ( Rectangle ){ 0.0f, 0.0f, Num_1.width, Num_1.height };
	Num_2_Source = ( Rectangle ){ 0.0f, 0.0f, Num_2.width, Num_2.height };
	Num_3_Source = ( Rectangle ){ 0.0f, 0.0f, Num_3.width, Num_3.height };
	Num_4_Source = ( Rectangle ){ 0.0f, 0.0f, Num_4.width, Num_4.height };
	Num_5_Source = ( Rectangle ){ 0.0f, 0.0f, Num_5.width, Num_5.height };
	Num_6_Source = ( Rectangle ){ 0.0f, 0.0f, Num_6.width, Num_6.height };
	Num_7_Source = ( Rectangle ){ 0.0f, 0.0f, Num_7.width, Num_7.height };
	Num_8_Source = ( Rectangle ){ 0.0f, 0.0f, Num_8.width, Num_8.height };
	Display_0_Source = ( Rectangle ){ 0.0f, 0.0f, Display_0.width, Display_0.height },
	Display_1_Source = ( Rectangle ){ 0.0f, 0.0f, Display_1.width, Display_1.height },
	Display_2_Source = ( Rectangle ){ 0.0f, 0.0f, Display_2.width, Display_2.height },
	Display_3_Source = ( Rectangle ){ 0.0f, 0.0f, Display_3.width, Display_3.height },
	Display_4_Source = ( Rectangle ){ 0.0f, 0.0f, Display_4.width, Display_4.height },
	Display_5_Source = ( Rectangle ){ 0.0f, 0.0f, Display_5.width, Display_5.height },
	Display_6_Source = ( Rectangle ){ 0.0f, 0.0f, Display_6.width, Display_6.height },
	Display_7_Source = ( Rectangle ){ 0.0f, 0.0f, Display_7.width, Display_7.height },
	Display_8_Source = ( Rectangle ){ 0.0f, 0.0f, Display_8.width, Display_8.height },
	Display_9_Source = ( Rectangle ){ 0.0f, 0.0f, Display_9.width, Display_9.height };
}
void DnitTextures(){
};
bool EditModes(){
	return ( EditMode_1 || EditMode_2 || EditMode_3 || EditMode_4 || EditMode_5 || EditMode_6 );
}
void OpenRest( int Alpha, int Beta ){
	IsOpen.at( Alpha ).at( Beta ) = true;
	bool Stop = false;
	int Index = 0, IncreaseIndex = 0;
	vector< int > OpeningAlpha;
	vector< int > OpeningBeta;
	OpeningAlpha.push_back( Alpha );
	OpeningBeta.push_back( Beta );
	while( !Stop ){
		Stop = true;
		if( OpeningBeta.at( Index ) != 0 && OpeningAlpha.at( Index ) != 0 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) - 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) - 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) - 1 ) = true;
			}
		}
		if( OpeningBeta.at( Index ) != 0 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) );
					OpeningBeta.push_back( OpeningBeta.at( Index ) - 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) - 1 ) = true;
			}
		}
		if( OpeningBeta.at( Index ) != 0 && OpeningAlpha.at( Index ) != GridAlpha - 1 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) + 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) - 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) - 1 ) = true;
			}
		}
		if( OpeningAlpha.at( Index ) != 0 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) - 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) ) = true;
			}
		}
		if( OpeningAlpha.at( Index ) != GridAlpha - 1 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) + 1).at( OpeningBeta.at( Index ) ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) + 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) ) = true;
			}
		}
		if( OpeningBeta.at( Index ) != GridBeta - 1 && OpeningAlpha.at( Index ) != 0 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) - 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) + 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) - 1 ).at( OpeningBeta.at( Index ) + 1 ) = true;
			}
		}
		if( OpeningBeta.at( Index ) != GridBeta - 1 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) );
					OpeningBeta.push_back( OpeningBeta.at( Index ) + 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) ).at( OpeningBeta.at( Index ) + 1 ) = true;
			}
		}
		if( OpeningBeta.at( Index ) != GridBeta - 1 && OpeningAlpha.at( Index ) != GridAlpha - 1 ){
			if(
				!IsMine.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 ) &&
				!IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 ) &&
				!HasFlag.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 )
			){
				if( MineNumber.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 ) == 0 ){
					IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 ) = true;
					OpeningAlpha.push_back( OpeningAlpha.at( Index ) + 1 );
					OpeningBeta.push_back( OpeningBeta.at( Index ) + 1 );
					IncreaseIndex++;
					Stop = false;
				}
				else IsOpen.at( OpeningAlpha.at( Index ) + 1 ).at( OpeningBeta.at( Index ) + 1 ) = true;
			}
		}
		Index++;
		if( Index < IncreaseIndex )
			Stop = false;
		else Stop = true;
	}
}
void OpenFlag( int Alpha, int Beta ){
	int Value = 0;
	if( Alpha != 0 && Beta != 0 )
		if( HasFlag.at( Alpha - 1 ).at( Beta - 1 ) )
			Value++;
	if( Beta != 0 )
		if( HasFlag.at( Alpha ).at( Beta - 1 ) )
			Value++;
	if( Alpha != GridAlpha - 1 && Beta != 0 )
		if( HasFlag.at( Alpha + 1 ).at( Beta - 1 ) )
			Value++;
	if( Alpha != 0 )
		if( HasFlag.at( Alpha - 1 ).at( Beta ) )
			Value++;
	if( Alpha != GridAlpha - 1 )
		if( HasFlag.at( Alpha + 1 ).at( Beta ) )
			Value++;
	if( Alpha != 0 && Beta != GridBeta - 1 )
		if( HasFlag.at( Alpha - 1 ).at( Beta + 1 ) )
			Value++;
	if( Beta != GridBeta - 1 )
		if( HasFlag.at( Alpha ).at( Beta + 1 ) )
			Value++;
	if( Alpha != GridAlpha - 1 && Beta != GridBeta - 1 )
		if( HasFlag.at( Alpha + 1 ).at( Beta + 1 ) )
			Value++;
	if( Value == MineNumber.at( Alpha ).at( Beta ) ){
		if( Alpha != 0 && Beta != 0 ){
			if( !HasFlag.at( Alpha - 1 ).at( Beta - 1 ) ){
				IsOpen.at( Alpha - 1 ).at( Beta - 1 ) = true;
				OpenRest( Alpha - 1, Beta - 1 );
				if( IsMine.at( Alpha - 1 ).at( Beta - 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha - 1;
					DeadBeta = Beta - 1;
				}
			}
		}
		if( Beta != 0 ){
			if( !HasFlag.at( Alpha ).at( Beta - 1 ) ){
				IsOpen.at( Alpha ).at( Beta - 1 ) = true;
				OpenRest( Alpha, Beta - 1 );
				if( IsMine.at( Alpha ).at( Beta - 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha;
					DeadBeta = Beta - 1;
				}
			}
		}
		if( Alpha != GridAlpha - 1 && Beta != 0 ){
			if( !HasFlag.at( Alpha + 1 ).at( Beta - 1 ) ){
				IsOpen.at( Alpha + 1 ).at( Beta - 1 ) = true;
				OpenRest( Alpha + 1, Beta - 1 );
				if( IsMine.at( Alpha + 1 ).at( Beta - 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha + 1;
					DeadBeta = Beta - 1;
				}
			}
		}
		if( Alpha != 0 ){
			if( !HasFlag.at( Alpha - 1 ).at( Beta ) ){
				IsOpen.at( Alpha - 1 ).at( Beta ) = true;
				OpenRest( Alpha - 1, Beta );
				if( IsMine.at( Alpha - 1 ).at( Beta ) ){
					ILoss = true;
					DeadAlpha = Alpha - 1;
					DeadBeta = Beta;
				}
			}
		}
		if( Alpha != GridAlpha - 1 ){
			if( !HasFlag.at( Alpha + 1 ).at( Beta ) ){
				IsOpen.at( Alpha + 1 ).at( Beta ) = true;
				OpenRest( Alpha + 1, Beta );
				if( IsMine.at( Alpha + 1 ).at( Beta ) ){
					ILoss = true;
					DeadAlpha = Alpha + 1;
					DeadBeta = Beta;
				}
			}
		}
		if( Alpha != 0 && Beta != GridBeta - 1 ){
			if( !HasFlag.at( Alpha - 1 ).at( Beta + 1 ) ){
				IsOpen.at( Alpha - 1 ).at( Beta + 1 ) = true;
				OpenRest( Alpha - 1, Beta + 1 );
				if( IsMine.at( Alpha - 1 ).at( Beta + 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha - 1;
					DeadBeta = Beta + 1;
				}
			}
		}
		if( Beta != GridBeta - 1 ){
			if( !HasFlag.at( Alpha ).at( Beta + 1 ) ){
				IsOpen.at( Alpha ).at( Beta + 1 ) = true;
				OpenRest( Alpha, Beta + 1 );
				if( IsMine.at( Alpha ).at( Beta + 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha;
					DeadBeta = Beta + 1;
				}
			}
		}
		if( Alpha != GridAlpha - 1 && Beta != GridBeta - 1 ){
			if( !HasFlag.at( Alpha + 1 ).at( Beta + 1 ) ){
				IsOpen.at( Alpha + 1 ).at( Beta + 1 ) = true;
				OpenRest( Alpha + 1, Beta + 1 );
				if( IsMine.at( Alpha + 1 ).at( Beta + 1 ) ){
					ILoss = true;
					DeadAlpha = Alpha + 1;
					DeadBeta = Beta + 1;
				}
			}
		}
	}
}
void DrawNumber( int Alpha, int Beta, Rectangle Block, Vector2 BlockOrigin ){	
	switch( MineNumber.at( Alpha ).at( Beta ) ){
		case 1: DrawTexturePro( Num_1, Num_1_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 2: DrawTexturePro( Num_2, Num_2_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 3: DrawTexturePro( Num_3, Num_3_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 4: DrawTexturePro( Num_4, Num_4_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 5: DrawTexturePro( Num_5, Num_5_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 6: DrawTexturePro( Num_6, Num_6_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 7: DrawTexturePro( Num_7, Num_7_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
		case 8: DrawTexturePro( Num_8, Num_8_Source, Block, BlockOrigin, 0.0f, WHITE ); break;
	}
}
void DrawBorders(){
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, Offset * 0.5f, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, GetScreenHeight() - Offset, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 255, 255, 255, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, Offset * 0.5f, Offset * 0.5f, GetScreenHeight() - Offset * 1.5f },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ GetScreenWidth() - Offset, Offset * 0.5f, Offset * 0.5f, GetScreenHeight() - Offset * 1.5f },
		( Color ){ 255, 255, 255, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, Offset * 0.5f + BlockSize * 2.0f, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, BlockSize * 2.0f, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 255, 255, 255, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, GetScreenHeight() - 86.0f - Offset * 0.5f, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ Offset * 0.5f, GetScreenHeight() - 86.0f - Offset, GetScreenWidth() - Offset, Offset * 0.5f },
		( Color ){ 255, 255, 255, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ GetScreenWidth() / 2.0f - BlockSize, Offset, Offset * 0.5f, BlockSize * 2.0f - Offset },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ GetScreenWidth() / 2.0f + BlockSize - Offset * 0.5f, Offset, Offset * 0.5f, BlockSize * 2.0f - Offset },
		( Color ){ 255, 255, 255, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ GetScreenWidth() - BlockSize * 3.0f - Offset * 1.5f, Offset, Offset * 0.5f, BlockSize * 2.0f - Offset },
		( Color ){ 127, 127, 127, 255 }
	);
	DrawRectangleRec(
		( Rectangle ){ BlockSize * 3.0f + Offset, Offset, Offset * 0.5f, BlockSize * 2.0f - Offset },
		( Color ){ 255, 255, 255, 255 }
	);
}
void DrawDisplay(){
	RemainingMines = MinesCount - FlagsCount;
	if( RemainingMines < 0 ) RemainingMines = 0;
	FlagDisplayNumber_2 = RemainingMines % 10;
	FlagDisplayNumber_1 = ( RemainingMines / 10 ) % 10;
	FlagDisplayNumber_0 = ( RemainingMines / 100 ) % 10;
	if( Timer > 0 && !( ILoss || IWon ) ){
		TimerDisplayNumber_0 = ( time( 0 ) - Timer ) % 10;
		TimerDisplayNumber_1 = ( ( time( 0 ) - Timer ) / 10 ) % 10;
		TimerDisplayNumber_2 = ( ( time( 0 ) - Timer ) / 100 ) % 10;
	}
	switch( FlagDisplayNumber_0 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ Offset, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
	switch( FlagDisplayNumber_1 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ Offset + BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
	switch( FlagDisplayNumber_2 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ Offset + BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
	switch( TimerDisplayNumber_0 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
	switch( TimerDisplayNumber_1 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 2.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
	switch( TimerDisplayNumber_2 ){
		case 0: { DrawTexturePro( Display_0, Display_0_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 1: { DrawTexturePro( Display_1, Display_1_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 2: { DrawTexturePro( Display_2, Display_2_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 3: { DrawTexturePro( Display_3, Display_3_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 4: { DrawTexturePro( Display_4, Display_4_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 5: { DrawTexturePro( Display_5, Display_5_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 6: { DrawTexturePro( Display_6, Display_6_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 7: { DrawTexturePro( Display_7, Display_7_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 8: { DrawTexturePro( Display_8, Display_8_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
		case 9: { DrawTexturePro( Display_9, Display_9_Source, ( Rectangle ){ GetScreenWidth() - Offset - BlockSize * 3.0f, Offset, BlockSize, BlockSize * 2.0f - Offset }, ( Vector2 ){ 0.0f, 0.0f }, 0.0f, WHITE ); } break;
	}
}
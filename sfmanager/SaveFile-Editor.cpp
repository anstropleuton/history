#include <Editor-Core.h>
using namespace std;
int main( int argc, char* argv[] ){
	SetConfigFlags( FLAG_WINDOW_RESIZABLE );
	InitWindow( 1200, 600, "GamerZ BPLE Editor" );
	SetTargetFPS( 60 );
	vector< GZBPLE::RL_Window > GUI_Windows;
	GZBPLE::World World;
	GZBPLE::Load_Part_Textures();
	int SelectedPart = 0, SelectedSkin = 0;
	World.Load_Level( "Files/Levels/Level_Sandbox_06" );
	Camera2D PlayerCamera = { 0 };
	PlayerCamera.zoom = 20.0f;
	while( !WindowShouldClose() ){
		PlayerCamera.offset = Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
		if( IsMouseButtonDown( MOUSE_BUTTON_MIDDLE ) ){
			PlayerCamera.target.x -= GetMouseDelta().x / PlayerCamera.zoom;
			PlayerCamera.target.y -= GetMouseDelta().y / PlayerCamera.zoom;
		}
		PlayerCamera.zoom += ( GetMouseWheelMove() * PlayerCamera.zoom ) / 16.0f;
		if( GetMouseWheelMove() > 0.0f ){
			PlayerCamera.target.x += ( GetMousePosition().x - ( GetScreenWidth() * 0.5f ) ) / ( PlayerCamera.zoom * 16.0f );
			PlayerCamera.target.y += ( GetMousePosition().y - ( GetScreenHeight() * 0.5f ) ) / ( PlayerCamera.zoom * 16.0f );
		}
		else if( GetMouseWheelMove() < 0.0f ){
			PlayerCamera.target.x -= ( GetMousePosition().x - ( GetScreenWidth() * 0.5f ) ) / ( PlayerCamera.zoom * 16.0f );
			PlayerCamera.target.y -= ( GetMousePosition().y - ( GetScreenHeight() * 0.5f ) ) / ( PlayerCamera.zoom * 16.0f );
		}
		if( IsKeyPressed( KEY_Q ) ) SelectedPart++;
		if( IsKeyPressed( KEY_A ) ) SelectedPart--;
		if( IsKeyPressed( KEY_W ) ) SelectedSkin++;
		if( IsKeyPressed( KEY_S ) ) SelectedSkin--;
		if( SelectedPart < 1 ) SelectedPart = 1;
		if( SelectedSkin < 0 ) SelectedSkin = 0;
		if( SelectedPart >= GZBPLE::SkinsPerPart.size() ) SelectedPart = GZBPLE::SkinsPerPart.size() - 1;
		if( SelectedSkin >= GZBPLE::SkinsPerPart.at( SelectedPart ) ) SelectedSkin = GZBPLE::SkinsPerPart.at( SelectedPart ) - 1;
		if( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) World.Insert_Part( SelectedPart, SelectedSkin, ( int ) std::floor( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).x ), ( int ) std::ceil( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).y * -1.0f ), 0, 0 );
		if( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) World.Delete_Pos( ( int ) std::floor( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).x ), ( int ) std::ceil( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).y * -1.0f ), 4, 0 );
		if( IsKeyPressed( KEY_ENTER ) ) World.Save_Level( "Files/Levels/Level_Sandbox_06" );
		if( IsKeyPressed( KEY_INSERT ) ) World.Load_Level( "Files/Levels/Level_Sandbox_06" );
		if( IsKeyPressed( KEY_DELETE ) ) World.Part.clear();
		BeginDrawing();
		{
			ClearBackground( WHITE );
			BeginMode2D( PlayerCamera );
			{
				for( float Index = -500; Index < 500 && PlayerCamera.zoom > 5.0f; Index++ ){
					Vector2 Position = Vector2{ PlayerCamera.target.x, PlayerCamera.target.y };
					Position.x = round( Position.x );
					Position.y = round( Position.y );
					DrawLine( Position.x - 500.0f, Position.y + Index, Position.x + 500.0f, Position.y + Index, RED );
					DrawLine( Position.x + Index, Position.y - 500.0f, Position.x + Index, Position.y + 500.0f, BLUE );
				}
				GZBPLE::Draw_All_Parts( World );
			}
			EndMode2D();
			DrawText(
				TextFormat(
					"%i,%i\n%i,%i",
					SelectedPart,
					SelectedSkin,
					( int ) std::floor( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).x ),
					( int ) std::ceil( GetScreenToWorld2D( GetMousePosition(), PlayerCamera ).y * -1.0f )
				),
				20,
				20,
				20,
				BLACK
			);
		}
		EndDrawing();
	}
}
#include <cmath>
#include <string>
#include <sstream>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
int intExtract( std::string input ){
	std::stringstream convert;
	int output = 0, z;
	bool negative = false;
	for( z = 0; z < input.length (); z++ ){
		if( input[ z ] >= '0' && input[ z ] <= '9' ) convert << input[ z ] ;
		if( input[ z ] == '-' ) negative = true;
	}
	convert >> output;
	if( negative ) output = output * - 1.0f;
	return output;
};
int main(){
	char text[ 4 ][ 1024 ] = { "", "", "", "" };
	bool active[ 4 ] = { true, false, false, false };
	std::string text_output;
	SetConfigFlags( FLAG_WINDOW_RESIZABLE );
	InitWindow( 1000, 500, "Jet Calculator" );
	SetTargetFPS( 60 );
	while( !WindowShouldClose() ){ BeginDrawing(); ClearBackground( RAYWHITE );
		DrawText( "Jets Count", 20, 20, 20, GRAY );
		DrawText( "Jet Thrust", 20, 50, 20, GRAY );
		DrawText( "Tanks Count", 20, 80, 20, GRAY );
		DrawText( "Motors Count", 20, 110, 20, GRAY );
		if( GuiTextBoxAdvanced( Rectangle{ 180, 20, ( float ) GetScreenWidth () - 200, 20 }, ( char * ) text[ 0 ] , 1024, active[ 0 ] ) ) active[ 0 ] = !active[ 0 ] ;
		if( GuiTextBoxAdvanced( Rectangle{ 180, 50, ( float ) GetScreenWidth () - 200, 20 }, ( char * ) text[ 1 ] , 1024, active[ 1 ] ) ) active[ 1 ] = !active[ 1 ] ;
		if( GuiTextBoxAdvanced( Rectangle{ 180, 80, ( float ) GetScreenWidth () - 200, 20 }, ( char * ) text[ 2 ] , 1024, active[ 2 ] ) ) active[ 2 ] = !active[ 2 ] ;
		if( GuiTextBoxAdvanced( Rectangle{ 180, 110, ( float ) GetScreenWidth () - 200, 20 }, ( char * ) text[ 3 ] , 1024, active[ 3 ] ) ) active[ 3 ] = !active[ 3 ] ;
		if( active[ 0 ] && IsKeyPressed( KEY_TAB ) ){ active[ 0 ] = false; active[ 1 ] = true; }
		else if( active[ 1 ] && IsKeyPressed( KEY_TAB ) ){ active[ 1 ] = false; active[ 2 ] = true; }
		else if( active[ 2 ] && IsKeyPressed( KEY_TAB ) ){ active[ 2 ] = false; active[ 3 ] = true; }
		else if( active[ 3 ] && IsKeyPressed( KEY_TAB ) ){ active[ 3 ] = false; active[ 0 ] = true; }
		if( !active[ 0 ] && !active[ 1 ] && !active[ 2 ] && !active[ 3 ] && IsKeyPressed( KEY_TAB ) ) active[ 0 ] = true;
		if( IsKeyPressed( KEY_ENTER ) ){
			double jets = 0.0f, thrust = 0.0f, motors = 0.0f, tanks = 0.0f, drain_speed = 0.0f, refill_speed = 0.0f, increase = 0.0f, result = 0.0f, fuel_time = 0.0f;
			bool ceilMode = false;
			std::stringstream text_format;
			jets = intExtract( text[ 0 ] );
			thrust = intExtract( text[ 1 ] );
			tanks = intExtract( text[ 2 ] );
			motors = intExtract( text[ 3 ] );
			drain_speed = ( jets * ( ( 0.2f * ( thrust / 1000.0f ) ) + 0.1f ) );
			refill_speed = motors * 0.2f;
			if( thrust == 0.0f ) drain_speed = 0.0f;
			result = refill_speed - drain_speed;
			text_format << " + " << refill_speed << " KG/s (Refilling) == [ " << motors << " (Motors) * 0.2 (Refilling Speed) ] \n"
						<< " - " << drain_speed << " KG/s (Draining) == [ " << jets << " (Jet Engines) * " << drain_speed / jets << " (Jet Drain Formula) ] \n"
		 				<< " = " << result << " KG/s (Total) == [Refilling - Draining] \n";
			if( result < 0 ){
				fuel_time = ( tanks * 4.0f ) / ( result * - 1.0f );
				text_format << "-----Your Fuel Tank is draining-----\nIt would take " << fuel_time << "s to drain out all the tanks\n";
			}
			else{
				fuel_time = ( tanks * 4.0f ) / result;
				text_format << "+++Your Fuel Tank is not draining+++\nIt would take " << fuel_time << "s to fill up all the empty tanks\n";
			}
			if( drain_speed > tanks ){
				increase = std::ceil( drain_speed );
				fuel_time = ( increase * 4.0f ) / ( result * - 1.0f );
				text_format << "Due to the draining speed of a single tank (1.0 KG/s) \n"
							<< "You are required to add more tanks\n"
							<< "if you increase the amount of tanks to be " << increase << std::endl
							<< "Then it would take " << fuel_time << "s to drain out all the tanks\nOr else the jet engines won't get enough fuel to keep up with the thrust\n";
			}
			else if( refill_speed > tanks * 0.5f ){
				increase = std::ceil( refill_speed * 2.0f );
				text_format << "Due to the reffiling speed limit of a single tank (0.5 KG/s) \n"
							<< "You are required to add more tanks\n"
							<< "if you increase the amount of tanks to be " << increase << std::endl
							<< "Then all the motors gets in use to refilling up your tanks\nOr else some motors aren't in use in refilling tanks";
			}
			text_output = text_format.str();
			text_format.clear();
			text_format.str( "" );
		}
		DrawText( text_output.c_str() , 20, 140, 20, GRAY );
	EndDrawing(); }
}
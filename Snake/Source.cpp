// We are making the game "snake" in this code...

// Setup the environment for the game to render on the screen instead of the
// usual console

// Include Raylib as a framework to launch a window
#include <raylib.h>

// Include vector which is a type of array
#include <vector>

// Include time to set the randomizer
#include <ctime>

// Include math for std::pow
#include <cmath>

// Include file scream to load score list
#include <fstream>

// Include string stream to convert integer to string
#include <sstream>

// Definations are settings in this game

// How fast the zoom in / out takes place
#define ZOOM_SPEED 16.0f

// Game's frames per second count ( roughly )
#define GAME_FPS 60

// Snake's speed in seconds ( roughly )
#define SNAKE_DELAY 0.125f

// How far the apple must be spawned to the snake's head
#define SPAWN_AREA 8

// The boundery of the snake grid
#define BOUNDERY 64

// Get new apple position
Vector2 NewApplePosition( std::vector< Vector2 >& Input ){

	// Set random seed
	SetRandomSeed( std::time( 0 ) );

	// Make output vector
	Vector2 Output;

	// Check if the apple position if valid
	bool InValidPosition = true;

	// RetryChances
	long long unsigned int RetryChances = std::pow( BOUNDERY * 2, 8 );

	// If not then loop and repeat
	for( long long unsigned int Retries = 0; Retries < RetryChances && InValidPosition; Retries++ ){

		// Reset the flag
		InValidPosition = false;
		
		// Initialise output vector with new random position
		Output = {

			// Random value for x depending on snake's head position
			( float ) GetRandomValue(
				( int ) Input.at( 0 ).x - SPAWN_AREA,
				( int ) Input.at( 0 ).y + SPAWN_AREA
			),

			// Random value for y depending on snake's head position
			( float ) GetRandomValue(
				( int ) Input.at( 0 ).x - SPAWN_AREA,
				( int ) Input.at( 0 ).y + SPAWN_AREA
			)

		};

		// Check if the position is inside snake
		for( unsigned int Index = 0; Index < Input.size(); Index++ ){

			if(
				( int ) Output.x == ( int ) Input.at( Index ).x &&
				( int ) Output.y == ( int ) Input.at( Index ).y
			){
				// Set the flag that the position is invalid
				InValidPosition = true;

				// Break out of for loop
				break;

			}

		}

		// Check if the apple is outside boundery
		if(
			( int ) Output.x < -BOUNDERY ||
			( int ) Output.x > BOUNDERY - 1 ||
			( int ) Output.y < -BOUNDERY ||
			( int ) Output.y > BOUNDERY - 1
		) InValidPosition = true;

	}

	// If it was still invalid, Set it somewhere really far
	if( InValidPosition ) Output = {
		( float ) RetryChances,
		( float ) RetryChances
	};

	// And finally return the position
	return Output;

}

// Extract integer out of string
// Note: Negative number is unused in this game
int ConvertToInteger( std::string Input ){

	// Converter
    std::stringstream Convert;

	// Resulting integer
    int Output = 0;

	// Flag of if the number is negative
    bool Negative = false;

	// Check for number for each character
    for( unsigned int Index = 0; Index < Input.length(); Index++ ){

		// If the character is a number from 0 to 9 then add it to the converter
        if( Input[ Index ] >= '0' && Input[ Index ] <= '9' ) Convert << Input[ Index ];

		// If the character is a minus sign set the negative flag
        if( Input[ Index ] == '-' ) Negative = true;

    }

	// Convert the list of characters from the converter to integer
    Convert >> Output;

	// If the negative flag was set, Make the resulting integer to be negative
    if( Negative ) Output = Output * -1;

	// Finally return the result
    return Output;

}

// Main entry point of the program
int main(){

	// Set the configuration flag to let
	// allow resizing thw window as the user's wish
	SetConfigFlags( FLAG_WINDOW_RESIZABLE );

	// Start the game as maximized window
	SetConfigFlags( FLAG_WINDOW_MAXIMIZED );

	// Initialise window
	InitWindow( 1200, 600, "My Window Is Ready" );

	// Set the FPS of the game to run on
	SetTargetFPS( GAME_FPS );

	// Maximize the window if it fails to
	// initialise already being maximized
	MaximizeWindow();

	// Set new randomizer
	SetRandomSeed( std::time( 0 ) );

	// Setup the camera for the game
	Camera2D PlayerCamera = { 0 };
	PlayerCamera.zoom = 25.0f;
	PlayerCamera.offset = {
		GetScreenHeight() / 2.0f,
		GetScreenHeight() / 2.0f
	};

	// Make a vector to store the tail of the snake
	//
	// Note: Vector2 is the type of vector defined
	// in the raylib header that functions as
	// position holder
	//
	// Note: PlayerTail also holds the head
	std::vector< Vector2 > Snake;

	// Insert the head position in the vector
	Snake.push_back( Vector2{ -1.0f, -1.0f } );

	// The place where the apple is available
	Vector2 Apple = NewApplePosition( Snake );

	// Temporary Rectangle used to draw the snake
	Rectangle BoxBounds;

	// Temporary Color used to draw the snake ( only body )
	Color SnakeBodyColor;

	// Snake head color
	Color SnakeHeadColor = { 0, 192, 0, 255 };

	// Apple color
	Color AppleColor = { 255, 0, 0, 255 };

	// Consola typeface ( Font )
	Font Consola = LoadFont( "Files/Consola.ttf" );

	// Posible direction of the snake
	enum Directions{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// Make a variable for direction control
	Directions SnakeDirection = RIGHT;

	// Check if the player didn't entered two moved
	bool DirectionChanged = false;

	// Temporary variable for the timing of the snake
	int Timer = 0;

	// Store the data of if the player is won or loss
	// And data of if the game is paused
	bool Won = false, Loss = false, Paused = true;

	// Top score from score list
	int TopScore = 0;

	// Lost of scores
	std::vector< int > Scores;

	// Temporary string of score
	std::string Score = "0";

	// Load scores from list
	std::ifstream InFile( "Files/Scores.txt" );

	if( InFile.fail() )
		goto SkipScoreList;

	// While there is something in the file
	// Add new scores
	while( std::getline( InFile, Score ) ){

		// Convert the score line to integer
		TopScore = ConvertToInteger( Score );

		// And add the score to the list
		Scores.push_back( TopScore );

	}

	// Now find the best score from the list
	for( unsigned int Index = 0; Index < Scores.size(); Index++ )

		// If the top score was lower than the score from the list
		// set the top score to the score from the list
		if( TopScore < Scores.at( Index ) ) TopScore = Scores.at( Index );
	
	// If the file failed to open, start from here
SkipScoreList:

	// Will not close the window while it is running
	while( !WindowShouldClose() ){

		// Update timer only if the game is not won / loss / pause
		if( !Won && !Loss && !Paused ) Timer++;

		// Or set the game to be paused
		else Paused = true;

		// If player has pressed space or enter
		// Restart the game or pause the game
		if( IsKeyPressed( KEY_SPACE ) || IsKeyPressed( KEY_ENTER ) ){

			// If the player was loss or won
			if( Loss || Won ){

				// Clear the states
				Loss = false;
				Won = false;
				Paused = false;

				// Add the score to the list
				Scores.push_back( Snake.size() - 1 );

				// Reset the snake
				Snake.clear();
				Snake.push_back( Vector2{ -1.0f, -1.0f } );

				// Get a new place for the apple accoding to snake position
				Apple = NewApplePosition( Snake );

				// Reset the direction
				SnakeDirection = RIGHT;

				// Output the scores to the scores.txt file
				std::ofstream OutFile( "Files/Scores.txt" );

				// Output all the scores
				for( unsigned int Index = 0; Index < Scores.size(); Index++ )

					// One by one and new line for each score
					OutFile << Scores.at( Index ) << std::endl;

				// Now find the best score from the list
				for( unsigned int Index = 0; Index < Scores.size(); Index++ )

					// If the top score was lower than the score from the list
					// set the top score to the score from the list
					if( TopScore < Scores.at( Index ) ) TopScore = Scores.at( Index );

			}

			// If not then pause / resume the game
			else Paused = !Paused;
		}

		// Update Camera offset
		PlayerCamera.offset = {
			GetScreenWidth() / 2.0f,
			GetScreenHeight() / 2.0f
		};

		// Move the camera ( Use the mouse middle button to move the camera )
		if( IsMouseButtonDown( MOUSE_BUTTON_MIDDLE ) ){
			PlayerCamera.target.x -= GetMouseDelta().x / PlayerCamera.zoom;
			PlayerCamera.target.y -= GetMouseDelta().y / PlayerCamera.zoom;
		}

		// Detect if the mouse wheel if moved
		if( GetMouseWheelMove() != 0.0f ){

			// Zoom Controller ( Use the mouse wheel to zoom in / out )
			PlayerCamera.zoom += GetMouseWheelMove() * PlayerCamera.zoom
								 / ZOOM_SPEED;

			// Zooming in
			if( GetMouseWheelMove() > 0.0f ){
				PlayerCamera.target.x +=
					( GetMousePosition().x - PlayerCamera.offset.x )
					/ ( PlayerCamera.zoom * ZOOM_SPEED );
				PlayerCamera.target.y +=
					( GetMousePosition().y - PlayerCamera.offset.y )
					/ ( PlayerCamera.zoom * ZOOM_SPEED );
			}

			// Zooming out
			else if( GetMouseWheelMove() < 0.0f ){
				PlayerCamera.target.x -=
				( GetMousePosition().x - PlayerCamera.offset.x )
				/ ( PlayerCamera.zoom * ZOOM_SPEED );
				PlayerCamera.target.y -=
				( GetMousePosition().y - PlayerCamera.offset.y )
				/ ( PlayerCamera.zoom * ZOOM_SPEED );
			}
		}

		// Limit the zoom
		// For maximum
		if( PlayerCamera.zoom < 1.0f ) PlayerCamera.zoom = 1.0f;

		// For minimum
		if( PlayerCamera.zoom > 100.0f ) PlayerCamera.zoom = 100.0f;

		// Limit the target position
		if( ( int ) PlayerCamera.target.x < -BOUNDERY ) PlayerCamera.target.x = -BOUNDERY;
		if( ( int ) PlayerCamera.target.x > BOUNDERY - 1 ) PlayerCamera.target.x = BOUNDERY - 1;
		if( ( int ) PlayerCamera.target.y < -BOUNDERY ) PlayerCamera.target.y = -BOUNDERY;
		if( ( int ) PlayerCamera.target.y > BOUNDERY - 1 ) PlayerCamera.target.y = BOUNDERY - 1;

		// Check if the player has hit the apple
		// Note: Comparing floating points is not
		// a good idea so you need to convert it to int
		if(
			( int ) Snake.at( 0 ).x == ( int ) Apple.x &&
			( int ) Snake.at( 0 ).y == ( int ) Apple.y
		){

			// New apple position
			Apple = NewApplePosition( Snake );

			// Make a new tale for the snake
			Snake.push_back( Snake.at( Snake.size() - 1 ) );

		}

		// Snake control
		if( IsKeyPressed( KEY_W ) || IsKeyPressed( KEY_UP ) )
			if( SnakeDirection != DOWN && !Paused && !DirectionChanged ){
				SnakeDirection = UP;
				DirectionChanged = true;
			}

		if( IsKeyPressed( KEY_S ) || IsKeyPressed( KEY_DOWN ) )
			if( SnakeDirection != UP && !Paused && !DirectionChanged ){
				SnakeDirection = DOWN;
				DirectionChanged = true;
			}

		if( IsKeyPressed( KEY_A ) || IsKeyPressed( KEY_LEFT ) )
			if( SnakeDirection != RIGHT && !Paused && !DirectionChanged ){
				SnakeDirection = LEFT;
				DirectionChanged = true;
			}

		if( IsKeyPressed( KEY_D ) || IsKeyPressed( KEY_RIGHT ) )
			if( SnakeDirection != LEFT && !Paused && !DirectionChanged ){
				SnakeDirection = RIGHT;
				DirectionChanged = true;
			}

		// Update if it is time to update
		if( Timer % ( int )( GAME_FPS * SNAKE_DELAY ) == 0 && !Paused ){

			// Reset the timer to be used again
			Timer = 0;

			// Give chance for the player to change direction
			DirectionChanged = false;

			// Update according to the direction
			switch( SnakeDirection ){

				// Note that the y is down in raylib
				// so you have to decrease y axis to
				// do up

				case UP:
					Snake.insert(
						Snake.begin(),
						Vector2{ Snake.at( 0 ).x, Snake.at( 0 ).y - 1.0f }
					);
					Snake.erase( Snake.end() - 1 );
				break;

				case DOWN:
					Snake.insert(
						Snake.begin(),
						Vector2{ Snake.at( 0 ).x, Snake.at( 0 ).y + 1.0f }
					);
					Snake.erase( Snake.end() - 1 );
				break;

				case LEFT:
					Snake.insert(
						Snake.begin(),
						Vector2{ Snake.at( 0 ).x - 1.0f, Snake.at( 0 ).y }
					);
					Snake.erase( Snake.end() - 1 );
				break;

				case RIGHT:
					Snake.insert(
						Snake.begin(),
						Vector2{ Snake.at( 0 ).x + 1.0f, Snake.at( 0 ).y }
					);
					Snake.erase( Snake.end() - 1 );
				break;

			}

			// Check if the snake head is in the snake itself
			for( unsigned int Index = 1; Index < Snake.size(); Index++ ){

				// If the snake head is inside the snake body
				// The player has loss
				if(
					( int ) Snake.at( 0 ).x == ( int ) Snake.at( Index ).x &&
					( int ) Snake.at( 0 ).y == ( int ) Snake.at( Index ).y
				) Loss = true;

			}

			// If player goes beyond boundery
			// It will come from the other side
			if( ( int ) Snake.at( 0 ).x > BOUNDERY - 1 ) Snake.at( 0 ).x = -BOUNDERY;
			if( ( int ) Snake.at( 0 ).x < -BOUNDERY ) Snake.at( 0 ).x = BOUNDERY - 1;
			if( ( int ) Snake.at( 0 ).y > BOUNDERY - 1 ) Snake.at( 0 ).y = -BOUNDERY;
			if( ( int ) Snake.at( 0 ).y < -BOUNDERY ) Snake.at( 0 ).y = BOUNDERY - 1;

			// If the player size is bigger than the area
			// The player has won
			if( Snake.size() >= ( BOUNDERY + BOUNDERY ) * ( BOUNDERY + BOUNDERY ) )
				Won = true;

		}

		// Begin drawing to the window
		BeginDrawing();

		{

			// Make the background color to be white ( Or choose black if you want )
			ClearBackground( WHITE );

			// Begin drawing in the camera
			BeginMode2D( PlayerCamera );

			{

				// Loop through all the tail objects to draw
				// them on the screen excluding the head
				for( unsigned int Index = 1; Index < Snake.size(); Index++ ){

					// Initialise the temporary rectangle to be used in drawing
					BoxBounds = {
						Snake.at( Index ).x,
						Snake.at( Index ).y,
						1.0f,
						1.0f
					};

					// Initialise the temporary color to be used in drawing
					if( Index % 2 == 0 ) SnakeBodyColor = { 0, 128, 0, 255 };
					if( Index % 2 == 1 ) SnakeBodyColor = { 0, 96, 0, 255 };

					// Draw the initialised rectangle on the camera
					DrawRectangleRec( BoxBounds, SnakeBodyColor );

				}

				// Draw a special color for the head of the snake
				BoxBounds = { Snake.at( 0 ).x, Snake.at( 0 ).y, 1.0f, 1.0f };
				DrawRectangleRec( BoxBounds, SnakeHeadColor );

				// Now drawing apple on the camera
				BoxBounds = { Apple.x, Apple.y, 1.0f, 1.0f };
				DrawRectangleRec( BoxBounds, AppleColor );

				// Draw the grid
				for( int Index = -BOUNDERY; Index < BOUNDERY + 1; Index++ ){
					DrawLine( Index, -BOUNDERY, Index, BOUNDERY, BLACK );
					DrawLine( -BOUNDERY, Index, BOUNDERY, Index, BLACK );
				}

			}

			// End drawing on the camera, Anything after
			// this will draw directly on the screen
			EndMode2D();
		}

		// If the game has ended, Tell the player
		// that the game has

		// If loss
		if( Loss ){

			// Draw a white background
			DrawRectangle(
				0,
				0,
				GetScreenWidth(),
				GetScreenHeight(),
				ColorAlpha( RED, 0.2f )
			);

			// And then the text
			DrawTextEx(
				Consola,
				"           You loss!\nPress enter or space to continue",
				Vector2{
					GetScreenWidth() / 2.0f - 160.0f,
					GetScreenHeight() / 2.0f - 25.0f
				},
				20,
				0,
				BLACK
			);

		}

		// If won
		if( Won ){

			// Draw a white background
			DrawRectangle(
				0,
				0,
				GetScreenWidth(),
				GetScreenHeight(),
				ColorAlpha( BLUE, 0.2f )
			);

			// And then the text
			DrawTextEx(
				Consola,
				"           You Won!\nPress enter or space to continue",
				Vector2{
					GetScreenWidth() / 2.0f - 160.0f,
					GetScreenHeight() / 2.0f - 25.0f
				},
				20,
				0,
				BLACK
			);

		}

		// If paused
		if( !Won && !Loss && Paused ){

			// Draw a white background
			DrawRectangle(
				0,
				0,
				GetScreenWidth(),
				GetScreenHeight(),
				ColorAlpha( WHITE, 0.8f )
			);

			// And then the text
			DrawTextEx(
				Consola,
				"           Game Paused\n Press enter or space to continue\n   Move with arrow or wasd keys\n Pause/resume with space or enter\nMove grid with middle mouse button\n      Zoom with scroll wheel",
				Vector2{
					GetScreenWidth() / 2.0f - 172.0f,
					GetScreenHeight() / 2.0f - 75.0f
				},
				20,
				0,
				BLACK
			);

		}

		// Draw a score meter
		DrawTextEx(
			Consola,
			TextFormat( "Score: %i\nTop Score: %i", Snake.size() - 1, TopScore ),
			Vector2{
				GetScreenWidth() / 2.0f - 110.0f,
				25.0f
			},
			20,
			0,
			BLACK
		);

		// End the drawing
		EndDrawing();
	}

	// We won't be doing anything so we restart the loop

	// Close window once the user presses exit button or escape button
	CloseWindow();

	// Output the scores to the scores.txt file
	std::ofstream OutFile( "Files/Scores.txt" );

	// Output all the scores
	for( unsigned int Index = 0; Index < Scores.size(); Index++ )

		// One by one and new line for each score
		OutFile << Scores.at( Index ) << std::endl;

	// End of program
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MIT License                                                                     *
 *                                                                                 *
 * Copyright (c) 2022 GamerZ-0216                                                  *
 *                                                                                 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy    *
 * of this software and associated documentation files (the "Software"), to deal   *
 * in the Software without restriction, including without limitation the rights    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is           *
 * furnished to do so, subject to the following conditions:                        *
 *                                                                                 *
 * The above copyright notice and this permission notice shall be included in all  *
 * copies or substantial portions of the Software.                                 *
 *                                                                                 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 * SOFTWARE.                                                                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * BPLE Header file for World Control and management                               *
 * Program made by GamerZ-0216 ( @GamerZ-0216#7661 in Discord )                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Here is the official Discord server for BPLE                                    *
 * https://discord.gg/RD8vPwahPd                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Be sure to support me by subscribing to my YouTube channel                      *
 * https://www.youtube.com/channel/UCph0ej9zcBMCfosQLE1E7Jg                        *
 * Also be sure to join my discord server                                          *
 * https://discord.gg/xhXcpghqNb                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */





 // Include this header once
#pragma once

// Header Guard Detection
#if !defined( GAMERZ_BPLE_H )

// Header Guard Setup
#define GAMERZ_BPLE_H

// Uses strings instead of char array
#include <string>

// Uses vector to store part data
#include <vector>

// Used string-stream to convert data
#include <sstream>

// Used file-stream to load / save files
#include <fstream>

// Namespace specifies, Use "GZBPLE::" to access the features
namespace GZBPLE
{




	// Check wether an object is a frame or not
	extern std::vector< std::vector< bool > > PartObjectFrame;

	// Check wether an object has grips when placed next to a frame
	extern std::vector< std::vector< bool > > PartObjectGripped;

	// The type of property used to determine "Prop"
	// Wether or not use rotation or color as the basis
	// 0. None
	// 1. Rotation in 8 different angles
	// 2. Rotation in 4 different angles
	// 3. Rotation in 2 different angles
	// 4. Four colors
	extern std::vector< std::vector< int > > PartObjectPropType;



	// Size in the X axis of the part
	extern std::vector< std::vector< int > > PartObjectSizeX;

	// Size in the Y axis of the part
	extern std::vector< std::vector< int > > PartObjectSizeY;

	// Center in the X axis of the part
	extern std::vector< std::vector< int > > PartObjectCenterX;

	// Center in the Y axis of the part
	extern std::vector< std::vector< int > > PartObjectCenterY;



	// Get count of skins per part
	extern std::vector< int > SkinsPerPart;



	// Convert String to Integer
	int ConvertToInteger( std::string Input );





	// Part Container
	class PartObject
	{


		// Set constructor accessable
	public:

		// Data stored in the game savefile
		int Type, // Type of the part. eg Pig type index is 1
			Skin, // Skin of the part. eg Glue egg skin index is 5
			PosX, // Position of the part in X-Axis
			PosY, // Position of the part in Y-Axis
			Prop, // Potation of the part or sometimes color
			Flip; // Wether or not the part is flipped or not

		// The type of property used to determine "Prop"
		// Wether or not use rotation or color as the basis
		// As stated earlier

		int PropType;

		// Custom data
		bool Frame, // Wether the part is frame
			Gripped, // Wether the part has grips when placed next to a frame
			Selected; // Wether the part is selected

		// Size and Center if an object isn't a square
		int SizeX,
			SizeY,
			CenterX,
			CenterY;

		// Default constructor, The rest of the data is automatically set
		PartObject( int PartType, int PartSkin, int PartPosX, int PartPosY, int PartProp, int PartFlip );



	};





	// If the Part-Prop is rotation
	enum Prop_Rotation
	{
		Up, // 0 Degree
		Left, // 270 Degree
		Down, // 180 Degree
		Right, // 90 Degrees
		UpLeft, // 315 Degrees
		DownLeft, // 255 Degrees
		DownRight, // 135 Degrees
		UpRight // 45 Degrees
	};

	// If the Part-Prop is color
	enum Prop_Color
	{
		Green,
		Yellow,
		Red,
		Blue
	};





	// Level Data container
	class World
	{


		// Set functions accessable
	public:

		// Contains part data
		std::vector< PartObject > Part;



		// Load file
		void Load_Level( std::string File );

		// Save file
		void Save_Level( std::string File );





		// The index of Method variable:
		// 0. Delete frame with non-frame
		// 1. Delete only frame
		// 2. Delete only non-frame
		// 3. Delete frame or non-frame if no frame available
		// 4. Delete non-frame or frame if no non-frame available



		// The index of Mode variable:
		// Control:
		//     0. Inverse selection
		//     1. Always select
		//     2. Always de-select
		// Operation:
		//     0. All selected and de-selected
		//     1. Only selected
		//     2. Only de-selected



		// Select part based on index ( Modes ( Control ) : 0, 1, 2 )
		void Select_Part( int Index, int Mode = 0 );

		// Select part based on start and end position ( Both included ) ( Modes ( Control ) : 0, 1, 2 )
		void Select_Rectangle( int StartX, int StartY, int EndX, int EndY, int Method = 0, int Mode = 0 );



		// Move parts from the offset ( Modes ( Operation ) : 0, 1, 2 )
		void Move_Part( int OffsetX, int OffsetY, int Mode = 0 );



		// Insert Part according to the selection of combination
		void Insert_Part( int PartType, int PartSkin, int PartPosX, int PartPosY, int PartProp, int PartFlip );



		// Delete part based on index
		void Delete_Index( int Index );

		// Delete all the parts with specified Part-Type ( Methods: 0, 1, 2 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Type( int PartType, int Method = 0, int Mode = 0 );

		// Delete all the parts with specified Part-Type with Part-Skin ( Methods: 0, 1, 2 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Type_Skin( int PartType, int PartSkin, int Method = 0, int Mode = 0 );

		// Delete all the parts with specified Part-Type with Part-Skin with Part-Prop ( Methods: 0, 1, 2 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Type_Skin_Prop( int PartType, int PartSkin, int PartProp, int Method = 0, int Mode = 0 );

		// Delete all the parts with specified Part-Type with Part-Skin with Part-Flip ( Methods: 0, 1, 2 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Type_Skin_Flip( int PartType, int PartSkin, int PartFlip, int Method = 0, int Mode = 0 );

		// Delete all the parts with specified Part-Type with Part-Skin with Part-Prop with Part-Flip ( Methods: 0, 1, 2 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Type_Skin_Prop_Flip( int PartType, int PartSkin, int PartProp, int PartFlip, int Method = 0, int Mode = 0 );

		// Delete all the parts with specified Position ( Methods: 0, 1, 2, 3, 4 ), ( Modes ( Operation ) : 0, 1, 2 )
		void Delete_Pos( int PartPosX, int PartPosY, int Method = 0, int Mode = 0 );



	};



}



// Header Guard End
#endif
////////////////////////////////////////////////////////////////////////////////
//                        Copyright (c) Anstro Pleuton                        //
////////////////////////////////////////////////////////////////////////////////
//   Permission is hereby granted, free of charge, to any person obtaining a  //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,  //
//    and/or sell copies of the Software, and to permit persons to whom the   //
//    Software is furnished to do so, subject to the following conditions:    //
//                                                                            //
//   The above copyright notice and this permission notice shall be included  //
//           in all copies or substantial portions of the Software.           //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  //
//   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL  //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
//   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  //
//     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER    //
//                          DEALINGS IN THE SOFTWARE.                         //
////////////////////////////////////////////////////////////////////////////////
//                           版权 (C) Anstro Pleuton                           //
////////////////////////////////////////////////////////////////////////////////
//       特此向获得的任何人免费给予许可。该软件和相关文件文件(“软件”)的副本，       //
//       不受限制地处理软件，包括不受限制使用、复制、修改、合并、发布、分发、       //
//   转接许可的权利，和或出售软件的副本，并允许其软件提供这样做，但须符合下列条件：  //
//                                                                            //
//             上述版权公告和本许可通知应包括在软件的所有副本或大部分。            //
//                                                                            //
//             该软件是按原样提供的，没有任何形式的保证，快递或隐含的，            //
//               包括但不限于适销性的保证，适合某一特定目的和不侵犯。              //
//       在任何情况下都不能作者或版权持有人对任何索赔、损害赔偿或其他法律责任，      //
//                     不论是在合同诉讼、侵权行为或其他情况下，                   //
//                    产生与软件或使用或其他有关的或者软件交易。                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef ECORE_HPP
// ecore.hpp
#define ECORE_HPP

#include <vector>

// A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
#include "raylib.h"

// Required by the 'partSkinCount' vector
#include "bplemgr.hpp"

namespace editor
{
	// Convert it to int(Rotation::<some>)
	enum class Rotation
	{
		Up = 0, // Yes, 0 degree is up and don't ask me why.
		Left = 270,
		Down = 180,
		Right = 90,
		UpLeft = 315,
		DownLeft = 225,
		DownRight = 135,
		UpRight = 45,
		MAX
	};

	extern bool loadedTextures;
	extern std::string texturePath;
	extern Font defaultFont;

	Rotation getPropRotation(int a_lvl_prop);

	extern std::vector<std::vector<Texture>> partTexture;
	extern std::vector<Texture> partPropTexture;

	void loadPartTexture(std::string a_texturePathOrigin = "./res/textures/");
	void unloadPartTexture();

	extern std::vector<std::vector<float>> partSizeX;
	extern std::vector<std::vector<float>> partSizeY;
	extern std::vector<std::vector<float>> partCenterX;
	extern std::vector<std::vector<float>> partCenterY;

	void genEmptyList(std::vector<std::vector<float>> &list);
	void loadPartDimensions();
	void unloadPartDimensions();

	Texture getPartTexture(int a_type, int a_skin, int a_prop);
	Color getFrameTint(int a_skin);
	void drawPart(bple::Part a_part, Color a_selectionColor = GREEN, Color a_transparentColor = ColorAlpha(WHITE, 0.5f));
	void drawPartAll(std::vector<bple::Part> &a_parts);
	void drawPartAll(bple::Level &a_level);

	void insertPart(bple::Level &a_level, bple::Part a_part, Vector2 a_position, bool a_validate = false);
	void deletePart(bple::Level &a_level, Vector2 a_position);
}
// Header Guard End (ECORE_HPP)
#endif

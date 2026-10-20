////////////////////////////////////////////////////////////////////////////////
//                        Copyright (c) Anstro Plueton                        //
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

#ifndef UCORE_HPP
// ucore.hpp
#define UCORE_HPP

#include <cmath>

// A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
#include "raylib.h"

// A module of raylib to support low-level drawing
#include "rlgl.h"

namespace utils
{
	class PlayerCamera2D
	{
		float time;
		float timer;
		float scroll;
		float direction;

	public:
		Camera2D mainCamera;

		float zoomAmount;
		float zoomTime;
		float minimumZoom;
		float maximumZoom;

		PlayerCamera2D();
		void update(bool a_drag);
	};

	void DrawGrid2D(Vector2 a_position, int a_slicex, int a_skicey, float a_spacing, Color a_color);

	/// @brief Only recommended for 60 FPS,
	/// @param a_key key enum from raylib
	/// @return returns true when key was pressed and continuesly when the key is held for long enough time
	bool isKeyLongPressed(int a_key);
	char getCharLongPressed();

	/// @brief Get the normalized roundness value for rounded rectangle
	float getRecRoundness(Rectangle a_input, float a_roundness);
}

// Header Guard End (UCORE_HPP)
#endif

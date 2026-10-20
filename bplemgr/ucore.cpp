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

#include <ucore.hpp>

utils::PlayerCamera2D::PlayerCamera2D()
{
	// Defaults

	mainCamera = {0};
	mainCamera.zoom = 5.0f;

	zoomAmount = 1.2f;
	zoomTime = 0.375f;
	minimumZoom = 0.001f;
	maximumZoom = 7.5f;
}

void utils::PlayerCamera2D::update(bool a_drag)
{
	mainCamera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

	if (zoomTime > 1.0f)
		zoomTime = 1.0f;
	if (zoomTime < 0.0f)
		zoomTime = 0.0f;

	if (GetMouseWheelMove() != 0)
	{
		time = GetTime();
		direction = GetMouseWheelMove();
	}

	timer = (GetTime() - time > zoomTime) ? zoomTime : GetTime() - time;
	scroll = direction * (zoomTime - timer);

	if (a_drag)
	{
		mainCamera.target.x -= GetMouseDelta().x / mainCamera.zoom;
		mainCamera.target.y -= GetMouseDelta().y / mainCamera.zoom;
	}

	// For some reason I have to do it this way

	if (scroll > 0)
		mainCamera.zoom *= std::pow(zoomAmount, scroll);

	mainCamera.target.y += (GetMousePosition().y - mainCamera.offset.y) * scroll / (mainCamera.zoom) * zoomAmount;
	mainCamera.target.x += (GetMousePosition().x - mainCamera.offset.x) * scroll / (mainCamera.zoom) * zoomAmount;
	mainCamera.target.y -= (GetMousePosition().y - mainCamera.offset.y) * scroll / (mainCamera.zoom);
	mainCamera.target.x -= (GetMousePosition().x - mainCamera.offset.x) * scroll / (mainCamera.zoom);

	if (scroll < 0)
		mainCamera.zoom *= std::pow(zoomAmount, scroll);

	if (mainCamera.zoom > maximumZoom)
		mainCamera.zoom = maximumZoom;
	if (mainCamera.zoom < minimumZoom)
		mainCamera.zoom = minimumZoom;

	if (IsKeyPressed(KEY_R))
	{
		mainCamera = {0};
		mainCamera.zoom = 1.0f;
		mainCamera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
	}
}

void utils::DrawGrid2D(Vector2 a_position, int a_slicex, int a_slicey, float a_spacing, Color a_color)
{
	rlColor4ub(a_color.r, a_color.g, a_color.b, a_color.a);
	rlBegin(RL_LINES);

	for (int i = 0; i <= a_slicex; i++)
	{
		rlVertex2f(a_position.x + (float)i * a_spacing, a_position.y);
		rlVertex2f(a_position.x + (float)i * a_spacing, a_position.y + (float)a_slicey * a_spacing);
	}

	for (int i = 0; i <= a_slicey; i++)
	{
		rlVertex2f(a_position.x, a_position.y + (float)i * a_spacing);
		rlVertex2f(a_position.x + (float)a_slicex * a_spacing, a_position.y + (float)i * a_spacing);
	}

	rlEnd();
}

bool utils::isKeyLongPressed(int a_key)
{
	static double delay = 0.5;
	static double timer = delay;
	if (IsKeyPressed(a_key))
	{
		timer = delay - GetFrameTime();
		return true;
	}
	if (IsKeyDown(a_key))
	{
		if (timer > 0.0)
			timer -= GetFrameTime();
		if (timer <= 0.0)
			return true;
	}
	return false;
}

char utils::getCharLongPressed()
{
	// Don't mind me XDDDDD

	if (isKeyLongPressed(KEY_BACKSPACE))
		return 8;
	if (isKeyLongPressed(KEY_SPACE))
		return ' ';
	if (isKeyLongPressed(KEY_ENTER))
		return 10;

	if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT))
	{
		if (isKeyLongPressed(KEY_ONE))
			return '1';
		if (isKeyLongPressed(KEY_TWO))
			return '2';
		if (isKeyLongPressed(KEY_THREE))
			return '3';
		if (isKeyLongPressed(KEY_FOUR))
			return '4';
		if (isKeyLongPressed(KEY_FIVE))
			return '5';
		if (isKeyLongPressed(KEY_SIX))
			return '6';
		if (isKeyLongPressed(KEY_SEVEN))
			return '7';
		if (isKeyLongPressed(KEY_EIGHT))
			return '8';
		if (isKeyLongPressed(KEY_NINE))
			return '9';
		if (isKeyLongPressed(KEY_ZERO))
			return '0';
		if (isKeyLongPressed(KEY_MINUS))
			return '-';
		if (isKeyLongPressed(KEY_EQUAL))
			return '=';
		if (isKeyLongPressed(KEY_LEFT_BRACKET))
			return '[';
		if (isKeyLongPressed(KEY_RIGHT_BRACKET))
			return ']';
		if (isKeyLongPressed(KEY_BACKSLASH))
			return '\\';
		if (isKeyLongPressed(KEY_SEMICOLON))
			return ';';
		if (isKeyLongPressed(KEY_APOSTROPHE))
			return '\'';
		if (isKeyLongPressed(KEY_COMMA))
			return ',';
		if (isKeyLongPressed(KEY_PERIOD))
			return '.';
		if (isKeyLongPressed(KEY_SLASH))
			return '/';
		if (isKeyLongPressed(KEY_GRAVE))
			return '`';
		if (isKeyLongPressed(KEY_A))
			return 'a';
		if (isKeyLongPressed(KEY_B))
			return 'b';
		if (isKeyLongPressed(KEY_C))
			return 'c';
		if (isKeyLongPressed(KEY_D))
			return 'd';
		if (isKeyLongPressed(KEY_E))
			return 'e';
		if (isKeyLongPressed(KEY_F))
			return 'f';
		if (isKeyLongPressed(KEY_G))
			return 'g';
		if (isKeyLongPressed(KEY_H))
			return 'h';
		if (isKeyLongPressed(KEY_I))
			return 'i';
		if (isKeyLongPressed(KEY_J))
			return 'j';
		if (isKeyLongPressed(KEY_K))
			return 'k';
		if (isKeyLongPressed(KEY_L))
			return 'l';
		if (isKeyLongPressed(KEY_M))
			return 'm';
		if (isKeyLongPressed(KEY_N))
			return 'n';
		if (isKeyLongPressed(KEY_O))
			return 'o';
		if (isKeyLongPressed(KEY_P))
			return 'p';
		if (isKeyLongPressed(KEY_Q))
			return 'q';
		if (isKeyLongPressed(KEY_R))
			return 'r';
		if (isKeyLongPressed(KEY_S))
			return 's';
		if (isKeyLongPressed(KEY_T))
			return 't';
		if (isKeyLongPressed(KEY_U))
			return 'u';
		if (isKeyLongPressed(KEY_V))
			return 'v';
		if (isKeyLongPressed(KEY_W))
			return 'w';
		if (isKeyLongPressed(KEY_X))
			return 'x';
		if (isKeyLongPressed(KEY_Y))
			return 'y';
		if (isKeyLongPressed(KEY_Z))
			return 'z';
	}
	else
	{
		if (isKeyLongPressed(KEY_ONE))
			return '!';
		if (isKeyLongPressed(KEY_TWO))
			return '@';
		if (isKeyLongPressed(KEY_THREE))
			return '#';
		if (isKeyLongPressed(KEY_FOUR))
			return '$';
		if (isKeyLongPressed(KEY_FIVE))
			return '%';
		if (isKeyLongPressed(KEY_SIX))
			return '^';
		if (isKeyLongPressed(KEY_SEVEN))
			return '&';
		if (isKeyLongPressed(KEY_EIGHT))
			return '*';
		if (isKeyLongPressed(KEY_NINE))
			return '(';
		if (isKeyLongPressed(KEY_ZERO))
			return ')';
		if (isKeyLongPressed(KEY_MINUS))
			return '_';
		if (isKeyLongPressed(KEY_EQUAL))
			return '+';
		if (isKeyLongPressed(KEY_LEFT_BRACKET))
			return '{';
		if (isKeyLongPressed(KEY_RIGHT_BRACKET))
			return '}';
		if (isKeyLongPressed(KEY_BACKSLASH))
			return '|';
		if (isKeyLongPressed(KEY_SEMICOLON))
			return ':';
		if (isKeyLongPressed(KEY_APOSTROPHE))
			return '\"';
		if (isKeyLongPressed(KEY_COMMA))
			return '<';
		if (isKeyLongPressed(KEY_PERIOD))
			return '>';
		if (isKeyLongPressed(KEY_SLASH))
			return '?';
		if (isKeyLongPressed(KEY_GRAVE))
			return '~';
		if (isKeyLongPressed(KEY_A))
			return 'A';
		if (isKeyLongPressed(KEY_B))
			return 'B';
		if (isKeyLongPressed(KEY_C))
			return 'C';
		if (isKeyLongPressed(KEY_D))
			return 'D';
		if (isKeyLongPressed(KEY_E))
			return 'E';
		if (isKeyLongPressed(KEY_F))
			return 'F';
		if (isKeyLongPressed(KEY_G))
			return 'G';
		if (isKeyLongPressed(KEY_H))
			return 'H';
		if (isKeyLongPressed(KEY_I))
			return 'I';
		if (isKeyLongPressed(KEY_J))
			return 'J';
		if (isKeyLongPressed(KEY_K))
			return 'K';
		if (isKeyLongPressed(KEY_L))
			return 'L';
		if (isKeyLongPressed(KEY_M))
			return 'M';
		if (isKeyLongPressed(KEY_N))
			return 'N';
		if (isKeyLongPressed(KEY_O))
			return 'O';
		if (isKeyLongPressed(KEY_P))
			return 'P';
		if (isKeyLongPressed(KEY_Q))
			return 'Q';
		if (isKeyLongPressed(KEY_R))
			return 'R';
		if (isKeyLongPressed(KEY_S))
			return 'S';
		if (isKeyLongPressed(KEY_T))
			return 'T';
		if (isKeyLongPressed(KEY_U))
			return 'U';
		if (isKeyLongPressed(KEY_V))
			return 'V';
		if (isKeyLongPressed(KEY_W))
			return 'W';
		if (isKeyLongPressed(KEY_X))
			return 'X';
		if (isKeyLongPressed(KEY_Y))
			return 'Y';
		if (isKeyLongPressed(KEY_Z))
			return 'Z';
	}
	return 0;
}

float utils::getRecRoundness(Rectangle a_rectangle, float a_roundness)
{
	return 2.0f * a_roundness / std::min(a_rectangle.width, a_rectangle.height);
}

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

#include "bplemgr.hpp"

#include "ecore.hpp"
#include "gcore.hpp"
#include "pcore.hpp"
#include "score.hpp"
#include "ucore.hpp"

#include "rde.hpp"

#include <thread>

int mainProgram()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1200, 600, "BPLE++");
	SetWindowMinSize(120, 60);
	SetTargetFPS(60);
	SetExitKey(0);

	utils::PlayerCamera2D camera;
	Vector2 screenToWorld2D = {0};

	enum class Scenes
	{
		Loading,
		Home,
		Story,
		Level,
		Edit,
		Play,
		Settings,
		Credits,
		Unloading
	} scene;

	scene = Scenes::Loading;
	int loadingIndex = 0;
	bool lazyLoadingCheck = false;
	bool once = true;
	char *textFormat;
	Vector2 measureText;
	float fontSize = 40.0f;

	editor::loadPartDimensions();
	editor::loadPartTexture();

	// std::thread loadingThread(editor::loadPartTexture, "./res/textures/");

	while (!WindowShouldClose())
	{
		camera.update((IsKeyDown(KEY_SPACE) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE));
		screenToWorld2D = GetScreenToWorld2D(GetMousePosition(), camera.mainCamera);

		switch (scene)
		{
		case Scenes::Loading:
			if (editor::loadedTextures)
			{
				scene = Scenes::Home;
				// loadingThread.join();
			}
			break;

		case Scenes::Home:
			break;

		case Scenes::Story:
			break;

		case Scenes::Level:
			break;

		case Scenes::Edit:
			break;

		case Scenes::Play:
			break;

		case Scenes::Settings:
			break;

		case Scenes::Credits:
			break;

		default:
			break;
		}

		BeginDrawing();
		{
			ClearBackground(BLACK);
			switch (scene)
			{
			case Scenes::Loading:
				textFormat = (char *)TextFormat("Loading texture: %s", editor::texturePath.c_str());
				measureText = MeasureTextEx(editor::defaultFont, textFormat, fontSize, 1);
				DrawTextEx(editor::defaultFont, textFormat, {(GetScreenWidth() - measureText.x) / 2.0f, (GetScreenHeight() - measureText.y) / 2.0f}, fontSize, 1, WHITE);
				break;

			case Scenes::Home:

				break;

			case Scenes::Story:
				break;

			case Scenes::Level:
				break;

			case Scenes::Edit:
				BeginMode2D(camera.mainCamera);
				{
				}
				EndMode2D();
				break;

			case Scenes::Play:
				break;

			case Scenes::Settings:
				break;

			case Scenes::Credits:
				break;

			default:
				break;
			}
		}
		EndDrawing();
	}

	editor::unloadPartTexture();
	editor::unloadPartDimensions();

	CloseWindow();
	return 0;
}

int generateCanvase()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1200, 600, "Processing...");
	SetTargetFPS(30);

	editor::loadPartTexture();
	editor::loadPartDimensions();

	char *text = "Done!";
	float fontSize = 80.0f;
	Font font = LoadFontEx("./res/font/SegoeUI.ttf", fontSize, nullptr, 0);
	Vector2 measureText = MeasureTextEx(font, text, fontSize, 1);
	utils::PlayerCamera2D playerCamera2D;

	RenderTexture renderTexture = LoadRenderTexture(14000, 7000);

	std::vector<std::vector<Rectangle>> something;

	BeginTextureMode(renderTexture);
	{
		float partCounterPre = 0;
		std::vector<Rectangle> somethingTwo;
		for (int alpha = 0; alpha < editor::partTexture.size(); alpha++)
		{
			float partCounter = 0;
			float skinCounter = 0;
			for (int beta = 0; beta < editor::partTexture[alpha].size(); beta++)
			{
				DrawTexture(editor::partTexture[alpha][beta], skinCounter, partCounterPre, WHITE);
				somethingTwo.push_back({skinCounter, partCounterPre, (editor::partSizeX[alpha][beta] == 0.0f) ? 100.0f : editor::partSizeX[alpha][beta] * 100.0f, (editor::partSizeY[alpha][beta] == 0.0f) ? 100.0f : editor::partSizeY[alpha][beta] * 100.0f});

				if (editor::partSizeX[alpha][beta] == 0.0f)
					skinCounter += 100.0f;
				else
					skinCounter += editor::partSizeX[alpha][beta] * 100.0f;

				if (editor::partSizeX[alpha][beta] == 0.0f)
				{
					if (partCounter < 100.0f)
						partCounter = 100.0f;
				}
				else if (partCounter < editor::partSizeY[alpha][beta] * 100.0f)
					partCounter = editor::partSizeY[alpha][beta] * 100.0f;
			}
			partCounterPre += partCounter;
			something.push_back(somethingTwo);
		}
	}
	EndTextureMode();

	for (int alpha = 0; alpha < something.size(); alpha++)
	{
		for (int beta = 0; beta < something[alpha].size(); beta++)
			std::printf("%i.0f, %i.0f, %i.0f, %i.0f\n", (int)std::roundf(something[alpha][beta].x), (int)std::roundf(something[alpha][beta].y), (int)std::roundf(something[alpha][beta].width), (int)std::roundf(something[alpha][beta].height));
		std::printf("\n");
	}

	while (!WindowShouldClose())
	{
		playerCamera2D.update((IsKeyDown(KEY_SPACE) && (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE));
		BeginDrawing();
		{
			ClearBackground(BLACK);
			BeginMode2D(playerCamera2D.mainCamera);
			{
				DrawTexturePro(renderTexture.texture, (Rectangle){0.0f, 0.0f, renderTexture.texture.width, -renderTexture.texture.height}, (Rectangle){0.0f, 0.0f, GetScreenWidth(), GetScreenHeight()}, {0.0f, 0.0f}, 0.0f, WHITE);
			}
			EndMode2D();
			DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
			DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, WHITE);
		}
		EndDrawing();
	}

	UnloadFont(font);
	editor::unloadPartDimensions();
	editor::unloadPartTexture();

	CloseWindow();
	return 0;
}

int rdeTexting()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	InitWindow(1200, 600, "RDE Testing");
	SetTargetFPS(60);

	rde::Instance instance;
	instance.load();
	instance.windows.push_back(new rde::Instance::Window(instance, {950, 450, 200, 100}));
	Texture texture = LoadTexture("./background.png");

	while (!WindowShouldClose())
	{
		instance.update();
		if (IsKeyDown(KEY_TAB))
		{
			instance.beginBackground();
			{
				DrawTexturePro(texture, (Rectangle){0, 0, texture.width, texture.height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, {0}, 0.0f, WHITE);
				// DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
			}
			instance.endBackground();
		}

		BeginDrawing();
		{
			ClearBackground(WHITE);
			instance.draw(IsKeyDown(KEY_SPACE), IsKeyDown(KEY_ENTER));
		}
		EndDrawing();
	}

	instance.unload();
	CloseWindow();
	return 0;
}

int main()
{
	return mainProgram();
	// return generateCanvase();
	// return rdeTexting();
}

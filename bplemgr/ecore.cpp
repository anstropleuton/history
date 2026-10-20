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

#include "ecore.hpp"

std::vector<std::vector<Texture>> editor::partTexture;
std::vector<Texture> editor::partPropTexture;
std::vector<std::vector<float>> editor::partSizeX;
std::vector<std::vector<float>> editor::partSizeY;
std::vector<std::vector<float>> editor::partCenterX;
std::vector<std::vector<float>> editor::partCenterY;
bool editor::loadedTextures;
std::string editor::texturePath;
Font editor::defaultFont;

void editor::loadPartTexture(std::string a_texturePathOrigin)
{
	defaultFont = LoadFontEx("./res/font/SegoeUI.ttf", 100, nullptr, 0);
	// a_skin textures temp storage
	std::vector<Texture> nestedPartTexture;

	for (int part = 0; part < bple::partSkinCount.size(); part++)
	{
		for (int skin = 0; skin < bple::partSkinCount[part]; skin++)
		{
			texturePath = a_texturePathOrigin;

			// Exception for textures (either it's tinted iron frame, or has no texture, or has property-based textures)
			if ((part == 6 && skin > 11 && skin < 130) ||
				(part == 39 && skin == 7) ||
				(part == 41) ||
				(part == 44 && skin == 6) ||
				(part == 46 && skin == 3) ||
				(part == 47 && (skin < 5 || skin == 42)))
				texturePath += "0-0";

			else
				texturePath += std::to_string(part) + '-' + std::to_string(skin);

			texturePath += ".png";

			nestedPartTexture.push_back(LoadTexture(texturePath.c_str()));
		}

		partTexture.push_back(nestedPartTexture);
		nestedPartTexture.clear();
	}

	// Part id of 41
	for (int alpha = 0; alpha < 5; alpha++)
	{
		for (int beta = 0; beta < 4; beta++)
		{
			texturePath = a_texturePathOrigin + "41-";
			texturePath += std::to_string(alpha) + '-' + std::to_string(beta) + ".png";
			partPropTexture.push_back(LoadTexture(texturePath.c_str()));
		}
	}

	// Part id of 44-6
	texturePath = a_texturePathOrigin + "44-6-0.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));
	texturePath = a_texturePathOrigin + "44-6-1.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	// Part id of 46-3
	texturePath = a_texturePathOrigin + "46-3-0.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	texturePath = a_texturePathOrigin + "46-3-1.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	texturePath = a_texturePathOrigin + "46-3-2.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	// Part id of 47 (some)
	for (int alpha = 0; alpha < 4; alpha++)
	{
		for (int beta = 0; beta < 4; beta++)
		{
			texturePath = a_texturePathOrigin + "47-";
			texturePath += std::to_string(alpha) + '-' + std::to_string(beta) + ".png";
			partPropTexture.push_back(LoadTexture(texturePath.c_str()));
		}
		for (int beta = 0; beta < 3; beta++)
		{
			texturePath = a_texturePathOrigin + "47-";
			texturePath += std::to_string(alpha) + '-' + std::to_string(beta) + ".png";
			partPropTexture.push_back(LoadTexture(texturePath.c_str()));
		}
		alpha++;
	}

	// Part id of 47-4
	texturePath = a_texturePathOrigin + "47-4-0.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	texturePath = a_texturePathOrigin + "47-4-1.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	// Part id of 47-42
	texturePath = a_texturePathOrigin + "47-42-0.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	texturePath = a_texturePathOrigin + "47-42-1.png";
	partPropTexture.push_back(LoadTexture(texturePath.c_str()));

	loadedTextures = true;
}
void editor::unloadPartTexture()
{
	for (int a_part = 0; a_part < partTexture.size(); a_part++)
		for (int a_skin = 0; a_skin < partTexture[a_part].size(); a_skin++)
			if (partTexture[a_part][a_skin].id != 0)
				UnloadTexture(partTexture[a_part][a_skin]);
	partTexture.clear();

	for (int a_prop = 0; a_prop < partPropTexture.size(); a_prop++)
		if (partPropTexture[a_prop].id != 0)
			UnloadTexture(partPropTexture[a_prop]);
	partPropTexture.clear();

	loadedTextures = false;
}
void editor::loadPartDimensions()
{
	// Raw data, I should really optimize this to not include soo many literals... later

	partSizeX = {
		{0.0f},
		{1.0f, 1.0f, 1.06f, 1.0f, 1.11f, 1.24f, 1.12f, 1.0f, 1.0f, 1.16f, 1.0f,
		 1.55f, 1.44f, 1.33f, 1.35f, 1.69f, 1.2f, 1.01f, 1.46f, 1.13f},
		{1.0f},
		{3.0f, 3.0f, 3.0f, 3.17f, 3.0f, 3.1f, 2.41f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.13f, 1.03f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.16f, 1.1799f,
		 1.11f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 0.9f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.2f, 1.1f, 1.1f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.24f, 1.27f, 1.09f, 1.22f, 1.24f, 1.21f, 1.05f, 1.06f},
		{1.28f, 1.28f, 1.19f, 1.22f, 1.24f, 1.21f, 1.37f, 1.27f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{0.94f, 1.03f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.04f, 1.08f},
		{2.0f, 2.0f, 2.0f, 2.0f, 2.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{2.0f, 2.0f, 2.0f, 2.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.01f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.29f, 1.39f, 1.35f, 1.35f, 1.0f, 1.0f, 0.0f, 1.12f, 1.12f, 1.12f},
		{1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{3.0f, 3.0f, 3.0f, 3.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{0.9f, 0.9f, 0.9f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}};

	partSizeY = {
		{0.0f},
		{1.0f, 1.0f, 1.13f, 1.23f, 1.05f, 1.15f, 1.0f, 1.0f, 1.0f, 1.0f, 1.07f,
		 1.31f, 1.22f, 1.0f, 1.0f, 1.26f, 1.09f, 1.17f, 1.16f, 1.19f},
		{1.0f},
		{2.31f, 2.14f, 2.41f, 2.59f, 2.51f, 3.32f, 2.68f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.09f, 1.1799f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.04f, 1.11f, 1.05f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 0.9f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.07f, 1.07f, 1.0f, 1.06f, 1.07f, 1.04f, 1.04f, 1.06f, 1.07f, 1.23f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.2f, 1.1f, 1.1f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.14f, 1.19f, 1.05f, 1.11f, 1.13f, 1.01f, 1.05f, 1.08f},
		{1.27f, 1.23f, 1.3f, 1.27f, 1.25f, 1.22f, 1.09f, 1.1f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.27f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{2.68f, 2.68f, 2.72f, 2.36f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{0.9f, 0.9f, 0.9f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}};

	partCenterX = {
		{0.0f},
		{0.5f, 0.5f, 0.53f, 0.5f, 0.555f, 0.62f, 0.56f, 0.5f, 0.5f, 0.58f, 0.5f,
		 0.775f, 0.72f, 0.665f, 0.675f, 0.845f, 0.6f, 0.505f, 0.73f, 0.565f},
		{0.5f},
		{1.5f, 1.5f, 1.5f, 1.585f, 1.5f, 1.55f, 1.205f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.565f, 0.515f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.58f, 0.5899f,
		 0.555f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.45f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.575f, 0.55f, 0.55f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.62f, 0.635f, 0.545f, 0.61f, 0.62f, 0.605f, 0.525f, 0.53f},
		{0.64f, 0.64f, 0.595f, 0.61f, 0.62f, 0.605f, 0.685f, 0.635f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.47f, 0.515f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.52f, 0.54f},
		{1.5f, 1.5f, 1.5f, 1.5f, 1.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{1.5f, 1.5f, 1.5f, 1.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.505f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.645f, 0.695f, 0.675f, 0.675f, 0.5f, 0.5f, 0.0f, 0.56f, 0.56f,
		 0.56f},
		{0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{1.5f, 1.5f, 1.5f, 1.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{1.5f, 1.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.45f, 0.45f, 0.45f, 0.45f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f}};

	partCenterY = {
		{0.0f},
		{0.5f, 0.5f, 0.565f, 0.615f, 0.525f, 0.575f, 0.5f, 0.5f, 0.5f, 0.5f, 0.535f,
		 0.655f, 0.61f, 0.5f, 0.5f, 0.63f, 0.545f, 0.585f, 0.58f, 0.595f},
		{0.5f},
		{1.7325f, 1.605f, 1.8075f, 1.9425f, 1.8825f, 2.49f, 2.01f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.545f, 0.65f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.52f, 0.61f, 0.525f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.45f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.535f, 0.535f, 0.5f, 0.53f, 0.535f, 0.52f, 0.52f, 0.53f, 0.535f, 0.615f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.75f, 0.6f, 0.6f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.57f, 0.595f, 0.525f, 0.555f, 0.565f, 0.505f, 0.525f, 0.54f},
		{0.635f, 0.615f, 0.65f, 0.635f, 0.625f, 0.61f, 0.545f, 0.55f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.635f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{2.01f, 2.01f, 2.04f, 1.77f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
		{0.45f, 0.45f, 0.45f, 0.45f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f}};
}
void editor::unloadPartDimensions()
{
	for (int index = 0; index < partSizeX.size(); index++)
		partSizeX[index].clear();
	partSizeX.clear();
	for (int index = 0; index < partSizeY.size(); index++)
		partSizeY[index].clear();
	partSizeY.clear();
	for (int index = 0; index < partCenterX.size(); index++)
		partCenterX[index].clear();
	partCenterX.clear();
	for (int index = 0; index < partCenterY.size(); index++)
		partCenterY[index].clear();
	partCenterY.clear();
}
Texture editor::getPartTexture(int a_type, int a_skin, int a_prop)
{
	// Prop-specific textures are different
	if (a_type == 41)
		return partPropTexture[a_skin * 4 + a_prop];
	if (a_type == 44 && a_skin == 6)
		return partPropTexture[20 + a_prop];
	if (a_type == 46 && a_skin == 3)
		return partPropTexture[22 + a_prop];
	if (a_type == 47 && a_skin == 0)
		return partPropTexture[25 + a_prop];
	if (a_type == 47 && a_skin == 1)
		return partPropTexture[29 + a_prop];
	if (a_type == 47 && a_skin == 2)
		return partPropTexture[32 + a_prop];
	if (a_type == 47 && a_skin == 3)
		return partPropTexture[36 + a_prop];
	if (a_type == 47 && a_skin == 4)
		return partPropTexture[39 + a_prop];
	if (a_type == 47 && a_skin == 42)
		return partPropTexture[41 + a_prop];
	return partTexture[a_type][a_skin];
}
Color editor::getFrameTint(int a_skin)
{
	// *click noise* Magic!
	if (a_skin >= 12 && a_skin <= 29)
		return ColorFromHSV((a_skin - 12) * 20.0f, 0.66666f, 0.75f);
	if (a_skin >= 30 && a_skin <= 47)
		return ColorFromHSV((a_skin - 30) * 20.0f, 0.33334f, 0.75f);
	if (a_skin >= 48 && a_skin <= 65)
		return ColorFromHSV((a_skin - 48) * 20.0f, 0.66666f, 0.5f);
	if (a_skin >= 66 && a_skin <= 83)
		return ColorFromHSV((a_skin - 66) * 20.0f, 0.33334f, 0.5f);
	if (a_skin >= 84 && a_skin <= 101)
		return ColorFromHSV((a_skin - 84) * 20.0f, 0.66666f, 0.25f);
	if (a_skin >= 102 && a_skin <= 119)
		return ColorFromHSV((a_skin - 102) * 20.0f, 0.33334f, 0.25f);
	if (a_skin >= 120 && a_skin <= 129)
		return ColorFromHSV(0.0f, 0.0f, (10 - (a_skin - 120)) / 10.0f);
	return Color{0};
}
editor::Rotation editor::getPropRotation(int a_prop)
{
	// Just return the enume index I guess, lemme know if there is a better solution
	switch (a_prop)
	{
	case 0:
		return Rotation::Up;
	case 1:
		return Rotation::Left;
	case 2:
		return Rotation::Down;
	case 3:
		return Rotation::Right;
	case 4:
		return Rotation::UpLeft;
	case 5:
		return Rotation::DownLeft;
	case 6:
		return Rotation::DownRight;
	case 7:
		return Rotation::UpRight;
	default:
		return Rotation::MAX;
	}
}
void editor::drawPart(bple::Part a_part, Color a_selectionColor, Color a_transparentColor)
{
	// This a_part, dOeSn'T eXiStS.......... Nice
	if (a_part.type == 39 && a_part.skin == 7)
		return;

	Texture currentTexture;
	Color tint;

	// Part is a colored frame
	if (a_part.type == 6 && a_part.skin > 11 && a_part.skin < 130)
		currentTexture = partTexture[6][132];
	else
		currentTexture = getPartTexture(a_part.type, a_part.skin, a_part.prop);

	float rotation = (int)getPropRotation(a_part.prop);
	// These a_part-skin cannot be rotated
	if ((a_part.type == 41) ||
		(a_part.type == 44 && a_part.skin == 6) ||
		(a_part.type == 46 && a_part.skin == 3) ||
		(a_part.type == 47 && a_part.skin == 0) ||
		(a_part.type == 47 && a_part.skin == 1) ||
		(a_part.type == 47 && a_part.skin == 2) ||
		(a_part.type == 47 && a_part.skin == 3) ||
		(a_part.type == 47 && a_part.skin == 4) ||
		(a_part.type == 47 && a_part.skin == 42))
		rotation = 0.0f;

	// The usual selection indication colorization lolation if that makes any sensation
	if (a_part.selected)
		tint = a_selectionColor;

	// Colored frame
	else if (a_part.type == 6 && a_part.skin > 11 && a_part.skin < 130)
		tint = getFrameTint(a_part.skin);

	// Temp color for spreading-color a_part
	else if (a_part.type == 6 && a_part.skin > 131)
		tint = a_transparentColor;

	else
		tint = WHITE;

	DrawTexturePro(
		currentTexture,
		{0, 0, (float)currentTexture.width, (float)currentTexture.height},
		{a_part.posx + 0.5f, -a_part.posy - 0.5f, (partSizeX[a_part.type][a_part.skin] + 0.1f) * (1.0f - a_part.flip * 2.0f), partSizeY[a_part.type][a_part.skin] + 0.1f},
		{partCenterX[a_part.type][a_part.skin] + 0.05f, partCenterY[a_part.type][a_part.skin] + 0.05f},
		rotation,
		tint);
}
void editor::drawPartAll(std::vector<bple::Part> &a_parts)
{
	// First draw all the parts that is non-frame and then draw all the parts that is a frame
	for (int index = 0; index < a_parts.size(); index++)
	{
		if (!bple::isPartFrame[a_parts[index].type][a_parts[index].skin])
			drawPart(a_parts[index]);
	}
	for (int index = 0; index < a_parts.size(); index++)
	{
		if (bple::isPartFrame[a_parts[index].type][a_parts[index].skin])
			drawPart(a_parts[index]);
	}
}

void editor::drawPartAll(bple::Level &a_level)
{
	drawPartAll(a_level.parts);
}

void editor::insertPart(bple::Level &a_level, bple::Part a_part, Vector2 a_position, bool a_validate)
{
	a_level.insert(a_part.type, a_part.skin, std::floor(a_position.x), -std::ceil(a_position.y), a_part.prop, a_part.flip, a_validate);
}

void editor::deletePart(bple::Level &a_level, Vector2 a_position)
{
	a_level.deletePos(std::floor(a_position.x), -std::ceil(a_position.y));
}

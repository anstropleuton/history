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

#ifndef BPLEMGR_HPP
// bplemgr.hpp
#define BPLEMGR_HPP

// Used to load and save file
#include <fstream>

// Used in ellipse function
#include <cmath>
#include <array>

#include <vector>
#include <string>
#include <sstream>

namespace bple
{
	extern std::string bpleManagerVersion;
	extern std::string defaultGamePath;

	class PartData
	{
	public:
		bool frame;
		float sizex;
		float sizey;
		float centerx;
		float centery;
	};

	/// @brief Used with part type and part skin
	extern std::vector<std::vector<bool>> isPartFrame;

	/// @brief Used with part type
	extern std::vector<int> partSkinCount;

	class Part
	{
	public:
		// Actual part data from the bple level file

		int type = 0;
		int skin = 0;
		int posx = 0;
		int posy = 0;
		int prop = 0;
		int flip = 0;

		bool selected = false;

		/// @brief Also validates the part
		Part(int a_type, int a_skin, int a_posx, int a_posy, int a_prop, int a_flip);
		Part() = default;

		/// @brief Ensures that the part has valid data stored
		void validate();

		// Excludes selection

		Part &operator=(const Part &a_part);
		bool operator==(const Part &a_part) const;
		bool operator!=(const Part &a_part) const;
	};

	/// @brief The main core of this library, Used to manipulate the bple game file
	class Level
	{
		enum class Selection
		{
			All,
			Selected,
			NonSelected
		};

		enum class Layer
		{
			All,
			Frame,
			NonFrame,
			PossibleFrame,
			PossibleNonFrame
		};

		// You can make non-public functions to be public if you want to use them

	public:
		/// @brief All the parts from a level will be stored here
		std::vector<Part> parts;

		Level() = default;

		/// @brief Load a level when creating an object
		/// @param a_inputFile Input file to load the level from
		/// @param a_validation Whether or not the loading would also validate the parts
		Level(const std::string a_inputFile, bool a_validation = false);

		/// @brief Loading also supports comments, comments are from '#' to the end of the line. Note/warning that the comments will not be preserved.
		/// @param a_inputFile Input file to load the level from
		/// @param a_validation Whether or not the loading would also validate the parts
		void loadFile(const std::string a_inputFile, bool a_validation = false);
		void saveFile(const std::string a_outputFile);

	private:
		int getPartPosIndex(int a_posx, int a_posy, Layer a_layer = Layer::All) const;

	public:
		Part getPartPos(int a_posx, int a_posy, Layer a_layer = Layer::All) const;
		Part &getPartPosModifiable(int a_posx, int a_posy, Layer a_layer = Layer::All);
		void movePart(int a_index, int a_offx, int a_offy, Selection a_selection = Selection::All);
		void movePartAll(int a_offx, int a_offy, Selection a_selection = Selection::All);

	private:
		std::vector<int> getRectangle(int a_startx, int a_starty, int a_endx, int a_endy, bool a_fill);
		std::vector<int> getEllipse(int a_startx, int a_starty, int a_endx, int a_endy, bool a_fill);
		std::vector<int> getPolygon(std::vector<int> &a_pointx, std::vector<int> &a_pointy);

	private:
		int getPartInsertable(int a_type, int a_skin, int a_posx, int a_posy);

	public:
		void insert(int a_type, int a_skin, int a_posx, int a_posy, int a_prop, int a_flip, bool a_override = false);
		void insert(const Part &a_part, bool a_override = false);

	private:
		void deleteIndexLayer(int a_index, Layer a_layer = Layer::All);

	public:
		void deleteIndex(int a_index);
		void deleteAll(Layer a_layer = Layer::All, Selection a_selection = Selection::All);
		void deleteCustom(bool a_checkType, bool a_checkSkin, bool a_checkProp, bool a_checkFlip, int a_type, int a_skin, int a_prop, int a_flip, Layer a_layer = Layer::All, Selection a_selection = Selection::All);
		void deletePos(int a_posx, int a_posy, Layer a_layer = Layer::All, Selection a_selection = Selection::All);

	private:
		bool getSelected(int a_index, Selection a_selection = Selection::All);

	public:
		void selectIndex(int a_index, Layer a_layer = Layer::All, Selection a_selection = Selection::All);
		void selectCustom(bool a_checkType, bool a_checkSkin, bool a_checkProp, bool a_checkFlip, int a_type, int a_skin, int a_prop, int a_flip, Layer a_layer = Layer::All, Selection a_selection = Selection::All);
		void selectPos(int a_posx, int a_posy, Layer a_layer = Layer::All, Selection a_selection = Selection::All);

		/// @brief Add parts to this level that are not the same
		Level &operator+=(const Level &a_addLevel);

		/// @brief Remove parts from this level that are the same
		Level &operator-=(const Level &a_subLevel);

		Level &operator=(const Level &a_part);
		Level &operator+(const Level &a_addLevel) const;
		Level &operator-(const Level &a_subLevel) const;

		Part &operator[](int a_index);

		/// @brief Insert a part in the level
		/// @param a_part the desired part to be inserted
		void operator()(Part &a_part);

		/// @brief Get the part count when casting to an integer
		operator int();

		/// @brief Returns true when the level is not empty
		operator bool();

		Level &operator<<(const Part &a_inject);
		Level &operator>>(Part &a_extract);
		Level &operator<<(Level &a_inject);
		Level &operator>>(Level &a_extract);

		// Getting data into or out of stream

		friend std::ostream &operator<<(std::ostream &a_outStream, const Level &a_inLevel);
		friend std::istream &operator>>(std::istream &a_inStream, Level &a_outLevel);
	};
}

// Header Guard End (BPLEMGR_HPP)
#endif

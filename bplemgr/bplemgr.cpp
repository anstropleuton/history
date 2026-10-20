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

#include "bplemgr.hpp"

std::string bple::bpleManagerVersion = "BPLE 2022.1";

// /home/me/.wine/drive_c/users/me/AppData/LocalLow/Rovio/新创Unity/contraptionB
std::string bple::defaultGamePath = (std::string("/home/") + std::getenv("USER") + ".wine/drive_c/users/" + std::getenv("USER") + "/AppData/LocalLow/Rovio/新创Unity/contraptionsB/");

/// @brief Used with partType and partSkin
std::vector<std::vector<bool>> bple::isPartFrame{
	// All just hand-writen data... jk I made a program to generate it
	{false},
	{false, false, false, false, false, false, false, false, false, false,
	 false, false, false, false, false, false, false, false, false, false},
	{false},
	{false, false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{true, true, true, true, true, true, true, true, true, true, false},
	{true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, true, true,
	 true, true, true, true, true, true, true, true, true, true, false, true,
	 true, true},
	{false, false, false, false, false,
	 false, false, false, false, false, false},
	{false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false, false, false},
	{false, false, false, false, false},
	{false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false,
	 false, false, false, false, false, false},
	{false, false},
	{false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false, false, false},
	{false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false, false, false, false, false,
	 false, false, false, false, false, false, false, false, false, false,
	 false, false, false, false, false, false, false, false, false, false,
	 false, false, false, false, false, false, false, false, false, false,
	 false, false, false, false, false, false, false, false}};

/// @brief Used with partType
std::vector<int> bple::partSkinCount{
	1, 20, 1, 7, 6, 11, 134, 11, 7, 8, 8, 8, 6, 10, 10,
	5, 5, 4, 4, 10, 7, 7, 7, 4, 4, 6, 8, 8, 8, 8, 6, 4,
	4, 5, 4, 4, 4, 5, 4, 11, 2, 5, 6, 4, 7, 4, 6, 48};

bple::Part::Part(int a_type, int a_skin, int a_posx, int a_posy, int a_prop, int a_flip)
{
	type = a_type;
	skin = a_skin;
	posx = a_posx;
	posy = a_posy;
	prop = a_prop;
	flip = a_flip;
}

/// @brief Ensures that the part has valid data stored
void bple::Part::validate()
{
	if (type < 0)
		type = partSkinCount.size() - 1;
	if (type > partSkinCount.size() - 1)
		type = 0;
	if (skin < 0)
		skin = partSkinCount[type] - 1;
	if (skin > partSkinCount[type] - 1)
		skin = 0;
	if (type == 39 && skin == 7)
	{
		type = 1;
		skin = 0;
	}
}

// Excludes selection

bple::Part &bple::Part::operator=(const Part &a_part)
{
	type = a_part.type;
	skin = a_part.skin;
	posx = a_part.posx;
	posy = a_part.posy;
	prop = a_part.prop;
	flip = a_part.skin;
	return *this;
}

bool bple::Part::operator==(const Part &a_part) const
{
	if (type == a_part.type &&
		skin == a_part.skin &&
		posx == a_part.posx &&
		posy == a_part.posy &&
		prop == a_part.prop &&
		flip == a_part.flip)
		return true;
	return false;
}

bool bple::Part::operator!=(const Part &a_part) const
{
	return !(*this == a_part);
}

/// @brief Load a level when creating an object
/// @param a_inputFile Input file to load the level from
/// @param a_validation Whether or not the loading would also validate the parts
bple::Level::Level(const std::string a_inputFile, bool a_validation)
{
	loadFile(a_inputFile.c_str(), a_validation);
}

void bple::Level::loadFile(const std::string a_inputFile, bool a_validation)
{
	std::ifstream ifile;
	std::string lineData;
	std::stringstream convert;

	ifile.open(a_inputFile);
	if (ifile.fail())
	{
		ifile.close();
		return;
	}

	while (std::getline(ifile, lineData))
	{
		// Data is stored as a part per line, like:
		// 1,0,0,0,0,0
		// This is a pig located at the origin
		// sector allows to store data in proper order in proper position

		int sector = 0;
		int type = 0, skin = 0, posx = 0, posy = 0, prop = 0, flip = 0;

		// Final value too shall be converted
		lineData += ',';

		for (int index = 0; index < lineData.length(); index++)
		{
			if (lineData[index] == '#')
				goto abortPart;

			// convert and store one by one
			if (lineData[index] == ',' && convert.str().length() > 0)
			{
				switch (sector)
				{
				case 0:
					type = std::stoi(convert.str());
					break;
				case 1:
					skin = std::stoi(convert.str());
					break;
				case 2:
					posx = std::stoi(convert.str());
					break;
				case 3:
					posy = std::stoi(convert.str());
					break;
				case 4:
					prop = std::stoi(convert.str());
					break;
				case 5:
					flip = std::stoi(convert.str());
					break;
				}

				convert.str("");
				convert.clear();
				sector++;
			}
			else
				convert << lineData[index];
		}

	abortPart:

		if (sector > 0)
			if (a_validation)
			{
				// Slow process
				insert(type, skin, posx, posy, prop, flip);
				parts[parts.size() - 1].validate();
			}
			else
				parts.push_back(Part(type, skin, posx, posy, prop, flip));

		convert.str("");
		convert.clear();
	}

	ifile.close();
}

void bple::Level::saveFile(const std::string a_outputFile)
{
	std::ofstream ofile;
	ofile.open(a_outputFile);
	if (ofile.fail())
	{
		ofile.close();
		return;
	}
	for (int index = 0; index < parts.size(); index++)
	{
		// Just dump all the data
		ofile
			<< parts[index].type << ','
			<< parts[index].skin << ','
			<< parts[index].posx << ','
			<< parts[index].posy << ','
			<< parts[index].prop << ','
			<< parts[index].flip << '\n';
	}
	ofile.close();
}

int bple::Level::getPartPosIndex(int a_posx, int a_posy, Layer a_layer) const
{
	int possibleIndex = -1;
	for (int index = 0; index < parts.size(); index++)
		if (a_posx == parts[index].posx && a_posy == parts[index].posy)
			switch (a_layer)
			{
			case Layer::All:
				return index;
				break;

			case Layer::Frame:
				if (isPartFrame[parts[index].type][parts[index].skin])
					return index;
				break;

			case Layer::NonFrame:
				if (!isPartFrame[parts[index].type][parts[index].skin])
					return index;
				break;

			case Layer::PossibleFrame:
				if (isPartFrame[parts[index].type][parts[index].skin])
					return index;
				else
					possibleIndex = index; // Return at last once all the parts are checked
				break;

			case Layer::PossibleNonFrame:
				if (!isPartFrame[parts[index].type][parts[index].skin])
					return index;
				else
					possibleIndex = index; // Return at last once all the parts are checked
				break;

			default:
				return -1;
			}

	if (possibleIndex != -1)
		return possibleIndex;
	return -1;
}

bple::Part bple::Level::getPartPos(int a_posx, int a_posy, Layer a_layer) const
{
	Part nullPart(0, 0, 0, 0, 0, false);
	int possibleIndex = getPartPosIndex(a_posx, a_posy, a_layer);

	if (possibleIndex != -1)
		return parts[possibleIndex];
	return nullPart;
}

bple::Part &bple::Level::getPartPosModifiable(int a_posx, int a_posy, Layer a_layer)
{
	static Part nullPart(0, 0, 0, 0, 0, false);
	int possibleIndex = getPartPosIndex(a_posx, a_posy, a_layer);

	if (possibleIndex != -1)
		return parts[possibleIndex];
	return nullPart;
}

void bple::Level::movePart(int a_index, int a_offx, int a_offy, Selection a_selection)
{
	switch (a_selection)
	{
	case Selection::All:
		parts[a_index].posx += a_offx;
		parts[a_index].posy += a_offy;
		break;

	case Selection::Selected:
		if (parts[a_index].selected)
		{
			parts[a_index].posx += a_offx;
			parts[a_index].posy += a_offy;
		}
		break;

	case Selection::NonSelected:
		if (!parts[a_index].selected)
		{
			parts[a_index].posx += a_offx;
			parts[a_index].posy += a_offy;
		}
		break;

	default:
		return;
	}
}

void bple::Level::movePartAll(int a_offx, int a_offy, Selection a_selection)
{
	for (int index = 0; index < parts.size(); index++)
		movePart(index, a_offx, a_offy, a_selection);
}

int bple::Level::getPartInsertable(int a_type, int a_skin, int a_posx, int a_posy)
{
	for (int index = 0; index < parts.size(); index++)
		if (parts[index].posx == a_posx && parts[index].posy == a_posy)
			if (isPartFrame[parts[index].type][parts[index].skin] == isPartFrame[a_type][a_skin])
				return index;
	return -1;
}

void bple::Level::insert(int a_type, int a_skin, int a_posx, int a_posy, int a_prop, int a_flip, bool a_override)
{
	int insertableIndex = getPartInsertable(a_type, a_skin, a_posx, a_posy);
	if (insertableIndex != -1 && a_override)
	{
		deleteIndex(insertableIndex);
		insertableIndex = -1;
	}
	if (insertableIndex == -1)
		parts.push_back(Part(a_type, a_skin, a_posx, a_posy, a_prop, a_flip));
}

void bple::Level::insert(const Part &a_part, bool a_override)
{
	insert(a_part.type, a_part.skin, a_part.posx, a_part.posy, a_part.prop, a_part.flip, a_override);
}

void bple::Level::deleteIndexLayer(int a_index, Layer a_layer)
{
	int possibleIndex = -1;
	switch (a_layer)
	{
	case Layer::All:
		parts.erase(parts.begin() + a_index);
		return;

	case Layer::Frame:
		if (isPartFrame[parts[a_index].type][parts[a_index].skin])
			parts.erase(parts.begin() + a_index);
		return;

	case Layer::NonFrame:
		if (!isPartFrame[parts[a_index].type][parts[a_index].skin])
			parts.erase(parts.begin() + a_index);
		return;

	case Layer::PossibleFrame:
		if (isPartFrame[parts[a_index].type][parts[a_index].skin])
			parts.erase(parts.begin() + a_index);
		else
			possibleIndex = a_index;
		break;

	case Layer::PossibleNonFrame:
		if (!isPartFrame[parts[a_index].type][parts[a_index].skin])
			parts.erase(parts.begin() + a_index);
		else
			possibleIndex = a_index;
		break;

	default:
		return;
	}
	if (possibleIndex != -1)
		parts.erase(parts.begin() + possibleIndex);
}

void bple::Level::deleteIndex(int a_index)
{
	if (a_index >= 0 && a_index < parts.size())
		parts.erase(parts.begin() + a_index);
}

void bple::Level::deleteAll(Layer a_layer, Selection a_selection)
{
	for (int index = 0; index < parts.size(); index++)
		if (getSelected(index, a_selection))
			deleteIndexLayer(index, a_layer);
}

void bple::Level::deleteCustom(bool a_checkType, bool a_checkSkin, bool a_checkProp, bool a_checkFlip, int a_type, int a_skin, int a_prop, int a_flip, Layer a_layer, Selection a_selection)
{
	for (int index = 0; index < parts.size(); index++)
	{
		// Eliminate unmatched parts from being deleted

		if (a_checkType)
			if (parts[index].type != a_type)
				continue;

		if (a_checkSkin)
			if (parts[index].skin != a_skin)
				continue;

		if (a_checkProp)
			if (parts[index].prop != a_prop)
				continue;

		if (a_checkFlip)
			if (parts[index].flip != a_flip)
				continue;

		if (getSelected(index, a_selection))
			deleteIndexLayer(index, a_layer);
	}
}

void bple::Level::deletePos(int a_posx, int a_posy, Layer a_layer, Selection a_selection)
{
	for (int index = 0; index < parts.size(); index++)
	{
		if (parts[index].posx == a_posx && parts[index].posy == a_posy && getSelected(index, a_selection))
			deleteIndexLayer(index, a_layer);
	}
}

bool bple::Level::getSelected(int a_index, Selection a_selection)
{
	switch (a_selection)
	{
	case Selection::All:
		return true;
	case Selection::Selected:
		return parts[a_index].selected;
	case Selection::NonSelected:
		return !parts[a_index].selected;
	default:
		return false;
	}
}

void bple::Level::selectIndex(int a_index, Layer a_layer, Selection a_selection)
{
	int possibleIndex = -1;
	bool check;
	if (a_index >= 0 && a_index < parts.size())
	{
		switch (a_layer)
		{
		case Layer::All:
			check = true;
			break;

		case Layer::Frame:
			if (isPartFrame[parts[a_index].type][parts[a_index].skin])
				check = true;
			break;

		case Layer::NonFrame:
			if (!isPartFrame[parts[a_index].type][parts[a_index].skin])
				check = true;
			break;

		case Layer::PossibleFrame:
			if (isPartFrame[parts[a_index].type][parts[a_index].skin])
				check = true;
			else
				possibleIndex = a_index;
			break;

		case Layer::PossibleNonFrame:
			if (isPartFrame[parts[a_index].type][parts[a_index].skin])
				check = true;
			else
				possibleIndex = a_index;
			break;

		default:
			return;
		}
	}

	if (check)
		switch (a_selection)
		{
		case Selection::All:
			parts[a_index].selected = !parts[a_index].selected;
			break;
		case Selection::Selected:
			parts[a_index].selected = true;
			break;
		case Selection::NonSelected:
			parts[a_index].selected = false;
			break;
		}

	if (possibleIndex != -1)
		for (int Alpha = 0; Alpha < parts.size(); Alpha++)
			if (parts[Alpha].posx == parts[possibleIndex].posx && parts[Alpha].posy == parts[possibleIndex].posy)
			{
				if (a_selection == Selection::Selected)
					parts[possibleIndex].selected = true;
				if (a_selection == Selection::NonSelected)
					parts[possibleIndex].selected = false;
			}
}

void bple::Level::selectPos(int a_posx, int a_posy, Layer a_layer, Selection a_selection)
{
	for (int index = 0; index < parts.size(); index++)
		if (a_posx == parts[index].posx && a_posy == parts[index].posy)
			selectIndex(index, a_layer, a_selection);
}

bple::Level &bple::Level::operator=(const Level &a_level)
{
	// Clear part list and add the parts from the level
	parts.clear();
	for (int index = 0; index < a_level.parts.size(); index++)
		parts.push_back(a_level.parts[index]);
	return *this;
}

/// @brief Add parts to this level that are not the same
bple::Level &bple::Level::operator+=(const Level &a_addLevel)
{
	for (int index = 0; index < a_addLevel.parts.size(); index++)
		insert(a_addLevel.parts[index]);
	return *this;
}

/// @brief Remove parts from this level that are the same
bple::Level &bple::Level::operator-=(const Level &a_subLevel)
{
	for (int Alpha = 0; Alpha < parts.size(); Alpha++)
		for (int Beta = 0; Beta < a_subLevel.parts.size(); Beta++)
			if (parts[Alpha] == a_subLevel.parts[Beta])
			{
				deleteIndex(Alpha);
				Alpha--;
			}
	return *this;
}

bple::Level &bple::Level::operator+(const Level &a_addLevel) const
{
	static Level staticLevel = *this;
	staticLevel += a_addLevel;
	return staticLevel;
}

bple::Level &bple::Level::operator-(const Level &a_subLevel) const
{
	static Level staticLevel = *this;
	staticLevel -= a_subLevel;
	return staticLevel;
}

bple::Part &bple::Level::operator[](int a_index)
{
	if (a_index > parts.size() - 1)
		a_index = 0;
	if (a_index < 0)
		a_index = parts.size() - 1;
	return parts[a_index];
}

/// @brief Insert a part in the level
/// @param a_part the desired part to be inserted
void bple::Level::operator()(Part &a_part)
{
	insert(a_part);
}

/// @brief Get the part count when casting to an integer
bple::Level::operator int()
{
	return parts.size();
}

/// @brief Returns true when the level is not empty
bple::Level::operator bool()
{
	return (int(*this) != 0);
}

bple::Level &bple::Level::operator<<(const Part &a_inject)
{
	parts.push_back(a_inject);
	return *this;
}

bple::Level &bple::Level::operator>>(Part &a_extract)
{
	a_extract = parts[parts.size() - 1];
	parts.pop_back();
	return *this;
}

bple::Level &bple::Level::operator<<(Level &a_inject)
{
	Part a_part;
	while (a_inject.parts.size() != 0)
	{
		a_inject >> a_part;
		*this << a_part;
	}
	return *this;
}

bple::Level &bple::Level::operator>>(Level &a_extract)
{
	Part a_part;
	while (parts.size() != 0)
	{
		*this >> a_part;
		a_extract << a_part;
	}
	return *this;
}

namespace bple
{
	// Getting data into or out of stream
	// Almost same as save and load functions

	std::ostream &operator<<(std::ostream &a_streamOutput, const Level &a_levelInput)
	{
		for (int index = 0; index < a_levelInput.parts.size(); index++)
			a_streamOutput
				<< a_levelInput.parts[index].type << ','
				<< a_levelInput.parts[index].skin << ','
				<< a_levelInput.parts[index].posx << ','
				<< a_levelInput.parts[index].posy << ','
				<< a_levelInput.parts[index].prop << ','
				<< a_levelInput.parts[index].flip << '\n';

		return a_streamOutput;
	}

	std::istream &operator>>(std::istream &a_streamInput, Level &a_levelOutput)
	{
		std::string LineData;
		std::stringstream Convert;
		a_streamInput >> LineData;
		int sector = 0;
		LineData += ',';
		int type = 0, skin = 0, posx = 0, posy = 0, prop = 0, flip;
		for (int index = 0; index < LineData.length(); index++)
		{
			if (LineData[index] == ',')
			{
				switch (sector)
				{
				case 0:
					type = std::stoi(Convert.str());
					break;
				case 1:
					skin = std::stoi(Convert.str());
					break;
				case 2:
					posx = std::stoi(Convert.str());
					break;
				case 3:
					posy = std::stoi(Convert.str());
					break;
				case 4:
					prop = std::stoi(Convert.str());
					break;
				case 5:
					flip = std::stoi(Convert.str());
					break;
				}
				Convert.str("");
				Convert.clear();

				sector++;
			}
			else
				Convert << LineData[index];
		}
		a_levelOutput.parts.push_back(Part(type, skin, posx, posy, prop, flip));
		return a_streamInput;
	}
}

// Header Guard Detection
#ifndef SAVEFILE_MANAGER_H

// Manual Header Guard and Indicator Setup
#define SAVEFILE_MANAGER_H

// Uses strings instead of char array
#include <string>

// Uses vector to store part data
#include <vector>

// Used string-stream to convert data
#include <sstream>

// Used file-stream to load / save files
#include <fstream>

// Used math library for math functions
#include <cmath>

// GamerZ - Bad Piggies Mod Leading Edge - Contains useful features in managing
// SaveFile. Namespace is specified, Use "GZBPLE::" or "using namespace GZBPLE;"
// to access the features. This is to prevent name collision with your functions
namespace GZBPLE
{

    // Check whether an object is a frame or not
    extern std::vector<std::vector<bool>> PartObjectFrame;

    // The type of property used to determine "Prop"
    // Whether or not use rotation or color as the basis
    // 0: None ( Prop must be 0 )
    // 1: Rotation in 8 different angles
    // 2: Rotation in 4 different angles
    // 3: Rotation in 2 different angles
    // 4: Four colors
    extern std::vector<std::vector<int>> PartObjectPropType;

    // Get count of skins per part
    extern std::vector<int> SkinsPerPart;

    // Convert to String
    template <typename Type>
    std::string ConvertToString(Type Input);

    // Convert to Integer
    template <typename Type>
    int ConvertToInteger(Type Input);

    // If the Part-Prop is rotation ( In degrees )
    enum class Rotation
    {
        Up = 0,
        Left = 270,
        Down = 180,
        Right = 90,
        UpLeft = 315,
        DownLeft = 225,
        DownRight = 135,
        UpRight = 45
    };

    // If the Part-Prop is color
    enum class Color
    {
        Green,
        Yellow,
        Red,
        Blue
    };

    // Possible Actions
    enum class Action
    {
        InsertPart,
        DeletePart,
        SelectPart,
        DeSelectPart,
        NullPart
    };

    // Part Container
    class PartObject
    {

        // Set variables and constructor accessable
    public:
        // Data stored in the game SaveFile

        int Type;  // Type of the part. eg Pig type index is 1 ( Default Data 0 )
        int Skin;  // Skin of the part. eg Glue egg skin index is 5 ( Default Data 1 )
        int PosX;  // Position of the part in X-Axis ( Default Data 2 )
        int PosY;  // Position of the part in Y-Axis ( Default Data 3 )
        int Prop;  // Potation or color of the part ( Default Data 4 )
        bool Flip; // Whether or not the part is flipped ( Default Data 5 )

        // The type of property used to determine "Prop"
        // Whether or not use rotation or color as the basis
        int PropType;

        // Custom data

        bool Frame,	  // Whether or not the part is frame
            Selected; // Whether or not the part is selected

        // Default constructor, The rest of the data is automatically set
        PartObject(int PartType, int PartSkin, int PartPosX,
                   int PartPosY, int PartProp, bool PartFlip);
    };

    // Level Data container
    class Level
    {

        // Private functions, Not useful for external use
        // Change this label to public if you really want to use it
        // These functions are used by other functions in this scope
    private:
        // Delete parts based on index and method. Not useful,
        // void DeleteIndex( int Index ) is what you should use.
        void DeleteIndexMethod(int Index, int Method);

        // Get boolean of if a part is selected with using of Mode
        bool GetSelectedMode(int Index, int Mode);

        // Set useful functions to be accessable
    public:
        // Contains part data
        std::vector<PartObject> Part;

        // Loads SaveFile of CSV format
        // Use std::wstring.c_str() or L""
        void LoadFile(const wchar_t *File);

        // Saves SaveFile of CSV format
        // Use std::wstring.c_str() or L""
        void SaveFile(const wchar_t *File);

        // The index of Method variable:
        //
        //     0. Delete frame with non-frame
        //     1. Delete only frame
        //     2. Delete only non-frame
        //     3. Delete frame or non-frame if no frame available
        //     4. Delete non-frame or frame if no non-frame available
        //
        // The index of Mode variable:
        //
        //     Selection Function:
        //
        //         0. Inverse selection
        //         1. Always select
        //         2. Always de-select
        //
        //     Action Function:
        //
        //         0. All selected and de-selected
        //         1. Only selected
        //         2. Only de-selected
        //

        // Select part based on position
        void SelectPos(int PartPosX, int PartPosY, int Method, int Mode);

        // Select part based on index
        void SelectIndex(int Index, int Method, int Mode);

        // Select part based on start and end position ( Both included )
        void SelectRectangle(int StartX, int StartY, int EndX,
                             int EndY, int Method, int Mode, bool Fill);

        // Select part based on start and end position in an elaspe ( Both included )
        void SelectEllipse(int StartX, int StartY, int EndX,
                           int EndY, int Method, int Mode, bool Fill);

        // Move parts from the origin to the offset
        void MovePart(int OffsetX, int OffsetY, int Mode);

        // Insert Part according to the data
        void InsertPart(int PartType, int PartSkin, int PartPosX,
                        int PartPosY, int PartProp, bool PartFlip);

        // Get part based on position
        // ( Pig of no skin if no part available, Data: 1, 0, 0, 0, 0, false )
        PartObject GetPartPos(int PartPosX, int PartPosY, int Method);

        // Delete part based on index
        void DeleteIndex(int Index);

        // Delete parts based on selection
        void DeleteSelected(int Method, int Mode);

        // Delete parts with specified Part-Type
        void DeleteType(int PartType, int Method, int Mode);

        // Delete parts with specified Part-Type and Part-Skin
        void DeleteTypeSkin(int PartType, int PartSkin, int Method, int Mode);

        // Delete parts with specified
        // Part-Type, Part-Skin and Part-Prop
        void DeleteTypeSkinProp(int PartType, int PartSkin,
                                int PartProp, int Method, int Mode);

        // Delete parts with specified
        // Part-Type, Part-Skin and Part-Flip
        void DeleteTypeSkinFlip(int PartType, int PartSkin,
                                bool PartFlip, int Method, int Mode);

        // Delete parts with specified Part-Type,
        // Part-Skin, Part-Prop and Part-Flip
        void DeleteTypeSkinPropFlip(int PartType, int PartSkin, int PartProp,
                                    bool PartFlip, int Method, int Mode);

        // Delete parts with specified Position
        void DeletePos(int PartPosX, int PartPosY, int Method, int Mode);
    };

}

// Header Guard End
#endif

// Use "#define SFMANAGER" before including the file to implement features
#ifdef SFMANAGER

std::vector<std::vector<bool>> GZBPLE::PartObjectFrame{

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
     true, true, true, true, true, true, true, true, true, true, false,
     true, true, true},

    {false, false, false, false, false, false, false, false, false, false,
     false},

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

    {false, false, false, false, false, false, false, false, false, false,
     false},

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

std::vector<std::vector<int>> GZBPLE::PartObjectPropType{

    {false},

    {false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false},

    {false},

    {false, false, false, false, false, false, false},

    {false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false, false, false,
     false},

    {false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false},

    {false, false, false, false, false, false, false, false, false, false,
     false},

    {false, false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false},

    {false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false, false, false},

    {false, false, false, false, false, false, false, false, false, false},

    {true, true, true, true, true},

    {true, true, true, true, true},

    {true, true, true, true},

    {true, true, true, true},

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

    {true, true, true, true, true},

    {false, false, false, false},

    {true, true, true, true},

    {false, false, false, false},

    {false, false, false, false, false},

    {false, false, false, false},

    {true, true, true, true, true, true, true, false, true, true, true},

    {false, false},

    {false, false, false, false, false},

    {false, false, false, false, false, false},

    {false, false, false, false},

    {false, false, false, false, false, false, false},

    {true, true, true, true},

    {true, true, false, false, false, false},

    {false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false, false, false,
     false, false, false, false, false, false, false, false}};

std::vector<int> GZBPLE::SkinsPerPart{
    1,	 // Empry ( Type: 0 )
    20,	 // ( Type: 1 )
    1,	 // ( Type: 2 )
    7,	 // ( Type: 3 )
    6,	 // ( Type: 4 )
    11,	 // ( Type: 5 )
    134, // ( Type: 6 )
    11,	 // ( Type: 7 )
    7,	 // ( Type: 8 )
    8,	 // ( Type: 9 )
    8,	 // ( Type: 10 )
    8,	 // ( Type: 11 )
    6,	 // ( Type: 12 )
    10,	 // ( Type: 13 )
    10,	 // ( Type: 14 )
    5,	 // ( Type: 15 )
    5,	 // ( Type: 16 )
    4,	 // ( Type: 17 )
    4,	 // ( Type: 18 )
    10,	 // ( Type: 19 )
    7,	 // ( Type: 20 )
    7,	 // ( Type: 21 )
    7,	 // ( Type: 22 )
    4,	 // ( Type: 23 )
    4,	 // ( Type: 24 )
    6,	 // ( Type: 25 )
    8,	 // ( Type: 26 )
    8,	 // ( Type: 27 )
    8,	 // ( Type: 28 )
    8,	 // ( Type: 29 )
    6,	 // ( Type: 30 )
    4,	 // ( Type: 31 )
    4,	 // ( Type: 32 )
    5,	 // ( Type: 33 )
    4,	 // ( Type: 34 )
    4,	 // ( Type: 35 )
    4,	 // ( Type: 36 )
    5,	 // ( Type: 37 )
    4,	 // ( Type: 38 )
    11,	 // One empty at index 7 ( Type: 39 )
    2,	 // ( Type: 41 )
    5,	 // ( Type: 42 )
    6,	 // ( Type: 43 )
    4,	 // ( Type: 44 )
    7,	 // ( Type: 45 )
    4,	 // ( Type: 46 )
    6,	 // ( Type: 47 )
    48	 // ( Type: 48 )
};

GZBPLE::PartObject::PartObject(int PartType, int PartSkin, int PartPosX,
                               int PartPosY, int PartProp, bool PartFlip)
{
    Type = PartType;
    Skin = PartSkin;
    PosX = PartPosX;
    PosY = PartPosY;
    Prop = PartProp;
    Flip = PartFlip;
    Frame = PartObjectFrame.at(Type).at(Skin);
    PropType = PartObjectPropType.at(Type).at(Skin);
}

template <typename Type>
std::string GZBPLE::ConvertToString(Type Input)
{
    std::stringstream Convert;
    std::string Output;
    Convert << Input;
    Convert >> Output;
    return Output;
}

template <typename Type>
int GZBPLE::ConvertToInteger(Type Input)
{
    std::stringstream Convert;
    std::string InputString = ConvertToString(Input);
    int Output = 0;
    bool Negative = false;
    for (int Index = 0; Index < InputString.length(); Index++)
    {
        if (InputString.at(Index) >= '0' && InputString.at(Index) <= '9')
            Convert << InputString.at(Index);
        if (InputString.at(Index) == '-')
            Negative = true;
    }
    Convert >> Output;
    if (Negative)
        Output = Output * -1;
    return Output;
}

void GZBPLE::Level::LoadFile(const wchar_t *File)
{
    std::ifstream Ifile;
    std::string LineData;
    std::stringstream Convert;
    Part.clear();
    Ifile.open(File);
    if (Ifile.fail())
    {
        Ifile.close();
        return;
    }
    for (int Lines = 0; std::getline(Ifile, LineData); Lines++)
    {
        int Sector = 0;
        LineData = LineData + ",";
        int Type, Skin, PosX, PosY, Prop;
        bool Flip;
        for (int Index = 0; Index < LineData.length(); Index++)
        {
            if (LineData[Index] == ',')
            {
                switch (Sector)
                {
                case 0:
                    Type = ConvertToInteger(Convert.str());
                    break;
                case 1:
                    Skin = ConvertToInteger(Convert.str());
                    break;
                case 2:
                    PosX = ConvertToInteger(Convert.str());
                    break;
                case 3:
                    PosY = ConvertToInteger(Convert.str());
                    break;
                case 4:
                    Prop = ConvertToInteger(Convert.str());
                    break;
                case 5:
                    Flip = ConvertToInteger(Convert.str());
                    break;
                }
                Convert.str("");
                Convert.clear();
                Sector++;
            }
            else
                Convert << LineData[Index];
        }
        Part.push_back(PartObject(Type, Skin, PosX, PosY, Prop, Flip));
        Convert.str("");
        Convert.clear();
    }
    Ifile.close();
}

void GZBPLE::Level::SaveFile(const wchar_t *File)
{
    std::ofstream Ofile;
    Ofile.open(File);
    if (Ofile.fail())
    {
        Ofile.close();
        return;
    }
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Ofile << Part.at(Index).Type << ','
              << Part.at(Index).Skin << ','
              << Part.at(Index).PosX << ','
              << Part.at(Index).PosY << ','
              << Part.at(Index).Prop << ','
              << Part.at(Index).Flip << '\n';
    }
    Ofile.close();
}

void GZBPLE::Level::SelectIndex(int Index, int Method, int Mode)
{
    std::vector<int> PendingParts;
    if (Index >= 0 && Index < Part.size())
    {
        switch (Method)
        {
        case 0:
        {
            switch (Mode)
            {
            case 0:
                Part.at(Index).Selected = !Part.at(Index).Selected;
                break;
            case 1:
                Part.at(Index).Selected = true;
                break;
            case 2:
                Part.at(Index).Selected = false;
                break;
            }
        }
        break;
        case 1:
        {
            if (Part.at(Index).Frame)
            {
                switch (Mode)
                {
                case 0:
                    Part.at(Index).Selected = !Part.at(Index).Selected;
                    break;
                case 1:
                    Part.at(Index).Selected = true;
                    break;
                case 2:
                    Part.at(Index).Selected = false;
                    break;
                }
            }
        }
        break;
        case 2:
        {
            if (!Part.at(Index).Frame)
            {
                switch (Mode)
                {
                case 0:
                    Part.at(Index).Selected = !Part.at(Index).Selected;
                    break;
                case 1:
                    Part.at(Index).Selected = true;
                    break;
                case 2:
                    Part.at(Index).Selected = false;
                    break;
                }
            }
        }
        break;
        case 3:
        {
            if (Part.at(Index).Frame)
            {
                switch (Mode)
                {
                case 0:
                    Part.at(Index).Selected = !Part.at(Index).Selected;
                    break;
                case 1:
                    Part.at(Index).Selected = true;
                    break;
                case 2:
                    Part.at(Index).Selected = false;
                    break;
                }
            }
            else
                PendingParts.push_back(Index);
        }
        break;
        case 4:
        {
            if (!Part.at(Index).Frame)
            {
                switch (Mode)
                {
                case 0:
                    Part.at(Index).Selected = !Part.at(Index).Selected;
                    break;
                case 1:
                    Part.at(Index).Selected = true;
                    break;
                case 2:
                    Part.at(Index).Selected = false;
                    break;
                }
            }
            else
                PendingParts.push_back(Index);
        }
        break;
        default:
            return;
        }
    }
    if (PendingParts.size() != 0)
        for (int Alpha = 0; Alpha < Part.size(); Alpha++)
            for (int Beta = 0; Beta < PendingParts.size(); Beta++)
                if (Part.at(Alpha).PosX == Part.at(PendingParts.at(Beta)).PosX &&
                    Part.at(Alpha).PosY == Part.at(PendingParts.at(Beta)).PosY)
                {
                    if (Mode == 1)
                    {
                        Part.at(PendingParts.at(Beta)).Selected = true;
                    }
                    if (Mode == 2)
                    {
                        Part.at(PendingParts.at(Beta)).Selected = false;
                    }
                }
}

void GZBPLE::Level::DeleteIndexMethod(int Index, int Method)
{
    int TriedDisplaceIndex = -1;
    switch (Method)
    {
    case 0:
    {
        Part.erase(Part.begin() + Index);
    }
    break;
    case 1:
    {
        if (Part.at(Index).Frame)
            Part.erase(Part.begin() + Index);
    }
    break;
    case 2:
    {
        if (!Part.at(Index).Frame)
            Part.erase(Part.begin() + Index);
    }
    break;
    case 3:
    {
        if (Part.at(Index).Frame)
        {
            Part.erase(Part.begin() + Index);
            return;
        }
        else
            TriedDisplaceIndex = Index;
    }
    break;
    case 4:
    {
        if (!Part.at(Index).Frame)
        {
            Part.erase(Part.begin() + Index);
            return;
        }
        else
            TriedDisplaceIndex = Index;
    }
    break;
    default:
        return;
    }
    if (TriedDisplaceIndex != -1 && (Method == 3 || Method == 4))
    {
        Part.erase(Part.begin() + TriedDisplaceIndex);
    }
}

bool GZBPLE::Level::GetSelectedMode(int Index, int Mode)
{
    switch (Mode)
    {
    case 0:
        return true;
    case 1:
        return Part.at(Index).Selected;
    case 2:
        return !Part.at(Index).Selected;
    }
}

void GZBPLE::Level::SelectRectangle(int StartX, int StartY, int EndX,
                                    int EndY, int Method, int Mode, bool Fill)
{
    bool Confirm = false;
    std::vector<int> PendingParts;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        if (
            (Fill && (Part.at(Index).PosX >= StartX &&
                      Part.at(Index).PosY >= StartY &&
                      Part.at(Index).PosX <= EndX &&
                      Part.at(Index).PosY <= EndY)) ||
            (!Fill && ((
                           (Part.at(Index).PosX == StartX ||
                            Part.at(Index).PosX == EndX) &&
                           (Part.at(Index).PosY >= StartY &&
                            Part.at(Index).PosY <= EndY)) ||
                       ((Part.at(Index).PosY == StartY ||
                         Part.at(Index).PosY == EndY) &&
                        (Part.at(Index).PosX >= StartX &&
                         Part.at(Index).PosX <= EndX)))))
        {
            SelectIndex(Index, Method, Mode);
        }
    }
}

void GZBPLE::Level::SelectEllipse(int StartX, int StartY, int EndX,
                                  int EndY, int Method, int Mode, bool Fill)
{
    if (StartX >= EndX || StartY >= EndY)
        return;
    float CenterX, CenterY, Width, Height;
    bool Confirm = false;
    CenterX = (StartX + EndX) / 2.0f;
    CenterY = (StartY + EndY) / 2.0f;
    Width = EndX - StartX;
    Height = EndY - StartY;
    for (int Alpha = StartX; Alpha < EndX + 1; Alpha++)
        for (int Beta = StartY; Beta < EndY + 1; Beta++)
        {
            int Formula = std::pow(
                              (Alpha - CenterX) / (Width / 2, 2), 2) +
                          std::pow(
                              (Beta - CenterY) / (Height / 2), 2);
            if (Fill && (Formula <= 1) || !Fill && (Formula == 1))
                for (int Index = 0; Index < Part.size(); Index++)
                {
                    Confirm = GetSelectedMode(Index, Mode);
                    if (Part.at(Index).PosX == Alpha &&
                        Part.at(Index).PosY == Beta &&
                        Confirm)
                        SelectIndex(Index, Method, Mode);
                }
        }
}

void GZBPLE::Level::MovePart(int OffsetX, int OffsetY, int Mode)
{
    for (int Index = 0; Index < Part.size(); Index++)
    {
        switch (Mode)
        {
        case 0:
        {
            Part.at(Index).PosX += OffsetX;
            Part.at(Index).PosY += OffsetY;
        }
        break;
        case 1:
        {
            if (Part.at(Index).Selected)
            {
                Part.at(Index).PosX += OffsetX;
                Part.at(Index).PosY += OffsetY;
            }
        }
        break;
        case 2:
        {
            if (!Part.at(Index).Selected)
            {
                Part.at(Index).PosX += OffsetX;
                Part.at(Index).PosY += OffsetY;
            }
        }
        break;
        default:
            return;
        }
    }
}

void GZBPLE::Level::InsertPart(int PartType, int PartSkin, int PartPosX,
                               int PartPosY, int PartProp, bool PartFlip)
{
    bool Placable = true;
    for (int Index = 0; Index < Part.size(); Index++)
        if (Part.at(Index).PosX == PartPosX &&
            Part.at(Index).PosY == PartPosY)
            if (!(Part.at(Index).Frame ^
                  PartObjectFrame.at(PartType).at(PartSkin)))
                return;
    if (Placable)
        Part.push_back(PartObject(PartType, PartSkin, PartPosX,
                                  PartPosY, PartProp, PartFlip));
}

void GZBPLE::Level::DeleteIndex(int Index)
{
    if (Index >= 0 && Index < Part.size())
        Part.erase(Part.begin() + Index);
}

void GZBPLE::Level::DeleteType(int PartType, int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        if (Part.at(Index).Type == PartType && Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

void GZBPLE::Level::DeleteTypeSkin(int PartType, int PartSkin,
                                   int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Confirm = GetSelectedMode(Index, Mode);
        if (Part.at(Index).Type == PartType &&
            Part.at(Index).Skin == PartSkin &&
            Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

void GZBPLE::Level::DeleteTypeSkinProp(int PartType, int PartSkin,
                                       int PartProp, int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Confirm = GetSelectedMode(Index, Mode);
        if (Part.at(Index).Type == PartType &&
            Part.at(Index).Skin == PartSkin &&
            Part.at(Index).Prop == PartProp &&
            Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

void GZBPLE::Level::DeleteTypeSkinFlip(int PartType, int PartSkin,
                                       bool PartFlip, int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Confirm = GetSelectedMode(Index, Mode);
        if (Part.at(Index).Type == PartType &&
            Part.at(Index).Skin == PartSkin &&
            Part.at(Index).Flip == PartFlip &&
            Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

void GZBPLE::Level::DeleteTypeSkinPropFlip(int PartType, int PartSkin,
                                           int PartProp, bool PartFlip,
                                           int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Confirm = GetSelectedMode(Index, Mode);
        if (Part.at(Index).Type == PartType &&
            Part.at(Index).Skin == PartSkin &&
            Part.at(Index).Prop == PartProp &&
            Part.at(Index).Flip == PartFlip &&
            Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

void GZBPLE::Level::DeletePos(int PartPosX, int PartPosY, int Method, int Mode)
{
    bool Confirm = false;
    for (int Index = 0; Index < Part.size(); Index++)
    {
        Confirm = GetSelectedMode(Index, Mode);
        if (Part.at(Index).PosX == PartPosX &&
            Part.at(Index).PosY == PartPosY &&
            Confirm)
            DeleteIndexMethod(Index, Method);
    }
}

#endif
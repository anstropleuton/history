/*
 * BPLE Level manager
 * Note: BPLE only
 */

#ifndef BPLE_LEVEL_HPP
#define BPLE_LEVEL_HPP

#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "bple_data.hpp"

struct BPLELevelPart {
    int type;
    int skin;
    int posx;
    int posy;
    int prop;
    int flip;

    // Used by Level
    bool selected;

    BPLELevelPart() = default;

    // Level file format
    // <type>,<skin>,<posx>,<posy>,<prop>,<flip>[,comments]
    // No newline character btw (if it doesn't have a comment)
    BPLELevelPart(std::string partData)
    {
        from_string(partData);
    }

    void from_string(std::string partData)
    {
        if (partData.empty())
        {
            return;
        }

        std::stringstream ss = std::stringstream(partData);
        std::string value;

        std::getline(ss, value, ',');
        type = std::stoi(value);
        std::getline(ss, value, ',');
        skin = std::stoi(value);
        std::getline(ss, value, ',');
        posx = std::stoi(value);
        std::getline(ss, value, ',');
        posy = std::stoi(value);
        std::getline(ss, value, ',');
        prop = std::stoi(value);
        std::getline(ss, value, ',');
        flip = std::stoi(value);
    }

    // Newline not included
    std::string to_string() const
    {
        return std::format("{},{},{},{},{},{}", type, skin, posx, posy, prop, flip);
    }

    bool is_valid() const
    {
        if (type > 0 && type < skinsPerType.size() && skin >= 0 && skin < skinsPerType[type])
        {
            if (!(type == 39 && skin == 7))
            {
                return true;
            }
        }
        return false;
    }

    bool is_frame() const
    {
        return type == 5 || (type == 6 && skin != 130);
    }
};

struct BPLELevel {
    std::vector<BPLELevelPart> parts;
    std::string filename;

    BPLELevel() = default;

    BPLELevel(std::string filename)
    {
        this->filename = filename;
        open();
    }

    // Each line is a part
    // Return true on success
    bool open(std::string filename)
    {
        this->filename = filename;
        return open();
    }

    // Each line is a part
    // Return true on success
    bool open()
    {
        std::ifstream ifile = std::ifstream(filename);
        if (!ifile)
        {
            return false;
        }

        std::string line;
        while (std::getline(ifile, line))
        {
            parts.push_back(BPLELevelPart(line));
        }

        ifile.close();
        return true;
    }

    // Return true on success
    bool save(std::string filename)
    {
        this->filename = filename;
        return save();
    }

    // Return true on success
    bool save()
    {
        std::ofstream ofile = std::ofstream(filename);
        if (ofile.fail())
        {
            return false;
        }

        for (BPLELevelPart part : parts)
        {
            ofile << part.to_string() << std::endl;
        }

        ofile.close();
        return true;
    }

    BPLELevelPart *part_at(int x, int y, bool frame)
    {
        for (BPLELevelPart &part : parts)
        {
            if (part.posx == x && part.posy == y && part.is_frame() == frame)
            {
                return &part;
            }
        }
        return nullptr;
    }

    void insert_part(BPLELevelPart part, bool replace = false)
    {
        BPLELevelPart *foundPart = part_at(part.posx, part.posy, part.is_frame());
        if (foundPart && replace)
        {
            *foundPart = part;
        }
        else
        {
            parts.push_back(part);
        }
    }

    void invert_selection()
    {
        for (BPLELevelPart &part : parts)
        {
            part.selected = !part.selected;
        }
    }

    // Min inclusive, max exclusive
    void select_rectangle(int minx, int miny, int maxx, int maxy)
    {
        for (BPLELevelPart &part : parts)
        {
            // Note: we are not doing
            // part.selected = (THE_BELOW_CONDITION)
            // Because we keep selected parts as is
            if (part.posx >= minx && part.posx < maxx && part.posy >= miny && part.posy < maxy)
            {
                part.selected = true;
            }
        }
    }

    void move_selected_parts(int offsetx, int offsety)
    {
        for (BPLELevelPart &part : parts)
        {
            if (part.selected)
            {
                part.posx += offsetx;
                part.posy += offsety;
            }
        }
    }

    // Note: position of part parameter is ignored
    void fill_rectangle(int minx, int miny, int maxx, int maxy, BPLELevelPart part, bool replace = false)
    {
        for (int x = minx; x < maxx; x++)
        {
            for (int y = miny; y < maxy; y++)
            {
                part.posx = x;
                part.posy = y;
                insert_part(part, replace);
            }
        }
    }

    void copy_from(const BPLELevel &level, bool replace = false)
    {
        for (const BPLELevelPart &part : level.parts)
        {
            insert_part(part, replace);
        }
    }
};

#endif // BPLE_LEVEL_HPP

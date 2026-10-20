#ifndef MINI_LANG_COMMON_H
#define MINI_LANG_COMMON_H

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cassert>

struct Loc {
    int line = 1;
    int col = 1;
    std::string to_string() const {
        std::ostringstream os;
        os << "line " << line << " col " << col;
        return os.str();
    }
};

#endif // MINI_LANG_COMMON_H

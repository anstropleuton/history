#pragma once

#include "source_code.hpp"
#include <string>
#include <vector>

namespace adobans {
    enum class token_type {
        int_lit,    // Int literal
        float_lit,  // Float literal
        string_lit, // String literal
        char_lit,   // Char literal
        bool_lit,   // Bool literal
        identifier, // Identifier
        delimiter,  // Delimiter (and operator)
        keyword     // Keyword
    };

    using token_value = std::variant<int, float, std::string, char, bool>;

    struct token {
        token_type  type;
        text_range  range;
        token_value value;
    };

    struct tokenizer_result {
        std::vector<token>  tokens;
        std::vector<report> reports;
    };

    tokenizer_result tokenize(std::weak_ptr<source_code> code);
}

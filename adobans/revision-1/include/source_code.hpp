#pragma once

#include <memory>
#include <string>

namespace adobans {
    struct source_code {
        std::string name;
        std::string content;
    };

    struct text_range {
        std::size_t                begin  = 0;
        std::size_t                length = 0;
        std::weak_ptr<source_code> code;
    };

    enum class severity {
        note,
        warning,
        error,
        help
    };

    struct report {
        severity    sev;
        text_range  range;
        std::string message;
    };
}

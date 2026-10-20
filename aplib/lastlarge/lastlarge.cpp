#include "aplib.hpp"
#include <vector>

using namespace aplib::ansi;

int main(int argc, char **argv)
{
    // --------------------------------
    // Flag setup
    // --------------------------------

    enum class flag_t {
        flag_help = 0
    };
    std::vector<aplib::argp::flag> flags = {
        aplib::argp::flag { "Print this help message", { "help" }, { 'h', '?' }, {}, 0 }
    };

    // --------------------------------
    // Program variables
    // --------------------------------

    // --------------------------------
    // Option handler
    // --------------------------------

    auto option_help = [&](const aplib::argp::option &option) {
        aplib::argp::print_help(flags);
    };

    // --------------------------------
    // Argument parsing
    // --------------------------------

    std::vector<aplib::argp::option> options = aplib::argp::get_options_from_flags(argc, argv, flags);
    for (size_t i = 0; i < options.size(); i++)
    {
        const aplib::argp::option &option = options[i];
        if (!option.unrecognized_option.empty()) std::cout << yellow << "Warning: " << reset << "Unrecognized option: " << yellow << option.unrecognized_option << reset << std::endl;
        else if (option.flag_ptr == &flags[(int)flag_t::flag_help]) option_help(option);
    }
}
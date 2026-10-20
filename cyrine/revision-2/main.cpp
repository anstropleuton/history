#include "cyrine.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <string>

using namespace cyrine;

std::string input(std::string prompt) {
    std::print("{}", prompt);
    std::string string;
    std::getline(std::cin, string);
    return string;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);

    std::string home = std::getenv("HOME");

    std::string filename = home + "/.config/cyrine/main_vault.vlt";

    if (!std::filesystem::exists(home + "/.config/cyrine")) {
        std::filesystem::create_directory(home + "/.config/cyrine");
    }

    for (std::size_t arg_i = 0; arg_i < args.size(); arg_i++) {
        auto arg = [&] { return args.at(arg_i); };

        if (arg() == "-f" || arg() == "--file") {
            arg_i++;
            filename = arg();
        } else if (arg() == "-h" || arg() == "--help") {
            std::println("Usage: cyrine [-f filename] <add|list|show|remove|generate>");
        }

        else if (arg() == "add") {
            arg_i++;

            std::string master_password = input("Master password to unlock vault: ");

            if (!std::filesystem::exists(filename)) {
                std::println("Creating new vault...");
                std::ofstream outfile(filename);
                write_vault(vault{}, outfile, master_password);
            }

            std::ifstream infile(filename);
            auto          vault = read_vault(infile, master_password);

            raw_password new_password;
            new_password.username    = input("Username: ");
            new_password.website     = input("Website: ");
            new_password.description = input("Description: ");

        retry:
            new_password.password_text = input("Password: ");
            float score                = password_score(new_password.password_text);
            std::println("Password score: {} ({})", score, score_to_string(score));
            if (input("Retry? (y/n): ") == "y") goto retry;

            vault.insert(new_password);

            std::ofstream outfile(filename);
            write_vault(vault, outfile, master_password);
        } else if (arg() == "list") {
            arg_i++;

            if (!std::filesystem::exists(filename)) {
                std::println("File not found");
            } else {
                std::string master_password = input("Master password to unlock vault: ");

                std::ifstream infile(filename);
                auto          vault = read_vault(infile, master_password);

                for (auto password : vault) {
                    std::println("{} - {} : {}", password.username, password.website, password.description);
                }
            }
        } else if (arg() == "show") {
            arg_i++;

            if (!std::filesystem::exists(filename)) {
                std::println("File not found");
            } else {
                std::string master_password = input("Master password to unlock vault: ");

                std::ifstream infile(filename);
                auto          vault = read_vault(infile, master_password);

                raw_password password;
                password.username = input("Username: ");
                password.website  = input("Website: ");

                if (auto found = vault.find(password); found != vault.end()) {
                    std::println("{}", found->password_text);
                } else {
                    std::println("Password not found");
                }
            }
        } else if (arg() == "remove") {
            arg_i++;

            if (!std::filesystem::exists(filename)) {
                std::println("File not found");
            } else {
                std::string master_password = input("Master password to unlock vault: ");

                std::ifstream infile(filename);
                auto          vault = read_vault(infile, master_password);

                raw_password password;
                password.username = input("Username: ");
                password.website  = input("Website: ");

                if (auto found = vault.find(password); found != vault.end()) {
                    vault.erase(found);
                    std::println("Password removed");
                } else {
                    std::println("Password not found");
                }

                std::ofstream outfile(filename);
                write_vault(vault, outfile, master_password);
            }
        } else if (arg() == "generate") {
            arg_i++;

            std::string master_password = input("Master password to unlock vault: ");

            std::ifstream infile(filename);
            auto          vault = read_vault(infile, master_password);

            raw_password new_password;
            new_password.username    = input("Username: ");
            new_password.website     = input("Website: ");
            new_password.description = input("Description: ");

            std::size_t length          = std::stoi(input("Password Length: "));
            bool        include_lower   = input("Include lowercase letters? (y/n): ") == "y";
            bool        include_upper   = input("Include uppercase letters? (y/n): ") == "y";
            bool        include_numbers = input("Include numbers? (y/n): ") == "y";
            bool        include_special = input("Include special characters? (y/n): ") == "y";

            std::string lower   = include_lower ? "abcdefghijklmnopqrstuvwxyz" : "";
            std::string upper   = include_upper ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "";
            std::string numbers = include_numbers ? "0123456789" : "";
            std::string special = include_special ? "~!@#$%^&*()-=_+[]\\{}|:;<>?,./" : "";
            std::string charset = lower + upper + numbers + special;

            new_password.password_text = random_password(length, charset);

            vault.insert(new_password);

            std::ofstream outfile(filename);
            write_vault(vault, outfile, master_password);
        }
    }
}

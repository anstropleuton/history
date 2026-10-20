/// @file
///
/// @authors   Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Main source for Cyrine.
///
/// This project is licensed under the terms of MIT License.

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "optrone/parser.hpp"
#include "optrone/template.hpp"

// Optrone argument templates

auto help_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Show help message.",
    .short_names = { 'h', '?' },
    .long_names  = { "help" },
});

auto version_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Show version information.",
    .short_names = { 'v' },
    .long_names  = { "version" },
});

auto file_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "File for the vault to save and load.",
    .short_names = { 'f' },
    .long_names  = { "file" },
    .params      = { "filename" },
    .defaults    = { "passwords.cv" },
});

auto help_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Show help message.",
    .names       = { "help" },
    .params      = { "subcommand" },
    .defaults    = { "" },
});

auto vault_create_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Create a new vault.",
    .names       = { "create" },
});

auto vault_delete_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Delete a vault (DANGEROUS).",
    .names       = { "delete" },
});

auto vault_password_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Change a vault's master password.",
    .names       = { "password" },
    .params      = { "new-password" },
});

auto vault_merge_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Merge vault with another vault.",
    .names       = { "merge", "combine" },
    .params      = { "first-vault", "second-vault", "merged-vault" },
    .defaults    = { "" },
});

auto vault_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description        = "Manage vaults.",
    .names              = { "vault" },
    .nested_subcommands = { vault_create_subcommand, vault_delete_subcommand, vault_password_subcommand, vault_merge_subcommand },
});

auto add_no_check_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Do not display the password's strength, I know what I am doing.",
    .short_names = { 'n' },
    .long_names  = { "no-check" },
});

auto add_generate_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Generate a random strong password.",
    .short_names = { 'g', 'r' },
    .long_names  = { "generate" },
});

auto add_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description    = "Add a passwords to the vault.",
    .names          = { "add" },
    .params         = { "username", "website", "description" },
    .defaults       = { "" },
    .nested_options = { add_no_check_option, add_generate_option },
});

auto remove_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Remove a password from the vault.",
    .names       = { "remove" },
    .params      = { "username", "website" },
});

auto list_username_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Filter usernames.",
    .short_names = { 'u' },
    .long_names  = { "username" },
    .params      = { "username" },
    .variadic    = true,
});

auto list_website_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Filter websites.",
    .short_names = { 'w' },
    .long_names  = { "website" },
    .params      = { "website" },
    .variadic    = true,
});

auto list_include_passwords_option = std::make_shared<optrone::option_template>(optrone::option_template{
    .description = "Include passwords in the list.",
    .long_names  = { "include-passwords" },
});

auto list_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description    = "List passwords in the vault.",
    .names          = { "list" },
    .nested_options = { list_username_option, list_website_option, list_include_passwords_option },
});

auto edit_username_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Edit the username.",
    .names       = { "username" },
    .params      = { "old-username", "website", "new-username" },
});

auto edit_website_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Edit the website.",
    .names       = { "website" },
    .params      = { "username", "old-website", "new-website" },
});

auto edit_password_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Edit the password text.",
    .names       = { "password" },
    .params      = { "username", "website" },
});

auto edit_description_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description = "Edit the description.",
    .names       = { "decription" },
    .params      = { "username", "website", "description" },
});

auto edit_subcommand = std::make_shared<optrone::subcommand_template>(optrone::subcommand_template{
    .description        = "Edit a password in the vault.",
    .names              = { "edit" },
    .nested_subcommands = { edit_username_subcommand, edit_website_subcommand, edit_password_subcommand, edit_description_subcommand },
});

std::vector options{
    help_option,
    version_option,
    file_option,
};

std::vector subcommands{
    vault_subcommand,
    add_subcommand,
    remove_subcommand,
    list_subcommand,
    edit_subcommand,
};

// Globals

std::string passwords_file = "passwords.cv";

// Subcommand-specific globals

// Handlers

void handle_help_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_version_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_file_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_help_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_vault_create_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_vault_delete_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_vault_password_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_vault_merge_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_vault_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_add_no_check_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_add_generate_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_add_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_remove_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_list_username_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_list_website_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_list_include_passwords_option(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_list_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_edit_username_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_edit_website_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_edit_password_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_edit_description_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

void handle_edit_subcommand(const std::vector<optrone::parsed_argument> &args, std::size_t &i)
{}

/// Main function
int main(int argc, char *argv[])
{
}

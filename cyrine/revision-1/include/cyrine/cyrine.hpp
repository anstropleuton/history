/// @file
///
/// @authors   Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// This header file is the root header file for the Cyrine.
///
/// This project is licensed under the terms of MIT License.

#pragma once

#include <cstddef>
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace cyrine {

// Binary

/// Alias for easier typing.
using binary = std::vector<unsigned char>;

/// Hash the entire container by reinterpreting it as bytes.
template <typename container, std::size_t element_size = sizeof(typename container::value_type)>
std::size_t hash_container(const container &ctr)
{
    return std::hash<std::string_view>()(std::string_view((const char *) ctr.data(), ctr.size() * element_size));
}

/// Combine two hashes.
template <typename... values_type>
std::size_t combine_hash(values_type &&...values)
{
    return hash_container(std::vector{ values... });
}

/// Hashing for binary.
struct binary_hash {
    std::size_t operator()(const binary &binary) const
    {
        return hash_container(binary);
    }
};

/// Read binary data from input file stream.
///
/// Binary data is in the form of:
///
/// ```
/// <size><data>
/// ```
///
/// This function reads the size, and then reads the bytes of data.
///
/// @exception std::invalid_argument Thrown when invalid stream is provided.
/// @exception std::runtime_error Thrown when size exceeds file size.
/// @exception std::runtime_error Thrown when stream's bad bit is set.
/// @note The input file stream must be set to binary mode.
binary read_binary(std::istream &in);

/// Write binary data to output file stream.
///
/// Binary data is in the form of:
///
/// ```
/// <size><data>
/// ```
///
/// This function writes the size, and then writes the bytes of data.
///
/// @exception std::invalid_argument Thrown when invalid stream is provided.
/// @exception std::runtime_error Thrown when stream's bad bit is set.
/// @note The output file stream must be set to binary mode.
void write_binary(const binary &data, std::ostream &out);

std::string string_from_binary(const binary &binary);
binary      string_to_binary(const std::string &string);

/// Derive an encryption key from master password and randomization salt.
binary derive_key(std::string_view master_password, const binary &salt);

// Encrypted string

/// Encrypted string.
struct encrypted_string {
    binary data; ///< Encrypted data of string contents.
    binary iv;   ///< Encryption Initial-vector.
    binary salt; ///< Encryption Salt.
    binary tag;  ///< Encryption Authentication tag.

    bool operator==(const encrypted_string &other_string) const
    {
        return data == other_string.data &&
               iv == other_string.iv &&
               salt == other_string.salt &&
               tag == other_string.tag;
    }
};

/// Hashing for encrypted string.
struct encrypted_string_hash {
    std::size_t operator()(const encrypted_string &string) const
    {
        return combine_hash(binary_hash()(string.data), binary_hash()(string.iv), binary_hash()(string.salt), binary_hash()(string.tag));
    }
};

/// Encrypt a string using key.
encrypted_string encrypt_string(const std::string &string, std::string_view master_password);

/// Decrypt a string using key.
std::string decrypt_string(const encrypted_string &string, std::string_view master_password);

/// Read encrypted string from stream.
///
/// Reads the binaries in the following format:
///
/// ```
/// <data><iv><salt><tag>
/// ```
encrypted_string read_encrypted_string(std::istream &in);

/// Write encrypted string to stream.
///
/// Writes the binaries in the following format:
///
/// ```
/// <data><iv><salt><tag>
/// ```
void write_encrypted_string(const encrypted_string &string, std::ostream &out);

// Password

/// Password structure to store passwords.
/// @tparam string_type Used for distinction between raw password (std::string) and encrypted password (estring).
template <typename string_type>
struct password {
    string_type username;      ///< Username (username, email, phone, etc.).
    string_type website;       ///< Website reference.
    string_type password_text; ///< Password string.
    string_type description;   ///< Description for the password (additional info, notes, etc.).

    bool operator==(const password &other) const
    {
        return username == other.username &&
               website == other.website &&
               password_text == other.password_text &&
               description == other.description;
    }
};

using encrypted_password = password<encrypted_string>;
using raw_password       = password<std::string>;

/// Hashing for encrypted password for sets.
struct encrypted_password_hash {
    std::size_t operator()(const encrypted_password &password) const
    {
        return combine_hash(encrypted_string_hash()(password.username), encrypted_string_hash()(password.website));
    }
};

/// Hashing for raw password for sets.
struct raw_password_hash {
    std::size_t operator()(const raw_password &password) const
    {
        return combine_hash(std::hash<std::string>()(password.username), std::hash<std::string>()(password.website));
    }
};

/// Equating for encrypted password for sets.
template <typename string_type>
struct password_equal_to {
    bool operator()(const password<string_type> &a, const password<string_type> &b) const
    {
        return a.username == b.username && a.website == b.website;
    }
};

using encrypted_password_equal_to = password_equal_to<encrypted_string>;
using raw_password_equal_to       = password_equal_to<std::string>;

/// Encrypts all the fields of a password using key.
encrypted_password encrypt_password(const raw_password &password, std::string_view master_password);

/// Decrypts all the fields of a password using key.
raw_password decrypt_password(const encrypted_password &password, std::string_view master_password);

/// Read encrypted password from stream.
///
/// Reads the encrypted strings in the following format:
///
/// ```
/// <username><website><password_text><description>
/// ```
encrypted_password read_encrypted_password(std::istream &in);

/// Write encrypted password from stream.
///
/// Writes the encrypted strings in the following format:
///
/// ```
/// <username><website><password_text><description>
/// ```
void write_encrypted_password(const encrypted_password &password, std::ostream &out);

// Vault

/// Vault is a collection of unique passwords.
///
/// Uniqueness is determined by the username and website fields of the password.
using vault = std::unordered_set<raw_password, raw_password_hash, raw_password_equal_to>;

/// Read the vault from stream (presumably file with .cv extension).
///
/// Reads an encrypted string from the stream, decrypts it, and then decrypts the individual passwords from it.
vault read_vault(std::istream &in, std::string_view master_password);

/// Write the vault to stream (presumably file with .cv extension).
///
/// Encrypts the individual passwords to a string, encrypts it, and then writes it to the stream.
void write_vault(const vault &vault, std::ostream &out, std::string_view master_password);

/// Merges two vaults.
/// @return Tuple of the three following items:
/// - Vault created by combining two vaults while exclusing collisions (symmetric difference).
/// - Vault with passwords in first and also in second (first intersection second, first's contents preserved).
/// - Vault with passwords in second and also in first (second intersection first, second's contents preserved).
std::tuple<vault, vault, vault> merge_vaults(const vault &first, const vault &second);

// Misc.

/// Returns a length-wise score [0,1] for password, used to determine the score of the password's security level.
float password_length_score(std::string_view password_text);

/// Returns a character variety-wise score [0,1] for password, used to determine the score of the password's security level.
float password_variety_score(std::string_view password_text);

/// Returns the score of the password's security level.
float password_score(std::string_view password_text);

/// Converts password security score to string:
/// - [0  ,0.2) = "Very weak".
/// - [0.2,0.4) = "Weak".
/// - [0.4,0.6) = "Moderate".
/// - [0.6,0.8) = "Strong".
/// - [0.8,1  ] = "Very strong".
std::string_view score_to_string(float score);

/// Generates a random password.
std::string random_password(std::size_t length = 16, std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

} // namespace cyrine

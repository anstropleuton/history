/// @file
///
/// @authors   Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// This source file provides implementation for Cyrine.
///
/// This project is licensed under the terms of MIT License.

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <format>
#include <functional>
#include <ios>
#include <iosfwd>
#include <istream>
#include <memory>
#include <ostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

#include "cyrine/cyrine.hpp"

/// AES-256 key derivation length.
const std::size_t aes_256_key_size = 32;

/// AES-256 hash function reiterations during key derivation.
const std::size_t aes_256_hash_iterations = 1'048'576;

/// AES-256 hash salt length.
const std::size_t aes_256_salt_size = 16;

/// AEC-256 AEAD authentication tag length.
const std::size_t aes_256_tag_size = 16;

cyrine::binary cyrine::read_binary(std::istream &in)
{
    if (!in)
    {
        throw std::invalid_argument("Invalid stream provided");
    }

    // Let's not worry about endianness...
    // Todo: worry about endianness

    std::size_t size;
    in.read((char *) &size, sizeof(std::size_t));

    if (!in)
    {
        throw std::runtime_error("Failed to read size from stream");
    }

    std::streampos current_pos = in.tellg();
    in.seekg(0, std::ios::end);
    std::streampos end_pos = in.tellg();
    in.seekg(current_pos);

    if (current_pos == -1 || end_pos == -1)
    {
        throw std::runtime_error("Failed to obtain stream positions");
    }

    std::size_t remaining = end_pos - current_pos;

    if (size > remaining)
    {
        throw std::runtime_error("Declared size exceeds remaining file length");
    }

    // Read data

    binary data(size);
    in.read((char *) (data.data()), size);

    if (!in)
    {
        throw std::runtime_error("Failed to read data from stream");
    }

    return data;
}

void cyrine::write_binary(const binary &data, std::ostream &out)
{
    if (!out)
    {
        throw std::invalid_argument("Invalid stream provided");
    }

    // Let's not worry about endianness...
    // Todo: worry about endianness

    std::size_t size = data.size();
    out.write((char *) &size, sizeof(std::size_t));

    if (!out)
    {
        throw std::runtime_error("Failed to write size to stream");
    }

    out.write((char *) data.data(), data.size());

    if (!out)
    {
        throw std::runtime_error("Failed to write data to stream");
    }
}

std::string cyrine::string_from_binary(const cyrine::binary &binary)
{
    return std::string(binary.begin(), binary.end());
}

cyrine::binary cyrine::string_to_binary(const std::string &string)
{
    return binary(string.begin(), string.end());
}

void check_openssl(bool success, std::string_view message)
{
    if (!success)
    {
        unsigned long err_code = ERR_get_error();
        std::string   err_str  = err_code ? ERR_error_string(err_code, nullptr) : "Unknown error in OpenSSL";
        throw std::runtime_error(std::format("{}: {}", message, err_str));
    }
}

cyrine::binary cyrine::derive_key(std::string_view master_password, const binary &salt)
{
    binary key(aes_256_key_size);

    check_openssl(PKCS5_PBKDF2_HMAC(master_password.data(), master_password.size(), salt.data(), salt.size(), aes_256_hash_iterations, EVP_sha512(), aes_256_key_size, key.data()), "Failed to derive key from password");

    return key;
}

cyrine::encrypted_string cyrine::encrypt_string(const std::string &string, std::string_view master_password)
{
    int iv_len   = EVP_CIPHER_get_iv_length(EVP_aes_256_gcm());
    int data_len = string.size();

    encrypted_string result;

    result.iv   = binary(iv_len);
    result.salt = binary(aes_256_salt_size);
    result.tag  = binary(aes_256_tag_size);

    check_openssl(RAND_bytes(result.salt.data(), result.salt.size()), "Failed to generate random salt");

    binary key = derive_key(master_password, result.salt);

    std::unique_ptr<void, std::function<void(void *)>> key_guard(nullptr, [&key](void *) -> void { OPENSSL_cleanse(key.data(), key.size()); });

    check_openssl(RAND_bytes(result.iv.data(), result.iv.size()), "Failed to generate random IV");

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    check_openssl(ctx != nullptr, "Failed to create cipher context");

    check_openssl(EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr), "Failed to initialize encryption");

    check_openssl(EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, iv_len, nullptr), "Failed to set IV length");

    check_openssl(EVP_EncryptInit_ex(ctx.get(), nullptr, nullptr, key.data(), result.iv.data()), "Failed to initialize key and IV");

    int    len            = 0;
    int    ciphertext_len = 0;
    binary ciphertext(data_len);

    const unsigned char *plaintext = (const unsigned char *) string.data();

    check_openssl(EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &len, plaintext, string.size()), "Failed to encrypt data");

    ciphertext_len += len;

    check_openssl(EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + len, &len), "Failed to finalize encryption");

    ciphertext_len += len;

    check_openssl(EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, result.tag.size(), result.tag.data()), "Failed to get authentication tag");

    result.data = ciphertext | std::views::take(ciphertext_len) | std::ranges::to<binary>();
    return result;
}

std::string cyrine::decrypt_string(const encrypted_string &string, std::string_view master_password)
{
    if (string.iv.size() != EVP_CIPHER_get_iv_length(EVP_aes_256_gcm()))
        throw std::runtime_error("Invalid IV size");
    if (string.salt.size() != aes_256_salt_size)
        throw std::runtime_error("Invalid salt size");
    if (string.tag.size() != aes_256_tag_size)
        throw std::runtime_error("Invalid tag size");

    binary key = derive_key(master_password, string.salt);

    std::unique_ptr<void, std::function<void(void *)>> key_guard(nullptr, [&key](void *) -> void { OPENSSL_cleanse(key.data(), key.size()); });

    std::string result;

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    check_openssl(ctx != nullptr, "Failed to create cipher context");

    check_openssl(EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr), "Failed to initialize decryption");

    check_openssl(EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, string.iv.size(), nullptr), "Failed to set IV length");

    check_openssl(EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, key.data(), string.iv.data()), "Failed to initialize key and IV");

    check_openssl(EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, string.tag.size(), (void *) string.tag.data()), "Failed to set authentication tag");

    int    len           = 0;
    int    plaintext_len = 0;
    binary plaintext(string.data.size());

    check_openssl(EVP_DecryptUpdate(ctx.get(), plaintext.data(), &len, string.data.data(), string.data.size()), "Failed to decrypt data");

    plaintext_len += len;

    check_openssl(EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + len, &len), "Failed to finalize decryption");

    plaintext_len += len;

    result = plaintext | std::views::take(plaintext_len) | std::ranges::to<std::string>();
    return result;
}

cyrine::encrypted_string cyrine::read_encrypted_string(std::istream &in)
{
    return {
        .data = read_binary(in),
        .iv   = read_binary(in),
        .salt = read_binary(in),
        .tag  = read_binary(in)
    };
}

void cyrine::write_encrypted_string(const encrypted_string &string, std::ostream &out)
{
    write_binary(string.data, out);
    write_binary(string.iv, out);
    write_binary(string.salt, out);
    write_binary(string.tag, out);
}

cyrine::encrypted_password cyrine::encrypt_password(const cyrine::raw_password &password, std::string_view master_password)
{
    return {
        .username      = encrypt_string(password.username, master_password),
        .website       = encrypt_string(password.website, master_password),
        .password_text = encrypt_string(password.password_text, master_password),
        .description   = encrypt_string(password.description, master_password)
    };
}

cyrine::raw_password cyrine::decrypt_password(const cyrine::encrypted_password &password, std::string_view master_password)
{
    return {
        .username      = decrypt_string(password.username, master_password),
        .website       = decrypt_string(password.website, master_password),
        .password_text = decrypt_string(password.password_text, master_password),
        .description   = decrypt_string(password.description, master_password)
    };
}

cyrine::encrypted_password cyrine::read_encrypted_password(std::istream &in)
{
    return {
        .username      = read_encrypted_string(in),
        .website       = read_encrypted_string(in),
        .password_text = read_encrypted_string(in),
        .description   = read_encrypted_string(in)
    };
}

void cyrine::write_encrypted_password(const encrypted_password &password, std::ostream &out)
{
    write_encrypted_string(password.username, out);
    write_encrypted_string(password.website, out);
    write_encrypted_string(password.password_text, out);
    write_encrypted_string(password.description, out);
}

std::tuple<cyrine::vault, cyrine::vault, cyrine::vault> cyrine::merge_vaults(const vault &first, const vault &second)
{
    vault symmetric_diff;
    vault intersection_first;
    vault intersection_second;

    for (const auto &item : first)
    {
        if (second.contains(item))
        {
            intersection_first.insert(item);
        }
        else
        {
            symmetric_diff.insert(item);
        }
    }

    for (const auto &item : second)
    {
        if (first.contains(item))
        {
            intersection_second.insert(item);
        }
        else
        {
            symmetric_diff.insert(item);
        }
    }

    return { symmetric_diff, intersection_first, intersection_second };
}

float cyrine::password_length_score(std::string_view password_text)
{
    return std::clamp(password_text.size() / 16.0f, 0.0f, 1.0f);
}

float cyrine::password_variety_score(std::string_view password_text)
{
    std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string digits    = "0123456789";

    bool contains_lowercase = true;
    bool contains_uppercase = true;
    bool contains_digits    = true;

    for (char c : password_text)
    {
        contains_lowercase &= lowercase.contains(c);
        contains_uppercase &= uppercase.contains(c);
        contains_digits &= digits.contains(c);
    }

    return (contains_lowercase +
            contains_uppercase +
            contains_digits) /
           3.0f;
}

float cyrine::password_score(std::string_view password_text)
{
    float length_score  = password_length_score(password_text);
    float variety_score = password_variety_score(password_text);

    return length_score * variety_score;
}

std::string_view cyrine::score_to_string(float score)
{
    if (score < 0.2f)
        return "Very weak";
    else if (score < 0.4f)
        return "Weak";
    else if (score < 0.6f)
        return "Moderate";
    else if (score < 0.8f)
        return "Strong";
    else
        return "Very strong";
}

#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace cyrine {
    using binary = std::vector<unsigned char>;

    template <typename container, std::size_t element_size = sizeof(typename container::value_type)>
    std::size_t hash_container(const container &ctr) {
        return std::hash<std::string_view>()(std::string_view((const char *)ctr.data(), ctr.size() * element_size));
    }

    template <typename... values_type>
    std::size_t combine_hash(values_type &&...values) {
        return hash_container(std::vector{values...});
    }

    struct binary_hash {
        std::size_t operator()(const binary &binary) const {
            return hash_container(binary);
        }
    };

    binary read_binary(std::istream &in);

    void write_binary(const binary &data, std::ostream &out);

    std::string string_from_binary(const binary &binary);
    binary      string_to_binary(const std::string &string);

    binary derive_key(std::string_view master_password, const binary &salt);

    struct encrypted_string {
        binary data;
        binary iv;
        binary salt;
        binary tag;

        bool operator==(const encrypted_string &other_string) const {
            return data == other_string.data &&
                   iv == other_string.iv &&
                   salt == other_string.salt &&
                   tag == other_string.tag;
        }
    };

    struct encrypted_string_hash {
        std::size_t operator()(const encrypted_string &string) const {
            return combine_hash(binary_hash()(string.data), binary_hash()(string.iv), binary_hash()(string.salt), binary_hash()(string.tag));
        }
    };

    encrypted_string encrypt_string(const std::string &string, std::string_view master_password);

    std::string decrypt_string(const encrypted_string &string, std::string_view master_password);

    encrypted_string read_encrypted_string(std::istream &in);

    void write_encrypted_string(const encrypted_string &string, std::ostream &out);

    template <typename string_type>
    struct password {
        string_type username;
        string_type website;
        string_type password_text;
        string_type description;

        bool operator==(const password &other) const {
            return username == other.username &&
                   website == other.website &&
                   password_text == other.password_text &&
                   description == other.description;
        }
    };

    using encrypted_password = password<encrypted_string>;
    using raw_password       = password<std::string>;

    struct encrypted_password_hash {
        std::size_t operator()(const encrypted_password &password) const {
            return combine_hash(encrypted_string_hash()(password.username), encrypted_string_hash()(password.website));
        }
    };

    struct raw_password_hash {
        std::size_t operator()(const raw_password &password) const {
            return combine_hash(std::hash<std::string>()(password.username), std::hash<std::string>()(password.website));
        }
    };

    template <typename string_type>
    struct password_equal_to {
        bool operator()(const password<string_type> &a, const password<string_type> &b) const {
            return a.username == b.username && a.website == b.website;
        }
    };

    using encrypted_password_equal_to = password_equal_to<encrypted_string>;
    using raw_password_equal_to       = password_equal_to<std::string>;

    encrypted_password encrypt_password(const raw_password &password, std::string_view master_password);

    raw_password decrypt_password(const encrypted_password &password, std::string_view master_password);

    encrypted_password read_encrypted_password(std::istream &in);

    void write_encrypted_password(const encrypted_password &password, std::ostream &out);

    using vault = std::unordered_set<raw_password, raw_password_hash, raw_password_equal_to>;

    vault read_vault(std::istream &in, std::string_view master_password);

    void write_vault(const vault &vault, std::ostream &out, std::string_view master_password);

    std::tuple<vault, vault, vault> merge_vaults(const vault &first, const vault &second);

    float password_length_score(std::string_view password_text);

    float password_variety_score(std::string_view password_text);

    float password_score(std::string_view password_text);

    std::string_view score_to_string(float score);

    std::string random_password(std::size_t length = 16, std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}

#include "common.h"

const uint32_t hash2u32(const std::string &str)
{
    uint32_t hash = 0x811c9dc5;
    static const uint32_t prime = 0x1000193;

    for (auto c : str)
    {
        uint8_t value = c;
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
} 

const uint64_t hash2u64(const std::string &str)
{
    uint64_t hash = 0xcbf29ce484222325;
    static const uint64_t prime = 0x100000001b3;

    for (auto c : str)
    {
        uint8_t value = c;
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}

constexpr uint32_t hash2u32_const(const char *const str, const uint32_t value)
{
    return (str[0] == '\0') ? value : hash2u32_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
}
constexpr uint64_t hash2u64_const(const char *const str, const uint64_t value)
{
    return (str[0] == '\0') ? value : hash2u64_const(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
}
/* Your input will be strings, in fact, all the strings that are defined
in the system’s dictionary. This is located in /usr/share/dict/words */

#include <cstdint>
#include <string>
#include <boost/math/distributions/chi_squared.hpp>

uint16_t x; // x has exactly 16 bits, and is unsigned

// String length (modulo 2^16)
uint16_t hash1(const std::string &s)
{
    return s.size() % 65536;
}

// First character
uint16_t hash2(const std::string &s)
{
    return s.at(0);
}

// additive checksum (add all characters together), modulo 2^16
uint16_t hash3(const std::string &s)
{
    uint16_t sum = 0;
    // multiplier
    auto m = 1;
    for (auto c : s)
    {
        sum += m * c;
        m *= 256;
    }
    return sum;
}

/* Remainder (use a modulo of 65413, this is the first prime that is
smaller than the table size). Remember that you cannot just add up
all the characters and then take the mod of the result; you have to
thread the modulo through the loop that computes the sum. */
uint16_t hash4(const std::string &s)
{
    uint16_t sum = 0;
    for (int i = 0; i < s.size(); ++i)
    {
        sum = (sum * 256 + s[i]) % 65413;
    }
    return sum;
}

/* Multiplicative (using the scheme described in class/in the lecture
notes). Again, remember that you can’t just use the final sum; you have
to incorporate the multiplicative calculation into hashing loop.*/
uint16_t hash5(const std::string &s)
{
}

int main()
{
    return 0;
}
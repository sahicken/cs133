/* Your input will be strings, in fact, all the strings that are defined
in the system’s dictionary. This is located in /usr/share/dict/words */

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/math/distributions/chi_squared.hpp>

// String length (modulo 2^16)
uint16_t hash1(const std::string &s) { return s.size() % 65536; }

// First character
uint16_t hash2(const std::string &s) { return s.at(0); }

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
        sum = (sum * 256 + s[i]) % 65413;
    return sum;
}

/* Multiplicative (using the scheme described in class/in the lecture
notes). Again, remember that you can’t just use the final sum; you have
to incorporate the multiplicative calculation into hashing loop.*/
uint16_t hash5(const std::string &s)
{
    float k = 0;
    float A = (sqrt(5)-1)/2;
    for (int i = 0; i<s.size(); ++i)
        k=std::fmod((k*256+(s[i]))*A,1);
    return floor(k*65413);
}



int main()
{
    // Create an input stream object to open the file
    std::ifstream file("/usr/share/dict/words");

    // Check if the file is opened successfully
    if (file.is_open())
    {
        // Create a vector of strings to store the words
        std::vector<std::string> words;

        // Create a string variable to store each word
        std::string word;

        // Read the file word by word until the end of the file
        while (file >> word)
        {
            // Add the word to the vector
            words.push_back(word);
        }

        // Close the file
        file.close();

        // Print the size and the contents of the vector
        std::cout << "The vector has " << words.size() << " words.\n";
        std::cout << "The words are:\n";
        for (const auto& w : words)
        {
            std::cout << w << "\n";
        }
    }
    else
    {
        // Print an error message if the file cannot be opened
        std::cerr << "Unable to open the file.\n";
    }

    return 0;
}

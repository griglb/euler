// Each character on a computer is assigned a unique code and the preferred standard
// is ASCII (American Standard Code for Information Interchange).
// For example, uppercase A = 65, asterisk (*) = 42, and lowercase k = 107.
//
// A modern encryption method is to take a text file, convert the bytes to ASCII,
// then XOR each byte with a given value, taken from a secret key. The advantage
// with the XOR function is that using the same encryption key on the cipher text,
// restores the plain text; for example, 65 XOR 42 = 107, then 107 XOR 42 = 65.
//
// For unbreakable encryption, the key is the same length as the plain text message,
// and the key is made up of random bytes. The user would keep the encrypted message
// and the encryption key in different locations, and without both "halves", it is
// impossible to decrypt the message.
//
// Unfortunately, this method is impractical for most users, so the modified method
// is to use a password as a key. If the password is shorter than the message, which
// is likely, the key is repeated cyclically throughout the message. The balance for
// this method is using a sufficiently long password key for security, but short
// enough to be memorable.
//
// Your task has been made easy, as the encryption key consists of three lower case
// characters. Using p059_cipher.txt (right click and 'Save Link/Target As...'), a
// file containing the encrypted ASCII codes, and the knowledge that the plain text
// must contain common English words, decrypt the message and find the sum of the
// ASCII values in the original text.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<uint8_t> get_ciphertext() {
    std::vector<uint8_t> ret;

    // The file has no carriage returns, so read entire file into string.
#ifdef _CONSOLE
    std::ifstream infile("./p059_cipher.txt");
#else
    std::ifstream infile("./ProjectEuler0059/p059_cipher.txt");
#endif

    int16_t token;

    while (!infile.eof()) {
        char comma;
        infile >> token >> comma;
        
        ret.push_back(token);
        // if (ret.size() > 75)
        //     break;
    }

    return ret;
}


void test_ciphers() {
    const auto cipher = get_ciphertext();
    auto cipher_len = cipher.size();

    for (uint8_t key1 = 'a'; key1 <= 'z'; ++key1) {
        for (uint8_t key2 = 'a'; key2 <= 'z'; ++key2) {
            for (uint8_t key3 = 'a'; key3 <= 'z'; ++key3) {
                uint8_t k[] = { key1, key2, key3 };
                auto plain = cipher;
                bool good = true;
                for (uint16_t i = 0; i < cipher_len; ++i) {
                    plain[i] ^= k[i % 3];
                    if ((plain[i] < 32) || (plain[i] > 126))
                        good = false;
                }
               if (good) {
                    std::cout << "(" << key1 << ", " << key2 << ", " << key3 << ") = ";
                    for (const auto &ch : plain)
                        std::cout << ch;
                    std::cout << std::endl;
               }
            }
        }
    }

    
    uint8_t k[] = { 'e', 'x', 'p' };
    auto plain = cipher;
    for (uint16_t i = 0; i < cipher_len; ++i)
        plain[i] ^= k[i % 3];

    std::cout << "(" << static_cast<int16_t>(k[0]) << ", " << static_cast<int16_t>(k[1]) << ", " << static_cast<int16_t>(k[2]) << ") = ";
    for (const auto &ch : plain)
        std::cout << ch;
    std::cout << std::endl;
    uint64_t sum{0};
    for (const auto &ch : plain)
        sum += ch;
    std::cout << "sum = " << sum << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";

    {
        auto cipher = get_ciphertext();
        for (const auto &ch : cipher)
            std::cout << static_cast<int16_t>(ch) << " ";
        std::cout << std::endl;
    }
    test_ciphers();

    std::cout << "19 XOR 108 = " << (19 ^ 108) << std::endl;
}

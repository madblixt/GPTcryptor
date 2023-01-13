#include <iostream>
#include <openssl/rand.h>

constexpr int KEY_SIZE = 32;
constexpr int IV_SIZE = 16;

int main() {
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];

    if (RAND_bytes(key, KEY_SIZE) != 1) {
        std::cout << "Error generating key" << std::endl;
        return -1;
    }

    if (RAND_bytes(iv, IV_SIZE) != 1) {
        std::cout << "Error generating IV" << std::endl;
        return -1;
    }
    
    std::cout << "Key: ";
    for (int i = 0; i < KEY_SIZE; i++) {
        std::cout << std::hex << (int)key[i];
    }
    std::cout << std::endl;

    std::cout << "IV: ";
    for (int i = 0; i < IV_SIZE; i++) {
        std::cout << std::hex << (int)iv[i];
    }
    std::cout << std::endl;

    return 0;
}

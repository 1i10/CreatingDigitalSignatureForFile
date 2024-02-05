#pragma once

#include <vector>

/**
* @class KeyManager
* @brief Manages cryptographic keys for digital signatures.
*
* The KeyManager class provides functionality to generate, retrieve, and set cryptographic keys
* for use in digital signatures. It uses OpenSSL for secure random key generation.
*/
class KeyManager
{
private:
    const size_t KEY_SIZE = 32; ///< The size of the cryptographic keys
    std::vector<unsigned char> privateKey; ///< The private cryptographic key
    std::vector<unsigned char> publicKey;  ///< The public cryptographic key

public:
    /**
    * @brief Default constructor for KeyManager.
    *
    * Initializes the private and public keys with the specified size.
    */
    KeyManager() : privateKey(KEY_SIZE), publicKey(KEY_SIZE) {}

    const std::vector<unsigned char>& getPrivateKey() const;
    const std::vector<unsigned char>& getPublicKey() const;
    size_t getKeySize() const;

    void setPrivateKey(const std::vector<unsigned char>& key);
    void setPublicKey(const std::vector<unsigned char>& key);

    void generateKeys();
    bool isKeyEmpty(const std::vector<unsigned char>& key) const;
};


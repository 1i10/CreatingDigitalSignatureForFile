#include "KeyManager.h"
#include <openssl/rand.h>

/**
* @brief Get the private cryptographic key.
* @return A constant reference to the private key vector.
*/
const std::vector<unsigned char>& KeyManager::getPrivateKey() const
{
    return this->privateKey;
}

/**
* @brief Get the public cryptographic key.
* @return A constant reference to the public key vector.
*/
const std::vector<unsigned char>& KeyManager::getPublicKey() const
{
    return this->publicKey;
}

/**
* @brief Get the size of the cryptographic keys.
* @return The size of the keys.
*/
size_t KeyManager::getKeySize() const
{
    return KEY_SIZE;
}

/**
* @brief Set the private cryptographic key.
* @param key The new private key vector.
*/
void KeyManager::setPrivateKey(const std::vector<unsigned char>& key)
{
    this->privateKey = key;
}

/**
* @brief Set the public cryptographic key.
* @param key The new public key vector.
*/
void KeyManager::setPublicKey(const std::vector<unsigned char>& key)
{
    this->publicKey = key;
}

/**
* @brief Generate new cryptographic keys using OpenSSL.
*/
void KeyManager::generateKeys()
{
    RAND_bytes(this->privateKey.data(), static_cast<int>(KEY_SIZE));
    RAND_bytes(this->publicKey.data(), static_cast<int>(KEY_SIZE));
}

/**
* @brief Check if the key is empty.
*
* The method checks whether the given key is empty, considering it empty if all bytes are zero.
*
* @param key The key to be checked.
* @return True if the key is empty (all bytes are zero), false otherwise.
*/
bool KeyManager::isKeyEmpty(const std::vector<unsigned char>& key) const
{
    for (unsigned char byte : key)
    {
        if (byte != 0)
        {
            return false;
        }
    }
    return true;
}

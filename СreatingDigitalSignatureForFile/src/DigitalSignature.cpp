#include "DigitalSignature.h"
#include <openssl/evp.h>
#include <boost/filesystem/fstream.hpp>

/**
* @brief Get the stored digital signature.
*
* @return A constant reference to the stored digital signature data.
*/
const std::vector<unsigned char>& DigitalSignature::getSignature() const
{
    return this->signatureData;
}

/**
* @brief Sign a file using XOR and both private and public keys.
*
* @param file The File object to sign.
*/
void DigitalSignature::sign(const File& file)
{
    this->keys.generateKeys();

    std::vector<unsigned char> privateKey = this->keys.getPrivateKey();
    std::vector<unsigned char> publicKey = this->keys.getPublicKey();

    const FileMetadata& metadata = file.getMetadata();
    const std::vector<char>& data = file.getContent();

    std::vector<unsigned char> combinedHash = hashData(metadata, data);

    // Sign the file using XOR and both keys
    for (std::size_t i = 0; i < combinedHash.size(); ++i)
    {
        this->signatureData.push_back((combinedHash[i] ^ privateKey[i % privateKey.size()]) ^ publicKey[i % publicKey.size()]);
    }
}

/**
* @brief Verify the digital signature of a file using both private and public keys.
*
* @param file The File object to verify.
* @return True if the signature is valid, false otherwise.
*/
bool DigitalSignature::verify(const File& file)
{
    std::vector<unsigned char> privateKey = this->keys.getPrivateKey();
    std::vector<unsigned char> publicKey = this->keys.getPublicKey();

    if (this->signatureData.empty() || this->keys.isKeyEmpty(privateKey) || this->keys.isKeyEmpty(publicKey))
    {
        return false; // No signature and/or keys
    }

    const FileMetadata& metadata = file.getMetadata();
    const std::vector<char>& data = file.getContent();

    std::vector<unsigned char> combinedHash = hashData(metadata, data);

    // Verify the signature using both keys and XOR
    for (std::size_t i = 0; i < combinedHash.size(); ++i)
    {
        if (((combinedHash[i] ^ publicKey[i % publicKey.size()]) ^ privateKey[i % privateKey.size()]) != this->signatureData[i])
        {
            return false; // If at least one byte does not match, the signature is invalid
        }
    }

    return true;
}

/**
* @brief Delete the stored signature data and remove the file.
*
* @param filePath The path to the file to be deleted.
*/
void DigitalSignature::deleteSignatureInFile(const std::string& filePath)
{
    this->signatureData.clear();

    boost::filesystem::remove(filePath);
}

/**
* @brief Read private key, public key, and digital signature from a file.
*
* @param filePath The path to the file containing keys and signature.
*/
void DigitalSignature::readKeyAndSignatureFromFile(const std::string& filePath)
{
    boost::filesystem::ifstream file(filePath, std::ios::binary);
    if (file)
    {
        // Read the private key
        file >> std::noskipws;

        size_t keySize = this->keys.getKeySize();
        std::vector<unsigned char> privateKey(keySize);
        file.read(reinterpret_cast<char*>(privateKey.data()), keySize);
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip newline character
        this->keys.setPrivateKey(privateKey);

        // Read the public key
        std::vector<unsigned char> publicKey(keySize);
        file.read(reinterpret_cast<char*>(publicKey.data()), keySize);
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip newline character
        this->keys.setPublicKey(publicKey);

        // Read the digital signature from the file into the signatureData vector
        std::copy(std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>(), std::back_inserter(this->signatureData));
        this->signatureData.shrink_to_fit(); // Reduce capacity to size
        file.close();
    }
}

/**
* @brief Write private key, public key, and digital signature to a file.
*
* @param filePath The path to the file to write keys and signature.
* @throw std::runtime_error if file writing fails.
*/
void DigitalSignature::writeKeyAndSignatureToFile(const std::string& filePath) const
{
    boost::filesystem::ofstream file(filePath, std::ios::binary);
    if (file)
    {
        size_t keySize = this->keys.getKeySize();
        std::vector<unsigned char> privateKey = this->keys.getPrivateKey();
        std::vector<unsigned char> publicKey = this->keys.getPublicKey();

        // Write the private key
        file.write(reinterpret_cast<const char*>(privateKey.data()), keySize);
        file << '\n'; // Write newline character

        // Write the public key
        file.write(reinterpret_cast<const char*>(publicKey.data()), keySize);
        file << '\n'; // Write newline character

        // Write the digital signature to the file
        std::copy(this->signatureData.begin(), this->signatureData.end(), std::ostream_iterator<unsigned char>(file));
        file.close();
    }
    else
    {
        throw std::runtime_error("Failed to open file for writing");
    }
}

/**
* @brief Hash the file metadata and content data using SHA-256.
*
* @param metadata The FileMetadata object containing file information.
* @param data The content data of the file.
* @return The SHA-256 hash of the combined file metadata and content.
* @throw std::runtime_error if hashing fails.
*/
std::vector<unsigned char> DigitalSignature::hashData(const FileMetadata& metadata, const std::vector<char>& data)
{
    std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> hashCtx(EVP_MD_CTX_new(), &EVP_MD_CTX_free);

    if (!hashCtx)
    {
        throw std::runtime_error("Failed to create hash context");
    }

    if (EVP_DigestInit(hashCtx.get(), EVP_sha256()) != 1)
    {
        throw std::runtime_error("Failed to initialize hash context");
    }

    if (EVP_DigestUpdate(hashCtx.get(), metadata.getFileName().c_str(), metadata.getFileName().size()) != 1)
    {
        throw std::runtime_error("Failed to update hash with file name");
    }

    std::time_t creationTime = metadata.getCreationTime();
    if (EVP_DigestUpdate(hashCtx.get(), &creationTime, sizeof(creationTime)) != 1)
    {
        throw std::runtime_error("Failed to update hash with creation time");
    }

    std::size_t fileSize = metadata.getFileSize();
    if (EVP_DigestUpdate(hashCtx.get(), &fileSize, sizeof(fileSize)) != 1)
    {
        throw std::runtime_error("Failed to update hash with file size");
    }

    if (EVP_DigestUpdate(hashCtx.get(), metadata.getFileExtension().c_str(), metadata.getFileExtension().size()) != 1)
    {
        throw std::runtime_error("Failed to update hash with file extension");
    }

    if (EVP_DigestUpdate(hashCtx.get(), data.data(), data.size()) != 1)
    {
        throw std::runtime_error("Failed to update hash with file content");
    }

    std::vector<unsigned char> combinedHash(EVP_MD_size(EVP_sha256()));
    if (EVP_DigestFinal(hashCtx.get(), combinedHash.data(), nullptr) != 1)
    {
        throw std::runtime_error("Failed to finalize hash");
    }

    return combinedHash;
}


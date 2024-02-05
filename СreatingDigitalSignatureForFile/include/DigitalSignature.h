#pragma once

#include <vector>
#include "KeyManager.h"
#include "File.h"

/**
* @brief Class for handling digital signatures.
*
* This class provides functionality for creating, verifying, and managing digital signatures.
*/
class DigitalSignature
{
private:
    KeyManager keys; ///< KeyManager instance for key generation
    std::vector<unsigned char> signatureData; ///< Storage for the digital signature
public:
    /**
    * @brief Default constructor for DigitalSignature class.
    *
    * Initializes the DigitalSignature object with default KeyManager and empty signature data.
    */
    DigitalSignature() : keys(), signatureData() {}

    const std::vector<unsigned char>& getSignature() const;

    void sign(const File& file);
    bool verify(const File& file);
    void deleteSignatureInFile(const std::string& filePath);

    void readKeyAndSignatureFromFile(const std::string& filePath);
    void writeKeyAndSignatureToFile(const std::string& filePath) const;

private:
    std::vector<unsigned char> hashData(const FileMetadata& metadata, const std::vector<char>& data);
};


#pragma once

#include "FileMetadata.h"
#include <boost/filesystem.hpp>
#include <vector>
#include <stdexcept>

/**
* @class File
* @brief Class for representing a file with metadata and content.
*
* This class includes file metadata (FileMetadata) and the file content as a vector of bytes.
*/
class File 
{
private:
    FileMetadata metadata; ///< The metadata of the file
    std::vector<char> fileContent; ///< The content of the file
public:
    File(const boost::filesystem::path& filePath);

    const FileMetadata& getMetadata() const;
    const std::vector<char>& getContent() const;
};


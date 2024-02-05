#include "File.h"
#include <boost/filesystem/fstream.hpp>

/**
* @brief Constructor for the File class.
*
* @param filePath The path to the file.
* @throw std::invalid_argument if the file does not exist,
*        std::runtime_error in case of an error opening the file.
*/
File::File(const boost::filesystem::path& filePath) : metadata(filePath)
{
    boost::filesystem::ifstream fileStream(filePath, std::ios::binary);
    if (!fileStream.is_open())
    {
        throw std::runtime_error("Error opening file");
    }

    this->fileContent.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
}

/**
* @brief Get the metadata of the file.
*
* @return A constant reference to the FileMetadata object.
*/
const FileMetadata& File::getMetadata() const
{
    return this->metadata;
}

/**
* @brief Get the content of the file.
*
* @return A constant reference to the vector of bytes representing the file content.
*/
const std::vector<char>& File::getContent() const
{
    return this->fileContent;
}


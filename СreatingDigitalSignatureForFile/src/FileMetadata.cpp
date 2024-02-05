#include "FileMetadata.h"
#include <stdexcept>

/**
* @brief Constructor for the FileMetadata class.
*
* @param filePath The path to the file.
* @throw std::invalid_argument if the file does not exist.
*/
FileMetadata::FileMetadata(const boost::filesystem::path& filePath)
{
    if (!boost::filesystem::exists(filePath))
    {
        throw std::invalid_argument("File does not exist");
    }

    this->fileName = filePath.filename().string();
    this->creationTime = boost::filesystem::last_write_time(filePath);
    this->fileSize = boost::filesystem::file_size(filePath);
    this->fileExtension = filePath.extension().string();
}

/**
* @brief Get the name of the file.
*
* @return The name of the file as a string.
*/
std::string FileMetadata::getFileName() const
{
    return this->fileName;
}

/**
* @brief Get the creation time of the file.
*
* @return The creation time of the file in std::time_t format.
*/
std::time_t FileMetadata::getCreationTime() const
{
    return this->creationTime;
}

/**
* @brief Get the size of the file.
*
* @return The size of the file in bytes.
*/
std::size_t FileMetadata::getFileSize() const
{
    return this->fileSize;
}

/**
* @brief Get the extension of the file.
*
* @return The extension of the file as a string.
*/
const std::string& FileMetadata::getFileExtension() const
{
    return this->fileExtension;
}
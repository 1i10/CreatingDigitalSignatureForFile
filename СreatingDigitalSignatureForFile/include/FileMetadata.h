#pragma once

#include <boost/filesystem.hpp>
#include <ctime>
#include <string>

/**
* @class FileMetadata
* @brief Class for representing file metadata.
*
* This class contains information about a file, such as the file name, creation time,
* file size, and file extension.
*/
class FileMetadata 
{
private:
    std::string fileName; ///< The name of the file
    std::time_t creationTime; ///< The creation time of the file
    std::size_t fileSize; ///< The size of the file in bytes
    std::string fileExtension; ///< The extension of the file
public:
    FileMetadata(const boost::filesystem::path& filePath);

    std::string getFileName() const;
    std::time_t getCreationTime() const;
    std::size_t getFileSize() const;
    const std::string& getFileExtension() const;
};




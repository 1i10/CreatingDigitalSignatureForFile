#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include "File.h"
#include "DigitalSignature.h"

std::string vecToString(const std::vector<unsigned char>& vec);

/**
* @test searchingForCollisionsWhileCreatingSignatures
* This test checks the uniqueness of digital signatures for 1024 documents.
* It creates a digital signature for each document and checks for collisions.
* The test passes if the number of collisions is 0.
*/
BOOST_AUTO_TEST_CASE(searchingForCollisionsWhileCreatingSignatures)
{
    std::unordered_map<std::string, int> map;
    int numCollisions = 0;

    // Function to add a key (signature) to the map
    auto addKey = [&map, &numCollisions](const auto& key) 
        {
            auto pair = map.insert({key, 1});
            if (!pair.second) // If the key already exists in the map
            {
                pair.first->second++; // Increment the counter
                numCollisions++;
            }
        };

    // Find the directory with the source documents
    boost::filesystem::path p(".\\testData");
    for (auto& entry : boost::filesystem::directory_iterator(p)) 
    {
        if (boost::filesystem::is_regular_file(entry))// Find the directory with the source documents
        {
            //BOOST_TEST_MESSAGE("Current path is " << entry.path());
            File fileToSign(entry.path());

            DigitalSignature digitalSignature;
            digitalSignature.sign(fileToSign);

            addKey(vecToString(digitalSignature.getSignature()));
        }
    }

    BOOST_TEST(numCollisions == 0);
}

std::string vecToString(const std::vector<unsigned char>& vec)
{
    return std::string(vec.begin(), vec.end());
}
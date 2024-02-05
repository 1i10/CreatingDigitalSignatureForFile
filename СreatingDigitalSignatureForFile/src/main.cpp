#include <iostream>
#include "File.h"
#include "DigitalSignature.h"

int main() 
{
    try 
    {
        std::string filePath;
        std::cout << "Enter the path to the file: ";
        std::getline(std::cin, filePath);

        File fileToSign(filePath);

        DigitalSignature digitalSignature;

        const std::string fileWithSignature = filePath + ".sig";
        digitalSignature.readKeyAndSignatureFromFile(fileWithSignature);

        // Checking that the signature exists and matches the content
        if (digitalSignature.verify(fileToSign))
        {
            std::cout << "Digital signature exists for the file.\n";
            std::cout << "-----------------------------------------\n";
            std::cout << "Do you want to remove the signature? (Y/N): ";
            char choice;
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') 
            {
                digitalSignature.deleteSignatureInFile(fileWithSignature);
            }
        }
        else 
        {
            std::cout << "No digital signature found for the file.\n";
            std::cout << "-----------------------------------------\n";
            std::cout << "Do you want to add a signature? (Y/N): ";
            char choice;
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') 
            {
                digitalSignature.sign(fileToSign);

                digitalSignature.writeKeyAndSignatureToFile(fileWithSignature);
            }
        }

        std::cout << "Operation completed successfully.\n";

    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

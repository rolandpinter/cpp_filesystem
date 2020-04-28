#ifndef functions_hpp
#define functions_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <numeric>   //for std::accumulate
namespace fs = std::__fs::filesystem;

int findIndexOfElementInVector(std::vector<unsigned long> uniqueFileSizes, unsigned long sizeToFind);

class explorer
{
private:
    std::vector<fs::path> m_fileNames;
    std::vector<unsigned long> m_fileSizes;   // dimension = Bytes
    
    std::vector<fs::path> m_folderNames;
    std::vector<unsigned long> m_folderSizes; // dimension = Bytes (size of folder:= sum of size of FILES within the folder, EXCLUDING folders)
    std::vector<unsigned> m_numberOfThingsInAFolder;
    
public:
    std::string m_givenPath;
    
    // Default constructor
    explorer()
    : m_givenPath{ "" }
    {};
    
    // Constructor based on argument list
    explorer(std::string givenPath)
    {
        m_givenPath = givenPath;
        
        // Need to add manually givenPath to folderNames
        // because recursive iteration will start FROM that folder
        // and not WITH that folder.
        m_folderNames.push_back(fs::path(givenPath));
        m_folderSizes.push_back(0);
        m_numberOfThingsInAFolder.push_back(0);
    };
    
    
    // Class member functions
    void setPath(std::string givenPath); //Manually set the path of the class which should be examined.
    void iterateThroughGivenPath(void); //Iterate the path and collect information.
    void biggestFiles(const int n); //Determine the path of the n biggest files.
    void biggestFolders(const int n); // Determine the path of the n biggest folders.
    void avgFileSize(void); //Determine the average file size.
    void avgFolderSize(void); //Determine the average folder size.
    void medianFileSize(void); //Determine median file size.
    void medianFolderSize(void); //Determine median folder size.
    void distributionFileSizes(void); //Determine the distribution of file sizes.
    
    
    
//-------------------------- Functions just for development phase, sanity checks. --------------------------//
    void printFileInfo(void)
    {
        for (int i=0; i<m_fileNames.size(); ++i)
        {
            std::cout << m_fileSizes[i] << "\t" << m_fileNames[i] <<  "\n \t" << m_fileNames[i].parent_path() <<std::endl;
        }
    }
    
    void printFolderInfo(void)
    {
        for(int i=0;i<m_folderNames.size();++i)
        {
            std::cout << m_folderNames[i] << "\n" << "# of elements: " << m_numberOfThingsInAFolder[i] << "\n" << "size: " << m_folderSizes[i] << std::endl;
        }
    }

   
    
};

#endif /* functions_hpp */

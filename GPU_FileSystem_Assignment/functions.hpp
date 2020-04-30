#ifndef functions_hpp
#define functions_hpp

#include <stdio.h>    // for basic IO operations
#include <iostream>   // IO stream
#include <vector>     // for std::vector iterable container
#include <fstream>    // needed for filesystem
#include <string>     // for strings, of course
#include <numeric>    // for std::accumulate
#include <algorithm>
namespace fs = std::__fs::filesystem;

int findIndexOfElementInVector(std::vector<unsigned long> uniqueFileSizes, unsigned long sizeToFind);
std::string askForPathToExplore();
int askForCommand();

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
        
        /// Need to add manually givenPath to folderNames,
        /// because recursive iteration will start FROM that folder,
        /// and not WITH that folder.
        m_folderNames.push_back(fs::path(givenPath));
        m_folderSizes.push_back(0);
        m_numberOfThingsInAFolder.push_back(0);
    };
    
    /// Class member functions
    void setPath(std::string givenPath); // Manually set the path of the class which should be examined. (Not used, but works fine.)
    void iterateThroughGivenPath(void);  // Iterate the path and collect information.
    void biggestFiles(const int n);      // Determine the path of the n biggest files.
    void biggestFolders(const int n);    // Determine the path of the n biggest folders.
    void avgFileSize(void);              // Determine the average file size.
    void avgFolderSize(void);            // Determine the average folder size.
    void medianFileSize(void);           // Determine median file size.
    void medianFolderSize(void);         // Determine median folder size.
    void distributionFileSizes(void);    // Determine the distribution of file sizes.
    void distributionFolderSizes(void);  // Determine the distribution of folder sizes.
    void printFileInfo(void);            // Print information about files: name and size.
    void printFolderInfo(void);          // Print information about folders: name, size, number of things they contain.
};

#endif /* functions_hpp */

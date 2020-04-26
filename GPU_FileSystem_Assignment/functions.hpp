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
namespace fs = std::__fs::filesystem;

class explorer
{
private:
    // private class member variables
    std::vector<fs::path> m_fileNames;
    std::vector<unsigned long> m_fileSizes; // dimension = Bytes
    
    std::vector<fs::path> m_folderNames;
    std::vector<unsigned long> m_folderSizes;
    std::vector<unsigned> m_numberOfThingsInAFolder;
    
public:
    // public class member variables
    std::string m_givenPath;
    
    // Default constructor
    explorer()
    : m_givenPath{ "" }
    {};
    
    // Constructor based on argument list
    explorer(std::string givenPath)
    {
        m_givenPath = givenPath;
    };
    
    
    // Class member functions
// (1) manually set the path of the class which should be examined
    void setPath(std::string givenPath)
    {
        m_givenPath = givenPath;
    }
    
// (2) iterate the path and collect information
    void iterateThroughGivenPath(void)
    {
        for(auto& p: fs::recursive_directory_iterator(m_givenPath))
        {
            // if not directory
            if(!fs::is_directory(p.symlink_status()))
            {
                m_fileNames.push_back(p.path());                // save file names
                m_fileSizes.push_back(fs::file_size(p.path())); // save file sizes
            }
            
            // if directory: TODO
            else
            {
                continue;
            }
              
        }
    }
    
// (3) determine the n biggest files
    void biggestFiles(const unsigned int n)
    {
        std::cout << "TODO" <<std::endl;
    }
    
    
    // just for development phase, sanity checks
    void printFileInfo(void)
    {
        for (int i=0; i<m_fileNames.size(); ++i)
        {
            std::cout << m_fileSizes[i] << "\t" << m_fileNames[i] << std::endl;
        }
    }

   
    
};


#endif /* functions_hpp */

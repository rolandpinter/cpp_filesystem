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
    std::vector<fs::path> m_fileNames;
    std::vector<fs::path> m_folderNames;
    
public:
    // Class variables
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
    
    
    // Class functions
    void setPath(std::string givenPath)
    {
        m_givenPath = givenPath;
    }
    
    void iterateThroughGivenPath(void)
    {
        for(auto& p: fs::recursive_directory_iterator(m_givenPath))
        {
            if(!fs::is_directory(p.symlink_status()))
            {
                if (p.path().extension() == "")
                    std::cout << p.path() << std::endl;
            }
              
        }
    }

   
    
};


#endif /* functions_hpp */

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
    std::vector<unsigned long> m_fileSizes;   // dimension = Bytes
    
    std::vector<fs::path> m_folderNames;
    std::vector<unsigned long> m_folderSizes; // dimension = Bytes
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
// (1) manually set the path of the class which should be examined
    void setPath(std::string givenPath)
    {
        m_givenPath = givenPath;
        m_folderNames.push_back(fs::path(givenPath));
        m_folderSizes.push_back(0);
        m_numberOfThingsInAFolder.push_back(0);
        
    }
    
// (2) iterate the path and collect information
    void iterateThroughGivenPath(void)
    {
        for(auto& p: fs::recursive_directory_iterator(m_givenPath))
        {
            // if directory
            if(fs::is_directory(p.symlink_status()))
            {
                m_folderNames.push_back(p.path()); // save folder names
                m_folderSizes.push_back(0);
                m_numberOfThingsInAFolder.push_back(0);
            }
            
            // if not directory
            else
            {
                m_fileNames.push_back(p.path());                // save file names
                unsigned long sizeOfFile = fs::file_size(p.path());
                m_fileSizes.push_back(sizeOfFile); // save file sizes
                
                // if it's a file, it's parent_path() will be the directory within it's contained
                fs::path dirNameOfFile = p.path().parent_path();  // directory name of file
                
                // locate the directory's index in m_folderNames := indexOfFolder
                unsigned int indexOfFolder = 0;
                std::vector<fs::path>::iterator itr;
                for(itr = m_folderNames.begin(); itr != m_folderNames.end(); ++itr)
                {
                    if(*itr == dirNameOfFile)
                    {
                        // we found the directory, can stop iteration
                        break;
                    }
                    else
                        indexOfFolder += 1;
                }
                
                m_numberOfThingsInAFolder[indexOfFolder] += 1;
                m_folderSizes[indexOfFolder] += sizeOfFile;
                
            }
              
        }
    }
    
// (3) determine the path of the n biggest files
    void biggestFiles(const int n)
    {
        // handling bad use-case
        if (n < 1)
        {
            std::cerr << "You have asked for the path of the " << n << " biggest files. A bit silly, ehh? Try again please, exiting..." <<std::endl;
            exit(-1);
        }
        
        std::vector<std::pair<unsigned long, fs::path> > pairFilesWithSizes; // make a vector of pairs, to make sort easy
        
        // fill the vector of pairs
        for(unsigned int iFile=0; iFile < m_fileNames.size(); ++iFile)
            pairFilesWithSizes.push_back(std::make_pair(m_fileSizes[iFile], m_fileNames[iFile]));
        
        std::sort(pairFilesWithSizes.begin(), pairFilesWithSizes.end()); // sort in ascending order
        
        unsigned long numberOfFiles = m_fileNames.size();
        if(n > numberOfFiles)
        {
            std::cerr << "You've required to print the path of the first " << n <<" biggest files, but there are only " << m_fileNames.size() << " files under the given path, exiting..." << std::endl;
            exit(-1);
        }
        
        else
        {
            if (n == 1)
                std::cout << "The required path of the biggest file is the following:" << std::endl;
            else
                std::cout << "The required path of the " << n << " biggest files are the followings:" << std::endl;
            
            for(unsigned long iBig = numberOfFiles; iBig >(numberOfFiles - n); --iBig)
                std::cout << pairFilesWithSizes[iBig-1].second << "\t" << pairFilesWithSizes[iBig-1].first << " Bytes" << std::endl;
        }
        
        
    }
    
    
// Functions just for development phase, sanity checks
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

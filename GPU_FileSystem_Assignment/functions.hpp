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
//-------------------------- (1) Manually set the path of the class which should be examined. --------------------------//
    void setPath(std::string givenPath)
    {
        m_givenPath = givenPath;
        m_folderNames.push_back(fs::path(givenPath));
        m_folderSizes.push_back(0);
        m_numberOfThingsInAFolder.push_back(0);
        
    }
    
//-------------------------- (2) Iterate the path and collect information. --------------------------//
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
    
//-------------------------- (3) Determine the path of the n biggest files. --------------------------//
    void biggestFiles(const int n)
    {
        // handling bad use-case
        if (n < 1)
        {
            std::cerr << "You have asked for the path of the " << n << " biggest files. A bit silly, ehh? Try again please, exiting..." <<std::endl;
            exit(-1);
        }
        
        unsigned long numberOfFiles = m_fileNames.size();
        if(n > numberOfFiles)
        {
            std::cerr << "You've required to print the path of the first " << n <<" biggest files, but there are only " << numberOfFiles << " files under the given path, exiting..." << std::endl;
            exit(-1);
        }
        
        // done handling bad use-cases, let's solve the problem now
        std::vector<std::pair<unsigned long, fs::path> > pairFilesWithSizes; // make a vector of pairs, to make sort easy
        
        // fill the vector of pairs
        for(unsigned int iFile=0; iFile < numberOfFiles; ++iFile)
            pairFilesWithSizes.push_back(std::make_pair(m_fileSizes[iFile], m_fileNames[iFile]));
        
        std::sort(pairFilesWithSizes.begin(), pairFilesWithSizes.end()); // sort in ascending order
        
        if (n == 1)
            std::cout << "The required path of the biggest file is the following:" << std::endl;
        else
            std::cout << "The required path of the " << n << " biggest files are the followings:" << std::endl;
        
        for(unsigned long iBig = numberOfFiles; iBig >(numberOfFiles - n); --iBig)
            std::cout << pairFilesWithSizes[iBig-1].second << "\t" << pairFilesWithSizes[iBig-1].first << " Bytes" << std::endl;
    }
    
//-------------------------- (4) Determine the path of the n biggest folders. --------------------------//
    void biggestFolders(const int n)
    {
        // handling bad use-case
        if (n < 1)
        {
            std::cerr << "You have asked for the path of the " << n << " biggest folders. A bit silly, ehh? Try again please, exiting..." <<std::endl;
            exit(-1);
        }
        
        unsigned long numberOfFolders = m_folderNames.size();
        if(n > numberOfFolders)
        {
            std::cerr << "You've required to print the path of the first " << n <<" biggest folders, but there are only " << numberOfFolders << " folders under the given path, exiting..." << std::endl;
            exit(-1);
        }
        
        // done handling bad use-cases, let's solve the problem now
        std::vector<std::pair<unsigned long, fs::path> > pairFoldersWithSizes; // make a vector of pairs, to make sort easy
        
        // fill the vector of pairs
        for(unsigned int iFolder=0; iFolder < numberOfFolders; ++iFolder)
            pairFoldersWithSizes.push_back(std::make_pair(m_folderSizes[iFolder], m_folderNames[iFolder]));
        
        std::sort(pairFoldersWithSizes.begin(), pairFoldersWithSizes.end()); // sort in ascending order
        
        if (n == 1)
            std::cout << "The required path of the biggest folder is the following:" << std::endl;
        else
            std::cout << "The required path of the " << n << " biggest folders are the followings:" << std::endl;
        
        for(unsigned long iBig = numberOfFolders; iBig >(numberOfFolders - n); --iBig)
            std::cout << pairFoldersWithSizes[iBig-1].second << "\t" << pairFoldersWithSizes[iBig-1].first << " Bytes" << std::endl;
    }
    
//-------------------------- (5) Determine the average file size. --------------------------//
    void avgFileSize(void)
    {
        auto sum = std::accumulate(m_fileSizes.cbegin(), m_fileSizes.cend(), 0.0);
        auto average = sum / m_fileSizes.size();
        
        std::cout << "The average of the file sizes is: " << average << " Bytes." <<std::endl;
    }
    
//-------------------------- (6) Determine the average folder size. --------------------------//
    void avgFolderSize(void)
    {
        auto sum = std::accumulate(m_folderSizes.cbegin(), m_folderSizes.cend(), 0.0);
        auto average = sum / m_folderSizes.size();
        
        std::cout << "The average of the folder sizes is: " << average << " Bytes." <<std::endl;
    }

//-------------------------- (7) Determine median file size. --------------------------//
    void medianFileSize(void)
    {
        std::vector<unsigned long> fileSizesCopy = m_fileSizes; // copy by value, created a new vector! (to avoid messing with original vector)
        
        //std::vector<unsigned long> testingVec{1,9,6};
        //std::vector<unsigned long> fileSizesCopy = testinVec; // copy by value, created a new vector! (to avoid messing with original vector)
        
        size_t n = fileSizesCopy.size() / 2;
        std::nth_element(fileSizesCopy.begin(), fileSizesCopy.begin() + n, fileSizesCopy.end()); // sort until the half of the vector
        unsigned long centerValue = fileSizesCopy[n];
        
        if(fileSizesCopy.size() % 2 == 1)
            std::cout << "The median file size is: " <<centerValue<< " Bytes." << std::endl;
        
        else
        {
            std::nth_element(fileSizesCopy.begin(), fileSizesCopy.begin() + n-1, fileSizesCopy.end());
            unsigned long median = 0.5*(centerValue + fileSizesCopy[n-1]);
            std::cout << "The median file size is: " <<median<< " Bytes." << std::endl;
        }
    }
    
//-------------------------- (8) Determine median folder size. --------------------------//
    void medianFolderSize(void)
    {
        std::vector<unsigned long> folderSizesCopy = m_folderSizes; // copy by value, created a new vector! (to avoid messing with original vector)
        
        size_t n = folderSizesCopy.size() / 2;
        std::nth_element(folderSizesCopy.begin(), folderSizesCopy.begin() + n, folderSizesCopy.end()); // sort until the half of the vector
        unsigned long centerValue = folderSizesCopy[n];
        
        if(folderSizesCopy.size() % 2 == 1)
            std::cout << "The median folder size is: " <<centerValue<< " Bytes." << std::endl;
        
        else
        {
            std::nth_element(folderSizesCopy.begin(), folderSizesCopy.begin() + n-1, folderSizesCopy.end());
            unsigned long median = 0.5*(centerValue + folderSizesCopy[n-1]);
            std::cout << "The median folder size is: " <<median<< " Bytes." << std::endl;
        }
    }
    
    
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

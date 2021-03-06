#include "functions.hpp"

int findIndexOfElementInVector(std::vector<unsigned long> uniqueFileSizes, unsigned long sizeToFind)
{
    /// Function to find the index of a given element in a vector
    int index = 0;
    bool found = false;
    
    std::vector<unsigned long>::iterator itr;
    for(itr = uniqueFileSizes.begin(); itr != uniqueFileSizes.end(); ++itr)
    {
        if(*itr == sizeToFind)
        {
            found = true;
            break;
        }
        ++index;
    }
    
    if (found) return index;
    else return -999;
}

std::string askForPathToExplore()
{
    /// Function to ask user for path to explore
    std::string pathToExplore;
    std::cout << "Give me a folder's path to explore or write TEST to use a pre-defined default test path: ";
    std::cin >> pathToExplore;
    
    if(pathToExplore=="TEST") pathToExplore = "/Users/Roli/Desktop/MSc/2.félév/gpu/beadando/GPU_FileSystem_Assignment/cpp_filesystem/test01";
    
    fs::path temp(pathToExplore);
    if(!fs::exists(temp))
    {
        std::cerr<<"Path does not exists, exiting..."<<std::endl;
        exit(-1);
    }
    
    return pathToExplore;
}

int askForCommand()
{
    /// Function to drive the main()
    int command;
    std::cout << "\nWhat would you like to do? Please type in a number from the list:" << std::endl;
    std::cout << "0: Exit this program and say goodbye." << std::endl;
    std::cout << "1: Determine the path of the n biggest files." << std::endl;
    std::cout << "2: Determine the path of the n biggest folders." << std::endl;
    std::cout << "3: Determine the average file size." << std::endl;
    std::cout << "4: Determine the average folder size." << std::endl;
    std::cout << "5: Determine the median file size." << std::endl;
    std::cout << "6: Determine the median folder size." << std::endl;
    std::cout << "7: Determine the distribution of file sizes." << std::endl;
    std::cout << "8: Determine the distribution of folder sizes." << std::endl;
    std::cout << "9: List all the files under the given path with their sizes." << std::endl;
    std::cout << "10: List all the folders under the given path with their size and show how many files do they contain." << std::endl;
    std::cin >> command;
    
    if(command < 0 || command > 10)
    {
        std::cerr << "You've given a wrong number, exiting..." << std::endl;
        exit(-1);
    }
    
    if(command == 0)
    {
        std::cout << "You've requested to exit, so take care, cheers!" << std::endl;
        exit(-1);
    }
    
    return command;
}

///explorer() class member function definitions
//-------------------------- (1) Manually set the path of the class which should be examined. --------------------------//
    void explorer::setPath(std::string givenPath)
    {
        m_givenPath = givenPath;
        m_folderNames.push_back(fs::path(givenPath));
        m_folderSizes.push_back(0);
        m_numberOfThingsInAFolder.push_back(0);
    }
    
//-------------------------- (2) Iterate the path and collect information. --------------------------//
    void explorer::iterateThroughGivenPath(void)
    {
        for(auto& p: fs::recursive_directory_iterator(m_givenPath))
        {
            /// if directory
            if(fs::is_directory(p.symlink_status()))
            {
                m_folderNames.push_back(p.path()); // save folder names
                m_folderSizes.push_back(0);
                m_numberOfThingsInAFolder.push_back(0);
            }
            
            /// if not directory
            else
            {
                m_fileNames.push_back(p.path());                // save file names
                unsigned long sizeOfFile = fs::file_size(p.path());
                m_fileSizes.push_back(sizeOfFile); // save file sizes
                
                /// if it's a file, it's parent_path() will be the directory within it's contained
                fs::path dirNameOfFile = p.path().parent_path();  // directory name of file
                
                /// locate the directory's index in m_folderNames := indexOfFolder
                unsigned int indexOfFolder = 0;
                std::vector<fs::path>::iterator itr;
                for(itr = m_folderNames.begin(); itr != m_folderNames.end(); ++itr)
                {
                    if(*itr == dirNameOfFile)
                    {
                        /// we found the directory, can stop iteration
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
    void explorer::biggestFiles(const int n)
    {
        /// handling bad use-case
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
        
        /// done handling bad use-cases, let's solve the problem now
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
    void explorer::biggestFolders(const int n)
    {
        /// handling bad use-case
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
        
        /// done handling bad use-cases, let's solve the problem now
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
    void explorer::avgFileSize(void)
    {
        auto sum = std::accumulate(m_fileSizes.cbegin(), m_fileSizes.cend(), 0.0);
        auto average = sum / m_fileSizes.size();
        
        std::cout << "The average of the file sizes is: " << average << " Bytes." <<std::endl;
    }
    
//-------------------------- (6) Determine the average folder size. --------------------------//
    void explorer::avgFolderSize(void)
    {
        auto sum = std::accumulate(m_folderSizes.cbegin(), m_folderSizes.cend(), 0.0);
        auto average = sum / m_folderSizes.size();
        
        std::cout << "The average of the folder sizes is: " << average << " Bytes." <<std::endl;
    }

//-------------------------- (7) Determine median file size. --------------------------//
    void explorer::medianFileSize(void)
    {
        std::vector<unsigned long> fileSizesCopy = m_fileSizes; // copy by value, created a new vector! (to avoid messing with original vector)
        
        /// test with the following 2 lines, seems to work!
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
    void explorer::medianFolderSize(void)
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
    
//-------------------------- (9) Determine the distribution of file sizes. --------------------------//
    void explorer::distributionFileSizes(void)
    {
        std::vector<unsigned long> uniqueFileSizes; // vector for unique file sizes (i.e.: each different size appears once here)
        std::vector<int> countUniqueFileSizes;      // vector for counting the occurance of the unique file sizes
        
        std::vector<unsigned long>::iterator itr;
        for(itr = m_fileSizes.begin(); itr != m_fileSizes.end(); ++itr)
        {
            int index = findIndexOfElementInVector(uniqueFileSizes, *itr);
            if (index == -999)
            {
                uniqueFileSizes.push_back(*itr);
                countUniqueFileSizes.push_back(1);
            }
            else
                countUniqueFileSizes[index] += 1;
        }
        
        
        std::cout<<"File sizes [Bytes]   Counts      Histogram"<<std::endl; // 22. char is C in Counts, 34. char is H in Histogram
        
        int positionOfC = 22;
        int positionOfH = 34;
        
        for(int i=0; i<uniqueFileSizes.size(); ++i)
        {
            std::cout << uniqueFileSizes[i];
            
            /// string magic to have the number perfectly aligned under Counts!
            std::string foo = std::to_string(uniqueFileSizes[i]);
            unsigned long lengthOfNumber = foo.size();
            for(int j = 0; j < positionOfC - lengthOfNumber - 1; ++j) std::cout<<" ";
            
            std::cout <<countUniqueFileSizes[i];
            
            /// string magic to have the histogram marking * symbols perfectly aligned under Histogram!
            std::string fooo = std::to_string(countUniqueFileSizes[i]);
            unsigned long lenghtOfCountsOfNumber = fooo.size();
            for(int k = 0; k <positionOfH - positionOfC - lenghtOfCountsOfNumber;++k) std::cout<<" ";
            
            for(int h=0; h<countUniqueFileSizes[i];++h) std::cout<<"*";
            std::cout<<std::endl;
            
        }
    }

//-------------------------- (10) Determine the distribution of folder sizes. --------------------------//
void explorer::distributionFolderSizes(void)
{
    std::vector<unsigned long> uniqueFolderSizes; // vector for unique file sizes (i.e.: each different size appears once here)
    std::vector<int> countUniqueFolderSizes;      // vector for counting the occurance of the unique file sizes
    
    std::vector<unsigned long>::iterator itr;
    for(itr = m_folderSizes.begin(); itr != m_folderSizes.end(); ++itr)
    {
        int index = findIndexOfElementInVector(uniqueFolderSizes, *itr);
        if (index == -999)
        {
            uniqueFolderSizes.push_back(*itr);
            countUniqueFolderSizes.push_back(1);
        }
        else
            countUniqueFolderSizes[index] += 1;
    }
    
    std::cout<<"Folder sizes [Bytes]   Counts      Histogram"<<std::endl; // 24. char is C in Counts, 36. char is H in Histogram
    
    int positionOfC = 24;
    int positionOfH = 36;
    
    for(int i=0; i<uniqueFolderSizes.size(); ++i)
    {
        std::cout << uniqueFolderSizes[i];
        
        /// string magic to have the number perfectly aligned under Counts!
        std::string foo = std::to_string(uniqueFolderSizes[i]);
        unsigned long lengthOfNumber = foo.size();
        for(int j = 0; j < positionOfC - lengthOfNumber - 1; ++j) std::cout<<" ";
        
        std::cout <<countUniqueFolderSizes[i];
        
        /// string magic to have the histogram marking * symbols perfectly aligned under Histogram!
        std::string fooo = std::to_string(countUniqueFolderSizes[i]);
        unsigned long lenghtOfCountsOfNumber = fooo.size();
        for(int k = 0; k <positionOfH - positionOfC - lenghtOfCountsOfNumber;++k) std::cout<<" ";
        
        for(int h=0; h<countUniqueFolderSizes[i];++h) std::cout<<"*";
        std::cout<<std::endl;
        
    }
}

//-------------------------- (11) Print information about files: name and size. --------------------------//
void explorer::printFileInfo(void)
{
       for (int i = 0; i <m_fileNames.size(); ++i)
           std::cout << m_fileSizes[i] << " Bytes\t" << m_fileNames[i] << std::endl;
}
 
//-------------------------- (12) Print information about folders: name, size, number of things they contain. --------------------------//
void explorer::printFolderInfo(void)
{
       for(int i = 0;i < m_folderNames.size(); ++i)
           std::cout << m_folderNames[i] << "\n" << "# of elements: " << m_numberOfThingsInAFolder[i] << "\n" << "size: " << m_folderSizes[i] << " Bytes" << std::endl;
}

#include "functions.hpp"

int main()
{
    std::string pathToExplore = askForPathToExplore();
    explorer foo(pathToExplore);
    
    std::cout << "Exploring the given path and collecting information about files and folders..." << std::endl;
    foo.iterateThroughGivenPath();
    
    while(true)
    {
        int command = askForCommand();
        if(command == 1)
        {
            int n;
            std::cout << "n = ";
            std::cin >> n;
            foo.biggestFiles(n);
        }
        else if(command == 2)
        {
            int n;
            std::cout << "n = ";
            std::cin >> n;
            foo.biggestFolders(n);
        }
        else if(command == 3) foo.avgFileSize();
        else if(command == 4) foo.avgFolderSize();
        else if(command == 5) foo.medianFileSize();
        else if(command == 6) foo.medianFolderSize();
        else if(command == 7) foo.distributionFileSizes();
        else if(command == 8) foo.distributionFolderSizes();
        
    }
    
    return 0;
}

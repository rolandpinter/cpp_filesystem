#include "functions.hpp"

int main()
{
    explorer foo("/Users/Roli/Desktop/MSc/2.félév/gpu/beadando/GPU_FileSystem_Assignment/cpp_filesystem/test01");
    foo.iterateThroughGivenPath();
    //foo.printFileInfo();
    //foo.printFolderInfo();
    //foo.biggestFiles(3);
    //foo.biggestFolders(2);
    //foo.avgFileSize();
    //foo.avgFolderSize();
    //foo.medianFileSize();
    //foo.medianFolderSize();
    foo.distributionFileSizes();
    //foo.distributionFolderSizes();
    
    
    
    return 0;
}

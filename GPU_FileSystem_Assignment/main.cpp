#include "functions.cpp"

int main(int argc, const char * argv[])
{
    /*
    fs::path pathToShow("/Users/Roli/Desktop/MSc/2.félév");
    std::cout << "exists() = " << fs::exists(pathToShow) << "\n"
         << "root_name() = " << pathToShow.root_name() << "\n"
         << "root_path() = " << pathToShow.root_path() << "\n"
         << "relative_path() = " << pathToShow.relative_path() << "\n"
         << "parent_path() = " << pathToShow.parent_path() << "\n"
         << "filename() = " << pathToShow.filename() << "\n"
         << "stem() = " << pathToShow.stem() << "\n"
         << "extension() = " << pathToShow.extension() << "\n";
    */
    
    explorer a("/Users/Roli/Desktop/MSc/2.félév/gpu/");
    a.iterateThroughGivenPath();
    
    return 0;
}

#include <iostream>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include "camerawindow.h"

using namespace std;

int main()
{
    CameraWindow window(30);

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream inputFilePath;
    inputFilePath << cwd << "/../../Applications/ProjectionTest/board.";
    if(__APPLE__) {
        inputFilePath << "mov";
    } else {
        inputFilePath << "avi";
    }
    std::string inputFile = inputFilePath.str();
    std::cout << "using " << inputFile << " as video source" << std::endl;

    window.startCapturing(/*inputFile*/);
}


#include <iostream>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <Ardoor/CameraWindow/CameraWindow.hpp>

#if defined(__APPLE__)
#define VIDEO_FORMAT "mov"
#else
#define VIDEO_FORMAT "avi"
#endif

using namespace std;

int main()
{
    CameraWindow window(30);

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream inputFilePath;
    inputFilePath << cwd << "/../../Applications/ProjectionTest/board." << VIDEO_FORMAT;
    std::string inputFile = inputFilePath.str();
    std::cout << "using " << inputFile << " as video source" << std::endl;

    window.startCapturing(/*inputFile*/);
}


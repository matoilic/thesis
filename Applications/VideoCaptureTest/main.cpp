#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <errno.h>
#include <string.h>

int main()
{
    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    std::stringstream inputFilePath;
    inputFilePath << cwd << "/../../Applications/ProjectionTest/board.mov";
    std::string inputFile = inputFilePath.str();

    int key = 0;
    // Initialize camera and OpenCV image
    //CvCapture* capture = cvCaptureFromCAM( 0 );
    CvCapture* capture = cvCaptureFromAVI( inputFile.c_str() );
    IplImage* frame = cvQueryFrame( capture );

    // Check
    if ( !capture )
    {
        fprintf( stderr, "Cannot open AVI!\n" );
        return 1;
    }

    // Get the fps, needed to set the delay
    int fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );

    // Create a window to display the video
    cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );

    while( key != 'x' )
    {
        // get the image frame
        frame = cvQueryFrame( capture );

        // exit if unsuccessful
        if( !frame ) break;

        // display current frame
        cvShowImage( "video", frame );

        // exit if user presses 'x'
        key = cvWaitKey( 1000 / fps );
    }

    // Tidy up
    cvDestroyWindow( "video" );
    cvReleaseCapture( &capture );

    return 0;
}

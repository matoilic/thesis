#include <stdio.h>
#if (defined WIN32 || defined _WIN32)
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Ardoor/CameraCalib/CameraCalib.hpp>

using namespace std;

void printMat(const cv::Mat_<float> &mat);

int main()
{
    cv::Size boardSize(9, 6);
    CameraCalib c1(boardSize), c2(boardSize);

    char cwd[FILENAME_MAX];
    getcwd(cwd, sizeof(cwd));
    vector<string> fileList;

    for(int i = 1; i <= 25; i++)
    {
        stringstream path;
        string file;
        path << cwd << "/../../Applications/CameraCalibTest/Files/" << i << ".jpg";
        file = path.str();
        fileList.push_back(file);
    }

    cv::Mat exampleImage = cv::imread(fileList.at(0), CV_LOAD_IMAGE_COLOR);
    cv::Size imageSize(exampleImage.cols, exampleImage.rows);
    cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    cv::imshow( "Display window", exampleImage );


    cout << "starting first calibration..." << endl;
    cout << c1.addChessboardPoints(fileList) << " successful detections" << endl;
    c1.calibrate(imageSize);
    printMat(c1.getIntrinsicsMatrix());
    cout << endl << endl;

    fileList.clear();
    for(int i = 13; i <= 38; i++)
    {
        stringstream path;
        string file;
        path << cwd << "/../../CameraCalibTest/Files/" << i << ".jpg";
        file = path.str();
        fileList.push_back(file);
    }

    cout << "starting second calibration..." << endl;
    cout << c2.addChessboardPoints(fileList) << " successful detections" << endl;
    c2.calibrate(imageSize);
    printMat(c2.getIntrinsicsMatrix());

    return 0;
}

void printMat(const cv::Mat_<float> &mat)
{
    for (int r = 0; r < mat.rows; r++)
    {
        for (int c = 0; c < mat.cols; c++)
        {
            cout << mat.row(r).col(c) << "\t";
        }

        cout << endl;
    }
}

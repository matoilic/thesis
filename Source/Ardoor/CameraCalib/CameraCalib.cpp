#ifdef DEBUG
    #include <string>
    #include <sstream>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

#include  <Ardoor/CameraCalib/CameraCalib.hpp>

CameraCalib::CameraCalib(cv::Size &bs)
{
    mustInitUndistort = false;
    boardSize = bs;

    initObjectCorners();
}

void CameraCalib::initObjectCorners()
{
    float size = 1.0f;
    float xOffsetFactor = -(boardSize.width - 1.0f) / 2.0f;
    float yOffsetFactor = -(boardSize.height - 1.0f) / 2.0f;

    for(int i = 0; i < boardSize.height; i++)
    {
        for(int j = 0; j < boardSize.width; j++)
        {
            float x = size * (xOffsetFactor + j);
            float y = size * (yOffsetFactor + i);
            objectCorners.push_back(cv::Point3f(x, y, 0.0f));
        }
    }
}

cv::Size CameraCalib::getBoardSize()
{
    return boardSize;
}

cv::Mat CameraCalib::getDistortionCoeffs()
{
    return distCoeffs;
}

cv::Mat CameraCalib::getIntrinsicsMatrix()
{
    return cameraMatrix;
}

int CameraCalib::addChessboardPoints(const std::vector<std::string> &filelist)
{
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    cv::Mat image;
    int successes = 0;

    for (unsigned int i = 0; i < filelist.size(); i++)
    {
        image = cv::imread(filelist[i],0);

        #ifdef DEBUG
            if (findAndDrawChessboardPoints(image, imageCorners, objectCorners)) {
                addPoints(imageCorners, objectCorners);
                successes++;

                std::cout << filelist[i] << std::endl;
                #ifndef WIN32
                    mkdir("calib", 0777);
                    std::stringstream path;
                    path << "calib/" << successes << ".jpg";
                    cv::imwrite(path.str(), image);
                #endif
            }
        #else
            if (findChessboardPoints(image, imageCorners, objectCorners)) {
                addPoints(imageCorners, objectCorners);
                successes++;
            }
        #endif
    }

    return successes;
}

void CameraCalib::addPoints(const std::vector<cv::Point2f> &imageCorners, std::vector<cv::Point3f> &objectCorners)
{
    mustInitUndistort = true;
    imagePoints.push_back(imageCorners);
    objectPoints.push_back(objectCorners);
}

double CameraCalib::calibrate(cv::Size &imageSize)
{
    mustInitUndistort= true;
    std::vector<cv::Mat> rotationVecs, translationVecs;

    return cv::calibrateCamera(
        objectPoints,
        imagePoints,
        imageSize,
        cameraMatrix,
        distCoeffs,
        rotationVecs,
        translationVecs
    );
}

bool CameraCalib::findAndDrawChessboardPoints(const cv::Mat &image, std::vector<cv::Point2f> &imageCorners, std::vector<cv::Point3f> &objectCorners)
{
    bool success = findChessboardPoints(image, imageCorners, objectCorners);

    cv::drawChessboardCorners(image, boardSize, imageCorners, success);

    return success;
}

bool CameraCalib::findChessboardPoints(const cv::Mat &image, std::vector<cv::Point2f> &imageCorners, std::vector<cv::Point3f> &objectCorners)
{
    objectCorners.clear();
    objectCorners.resize(objectCorners.size());
    objectCorners.insert(objectCorners.begin(), this->objectCorners.begin(), this->objectCorners.end());

    cv::Mat greyImage, eightBitImage;
    if(image.type() != CV_8UC1)
    {
        image.convertTo(eightBitImage, CV_8U);
        cv::cvtColor(eightBitImage, greyImage, CV_BGR2GRAY);
    }
    else
    {
        greyImage = image;
    }

    bool success = cv::findChessboardCorners(greyImage, boardSize, imageCorners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if(success) {
        cv::cornerSubPix(
            greyImage,
            imageCorners,
            cv::Size(5,5),
            cv::Size(-1,-1),
            cv::TermCriteria(
                cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
                30,  // max number of iterations
                0.1  // min accuracy
            )
        );
    }

    objectCorners.resize(imageCorners.size(), objectCorners[0]);

    return imageCorners.size() == boardSize.area();
}

cv::Mat CameraCalib::remap(const cv::Mat &image)
{
    cv::Mat undistorted;

    if(mustInitUndistort) {
        cv::initUndistortRectifyMap(
            cameraMatrix,
            distCoeffs,
            cv::Mat(),
            cv::Mat(),
            image.size(),
            CV_32F,
            mapX,
            mapY
        );

        printMat(cameraMatrix, "Camera Matrix");
        printMat(distCoeffs, "Disortion Coefficients");

        mustInitUndistort = false;
    }

    cv::remap(image, undistorted, mapX, mapY, cv::INTER_LINEAR);

    return undistorted;
}

void CameraCalib::reset()
{
    imagePoints.clear();
    objectPoints.clear();
}

void CameraCalib::printMat(const cv::Mat &mat, std::string name)
{
    int r, c;

    std::cout << name << std::endl;

    for(r = 0; r < mat.rows; r++)
    {
        for(c = 0; c < mat.cols; c++)
        {
            std::cout << mat.row(r).col(c) << " ";
        }

        std::cout << std::endl;
    }
}

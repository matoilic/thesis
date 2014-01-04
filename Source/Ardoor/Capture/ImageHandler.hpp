#ifndef IMAGEHANDLER_HPP
#define IMAGEHANDLER_HPP

#include <opencv2/core/core.hpp>
#include <Ardoor/Ardoor.hpp>

class ARD_EXPORT ImageHandler
{
public:
    virtual void operator() (cv::Mat &image) = 0;
};

#endif // IMAGEHANDLER_HPP

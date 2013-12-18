#ifndef MATGEN_H
#define MATGEN_H

#include <opencv2/core/core.hpp>

class MatGen
{
public:
    static cv::Matx44f ortho(float left, float right, float bottom, float top, float near, float far);
    static cv::Matx44f translate(float x, float y, float z);
};

#endif // MATGEN_H

#include "matgen.h"

cv::Matx44f MatGen::ortho(float left, float right, float bottom, float top, float near, float far)
{
    cv::Matx44f ortho = cv::Matx44f::eye();

    ortho(0, 0) = 2 / (right - left);
    ortho(0, 3) = -(right + left) / (right - left);
    ortho(1, 1) = 2 / (top - bottom);
    ortho(1, 3) = -(top + bottom) / (top - bottom);
    ortho(2, 2) = -2 / (far - near);
    ortho(2, 3) = -(far + near) / (far - near);

    return ortho;
}

cv::Matx44f MatGen::translate(float x, float y, float z)
{
    cv::Matx44f translate = cv::Matx44f::eye();

    translate(0, 3) = x;
    translate(1, 3) = y;
    translate(2, 3) = z;

    return translate;
}

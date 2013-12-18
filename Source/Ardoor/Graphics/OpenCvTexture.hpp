#ifndef OPENCVTEXTURE_HPP
#define OPENCVTEXTURE_HPP

#include <Ardoor/Ardoor.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>

class ARD_EXPORT OpenCvTexture
{
private:
    GLuint textureId;

public:
    OpenCvTexture();

    GLuint getId();
    void generate();
    void update(const cv::Mat &image);
    void bind();
};

#endif // OPENCVTEXTURE_HPP

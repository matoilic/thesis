#include <Ardoor/Graphics/OpenCvTexture.hpp>

OpenCvTexture::OpenCvTexture()
{
    textureId = GL_FALSE;
}

GLuint OpenCvTexture::getId()
{
    return textureId;
}

void OpenCvTexture::generate()
{
    if (textureId == GL_FALSE) {
        glGenTextures(1, &textureId);
    }
}

void OpenCvTexture::update(const cv::Mat &image)
{
    generate();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Upload texture data
    if (image.channels() == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    } else if (image.channels() == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    } else if (image.channels() == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.data);
    }
}

void OpenCvTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

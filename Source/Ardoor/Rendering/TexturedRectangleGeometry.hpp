#ifndef TEXTUREDRECTANGLEGEOMETRY_HPP
#define TEXTUREDRECTANGLEGEOMETRY_HPP

#include <opencv2/core/core.hpp>
#include <Ardoor/Rendering/Geometry.hpp>

class ARD_EXPORT TexturedRectangleGeometry : public Geometry
{
public:
    TexturedRectangleGeometry() { init(); }
    void update(const cv::Mat &image);
    void draw(QOpenGLShaderProgram *program);

protected:
    void initGeometry();

private:
    GLuint textureId;
};

#endif // TEXTUREDRECTANGLEGEOMETRY_HPP

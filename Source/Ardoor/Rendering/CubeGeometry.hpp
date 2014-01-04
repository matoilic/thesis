#ifndef CUBEGEOMETRY_HPP
#define CUBEGEOMETRY_HPP

#include <QColor>
#include <Ardoor/Rendering/Geometry.hpp>

class ARD_EXPORT CubeGeometry : public Geometry
{
public:
    CubeGeometry() { init(); }

    QColor color();
    void setColor(QColor color);
    void draw(QOpenGLShaderProgram *program);

protected:
    void initGeometry();

private:
    GLuint vboLines;
    QColor m_color;
};

#endif // CUBEGEOMETRY_HPP

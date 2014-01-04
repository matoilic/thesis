#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <Ardoor/Ardoor.hpp>

class ARD_EXPORT Geometry : protected QOpenGLFunctions
{
public:
    virtual ~Geometry();

    void init();
    virtual void draw(QOpenGLShaderProgram *program) = 0;

protected:
    virtual void initGeometry() = 0;
    GLuint vboIds[2];
};
#endif // GEOMETRY_HPP

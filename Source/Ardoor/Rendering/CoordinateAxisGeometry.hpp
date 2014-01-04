#ifndef COORDINATEAXISGEOMETRY_HPP
#define COORDINATEAXISGEOMETRY_HPP

#include <Ardoor/Rendering/Geometry.hpp>

class ARD_EXPORT CoordinateAxisGeometry : public Geometry
{
public:
    CoordinateAxisGeometry() { init(); }
    void draw(QOpenGLShaderProgram *program);

protected:
    void initGeometry();
};

#endif // COORDINATEAXISGEOMETRY_HPP

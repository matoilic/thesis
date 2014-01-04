#include "Geometry.hpp"

Geometry::~Geometry()
{
    glDeleteBuffers(2, vboIds);
}

void Geometry::init()
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    glGenBuffers(2, vboIds);

    // Initializes geometry and transfers it to VBOs
    initGeometry();
}

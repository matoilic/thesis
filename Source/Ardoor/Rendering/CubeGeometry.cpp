#include "CubeGeometry.hpp"

void CubeGeometry::setColor(QColor color)
{
    m_color = color;
}

QColor CubeGeometry::color()
{
    return m_color;
}

void CubeGeometry::initGeometry()
{
    QVector3D vertices[] = {
        // Vertex data for face 0
        QVector3D(-1.0, -1.0,  1.0),  // 0
        QVector3D( 1.0, -1.0,  1.0),  // 1
        QVector3D(-1.0,  1.0,  1.0),  // 2
        QVector3D( 1.0,  1.0,  1.0),  // 3

        // Vertex data for face 1
        QVector3D( 1.0, -1.0,  1.0),
        QVector3D( 1.0, -1.0, -1.0),
        QVector3D( 1.0,  1.0,  1.0),
        QVector3D( 1.0,  1.0, -1.0),

        // Vertex data for face 2
        QVector3D( 1.0, -1.0, -1.0), // 8
        QVector3D(-1.0, -1.0, -1.0), // 9
        QVector3D( 1.0,  1.0, -1.0), // 10
        QVector3D(-1.0,  1.0, -1.0), // 11

        // Vertex data for face 3
        QVector3D(-1.0, -1.0, -1.0),
        QVector3D(-1.0, -1.0,  1.0),
        QVector3D(-1.0,  1.0, -1.0),
        QVector3D(-1.0,  1.0,  1.0),

        // Vertex data for face 4
        QVector3D(-1.0, -1.0, -1.0),
        QVector3D( 1.0, -1.0, -1.0),
        QVector3D(-1.0, -1.0,  1.0),
        QVector3D( 1.0, -1.0,  1.0),

        // Vertex data for face 5
        QVector3D(-1.0,  1.0,  1.0),
        QVector3D( 1.0,  1.0,  1.0),
        QVector3D(-1.0,  1.0, -1.0),
        QVector3D( 1.0,  1.0, -1.0),
    };

    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0
         4,  4,  5,  6,  7,  7, // Face 1
         8,  8,  9, 10, 11, 11, // Face 2
        12, 12, 13, 14, 15, 15, // Face 3
        16, 16, 17, 18, 19, 19, // Face 4
        20, 20, 21, 22, 23      // Face 5
    };

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(QVector3D), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 34 * sizeof(GLushort), indices, GL_STATIC_DRAW);


    glGenBuffers(1, &vboLines);

    GLushort lineIndices[] = {
         0,  1,  1,  3,  3,  2,  2,  0,
         8,  9,  9, 11, 11, 10, 10,  8,
         0,  9,  1,  8,  3, 10,  2, 11
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLushort), lineIndices, GL_STATIC_DRAW);
}

void CubeGeometry::draw(QOpenGLShaderProgram *program)
{
    // Apply color
    int colorLocation = program->uniformLocation("u_color");
    glUniform4f(colorLocation, m_color.redF(), m_color.greenF(), m_color.blue(), m_color.alphaF());

    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    // Line color
    glUniform4f(colorLocation, 0.0, 0.0, 0.0, 1.0);

    // Draw Lines
    glLineWidth(4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboLines);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Disable the vertex arrays
    program->disableAttributeArray(vertexLocation);
}

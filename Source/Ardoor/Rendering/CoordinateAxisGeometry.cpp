#include "CoordinateAxisGeometry.hpp"

void CoordinateAxisGeometry::initGeometry()
{
    QVector3D vertices[] = {
        // Origin
        QVector3D(0.0, 0.0, 0.0),

        // Line 1
        QVector3D(1.0, 0.0, 0.0),

        // Line 2
        QVector3D(0.0, 1.0, 0.0),

        // Line 3
        QVector3D(0.0, 0.0, 1.0)
    };

    GLushort indices[] = { 0, 1, 0, 2, 0, 3 };

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(QVector3D), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void CoordinateAxisGeometry::draw(QOpenGLShaderProgram *program)
{
    glLineWidth(2);

    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);

    int colorLocation = program->uniformLocation("u_color");

    // Draw line 1
    glUniform4f(colorLocation, 1.0, 0.0, 0.0, 1.0);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (void*) (0 * sizeof(GLushort)));

    // Draw line 2
    glUniform4f(colorLocation, 0.0, 1.0, 0.0, 1.0);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (void*) (2 * sizeof(GLushort)));

    // Draw line 3
    glUniform4f(colorLocation, 0.0, 0.0, 1.0, 1.0);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (void*) (4 * sizeof(GLushort)));

    // Deactivate buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Disable the vertex arrays
    program->disableAttributeArray(vertexLocation);
}

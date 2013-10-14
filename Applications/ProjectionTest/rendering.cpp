#include "rendering.hpp"

GLdouble projection[16], inverse[16];
bool windowInitialized = false;

void initializeGL(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(50, 50);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glutCreateWindow("Screen");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutMainLoop();
}


void resizeWindow(int width, int height)
{
    if (!windowInitialized) {
        glutReshapeWindow(width, height);
        windowInitialized = true;
    }
}

void redisplay()
{
    glutPostRedisplay();
}


void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    initializePerspective();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Init Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[]  = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat diffuseLight[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawScene();

    glutSwapBuffers();
}

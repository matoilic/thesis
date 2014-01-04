#ifndef GLUTILS_HPP
#define GLUTILS_HPP


#define GET_GL_ERROR GLUtils::getGLError(__FILE__, __LINE__);

class GLUtils
{
public:
    static void getGLError(char* file, int line);
};

#endif // GLUTILS_HPP

#include "GLUtils.hpp"
#include <QDebug>
#include <QOpenGLFunctions>

void GLUtils::getGLError(char* file, int line)
{
   GLenum err;
   if ((err = glGetError()) != GL_NO_ERROR)
   {  char* errStr;
         switch(err)
      {  case GL_INVALID_ENUM:
            errStr = "GL_INVALID_ENUM"; break;
         case GL_INVALID_VALUE:
            errStr = "GL_INVALID_VALUE"; break;
         case GL_INVALID_OPERATION:
            errStr = "GL_INVALID_OPERATION"; break;
         case GL_INVALID_FRAMEBUFFER_OPERATION:
            errStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
         case GL_OUT_OF_MEMORY:
            errStr = "GL_OUT_OF_MEMORY"; break;
         default: errStr = "Unknown error";
      }

      // Build error string as a concatenation of file, line & error
      qDebug() << file << ": line: " << line << ": " << errStr;
   }
}

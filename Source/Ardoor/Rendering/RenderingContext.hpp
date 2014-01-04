#ifndef RENDERINGCONTEXT_HPP
#define RENDERINGCONTEXT_HPP

#include <Ardoor/Ardoor.hpp>

class ARD_EXPORT RenderingContext
{
public:
    RenderingContext() {}
    virtual ~RenderingContext() {}
    virtual void initialize() {}
    virtual void resize(int width, int height) {}
    virtual void display() {}
};

#endif // RENDERINGCONTEXT_HPP

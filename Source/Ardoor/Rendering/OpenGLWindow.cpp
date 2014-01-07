#include <Ardoor/Rendering/OpenGLWindow.hpp>

#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>


OpenGLWindow::OpenGLWindow(RenderingContext *context, QWindow *parent)
    : QWindow(parent)
    , m_renderingContext(context)
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    connect(this, SIGNAL(widthChanged(int)), this, SLOT(updateSize()));
    connect(this, SIGNAL(heightChanged(int)), this, SLOT(updateSize()));
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
}

void OpenGLWindow::updateSize()
{
    if (m_renderingContext && m_context)
        m_renderingContext->resize(width(), height());
}

void OpenGLWindow::render()
{
    if (m_renderingContext)
        m_renderingContext->display();
}

void OpenGLWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        if (m_renderingContext) {
            m_renderingContext->initialize();
        }
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void OpenGLWindow::setRenderingContext(RenderingContext *context)
{
    m_renderingContext = context;
}

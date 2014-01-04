#include <Ardoor/Rendering/RenderingContext.hpp>

#include <QtGui/QWindow>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class ARD_EXPORT OpenGLWindow : public QWindow
{
    Q_OBJECT

public:
    explicit OpenGLWindow(RenderingContext *context = 0, QWindow *parent = 0);
    ~OpenGLWindow();

    QOpenGLContext* openglContext() { return m_context; }

    virtual void render();
    void setAnimating(bool animating);
    void setRenderingContext(RenderingContext *context);

public slots:
    void renderLater();
    void renderNow();
    void updateSize();

protected:
    bool event(QEvent *event);
    void exposeEvent(QExposeEvent *event);

private:
    bool m_update_pending;
    bool m_animating;

    RenderingContext *m_renderingContext;
    QOpenGLContext *m_context;
};

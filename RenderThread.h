#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QSemaphore>

class RenderServer;
class RenderThread : public QThread
{
	Q_OBJECT
public:
	RenderThread(QWidget *pRenderWidget);
	~RenderThread();

public:
	void SetNewFrame(const QImage &Img);

protected:
	void run();

private:
	RenderServer *m_pRenderServer;
	QWidget* m_pRenderWidget;
	QSemaphore m_PortSem;
};

#endif // RENDERTHREAD_H

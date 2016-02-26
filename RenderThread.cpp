#include "RenderThread.h"
#include "RenderServer.h"

#include <QDebug>

RenderThread::RenderThread(QWidget *pRenderWidget) :
	m_pRenderServer(NULL),
	m_pRenderWidget(pRenderWidget)
{
}

RenderThread::~RenderThread()
{
	if (NULL != m_pRenderServer) {
		delete m_pRenderServer;
	}
}

void RenderThread::run()
{
	qDebug() << "RenderThread::run";

	m_pRenderServer = new RenderServer(1234, m_pRenderWidget);

	exec();
}

void RenderThread::SetNewFrame(const QImage &Img)
{
	int Width = Img.width();
	int Height = Img.height();

	if ((0 >= Width) || (0 >= Height)) {
		return;
	}

	if (NULL != m_pRenderServer) {
		m_pRenderServer->SetNewFrame(Img);
	}
}

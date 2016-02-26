#include "WidgetStreamer.h"
#include "RenderThread.h"

#include <QWidget>
#include <QTimer>
#include <QWebSocket>
#include <QBuffer>

WidgetStreamer::WidgetStreamer(QWidget *pRenderWidget)
{
	m_pRenderWidget = pRenderWidget;
	m_pRenderThread = new RenderThread(m_pRenderWidget);
	m_pRenderThread->start();

	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(OnGetNewFrame()));
	pTimer->start(40);
}

WidgetStreamer::~WidgetStreamer()
{
}

void WidgetStreamer::OnGetNewFrame()
{
	if ((NULL == m_pRenderThread) || (NULL == m_pRenderThread)) {
		return;
	}

	int Width = m_pRenderWidget->width();
	int Height = m_pRenderWidget->height();

	if ((0 >= Width) || (0 >= Height)) {
		return;
	}

	QPixmap Pixmap(Width, Height);

	/* MUST: gui thread */
	m_pRenderWidget->render(&Pixmap);

	QImage Img = Pixmap.toImage();

	m_pRenderThread->SetNewFrame(Img);
}


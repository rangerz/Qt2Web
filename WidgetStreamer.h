#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>

class RenderThread;
class WidgetStreamer : public QObject
{
	Q_OBJECT

public:
	WidgetStreamer(QWidget *pRenderWidget);
	~WidgetStreamer();

private slots:
	void OnGetNewFrame();

private:
	QWidget *m_pRenderWidget;
	RenderThread *m_pRenderThread;
};

#endif // WIDGET_H

#ifndef RENDERSERVER_H
#define RENDERSERVER_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QWebSocketServer>
#include <QWebSocket>

class RenderServer : public QObject
{
	Q_OBJECT
public:
	explicit RenderServer(int Port = 0, QWidget *pRenderWidget = NULL);
	~RenderServer();

public:
	void SetNewFrame(const QImage &Img);

Q_SIGNALS:
	void SigHasNewFrame();
	void SigClosed();

private Q_SLOTS:
	void OnProcessNewFrame();
	void OnNewConnection();
	void OnProcessTextMessage(QString message);
	void OnProcessBinaryMessage(QByteArray message);
	void OnSocketDisconnected();
	void OnPingClients();
	void OnPong();

private:
	bool m_blHasNewFrame;
	int m_Port;
	QImage m_Image;
	QMutex m_ImageMutex;
	QWidget *m_pRenderWidget;
	QWebSocketServer *m_pWebSocketServer;
	QList<QWebSocket *> m_Clients;
};

#endif // RENDERSERVER_H

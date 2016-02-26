#include "RenderServer.h"
#include <QTimer>
#include <QBuffer>

RenderServer::RenderServer(int Port, QWidget *pRenderWidget) :
	m_Port(0),
	m_pRenderWidget(pRenderWidget),
	m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
											QWebSocketServer::NonSecureMode, this)),
	m_Clients()
{
	m_blHasNewFrame = false;

	connect(this, SIGNAL(SigHasNewFrame()),
			this, SLOT(OnProcessNewFrame()));

	if (m_pWebSocketServer->listen(QHostAddress::Any, Port)) {
		qDebug() << "Server listening on port" << Port;

		connect(m_pWebSocketServer, SIGNAL(newConnection()),
				this, SLOT(OnNewConnection()));
        //connect(m_pWebSocketServer, SIGNAL(closed()), this, SLOT(SigClosed()));

		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(OnPingClients()));
		timer->start(50000);
		m_Port = m_pWebSocketServer->serverPort();
	}
}

RenderServer::~RenderServer()
{
	m_pWebSocketServer->close();
	qDeleteAll(m_Clients);
}

void RenderServer::SetNewFrame(const QImage &Img)
{
	m_ImageMutex.lock();

	m_blHasNewFrame = true;
	m_Image = Img;

	m_ImageMutex.unlock();

	emit SigHasNewFrame();
}

void RenderServer::OnProcessNewFrame()
{
	QByteArray ByteArray;
	QBuffer Buffer(&ByteArray);
	Buffer.open(QIODevice::WriteOnly);


	m_ImageMutex.lock();

	if (false == m_blHasNewFrame) {
		m_ImageMutex.unlock();
		return;
	}

	m_Image.save(&Buffer, "PNG");
	m_blHasNewFrame = false;

	m_ImageMutex.unlock();

	foreach (QWebSocket *pSocket, m_Clients) {
		pSocket->sendTextMessage(QString(ByteArray.toBase64()));
		pSocket->flush();
	}
}

void RenderServer::OnNewConnection()
{
	qDebug() << "Server got new connection";
	QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
	connect(pSocket, SIGNAL(textMessageReceived()), this, SLOT(OnProcessTextMessage()));
	connect(pSocket, SIGNAL(binaryMessageReceived()), this, SLOT(OnProcessBinaryMessage()));
	connect(pSocket, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
	connect(pSocket, SIGNAL(pong()), this, SLOT(OnPong()));

	m_Clients << pSocket;
}

void RenderServer::OnPingClients()
{
	if (false == m_Clients.empty()) {
		qDebug() << "Ping clients: " << m_Clients.length();
		foreach (QWebSocket *pSocket, m_Clients) {
			pSocket->ping();
		}
	}
}

void RenderServer::OnPong()
{
}

void RenderServer::OnProcessTextMessage(QString strMsg)
{
	qDebug() << "Server got new message:" << strMsg;

	foreach (QWebSocket *pSocket, m_Clients) {
		pSocket->sendTextMessage(strMsg);;
	}
}

void RenderServer::OnProcessBinaryMessage(QByteArray Msg)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (pClient) {
        pClient->sendBinaryMessage(Msg);
	}
}

void RenderServer::OnSocketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (pClient) {
		m_Clients.removeAll(pClient);
		pClient->deleteLater();
	}
}


#include "TcpThread.h"

TcpThread::TcpThread()
{
	clientIsConnected = false;
}

TcpThread::~TcpThread()
{
	tcpServer->close();

}

void TcpThread::socketSend(QString sendStr)
{
	qDebug() << "tcp send thread:" << QThread::currentThreadId();
	if (!clientIsConnected)
	{
		qDebug() << "�ͻ���δ����!";
		return;
	}
	clientTcpSocket->write(sendStr.toStdString().c_str());
}

void TcpThread::startTcp()
{
	qDebug() << "startTcp thread:"<<QThread::currentThreadId();
	tcpServer = new QTcpServer();
	if (!tcpServer->listen(QHostAddress::Any, 6666))//��������
	{
		//�˿ں�Ϊ6666�������������������Ϣ
		qDebug() << tcpServer->errorString();
		return;
	}
	GlobaData::statusText = GlobaData::statusText + "waiting for connecting...";
	connect(tcpServer, &QTcpServer::newConnection, this, &TcpThread::socketConnet);

}

void TcpThread::closeServer()
{
	qDebug() << "closeServer";
	tcpServer->close();
}

void TcpThread::onSendStr(QString str)
{
	socketSend(str);
}

void TcpThread::socketConnet()
{
	//���client socket
	clientTcpSocket = tcpServer->nextPendingConnection();
	//���źŲۣ��������ݣ����ҵ����ӹر���ɾ������
	connect(clientTcpSocket, SIGNAL(readyRead()), this, SLOT(socketReceive()));
	connect(clientTcpSocket, SIGNAL(disconnected()), clientTcpSocket, SLOT(deleteLater()));
	connect(clientTcpSocket, &QTcpSocket::disconnected, this, [=]() {
		clientIsConnected = false; 
		qDebug() << "clientIsConnected :" << clientIsConnected;
	});
	//��ʾ�ͻ���������Ϣ
	QString clientIp = clientTcpSocket->peerAddress().toString();
	QString clientPort = QString::number(clientTcpSocket->peerPort());
	GlobaData::statusText = "conneted with " + clientIp + ":" + clientPort ;
	qDebug() << GlobaData::statusText;
	clientIsConnected = true;
	qDebug() << "clientIsConnected :" << clientIsConnected;
}

void TcpThread::socketReceive()
{
	//�������ݲ���ʾ���ֽ�ת�������ַ���
	QString recvStr = clientTcpSocket->readAll();
	qDebug() <<clientTcpSocket->peerAddress().toString() <<":"<<"send:"<< recvStr;
	//��������ͻ�ȥ
	socketSend("From server: " + recvStr);
}

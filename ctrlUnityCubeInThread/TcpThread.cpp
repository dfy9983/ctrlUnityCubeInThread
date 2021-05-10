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
		qDebug() << "客户端未连接!";
		return;
	}
	clientTcpSocket->write(sendStr.toStdString().c_str());
}

void TcpThread::startTcp()
{
	qDebug() << "startTcp thread:"<<QThread::currentThreadId();
	tcpServer = new QTcpServer();
	if (!tcpServer->listen(QHostAddress::Any, 6666))//监听所有
	{
		//端口号为6666，如果出错就输出错误信息
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
	//获得client socket
	clientTcpSocket = tcpServer->nextPendingConnection();
	//绑定信号槽，接收数据，并且当连接关闭是删除连接
	connect(clientTcpSocket, SIGNAL(readyRead()), this, SLOT(socketReceive()));
	connect(clientTcpSocket, SIGNAL(disconnected()), clientTcpSocket, SLOT(deleteLater()));
	connect(clientTcpSocket, &QTcpSocket::disconnected, this, [=]() {
		clientIsConnected = false; 
		qDebug() << "clientIsConnected :" << clientIsConnected;
	});
	//显示客户端连接信息
	QString clientIp = clientTcpSocket->peerAddress().toString();
	QString clientPort = QString::number(clientTcpSocket->peerPort());
	GlobaData::statusText = "conneted with " + clientIp + ":" + clientPort ;
	qDebug() << GlobaData::statusText;
	clientIsConnected = true;
	qDebug() << "clientIsConnected :" << clientIsConnected;
}

void TcpThread::socketReceive()
{
	//接收数据并显示，字节转换成了字符串
	QString recvStr = clientTcpSocket->readAll();
	qDebug() <<clientTcpSocket->peerAddress().toString() <<":"<<"send:"<< recvStr;
	//经处理后发送回去
	socketSend("From server: " + recvStr);
}

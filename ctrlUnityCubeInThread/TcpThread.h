#pragma once
#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "GlobaData.h"
class TcpThread :public QObject
{
	Q_OBJECT
public:
	TcpThread();
	~TcpThread();
	void socketSend(QString sendStr);
public slots:
	void startTcp();
	void closeServer();
	void onSendStr(QString str);
private:
	QTcpServer *tcpServer; //服务器
	QTcpSocket *clientTcpSocket; //客户端socket
	bool clientIsConnected;//应对正常关闭连接的情况
private slots:
	void socketConnet();
	void socketReceive();

};


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
	QTcpServer *tcpServer; //������
	QTcpSocket *clientTcpSocket; //�ͻ���socket
	bool clientIsConnected;//Ӧ�������ر����ӵ����
private slots:
	void socketConnet();
	void socketReceive();

};


#include "ctrlunitycubeinthread.h"

CtrlUnityCubeInThread::CtrlUnityCubeInThread(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	qDebug() << "main thread:" << QThread::currentThreadId();
	tcpQthreadIsStarted = false;




	
}

CtrlUnityCubeInThread::~CtrlUnityCubeInThread()
{
	if (tcpQthreadIsStarted)
	{
		tcpQthread->quit();
		tcpQthread->wait();
	}

}

void CtrlUnityCubeInThread::on_leftBtn_clicked()
{
	qDebug() << "left ";
	//myTcp->socketSend("leftrotate");//试验1
	emit sendStr("leftrotate");
}

void CtrlUnityCubeInThread::on_rightBtn_clicked()
{
	qDebug() << "right ";
	emit sendStr("rightrotate");//试验2 通过信号发送
}

void CtrlUnityCubeInThread::on_startThreadBtn_clicked()
{
	qDebug() << "ThreadBtn clicked";

	if (tcpQthreadIsStarted)
	{

		tcpQthread->quit();
		tcpQthread->wait();
		qDebug() << "stop tcpQthread";
		tcpQthreadIsStarted = false;
		ui.startThreadBtn->setText("startThread");
	}
	else 
	{
		tcpQthread = new QThread();
		myTcp = new TcpThread();
		myTcp->moveToThread(tcpQthread);
		connect(tcpQthread, &QThread::started, myTcp, &TcpThread::startTcp);
		connect(tcpQthread, &QThread::finished, myTcp, &QObject::deleteLater);
		connect(tcpQthread, &QThread::finished, tcpQthread, &QObject::deleteLater);
		connect(this, &CtrlUnityCubeInThread::sendStr, myTcp, &TcpThread::onSendStr);
		connect(tcpQthread, &QThread::finished, myTcp, &TcpThread::closeServer);
		tcpQthread->start();
		tcpQthreadIsStarted = true;
		ui.startThreadBtn->setText("stopThread");
	}
}


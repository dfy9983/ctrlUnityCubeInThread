#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ctrlunitycubeinthread.h"
#include "TcpThread.h"
class CtrlUnityCubeInThread : public QMainWindow
{
    Q_OBJECT

public:
    CtrlUnityCubeInThread(QWidget *parent = Q_NULLPTR);
	~CtrlUnityCubeInThread();
signals:
	void sendStr(QString str);

private slots:
	void on_leftBtn_clicked();
	void on_rightBtn_clicked();
	void on_startThreadBtn_clicked();
private:
    Ui::CtrlUnityCubeInThreadClass ui;
	TcpThread *myTcp;
	QThread * tcpQthread;
	bool tcpQthreadIsStarted;
};

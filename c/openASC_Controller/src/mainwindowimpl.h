#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rotatordialog.h"
#include "commclass.h"
#include "settingsdialog.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QTimer>

#define INTERFACE_TYPE_SERIAL	1
#define INTERFACE_TYPE_TCP		2

typedef struct {
	unsigned char currentBand;
	unsigned char currentAntennas;
	unsigned char currentRXAntennas;
} status_struct;

class MainWindowImpl : public QMainWindow, public Ui::MainWindowImpl {
Q_OBJECT
public:
		MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		RotatorDialog *rotatorWindow;
		SettingsDialog *settingsDialog;
		CommClass comm;
		void pushButtonPressed(unsigned char button);
private:
		int interfaceType;
		QTimer *timerPollRXQueue;
		QTimer *timerPollStatus;
		void getRXAntennaInfo();
		void getTXAntennaInfo();
		status_struct status;
protected:
private slots:
public slots:
		void WindowRotatorsTriggered();
		void actionConnectTriggered();
		void actionDisconnectTriggered();
		void pushButtonTX1Clicked(bool state);
		void pushButtonTX2Clicked(bool state);
		void pushButtonTX3Clicked(bool state);
		void pushButtonTX4Clicked(bool state);

		void comboBoxBandIndexChanged(int index);

		void pushButtonRXAnt1Clicked(bool state);
		void pushButtonRXAnt2Clicked(bool state);
		void pushButtonRXAnt3Clicked(bool state);
		void pushButtonRXAnt4Clicked(bool state);
		void pushButtonRXAnt5Clicked(bool state);
		void pushButtonRXAnt6Clicked(bool state);
		void pushButtonRXAnt7Clicked(bool state);
		void pushButtonRXAnt8Clicked(bool state);
		void pushButtonRXAnt9Clicked(bool state);
		void pushButtonRXAnt10Clicked(bool state);

		void pushButtonRXAntClicked(bool state);

		void timerPollRXQueueUpdate();
		void timerPollStatusUpdate();

		void actionSettingsEditTriggered();
};
#endif






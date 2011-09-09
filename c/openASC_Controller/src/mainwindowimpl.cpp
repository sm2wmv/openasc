#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"
//#include "commclass.h"

void MainWindowImpl::WindowRotatorsTriggered() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else
		rotatorWindow->show();
}
  
void MainWindowImpl::pushButtonPressed(unsigned char button) {
}

void MainWindowImpl::pushButtonTX1Clicked(bool state) {
}

void MainWindowImpl::pushButtonTX2Clicked(bool state) {
}

void MainWindowImpl::pushButtonTX3Clicked(bool state) {
}

void MainWindowImpl::pushButtonTX4Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt1Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt2Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt3Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt4Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt5Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt6Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt7Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt8Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt9Clicked(bool state) {
}

void MainWindowImpl::pushButtonRXAnt10Clicked(bool state) {
}

void MainWindowImpl::actionSettingsEditTriggered() {
		settingsDialog->show();
}

void MainWindowImpl::actionDisconnectTriggered() {
}

void MainWindowImpl::getRXAntennaInfo() {
}

void MainWindowImpl::getTXAntennaInfo() {
}

void MainWindowImpl::actionConnectTriggered() {
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {

}

void MainWindowImpl::timerPollStatusUpdate(void) {
}

void MainWindowImpl::pushButtonRXAntClicked(bool state) {
}

void MainWindowImpl::timerPollRXQueueUpdate(void) {

}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	status.currentAntennas = 0;
	status.currentBand = 0;
	status.currentRXAntennas = 0;

	settingsDialog = new SettingsDialog(this);
	settingsDialog->hide();

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();

	timerPollRXQueue = new QTimer(this);
	connect(timerPollRXQueue, SIGNAL(timeout()), this, SLOT(timerPollRXQueueUpdate()));

	timerPollStatus = new QTimer(this);
	connect(timerPollStatus, SIGNAL(timeout()), this, SLOT(timerPollStatusUpdate()));

	connect(actionSettingsEdit, SIGNAL(triggered()), this, SLOT(actionSettingsEditTriggered()));
	connect(actionWindowsRotators, SIGNAL(triggered()), this, SLOT(WindowRotatorsTriggered()));
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
        connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));

	connect(comboBoxBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxBandIndexChanged(int)));

	connect(pushButtonTX1, SIGNAL(clicked(bool)), this, SLOT(pushButtonTX1Clicked(bool)));
	connect(pushButtonTX2, SIGNAL(clicked(bool)), this, SLOT(pushButtonTX2Clicked(bool)));
	connect(pushButtonTX3, SIGNAL(clicked(bool)), this, SLOT(pushButtonTX3Clicked(bool)));
	connect(pushButtonTX4, SIGNAL(clicked(bool)), this, SLOT(pushButtonTX4Clicked(bool)));

	connect(pushButtonRXAnt1, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt1Clicked(bool)));
	connect(pushButtonRXAnt2, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt2Clicked(bool)));
	connect(pushButtonRXAnt3, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt3Clicked(bool)));
	connect(pushButtonRXAnt4, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt4Clicked(bool)));
	connect(pushButtonRXAnt5, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt5Clicked(bool)));
	connect(pushButtonRXAnt6, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt6Clicked(bool)));
	connect(pushButtonRXAnt7, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt7Clicked(bool)));
	connect(pushButtonRXAnt8, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt8Clicked(bool)));
	connect(pushButtonRXAnt9, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt9Clicked(bool)));
	connect(pushButtonRXAnt10, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAnt10Clicked(bool)));

	connect(pushButtonRXAnt, SIGNAL(clicked(bool)), this, SLOT(pushButtonRXAntClicked(bool)));
}

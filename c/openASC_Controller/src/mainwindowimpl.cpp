#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"

//! Character for enter
#define CHAR_ENTER 13
//! Character for space
#define CHAR_SPACE 32
//! Character for ESC
#define CHAR_ESC 27
//! Character for backspace
#define CHAR_BS 8


void MainWindowImpl::WindowRotatorsTriggered() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else {
		rotatorWindow->loadBand(5);
		rotatorWindow->show();
	}
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
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 1\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt2Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 2\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt3Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 3\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt4Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 4\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt5Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 5\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt6Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 6\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt7Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 7\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt8Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 8\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt9Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 9\r");
		comm->addTXMessage("getrxant\r");
	}
}

void MainWindowImpl::pushButtonRXAnt10Clicked(bool state) {
	if (comm->isOpen()) {
		comm->addTXMessage("rxant 10\r");
		comm->addTXMessage("getrxant\r");
	}
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
	if (settingsDialog->getDeviceInterfaceType() == INTERFACE_TYPE_SERIAL) {
		comm->setInterfaceType(INTERFACE_TYPE_SERIAL);
		comm->setSerialPort(settingsDialog->getCOMDeviceName());

		if (comm->openPort() == 1)
			QMessageBox::critical(0,"Serial port error!","Could not open the specified port!");
		else {
			comm->start();
			timerPollRXQueue->start();
			timerPollStatus->start();
		}
	}
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {

}

void MainWindowImpl::timerPollStatusUpdate(void) {
	if (comm->isOpen()) {

	}
}

void MainWindowImpl::pushButtonRXAntClicked(bool state) {
	if (comm->isOpen()) {

	}

}

void MainWindowImpl::timerPollRXQueueUpdate(void) {

	if (comm->getRXQueueCount() > 0) {

	}
}

void MainWindowImpl::parseCommData(QString attr, QString data) {
	if (attr == "ack") { //Indicates that its OK to send the next request
		comm->messageAcked();
	}
	else if (attr == "band") {
		labelCurrentBandInfo->setText(data);

		if (data == "160m")
			currentBandIndex = 1;
		else if (data == "80m")
			currentBandIndex = 2;
		else if (data == "40m")
			currentBandIndex = 3;
		else if (data == "30m")
			currentBandIndex = 4;
		else if (data == "20m")
			currentBandIndex = 5;
		else if (data == "17m")
			currentBandIndex = 6;
		else if (data == "15m")
			currentBandIndex = 7;
		else if (data == "12m")
			currentBandIndex = 8;
		else if (data == "10m")
			currentBandIndex = 9;
		else
			currentBandIndex = 0;

		currentBandName = data;

		if (data != lastBand) {
			rotatorWindow->loadBand(currentBandIndex);

			//comm->addTXMessage("getants");
			lastBand = data;
		}
	}
	else if (attr == "ant1") {
		labelAnt1->setText(data);
	}
	else if (attr == "ant2") {
		labelAnt2->setText(data);
	}
	else if (attr == "ant3") {
		labelAnt3->setText(data);
	}
	else if (attr == "ant4") {
		labelAnt4->setText(data);
	}
	else if (attr == "dir1") {
		rotatorWindow->setRotatorAngle(0,data.toInt());
		labelAnt1Dir->setText(data +'°');
	}
	else if (attr == "dir2") {
		rotatorWindow->setRotatorAngle(1,data.toInt());
		labelAnt2Dir->setText(data +'°');
	}
	else if (attr == "dir3") {
		rotatorWindow->setRotatorAngle(2,data.toInt());
		labelAnt3Dir->setText(data +'°');
	}
	else if (attr == "dir4") {
		rotatorWindow->setRotatorAngle(3,data.toInt());
		labelAnt4Dir->setText(data +'°');
	}
	else if (attr == "rxant") {
		labelCurrentRXAntenna->setText(data);
	}
	else if (attr == "rxant1") {
		pushButtonRXAnt1->setText(data);
	}
	else if (attr == "rxant2") {
		pushButtonRXAnt2->setText(data);
	}
	else if (attr == "rxant3") {
		pushButtonRXAnt3->setText(data);
	}
	else if (attr == "rxant4") {
		pushButtonRXAnt4->setText(data);
	}
	else if (attr == "rxant5") {
		pushButtonRXAnt5->setText(data);
	}
	else if (attr == "rxant6") {
		pushButtonRXAnt6->setText(data);
	}
	else if (attr == "rxant7") {
		pushButtonRXAnt7->setText(data);
	}
	else if (attr == "rxant8") {
		pushButtonRXAnt8->setText(data);
	}
	else if (attr == "rxant9") {
		pushButtonRXAnt9->setText(data);
	}
	else if (attr == "rxant10") {
		pushButtonRXAnt10->setText(data);
	}
}

QString MainWindowImpl::getCurrentBandName() {
	return(currentBandName);
}

int MainWindowImpl::getCurrentBandIndex() {
	return(currentBandIndex);
}

void MainWindowImpl::lineEditCommandPressed(void) {
	if (comm->isOpen()) {
		comm->sendText(lineEditCommand->text()+CHAR_ENTER);
		lineEditCommand->clear();
	}
}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	status.currentAntennas = 0;
	status.currentBand = 0;
	status.currentRXAntennas = 0;

	settingsDialog = new SettingsDialog(this);
	settingsDialog->hide();

	comm = new CommClass();

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();
	rotatorWindow->setCommPtr(comm);


	timerPollRXQueue = new QTimer(this);
	timerPollRXQueue->setInterval(10);
	connect(timerPollRXQueue, SIGNAL(timeout()), this, SLOT(timerPollRXQueueUpdate()));

	timerPollStatus = new QTimer(this);
	timerPollStatus->setInterval(1000);
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
	connect(lineEditCommand, SIGNAL(returnPressed()),this, SLOT(lineEditCommandPressed()));
}

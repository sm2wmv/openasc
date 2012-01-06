#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"
#include "commclass.h"
#include "terminaldialog.h"
#include "../../../uC/remote_commands.h"
#include "../../../uC/ext_events.h"

QPalette antSelPal(QColor(Qt::darkGreen));
QPalette antNotSelPal(QColor(Qt::white));

QString MainWindowImpl::getBandName(int bandIndex) {
	QString bandName = "None";

	switch(bandIndex) {
		case 0: bandName="None";
			break;
		case 1: bandName = "160m";
			break;
		case 2: bandName = "80m";
			break;
		case 3: bandName = "40m";
			break;
		case 4: bandName = "30m";
			break;
		case 5: bandName = "20m";
			break;
		case 6: bandName = "17m";
			break;
		case 7: bandName = "15m";
			break;
		case 8: bandName = "12m";
			break;
		case 9: bandName = "10m";
			break;
	}

	return(bandName);
}

void MainWindowImpl::WindowRotatorsTriggered() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else
		rotatorWindow->show();
}
  
void MainWindowImpl::pushButtonPressed(unsigned char button) {
}

void MainWindowImpl::pushButtonTX1Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT1);
}

void MainWindowImpl::pushButtonTX2Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT2);
}

void MainWindowImpl::pushButtonTX3Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT3);
}

void MainWindowImpl::pushButtonTX4Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT4);
}

void MainWindowImpl::pushButtonRXAnt1Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT1);
}

void MainWindowImpl::pushButtonRXAnt2Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT2);
}

void MainWindowImpl::pushButtonRXAnt3Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT3);
}

void MainWindowImpl::pushButtonRXAnt4Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT4);
}

void MainWindowImpl::pushButtonRXAnt5Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT5);
}

void MainWindowImpl::pushButtonRXAnt6Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT6);
}

void MainWindowImpl::pushButtonRXAnt7Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT7);
}

void MainWindowImpl::pushButtonRXAnt8Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT8);
}

void MainWindowImpl::pushButtonRXAnt9Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT9);
}

void MainWindowImpl::pushButtonRXAnt10Clicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT10);
}

void MainWindowImpl::actionSettingsEditTriggered() {
	settingsDialog->show();
}

void MainWindowImpl::actionDisconnectTriggered() {
	timerPollStatus->stop();
	serial->closePort();

	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);
}

void MainWindowImpl::getRXAntennaInfo() {
}

void MainWindowImpl::getTXAntennaInfo() {
}

void MainWindowImpl::actionConnectTriggered() {
  qDebug("Opened serial port");
	if (serial->openPort(settingsDialog->getCOMDeviceName(),BAUD19200)) {
		qDebug("Failed to open serial device");
	}
	else {
		serial->start();

		actionConnect->setEnabled(false);
		actionDisconnect->setEnabled(true);

		timerPollRXQueue->setInterval(1);
		timerPollRXQueue->start();

		serial->addTXMessage(COMPUTER_COMM_REMOTE_SET_STATUS,1);

		timerPollStatus->setInterval(100);
		timerPollStatus->start();
	}
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_CHANGE_BAND,index);
}

void MainWindowImpl::timerPollStatusUpdate(void) {
	if (rotatorWindow->getRotationEventStatus() != 0) {
		char temp[3];
		temp[0] = rotatorWindow->getActiveAntenna();
		temp[1] = ((rotatorWindow->getTargetDir(rotatorWindow->getActiveAntenna()) & 0xFF00) >> 8);
		temp[2] = (rotatorWindow->getTargetDir(rotatorWindow->getActiveAntenna()) & 0x00FF);

		serial->addTXMessage(COMPUTER_COMM_REMOTE_ROTATE_ANTENNA,3,temp);
	}
}

void MainWindowImpl::pushButtonRXAntClicked() {
	if (serial->isOpen())
		serial->addTXMessage(COMPUTER_COMM_REMOTE_BUTTON_EVENT,EXT_CTRL_TOGGLE_RX_ANT_MODE);
}

void MainWindowImpl::parseMessage(struct_message message) {
	if (message.cmd == COMPUTER_COMM_REMOTE_BAND_INFO) {
		if (message.length > 10) {
			if (status.currentBand != message.data[0]) {
				labelAnt1->setText("");
				labelAnt2->setText("");
				labelAnt3->setText("");
				labelAnt4->setText("");
				labelAnt1Dir->setText("");
				labelAnt2Dir->setText("");
				labelAnt3Dir->setText("");
				labelAnt4Dir->setText("");

				status.currentBand = message.data[0];
				labelBand->setText("Band: "+getBandName(message.data[0]));
				//comboBoxBand->setCurrentIndex(message.data[0]);

				if (status.currentBand == 2) {
					status.subMenuType[0] = 1;
				}
			}

			unsigned int temp_error = (message.data[9] << 8) + message.data[10];

			if (temp_error != status.currentErrors) {
				status.currentErrors = temp_error;
				errorDialog->setErrorList(status.currentErrors);
				errorDialog->show();
			}

			if (message.data[1] != status.currentAntennas) {
				status.currentAntennas = message.data[1];

				if (status.currentAntennas & (1<<0)) {
					pushButtonTX1->setChecked(true);
					pushButtonTX1->setPalette(antSelPal);
				}
				else {
					pushButtonTX1->setChecked(false);
					pushButtonTX1->setPalette(antNotSelPal);
				}

				if (status.currentAntennas & (1<<1)) {
					pushButtonTX2->setChecked(true);
					pushButtonTX2->setPalette(antSelPal);
				}
				else {
					pushButtonTX2->setChecked(false);
					pushButtonTX2->setPalette(antNotSelPal);
				}

				if (status.currentAntennas & (1<<2)) {
					pushButtonTX3->setChecked(true);
					pushButtonTX3->setPalette(antSelPal);
				}
				else {
					pushButtonTX3->setChecked(false);
					pushButtonTX3->setPalette(antNotSelPal);
				}

				if (status.currentAntennas & (1<<3)) {
					pushButtonTX4->setChecked(true);
					pushButtonTX4->setPalette(antSelPal);
				}
				else {
					pushButtonTX4->setChecked(false);
					pushButtonTX4->setPalette(antNotSelPal);
				}
			}

			rotatorWindow->loadBand(message.data[0]);

			if (message.data[4] & (1<<0))	{ //RX ANT MODE STATUS
				pushButtonRXAnt->setChecked(true);

				if (message.data[2] == 0)
					labelCurrentRXAntenna->setText("None");
				else if (message.data[2] == 1)
					labelCurrentRXAntenna->setText(pushButtonRXAnt1->text());
				else if (message.data[2] == 2)
					labelCurrentRXAntenna->setText(pushButtonRXAnt2->text());
				else if (message.data[2] == 3)
					labelCurrentRXAntenna->setText(pushButtonRXAnt3->text());
				else if (message.data[2] == 4)
					labelCurrentRXAntenna->setText(pushButtonRXAnt4->text());
				else if (message.data[2] == 5)
					labelCurrentRXAntenna->setText(pushButtonRXAnt5->text());
				else if (message.data[2] == 6)
					labelCurrentRXAntenna->setText(pushButtonRXAnt6->text());
				else if (message.data[2] == 7)
					labelCurrentRXAntenna->setText(pushButtonRXAnt7->text());
				else if (message.data[2] == 8)
					labelCurrentRXAntenna->setText(pushButtonRXAnt8->text());
				else if (message.data[2] == 9)
					labelCurrentRXAntenna->setText(pushButtonRXAnt9->text());
				else if (message.data[2] == 10)
					labelCurrentRXAntenna->setText(pushButtonRXAnt10->text());
			}
			else {
				pushButtonRXAnt->setChecked(false);
				labelCurrentRXAntenna->setText("None");
			}

			status.antSubOptSelected[0] = message.data[5];
			status.antSubOptSelected[1] = message.data[6];
			status.antSubOptSelected[2] = message.data[7];
			status.antSubOptSelected[3] = message.data[8];

			if (status.subMenuType[0] != 0) {
				if (status.antSubOptSelected[0] == 0)
					labelAnt1Dir->setText("SW");
				else if (status.antSubOptSelected[0] == 1)
					labelAnt1Dir->setText("NW");
				else if (status.antSubOptSelected[0] == 2)
					labelAnt1Dir->setText("NE");
				else if (status.antSubOptSelected[0] == 3)
					labelAnt1Dir->setText("SE");
			}
		}
	}
	else if (message.cmd == COMPUTER_COMM_REMOTE_ANT_TEXT) {
		QString antText = QString::fromLocal8Bit(message.data+1);

		if (message.data[0] == 0)
			labelAnt1->setText(antText);
		else if (message.data[0] == 1)
			labelAnt2->setText(antText);
		else if (message.data[0] == 2)
			labelAnt3->setText(antText);
		else if (message.data[0] == 3)
			labelAnt4->setText(antText);
	}
	else if (message.cmd == COMPUTER_COMM_REMOTE_RXANT_TEXT){
		if (message.length > 0) {
			QString rxAntText = QString::fromLocal8Bit(message.data+1);

			if (message.data[0] == 0)
				pushButtonRXAnt1->setText(rxAntText);
			else if (message.data[0] == 1)
				pushButtonRXAnt2->setText(rxAntText);
			else if (message.data[0] == 2)
				pushButtonRXAnt3->setText(rxAntText);
			else if (message.data[0] == 3)
				pushButtonRXAnt4->setText(rxAntText);
			else if (message.data[0] == 4)
				pushButtonRXAnt5->setText(rxAntText);
			else if (message.data[0] == 5)
				pushButtonRXAnt6->setText(rxAntText);
			else if (message.data[0] == 6)
				pushButtonRXAnt7->setText(rxAntText);
			else if (message.data[0] == 7)
				pushButtonRXAnt8->setText(rxAntText);
			else if (message.data[0] == 8)
				pushButtonRXAnt9->setText(rxAntText);
			else if (message.data[0] == 9)
				pushButtonRXAnt10->setText(rxAntText);
		}
	}
	else if (message.cmd == COMPUTER_COMM_REMOTE_ANT_DIR_INFO) {
		if (message.length > 3) {
			unsigned int dir = 0;
			dir += ((int)(message.data[1])) << 8;
			dir += (int)(message.data[2]) & 0x00FF;

			if (message.data[0] == 0) {
				if (status.subMenuType[0] == 0)
					labelAnt1Dir->setText(QString::number(dir)+'°');
			}
			
			if (status.subMenuType[1] == 0) {
				if (message.data[0] == 1)
					labelAnt2Dir->setText(QString::number(dir)+'°');
			}

			if (status.subMenuType[2] == 0) {
				if (message.data[0] == 2)
					labelAnt3Dir->setText(QString::number(dir)+'°');
			}

			if (status.subMenuType[3] == 0) {
				if (message.data[0] == 3)
					labelAnt4Dir->setText(QString::number(dir)+'°');
			}

			rotatorWindow->setRotatorFlag(message.data[0],message.data[3]);
			rotatorWindow->setRotatorAngle(message.data[0],dir);
		}
	}
	else if (message.cmd == INT_COMM_PC_SEND_TO_ADDR) {
		terminalWindow->addNewMessage(message);
	}
	else if (message.cmd == COMPUTER_COMM_REMOTE_ANT_INFO) {
		if (message.length > 7) {
			status.antennaFlags[0] = message.data[0];
			status.antennaFlags[1] = message.data[1];
			status.antennaFlags[2] = message.data[2];
			status.antennaFlags[3] = message.data[3];

			status.subMenuType[0] = message.data[4];
			status.subMenuType[1] = message.data[5];
			status.subMenuType[2] = message.data[6];
			status.subMenuType[3] = message.data[7];

			if (status.antennaFlags[0] & (1<<0)) {
				pushButtonTX1->setEnabled(true);
				pushButtonRX1->setEnabled(true);
			}
			else {
				pushButtonTX1->setEnabled(false);
				pushButtonRX1->setEnabled(false);
				labelAnt1->clear();
				labelAnt1Dir->clear();
			}

			if (status.antennaFlags[1] & (1<<0)) {
				pushButtonTX2->setEnabled(true);
				pushButtonRX2->setEnabled(true);
			}
			else {
				pushButtonTX2->setEnabled(false);
				pushButtonRX2->setEnabled(false);
				labelAnt2->clear();
				labelAnt2Dir->clear();
			}

			if (status.antennaFlags[2] & (1<<0)) {
				pushButtonTX3->setEnabled(true);
				pushButtonRX3->setEnabled(true);
			}
			else {
				pushButtonTX3->setEnabled(false);
				pushButtonRX3->setEnabled(false);
				labelAnt3->clear();
				labelAnt3Dir->clear();
			}

			if (status.antennaFlags[3] & (1<<0)) {
				pushButtonTX4->setEnabled(true);
				pushButtonRX4->setEnabled(true);
			}
			else {
				pushButtonTX4->setEnabled(false);
				pushButtonRX4->setEnabled(false);
				labelAnt4->clear();
				labelAnt4Dir->clear();
			}
		}
	}
	else if (message.cmd == COMPUTER_COMM_REMOTE_SUBMENU_ARRAY_TEXT) {
		qDebug("GOT ARRAY INFO");
	}
}

void MainWindowImpl::actionTerminalTriggered() {
	terminalWindow->show();
}

void MainWindowImpl::timerPollRXQueueUpdate(void) {
	if (serial->getRXQueueSize() > 0) {
		struct_message message = serial->getRXQueueFirst();

	//	qDebug() << "MESSAGE: " << message.cmd;

		parseMessage(message);
	}
}

void MainWindowImpl::actionErrorDialogTriggered() {
	errorDialog->show();
}

void MainWindowImpl::pushButtonSubMenu1Clicked() {

}

void MainWindowImpl::pushButtonSubMenu2Clicked() {

}

void MainWindowImpl::pushButtonSubMenu3Clicked() {

}

void MainWindowImpl::pushButtonSubMenu4Clicked() {

}

void MainWindowImpl::pushButtonSubMenuArray1Clicked() {
	if (serial->isOpen())	{
		char data[2] = {0,0};
		serial->addTXMessage(COMPUTER_COMM_SET_SUBMENU_OPTION,2,data);
	}
}

void MainWindowImpl::pushButtonSubMenuArray2Clicked() {
	if (serial->isOpen())	{
		char data[2] = {0,1};
		serial->addTXMessage(COMPUTER_COMM_SET_SUBMENU_OPTION,2,data);
	}
}

void MainWindowImpl::pushButtonSubMenuArray3Clicked() {
	if (serial->isOpen())	{
		char data[2] = {0,2};
		serial->addTXMessage(COMPUTER_COMM_SET_SUBMENU_OPTION,2,data);
	}
}

void MainWindowImpl::pushButtonSubMenuArray4Clicked() {
	if (serial->isOpen())	{
		char data[2] = {0,3};
		serial->addTXMessage(COMPUTER_COMM_SET_SUBMENU_OPTION,2,data);
	}
}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	status.currentAntennas = 0;
	status.currentBand = 0;
	status.currentRXAntennas = 0;

	for (unsigned char i=0;i<4;i++) {
		status.antennaFlags[i] = 0;
		status.subMenuType[i] = 0;
	}

  settingsDialog = new SettingsDialog(this);
	settingsDialog->hide();

	terminalWindow = new terminalDialog(this);
	terminalWindow->hide();

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();

	serial = new CommClass();

	terminalWindow->setSerialPtr(serial);

	errorDialog = new ErrorDialog();
	errorDialog->hide();

	errorDialog->setSerialPtr(serial);

	bandChangedFlag = 0;

	timerPollRXQueue = new QTimer(this);
	connect(timerPollRXQueue, SIGNAL(timeout()), this, SLOT(timerPollRXQueueUpdate()));

	timerPollStatus = new QTimer(this);
	connect(timerPollStatus, SIGNAL(timeout()), this, SLOT(timerPollStatusUpdate()));

	connect(actionSettingsEdit, SIGNAL(triggered()), this, SLOT(actionSettingsEditTriggered()));
	connect(actionWindowsRotators, SIGNAL(triggered()), this, SLOT(WindowRotatorsTriggered()));
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
	connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));
	connect(actionTerminal, SIGNAL(triggered()), this, SLOT(actionTerminalTriggered()));
	connect(actionErrorDialog, SIGNAL(triggered()), this, SLOT(actionErrorDialogTriggered()));

	connect(comboBoxBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxBandIndexChanged(int)));

	connect(pushButtonTX1, SIGNAL(clicked()), this, SLOT(pushButtonTX1Clicked()));
	connect(pushButtonTX2, SIGNAL(clicked()), this, SLOT(pushButtonTX2Clicked()));
	connect(pushButtonTX3, SIGNAL(clicked()), this, SLOT(pushButtonTX3Clicked()));
	connect(pushButtonTX4, SIGNAL(clicked()), this, SLOT(pushButtonTX4Clicked()));

	connect(pushButtonRXAnt1, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt1Clicked()));
	connect(pushButtonRXAnt2, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt2Clicked()));
	connect(pushButtonRXAnt3, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt3Clicked()));
	connect(pushButtonRXAnt4, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt4Clicked()));
	connect(pushButtonRXAnt5, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt5Clicked()));
	connect(pushButtonRXAnt6, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt6Clicked()));
	connect(pushButtonRXAnt7, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt7Clicked()));
	connect(pushButtonRXAnt8, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt8Clicked()));
	connect(pushButtonRXAnt9, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt9Clicked()));
	connect(pushButtonRXAnt10, SIGNAL(clicked()), this, SLOT(pushButtonRXAnt10Clicked()));

	connect(pushButtonRXAnt, SIGNAL(clicked()), this, SLOT(pushButtonRXAntClicked()));

	connect(pushButtonSubMenu1, SIGNAL(clicked()), this, SLOT(pushButtonSubMenu1Clicked()));
	connect(pushButtonSubMenu2, SIGNAL(clicked()), this, SLOT(pushButtonSubMenu2Clicked()));
	connect(pushButtonSubMenu3, SIGNAL(clicked()), this, SLOT(pushButtonSubMenu3Clicked()));
	connect(pushButtonSubMenu4, SIGNAL(clicked()), this, SLOT(pushButtonSubMenu4Clicked()));

	connect(pushButtonSubMenuArray1, SIGNAL(clicked()), this, SLOT(pushButtonSubMenuArray1Clicked()));
	connect(pushButtonSubMenuArray2, SIGNAL(clicked()), this, SLOT(pushButtonSubMenuArray2Clicked()));
	connect(pushButtonSubMenuArray3, SIGNAL(clicked()), this, SLOT(pushButtonSubMenuArray3Clicked()));
	connect(pushButtonSubMenuArray4, SIGNAL(clicked()), this, SLOT(pushButtonSubMenuArray4Clicked()));
}

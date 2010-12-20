#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"
#include "commclass.h"

void MainWindowImpl::WindowRotatorsTriggered() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else
		rotatorWindow->show();
}
  
void MainWindowImpl::pushButtonPressed(unsigned char button) {
		unsigned char temp[3] = {REMOTE_CONTROL_BUTTON_PRESSED,1,button};

		comm.addTXMessage(INT_COMM_PC_CTRL,3,temp);
}

void MainWindowImpl::pushButtonTX1Clicked(bool state) {
		pushButtonPressed(EXT_CTRL_TOGGLE_TX_ANT1);

		pushButtonTX1->setChecked(false);
}

void MainWindowImpl::pushButtonTX2Clicked(bool state) {
		pushButtonPressed(EXT_CTRL_TOGGLE_TX_ANT2);

		pushButtonTX2->setChecked(false);
}

void MainWindowImpl::pushButtonTX3Clicked(bool state) {
		pushButtonPressed(EXT_CTRL_TOGGLE_TX_ANT3);

		pushButtonTX3->setChecked(false);
}

void MainWindowImpl::pushButtonTX4Clicked(bool state) {
		pushButtonPressed(EXT_CTRL_TOGGLE_TX_ANT4);

		pushButtonTX4->setChecked(false);
}

void MainWindowImpl::pushButtonRXAnt1Clicked(bool state) {
		//We never want to toggle the rx antenna button.
		//We wish to do that from the usart recieve input, that way
		//we have feedback from the openASC box.
						getTXAntennaInfo();

	pushButtonRXAnt1->setChecked(false);
		pushButtonPressed(EXT_CTRL_SEL_RX_ANT1);
}

void MainWindowImpl::pushButtonRXAnt2Clicked(bool state) {
		pushButtonRXAnt2->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT2);
}

void MainWindowImpl::pushButtonRXAnt3Clicked(bool state) {
		pushButtonRXAnt3->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT3);
}

void MainWindowImpl::pushButtonRXAnt4Clicked(bool state) {
		pushButtonRXAnt4->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT4);
}

void MainWindowImpl::pushButtonRXAnt5Clicked(bool state) {
		pushButtonRXAnt5->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT5);
}

void MainWindowImpl::pushButtonRXAnt6Clicked(bool state) {
		pushButtonRXAnt6->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT6);
}

void MainWindowImpl::pushButtonRXAnt7Clicked(bool state) {
		pushButtonRXAnt7->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT7);
}

void MainWindowImpl::pushButtonRXAnt8Clicked(bool state) {
		pushButtonRXAnt8->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT8);
}

void MainWindowImpl::pushButtonRXAnt9Clicked(bool state) {
		pushButtonRXAnt9->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT9);
}

void MainWindowImpl::pushButtonRXAnt10Clicked(bool state) {
		pushButtonRXAnt10->setChecked(false);

		pushButtonPressed(EXT_CTRL_SEL_RX_ANT10);
}

void MainWindowImpl::actionSettingsEditTriggered() {
		settingsDialog->show();
}

void MainWindowImpl::getRXAntennaInfo() {
	unsigned char temp[3] = {REMOTE_CONTROL_RX_ANT_TEXT,1,0};

	for (unsigned char i=0;i<10;i++) {
		temp[2] = i;
		comm.addTXMessage(INT_COMM_PC_CTRL,3,temp);
	}
}

void MainWindowImpl::getTXAntennaInfo() {
	unsigned char temp[3] = {REMOTE_CONTROL_ANT_TEXT,1,0};

	for (unsigned char i=0;i<4;i++) {
		temp[2] = i;
		comm.addTXMessage(INT_COMM_PC_CTRL,3,temp);
	}
}

void MainWindowImpl::actionConnectTriggered() {
	if (settingsDialog->getDeviceInterfaceType() == INTERFACE_TYPE_SERIAL) {
		if (comm.openPort(settingsDialog->getCOMDeviceName())) {
						QMessageBox::critical(0,"Serial port error!","Could not open the specified port!");
		}
		else {
						comm.start();
						actionDisconnect->setEnabled(true);
						actionConnect->setEnabled(false);

						timerPollRXQueue->start(1);
						timerPollStatus->start(500);

						getRXAntennaInfo();
						getTXAntennaInfo();


						//addDebugLine("Serial port opened: " + lineEditDevicePort->text());
		}
	}
	else if (settingsDialog->getDeviceInterfaceType() == INTERFACE_TYPE_TCP) {
		comm.openPort(settingsDialog->getNetworkIPAddress(),settingsDialog->getNetworkPort());
		comm.start();
	}
}

void MainWindowImpl::actionDisconnectTriggered() {
	if (comm.isRunning()) {
		comm.stopProcess();
		comm.closePort();
	}

	timerPollRXQueue->stop();
	timerPollStatus->stop();

	actionDisconnect->setEnabled(false);
	actionConnect->setEnabled(true);
}

void MainWindowImpl::timerPollRXQueueUpdate() {
	if (comm.getRXQueueSize() > 0) {
		QByteArray temp = comm.getRXQueueFirst();

		unsigned char cmd = (unsigned char)temp.at(0);
		unsigned char length = 0;

		length = (unsigned char)temp.at(1);
		unsigned char data[length];

		for (int i=0;i<length;i++)
			data[i] = temp.at(i+2);

		if (cmd == REMOTE_CONTROL_RX_ANT_TEXT) {
			QString temp;
			for (int i=1;i<length;i++)
				temp.append(data[i]);

			if (data[0] == 0x00)
				pushButtonRXAnt1->setText(temp);
			else if (data[0] == 0x01)
				pushButtonRXAnt2->setText(temp);
			else if (data[0] == 0x02)
				pushButtonRXAnt3->setText(temp);
			else if (data[0] == 0x03)
				pushButtonRXAnt4->setText(temp);
			else if (data[0] == 0x04)
				pushButtonRXAnt5->setText(temp);
			else if (data[0] == 0x05)
				pushButtonRXAnt6->setText(temp);
			else if (data[0] == 0x06)
				pushButtonRXAnt7->setText(temp);
			else if (data[0] == 0x07)
				pushButtonRXAnt8->setText(temp);
			else if (data[0] == 0x08)
				pushButtonRXAnt9->setText(temp);
			else if (data[0] == 0x09)
				pushButtonRXAnt10->setText(temp);
		}
		else if (cmd == REMOTE_CONTROL_ANT_TEXT) {
			QString temp;
			for (int i=1;i<length;i++)
				temp.append(data[i]);

			if (data[0] == 0x00)
				labelAnt1->setText(temp);
			else if (data[0] == 0x01)
				labelAnt2->setText(temp);
			else if (data[0] == 0x02)
				labelAnt3->setText(temp);
			else if (data[0] == 0x03)
				labelAnt4->setText(temp);
		}
		else if (cmd == REMOTE_CONTROL_GET_STATUS) {
			if (data[0] != status.currentBand) {
				getTXAntennaInfo();

				status.currentBand = data[0];

				if (status.currentBand == 1)
					labelTXAntennas->setText("160m - Antennas");
				else if (status.currentBand == 2)
					labelTXAntennas->setText("80m - Antennas");
				else if (status.currentBand == 3)
					labelTXAntennas->setText("40m - Antennas");
				else if (status.currentBand == 4)
					labelTXAntennas->setText("30m - Antennas");
				else if (status.currentBand == 5)
					labelTXAntennas->setText("20m - Antennas");
				else if (status.currentBand == 6)
					labelTXAntennas->setText("17m - Antennas");
				else if (status.currentBand == 7)
					labelTXAntennas->setText("15m - Antennas");
				else if (status.currentBand == 8)
					labelTXAntennas->setText("12m - Antennas");
				else if (status.currentBand == 9)
					labelTXAntennas->setText("10m - Antennas");
			}

			if (data[1] != status.currentAntennas) {
				status.currentAntennas = data[1];

				pushButtonTX1->setChecked(false);
				pushButtonTX2->setChecked(false);
				pushButtonTX3->setChecked(false);
				pushButtonTX4->setChecked(false);

				if (status.currentAntennas & 0x01)
					pushButtonTX1->setChecked(true);
				if (status.currentAntennas & 0x02)
					pushButtonTX2->setChecked(true);
				if (status.currentAntennas & 0x04)
					pushButtonTX3->setChecked(true);
				if (status.currentAntennas & 0x08)
					pushButtonTX4->setChecked(true);
			}

			if (data[2] != status.currentRXAntennas) {
				status.currentRXAntennas = data[2];

				/*QFont boldFont;
				boldFont.setBold(true);

				QFont regularFont;
				regularFont.setBold(false);

				pushButtonRXAnt1->setFont(regularFont);
				pushButtonRXAnt2->setFont(regularFont);
				pushButtonRXAnt3->setFont(regularFont);
				pushButtonRXAnt4->setFont(regularFont);
				pushButtonRXAnt5->setFont(regularFont);
				pushButtonRXAnt6->setFont(regularFont);
				pushButtonRXAnt7->setFont(regularFont);
				pushButtonRXAnt8->setFont(regularFont);
				pushButtonRXAnt9->setFont(regularFont);
				pushButtonRXAnt10->setFont(regularFont);
*/
				if (status.currentRXAntennas == 0) {
					labelCurrentRXAntenna->setText("");
				}
				else if (status.currentRXAntennas == 1) {
					//pushButtonRXAnt1->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt1->text());
				}
				else if (status.currentRXAntennas == 2) {
				//	pushButtonRXAnt2->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt2->text());
				}
				else if (status.currentRXAntennas == 3) {
					//pushButtonRXAnt3->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt3->text());
				}
				else if (status.currentRXAntennas == 4) {
					//pushButtonRXAnt4->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt4->text());
				}
				else if (status.currentRXAntennas == 5) {
					//pushButtonRXAnt5->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt5->text());
				}
				else if (status.currentRXAntennas == 6) {
					//pushButtonRXAnt6->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt6->text());
				}
				else if (status.currentRXAntennas == 7) {
					//pushButtonRXAnt7->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt7->text());
				}
				else if (status.currentRXAntennas == 8) {
					//pushButtonRXAnt8->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt8->text());
				}
				else if (status.currentRXAntennas == 9) {
					//pushButtonRXAnt9->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt9->text());
				}
				else if (status.currentRXAntennas == 10) {
					//pushButtonRXAnt10->setFont(boldFont);
					labelCurrentRXAntenna->setText(pushButtonRXAnt10->text());
				}
			}
		}
	}
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {
	unsigned char temp[3] = {REMOTE_CONTROL_CHANGE_BAND,1,0};

	temp[2] = index;
	comm.addTXMessage(INT_COMM_PC_CTRL,3,temp);
}

void MainWindowImpl::timerPollStatusUpdate(void) {
	comm.addTXMessage(INT_COMM_PC_CTRL,REMOTE_CONTROL_GET_STATUS);
}

void MainWindowImpl::pushButtonRXAntClicked(bool state) {
	pushButtonPressed(EXT_CTRL_TOGGLE_RX_ANT_MODE);
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

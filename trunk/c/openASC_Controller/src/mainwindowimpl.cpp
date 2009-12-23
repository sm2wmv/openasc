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

		comm.addTXMessage(COMPUTER_COMM_REDIRECT_DATA,3,temp);
}

void MainWindowImpl::pushButtonTX1Clicked(bool state) {
		pushButtonPressed(REMOTE_CTRL_BUTTON_TX1);

		pushButtonTX1->setChecked(false);
}

void MainWindowImpl::pushButtonTX2Clicked(bool state) {
		pushButtonPressed(REMOTE_CTRL_BUTTON_TX2);

		pushButtonTX2->setChecked(false);
}

void MainWindowImpl::pushButtonTX3Clicked(bool state) {
		pushButtonPressed(REMOTE_CTRL_BUTTON_TX3);

		pushButtonTX3->setChecked(false);
}

void MainWindowImpl::pushButtonTX4Clicked(bool state) {
		pushButtonPressed(REMOTE_CTRL_BUTTON_TX4);

		pushButtonTX4->setChecked(false);
}

void MainWindowImpl::pushButtonRXAnt1Clicked(bool state) {
		//We never want to toggle the rx antenna button.
		//We wish to do that from the usart recieve input, that way
		//we have feedback from the openASC box.
		pushButtonRXAnt1->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT1);
}

void MainWindowImpl::pushButtonRXAnt2Clicked(bool state) {
		pushButtonRXAnt2->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT2);
}

void MainWindowImpl::pushButtonRXAnt3Clicked(bool state) {
		pushButtonRXAnt3->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT3);
}

void MainWindowImpl::pushButtonRXAnt4Clicked(bool state) {
		pushButtonRXAnt4->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT4);
}

void MainWindowImpl::pushButtonRXAnt5Clicked(bool state) {
		pushButtonRXAnt5->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT5);
}

void MainWindowImpl::pushButtonRXAnt6Clicked(bool state) {
		pushButtonRXAnt6->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT6);
}

void MainWindowImpl::pushButtonRXAnt7Clicked(bool state) {
		pushButtonRXAnt7->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT7);
}

void MainWindowImpl::pushButtonRXAnt8Clicked(bool state) {
		pushButtonRXAnt8->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT8);
}

void MainWindowImpl::pushButtonRXAnt9Clicked(bool state) {
		pushButtonRXAnt9->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT9);
}

void MainWindowImpl::pushButtonRXAnt10Clicked(bool state) {
		pushButtonRXAnt10->setChecked(false);

		pushButtonPressed(REMOTE_CTRL_BUTTON_RXANT10);
}

void MainWindowImpl::actionSettingsEditTriggered() {
		settingsDialog->show();
}

void MainWindowImpl::actionConnectTriggered() {
	if (interfaceType == INTERFACE_TYPE_SERIAL) {
/*		if (comm.openPort(lineEditDevicePort->text())) {
						QMessageBox::critical(0,"Serial port error!","Could not open the specified port!");
		}
		else {
						serialPort.start();

						addDebugLine("Serial port opened: " + lineEditDevicePort->text());
		}*/
	}
	else if (interfaceType == INTERFACE_TYPE_TCP) {
		comm.openPort("127.0.0.1",7900);
		comm.start();
	}
}

void MainWindowImpl::actionDisconnectTriggered() {

}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	interfaceType = INTERFACE_TYPE_NETWORK;

	setupUi(this);

	settingsDialog = new SettingsDialog(this);
	settingsDialog->hide();

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();

	connect(actionSettingsEdit, SIGNAL(triggered()), this, SLOT(actionSettingsEditTriggered()));
	connect(actionWindowsRotators, SIGNAL(triggered()), this, SLOT(WindowRotatorsTriggered()));
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
	connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));

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
}

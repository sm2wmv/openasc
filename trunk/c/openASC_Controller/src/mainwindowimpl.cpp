#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"
#include "commclass.h"
	

void MainWindowImpl::WindowRotatorsClicked() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else
		rotatorWindow->show();
}
  
void MainWindowImpl::pushButtonPressed(unsigned char button) {
		unsigned char temp[3] = {REMOTE_CONTROL_BUTTON_PRESSED,1,button};

		serialPort.addTXMessage(COMPUTER_COMM_REDIRECT_DATA,3,temp);
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

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();
}

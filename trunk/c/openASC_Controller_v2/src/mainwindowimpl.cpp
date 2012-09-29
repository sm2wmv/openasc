#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "tcpclass.h"
#include "generic.h"
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
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT1);
	}
}

void MainWindowImpl::pushButtonTX2Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT2);
	}
}

void MainWindowImpl::pushButtonTX3Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT3);
	}
}

void MainWindowImpl::pushButtonTX4Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_TX_ANT4);
	}
}

void MainWindowImpl::pushButtonRXAnt1Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT1);
	}
}

void MainWindowImpl::pushButtonRXAnt2Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT2);
	}
}

void MainWindowImpl::pushButtonRXAnt3Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT3);
	}
}

void MainWindowImpl::pushButtonRXAnt4Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT4);
	}
}

void MainWindowImpl::pushButtonRXAnt5Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT5);
	}
}

void MainWindowImpl::pushButtonRXAnt6Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT6);
	}
}

void MainWindowImpl::pushButtonRXAnt7Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT7);
	}
}

void MainWindowImpl::pushButtonRXAnt8Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT8);
	}
}

void MainWindowImpl::pushButtonRXAnt9Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT9);
	}
}

void MainWindowImpl::pushButtonRXAnt10Clicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT10);
	}
}

void MainWindowImpl::pushButtonMenuClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_MENU_BUTTON);
	}
}

void MainWindowImpl::pushButtonMenuEnterClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_KNOB_BUTTON);
	}
}

void MainWindowImpl::pushButtonMenuLeftClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_KNOB_ROTATE_CCW);
	}
}

void MainWindowImpl::pushButtonMenuRightClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_KNOB_ROTATE_CW);
	}
}

void MainWindowImpl::actionSettingsEditTriggered() {
	settingsDialog->show();
}

void MainWindowImpl::actionDisconnectTriggered() {
	timerPollStatus->stop();

	TCPComm->stopConnection();

	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);
}

void MainWindowImpl::actionConnectTriggered() {
	TCPComm->connect(settingsDialog->getNetworkIPAddress(),settingsDialog->getNetworkPort());
	TCPComm->start();

	actionConnect->setEnabled(false);
	actionDisconnect->setEnabled(true);

	timerPollRXQueue->setInterval(1);
	timerPollRXQueue->start();
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {
}

void MainWindowImpl::timerPollStatusUpdate(void) {
}

void MainWindowImpl::pushButtonRXAntClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_RX_ANT_MODE);
	}
}

void MainWindowImpl::pushButtonSubClicked(){
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_SUBMENU_BUTTON);
	}
}

void MainWindowImpl::actionTerminalTriggered() {
	terminalWindow->show();
}

void MainWindowImpl::timerPollRXQueueUpdate(void) {
	if (TCPComm->isConnected()) {
		if (TCPComm->rxQueueSize() > 0) {
			QByteArray rxMessage = TCPComm->getMessage();
			unsigned int len = (rxMessage.at(1) << 8) + rxMessage.at(2);
			unsigned char cmd = rxMessage.at(0);

			qDebug("HR Size: %i\r\n",len);

			if (cmd == REMOTE_COMMAND_DISPLAY_DATA) {
				qDebug() << "REMOTE COMMAND DISPLAY DATA";

				if (len == 1024) {
					unsigned int i=3;

					for (unsigned int y=0;y<8;y++)
						for (unsigned int x=0;x<128;x++)
							glcd_buffer[y][x] = ~rxMessage.at(i++);

					repaint();
				}
			}
			else if (cmd == REMOTE_COMMAND_RX_ANT_INFO) {
				switch(rxMessage.at(3)) {
					case 0:
						pushButtonRXAnt1->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 1:
						pushButtonRXAnt2->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 2:
						pushButtonRXAnt3->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 3:
						pushButtonRXAnt4->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 4:
						pushButtonRXAnt5->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 5:
						pushButtonRXAnt6->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 6:
						pushButtonRXAnt7->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 7:
						pushButtonRXAnt8->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 8:
						pushButtonRXAnt9->setText(QString(rxMessage.mid(4,len-1)));
						break;
					case 9:
						pushButtonRXAnt10->setText(QString(rxMessage.mid(4,len-1)));
						break;
					default:
						break;
				}
			}
		}
	}
}

void MainWindowImpl::actionErrorDialogTriggered() {
	errorDialog->show();
}

void MainWindowImpl::actionRebootTriggered() {
}

void MainWindowImpl::updateDisplay() {

}

void MainWindowImpl::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setViewport(180,130,512,256);

	for (unsigned int y=0;y<8;y++) {
		for (unsigned int x=0;x<128;x++) {
			for (unsigned char row=0;row<8;row++){
				if ((glcd_buffer[y][x] & (1<<row)) == 0) {
					for (unsigned char x3=0;x3<3;x3++)
						for (unsigned char y3=0;y3<3;y3++)
							painter.drawPoint(3*x+x3,3*(y*8+row)+y3);
				}
			}
		}
	}
}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

  settingsDialog = new SettingsDialog(this);
	settingsDialog->hide();

	terminalWindow = new terminalDialog(this);
	terminalWindow->hide();

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();

	TCPComm = new TCPClass();

	errorDialog = new ErrorDialog();
	errorDialog->hide();

	//displayArea = new DisplayWidget();

	QFrame *status_frame = new QFrame();
	status_frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	QHBoxLayout *layout = new QHBoxLayout(status_frame);
	layout->setContentsMargins(0, 0, 0, 0);
	QProgressBar *bar = new QProgressBar(status_frame);
	bar->setMaximumHeight(10);
	bar->setMaximumWidth(100);

	QCheckBox *box = new QCheckBox(tr("Check Mode"), status_frame);
	box->setChecked(true);

	layout->addWidget(bar);
	layout->addWidget(box);
	//ui->statusBar->insertWidget(5, status_frame);

	/*QGridLayout dispLayout = new QGridLayout;
	dispLayout.addWidget(displayArea);
	displayArea->show();
	frame->setLayout(dispLayout);*/

	//Create paint area
	bandChangedFlag = 0;

	/*QPalette palette = frameDisplay->palette();
	palette.setColor(backgroundRole(),QColor(0,0,255));
	frameDisplay->setPalette(palette);
	frameDisplay->setAutoFillBackground(true);*/

	timerPollRXQueue = new QTimer(this);
	connect(timerPollRXQueue, SIGNAL(timeout()), this, SLOT(timerPollRXQueueUpdate()));

	timerPollStatus = new QTimer(this);
	connect(timerPollStatus, SIGNAL(timeout()), this, SLOT(timerPollStatusUpdate()));

	connect(actionReboot, SIGNAL(triggered()), this, SLOT(actionRebootTriggered()));
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

	connect(pushButtonMenu, SIGNAL(clicked()), this, SLOT(pushButtonMenuClicked()));
	connect(pushButtonMenuEnter, SIGNAL(clicked()), this, SLOT(pushButtonMenuEnterClicked()));
	connect(pushButtonMenuLeft, SIGNAL(clicked()), this, SLOT(pushButtonMenuLeftClicked()));
	connect(pushButtonMenuRight, SIGNAL(clicked()), this, SLOT(pushButtonMenuRightClicked()));

	connect(pushButtonSub, SIGNAL(clicked()),this,SLOT(pushButtonSubClicked()));

	for (unsigned int y=0;y<8;y++)
		for (unsigned int x=0;x<128;x++)
			glcd_buffer[y][x] = 255;
}

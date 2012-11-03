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
#include "../../../uC/front_panel/led_control.h"

//! Band changes are done manually
#define BAND_CHANGE_MODE_MANUAL		0
//! Band changes are done automatically
#define BAND_CHANGE_MODE_AUTO			1

#define BAND_UNDEFINED	0
#define BAND_160M				1
#define BAND_80M				2
#define BAND_40M				3
#define BAND_30M				4
#define BAND_20M				5
#define BAND_17M				6
#define BAND_15M				7
#define BAND_12M				8
#define BAND_10M				9
#define BAND_MAX				BAND_10M

#define PIXMAP_BLANK "../src/leds/led_blank_15x15.png"
#define PIXMAP_RED_ON "../src/leds/led_red_on_15x15.png"
#define PIXMAP_GREEN_ON "../src/leds/led_green_on_15x15.png"
#define PIXMAP_RED_OFF "../src/leds/led_red_off_15x15.png"
#define PIXMAP_GREEN_OFF "../src/leds/led_green_off_15x15.png"
#define PIXMAP_YELLOW_ON "../src/leds/led_yellow_on_15x15.png"
#define PIXMAP_YELLOW_OFF "../src/leds/led_yellow_off_15x15.png"

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
//  if (TCPComm->isConnected()) {
  TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_MENU_BUTTON);
//  }
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

	labelLEDRemote->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	TCPComm->stopConnection();

	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);
}

void MainWindowImpl::actionConnectTriggered() {
	TCPComm->connect(settingsDialog->getNetworkIPAddress(),settingsDialog->getNetworkPort());
//	TCPComm->start();

	labelLEDRemote->setPixmap(QPixmap(PIXMAP_GREEN_ON));

	actionConnect->setEnabled(false);
	actionDisconnect->setEnabled(true);

	timerPollRXQueue->setInterval(1);
	timerPollRXQueue->start();

  timerPollStatus->setInterval(1);
  timerPollStatus->start();
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {
	if (index < 10) {
		if (TCPComm->isConnected()) {
			TCPComm->addTXMessage(REMOTE_COMMAND_SET_NEW_BAND,comboBoxBand->currentIndex());
		}
	}
}

void MainWindowImpl::timerPollStatusUpdate(void) {
  TCPComm->receiveMsg();
  TCPComm->transmitMsg();
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

void MainWindowImpl::setLEDStatus(unsigned int led_status, unsigned char led_ptt_status) {
	if (led_ptt_status == LED_PTT_STATUS_OK) {
		labelLEDPTT->setPixmap(QPixmap(PIXMAP_BLANK));
	}
	else if (led_ptt_status == LED_PTT_STATUS_INHIBIT) {
		labelLEDPTT->setPixmap(QPixmap(PIXMAP_RED_ON));
	}
	else if (led_ptt_status == LED_PTT_STATUS_ACTIVE) {
		labelLEDPTT->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	}

	if (led_status & (1<<LED_STATUS_ERROR))
		labelLEDError->setPixmap(QPixmap(PIXMAP_RED_ON));
	else
		labelLEDError->setPixmap(QPixmap(PIXMAP_RED_OFF));

	if (led_status & (1<<LED_STATUS_TX_ANT1))
		labelLEDTX1->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDTX1->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_TX_ANT2))
		labelLEDTX2->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDTX2->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_TX_ANT3))
		labelLEDTX3->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDTX3->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_TX_ANT4))
		labelLEDTX4->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDTX4->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_RX_ANT1))
		labelLEDRX1->setPixmap(QPixmap(PIXMAP_RED_ON));
	else
		labelLEDRX1->setPixmap(QPixmap(PIXMAP_RED_OFF));

	if (led_status & (1<<LED_STATUS_RX_ANT2))
		labelLEDRX2->setPixmap(QPixmap(PIXMAP_RED_ON));
	else
		labelLEDRX2->setPixmap(QPixmap(PIXMAP_RED_OFF));

	if (led_status & (1<<LED_STATUS_RX_ANT3))
		labelLEDRX3->setPixmap(QPixmap(PIXMAP_RED_ON));
	else
		labelLEDRX3->setPixmap(QPixmap(PIXMAP_RED_OFF));

	if (led_status & (1<<LED_STATUS_RX_ANT4))
		labelLEDRX4->setPixmap(QPixmap(PIXMAP_RED_ON));
	else
		labelLEDRX4->setPixmap(QPixmap(PIXMAP_RED_OFF));

	if (led_status & (1<<LED_STATUS_AUX))
		labelLEDAUX->setPixmap(QPixmap(PIXMAP_YELLOW_ON));
	else
		labelLEDAUX->setPixmap(QPixmap(PIXMAP_YELLOW_OFF));

	if (led_status & (1<<LED_STATUS_ROTATING))
		labelLEDRotating->setPixmap(QPixmap(PIXMAP_YELLOW_ON));
	else
		labelLEDRotating->setPixmap(QPixmap(PIXMAP_YELLOW_OFF));

	if (led_status & (1<<LED_STATUS_ROTATE))
		labelLEDRotate->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDRotate->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_TXRX))
		labelLEDTXRX->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDTXRX->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_RXANT))
		labelLEDRXMode->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDRXMode->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	if (led_status & (1<<LED_STATUS_SUB))
		labelLEDSub->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDSub->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
}

void MainWindowImpl::timerPollRXQueueUpdate(void) {
	if (TCPComm->isConnected()) {
		if (TCPComm->rxQueueSize() > 0) {
			QByteArray rxMessage = TCPComm->getMessage();
        if (rxMessage.size() > 3) {
				unsigned int len = (rxMessage.at(1) << 8) + rxMessage.at(2);
				unsigned char cmd = rxMessage.at(0);
        if (cmd == REMOTE_COMMAND_DISPLAY_DATA) {
          if ((len == 1024) && (rxMessage.size() == 1027)) {
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
				else if (cmd == REMOTE_COMMAND_STATUS) {
					if (rxMessage.size() > 7) {
						setLEDStatus((rxMessage.at(4) << 8) | rxMessage.at(5),rxMessage.at(3));

						comboBoxBand->blockSignals(true);

						if (rxMessage.at(7) == BAND_CHANGE_MODE_AUTO)
							comboBoxBand->setCurrentIndex(10);

						switch(rxMessage.at(6)) {
							case BAND_UNDEFINED:
								labelBand->setText("Band: None");
								comboBoxBand->setCurrentIndex(0);
								break;
							case BAND_160M:
								labelBand->setText("Band: 160m");
								comboBoxBand->setCurrentIndex(1);
								break;
							case BAND_80M:
								labelBand->setText("Band: 80m");
								comboBoxBand->setCurrentIndex(2);
								break;
							case BAND_40M:
								labelBand->setText("Band: 40m");
								comboBoxBand->setCurrentIndex(3);
								break;
							case BAND_30M:
								labelBand->setText("Band: 30m");
								comboBoxBand->setCurrentIndex(4);
								break;
							case BAND_20M:
								labelBand->setText("Band: 20m");
								comboBoxBand->setCurrentIndex(5);
								break;
							case BAND_17M:
								labelBand->setText("Band: 17m");
								comboBoxBand->setCurrentIndex(6);
								break;
							case BAND_15M:
								labelBand->setText("Band: 15m");
								comboBoxBand->setCurrentIndex(7);
								break;
							case BAND_12M:
								labelBand->setText("Band: 12m");
								comboBoxBand->setCurrentIndex(8);
								break;
							case BAND_10M:
								labelBand->setText("Band: 10m");
								comboBoxBand->setCurrentIndex(9);
								break;
						default:
								labelBand->setText("Band: None");
								comboBoxBand->setCurrentIndex(0);
								break;
						}

						comboBoxBand->blockSignals(false);
					}
				}
      }
    }
	}
}

void MainWindowImpl::closeEvent ( QCloseEvent * event ) {
	event->ignore();

	TCPComm->stopConnection();

	event->accept();
};

void MainWindowImpl::actionRebootTriggered() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_FORCE_RESET);

		QTimer::singleShot(1000, this, SLOT(actionDisconnectTriggered()));
		QTimer::singleShot(5000, this, SLOT(actionConnectTriggered()));
	}
}

void MainWindowImpl::updateDisplay() {

}

void MainWindowImpl::actionKeypadTriggered() {
  keypadWindow->show();
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

  keypadWindow = new Keypad(this);
  keypadWindow->hide();

	TCPComm = new TCPClass();

  keypadWindow->setCOMMPtr(TCPComm);

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
  connect(actionKeypad, SIGNAL(triggered()), this, SLOT(actionKeypadTriggered()));

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

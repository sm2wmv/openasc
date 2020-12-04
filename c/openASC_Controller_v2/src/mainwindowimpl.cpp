#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>

#include "mainwindowimpl.h"
#include "tcpclass.h"
#include "generic.h"
#include "terminaldialog.h"
#include "powermeterdialog.h"
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

#define PIXMAP_BLANK QCoreApplication::applicationDirPath()+"/leds/led_blank_15x15.png"
#define PIXMAP_RED_ON QCoreApplication::applicationDirPath()+"/leds/led_red_on_15x15.png"
#define PIXMAP_GREEN_ON QCoreApplication::applicationDirPath()+"/leds/led_green_on_15x15.png"
#define PIXMAP_RED_OFF QCoreApplication::applicationDirPath()+"/leds/led_red_off_15x15.png"
#define PIXMAP_GREEN_OFF QCoreApplication::applicationDirPath()+"/leds/led_green_off_15x15.png"
#define PIXMAP_YELLOW_ON QCoreApplication::applicationDirPath()+"/leds/led_yellow_on_15x15.png"
#define PIXMAP_YELLOW_OFF QCoreApplication::applicationDirPath()+"/leds/led_yellow_off_15x15.png"

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
  pushButtonRXAnt[0]->setChecked(false);

	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT1);
	}
}

void MainWindowImpl::pushButtonRXAnt2Clicked() {
  pushButtonRXAnt[1]->setChecked(false);

	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT2);
  }
}

void MainWindowImpl::pushButtonRXAnt3Clicked() {
  pushButtonRXAnt[2]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT3);
  }
}

void MainWindowImpl::pushButtonRXAnt4Clicked() {
  pushButtonRXAnt[3]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT4);
  }
}

void MainWindowImpl::pushButtonRXAnt5Clicked() {
  pushButtonRXAnt[4]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT5);
	}
}

void MainWindowImpl::pushButtonRXAnt6Clicked() {
  pushButtonRXAnt[5]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT6);
	}
}

void MainWindowImpl::pushButtonRXAnt7Clicked() {
  pushButtonRXAnt[6]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT7);
	}
}

void MainWindowImpl::pushButtonRXAnt8Clicked() {
  pushButtonRXAnt[7]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT8);
	}
}

void MainWindowImpl::pushButtonRXAnt9Clicked() {
  pushButtonRXAnt[8]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT9);
	}
}

void MainWindowImpl::pushButtonRXAnt10Clicked() {
  pushButtonRXAnt[9]->setChecked(false);

  if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT10);
	}
}

void MainWindowImpl::pushButtonRXAnt11Clicked() {
  pushButtonRXAnt[10]->setChecked(false);

  if (TCPComm->isConnected()) {
      TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT11);
  }
}

void MainWindowImpl::pushButtonRXAnt12Clicked() {
  pushButtonRXAnt[11]->setChecked(false);

  if (TCPComm->isConnected()) {
      TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_SEL_RX_ANT12);
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
  timerPollRXQueue->stop();
  timerActivity->stop();

  powerMeterWindow->stopTimers();
  busClient->stopProcess();

	labelLEDRemote->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

	TCPComm->stopConnection();

	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);

  timerConnectionTimeoutCounter = 0;

  resetGUI();
}

void MainWindowImpl::actionConnectTriggered() {
  glcdPosX = frameDisplay->geometry().x();
  glcdPosY = frameDisplay->geometry().y();

  TCPComm->connectToHost(settingsDialog->getNetworkIPAddress(),settingsDialog->getNetworkPort());

  timerPollRXQueue->setInterval(10);
  timerPollRXQueue->start();

  timerPollStatus->setInterval(10);
  timerPollStatus->start();

  if (settingsDialog->getActivityTimer()) {
    activityTimeoutCounter = 0;

    timerActivity->setInterval(1000);
    timerActivity->start();
  }

  if (comboBoxBand->currentIndex() != 0)
    rotatorWindow->loadBand(comboBoxBand->currentIndex());

  if (settingsDialog->getPowerMeterEnabled()) {
    busClient->initClient(14, settingsDialog->getPowerMeterNetworkIPAddress(),settingsDialog->getPowerMeterNetworkPort());

    powerMeterWindow->setTimers(500,200);
    powerMeterWindow->startTimers();
  }
}

void MainWindowImpl::comboBoxBandIndexChanged(int index) {
	if (index < 10) {
		if (TCPComm->isConnected()) {
			TCPComm->addTXMessage(REMOTE_COMMAND_SET_NEW_BAND,comboBoxBand->currentIndex());
		}
	}
}

void MainWindowImpl::timerPollStatusUpdate(void) {
    if (TCPComm->isConnected() == false)
        actionDisconnectTriggered();
    else {
        TCPComm->receiveMsg();

        if (TCPComm->transmitMsg()) {
          if (settingsDialog->getActivityTimer()) {
            activityTimeoutCounter = 0;
          }
        }
    }

    if (TCPComm->isConnected()) {
        timerConnectionTimeoutCounter++;

        if (timerConnectionTimeoutCounter > TIMER_TIMEOUT_CONNECTION_LIMIT) {
            actionDisconnectTriggered();
	    QMessageBox::warning(
	      this,
	      tr("Disconnected"),
	      tr("Connection to the openASC box failed")
	    );

        timerConnectionTimeoutCounter = 0;
      }

    }
}

void MainWindowImpl::pushButtonRXAntEnableClicked() {
	if (TCPComm->isConnected()) {  
    TCPComm->addTXMessage(REMOTE_COMMAND_BUTTON_EVENT,EXT_CTRL_TOGGLE_RX_ANT_MODE);

    if (currStatus & (1<<LED_STATUS_RXANT))

    rotatorWindow->updateRXAntenna(currRXAntenna);
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

  if (led_status & (1<<LED_STATUS_RXANT)) {
		labelLEDRXMode->setPixmap(QPixmap(PIXMAP_GREEN_ON));

    if ((led_status & (1<<LED_STATUS_RXANT)) != (currStatus & (1<<LED_STATUS_RXANT))) {
      rotatorWindow->setRXAntStatus(true);
      rotatorWindow->updateRXAntenna(currRXAntenna);

    }
  }
  else {
		labelLEDRXMode->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

    if ((led_status & (1<<LED_STATUS_RXANT)) != (currStatus & (1<<LED_STATUS_RXANT))) {
      rotatorWindow->setRXAntStatus(false);
    }

  }

	if (led_status & (1<<LED_STATUS_SUB))
		labelLEDSub->setPixmap(QPixmap(PIXMAP_GREEN_ON));
	else
		labelLEDSub->setPixmap(QPixmap(PIXMAP_GREEN_OFF));

  currStatus = led_status;
}

void MainWindowImpl::timerPollRXQueueUpdate(void) {
  if (busClient->isConnected()) {
    busClient->receiveMsg();

    if (busClient->messageInRXQueue()) {
      BUS_MESSAGE message = busClient->getMessageInRXQueue();

      if (status.currentBand > 0) {
        if (message.from_addr == settingsDialog->getPowerMeterPickupAddr(comboBoxBand->currentIndex()-1)) {
          if (message.cmd == BUS_CMD_POWERMETER_STATUS) {
            mutex.lock();

            if (settingsDialog->getPowerMeterPickupType(comboBoxBand->currentIndex()) != 0) {
              powerMeterWindow->setValues((int)(settingsDialog->getPowerMeterPickupType(comboBoxBand->currentIndex()-1)-1),message.data[2] + (message.data[1]<<8),message.data[4] + (message.data[3]<<8),message.data[6] + (message.data[5]<<8));
            }
            else
              powerMeterWindow->setValues(message.data[0],message.data[2] + (message.data[1]<<8),message.data[4] + (message.data[3]<<8),message.data[6] + (message.data[5]<<8));

            mutex.unlock();
          }
        }
      }
    }
  }

  if (TCPComm->isConnected()) {
    if (TCPComm->rxQueueSize() > 0) {
        //We have recieved data from the client, reset the timeout counter
        timerConnectionTimeoutCounter = 0;

      QByteArray rxMessage = TCPComm->getMessage();

      if (rxMessage.size() >= 3) {
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
            if (rxAntennaLayout == nullptr) {
                rxAntennaLayout = new QVBoxLayout();
                labelRXAntennas = new QLabel("RX Antennas");

                rxAntennaLayout->addWidget(labelRXAntennas);
            }

            unsigned char id=0;

            if (rxMessage.at(3) < 11) {
              pushButtonRXAnt[rxMessage.at(3)] = new QPushButton(QString(rxMessage.mid(4,len-1)));
              pushButtonRXAnt[rxMessage.at(3)]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
              pushButtonRXAnt[rxMessage.at(3)]->setCheckable(true);
              pushButtonRXAnt[rxMessage.at(3)]->setStyleSheet("QPushButton:checked { background-color: green; }");
              rxAntennaLayout->addWidget(pushButtonRXAnt[rxMessage.at(3)]);

              if ((currRXAntenna > 0) && ((currRXAntenna-1) == rxMessage.at(3)))
                pushButtonRXAnt[rxMessage.at(3)]->setChecked(true);

            }

            switch(rxMessage.at(3)) {
            case 0:
              connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt1Clicked()));
              break;
            case 1:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt2Clicked()));
                break;
            case 2:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt3Clicked()));
                break;
            case 3:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt4Clicked()));
              break;
            case 4:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt5Clicked()));
              break;
            case 5:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt6Clicked()));
              break;
            case 6:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt7Clicked()));
              break;
            case 7:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt8Clicked()));
              break;
            case 8:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt9Clicked()));
              break;
            case 9:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt10Clicked()));
              break;
            case 10:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt11Clicked()));
              break;
            case 11:
                connect(pushButtonRXAnt[rxMessage.at(3)], SIGNAL(clicked()), this, SLOT(pushButtonRXAnt12Clicked()));
              break;
            default:
              break;
          }

            frameRXAntennas->setLayout(rxAntennaLayout);
            frameRXAntennas->show();

//            QSpacerItem *verticalSpacerRXAntennas = new QSpacerItem(22,1500, QSizePolicy::Fixed, QSizePolicy::Expanding);
//            rxAntennaLayout->addItem(verticalSpacerRXAntennas);
        }
        else if (cmd == REMOTE_COMMAND_STATUS) {
          if (rxMessage.size() > 7) {
            setLEDStatus((rxMessage.at(4) << 8) | rxMessage.at(5),rxMessage.at(3));

            comboBoxBand->blockSignals(true);

            if (currBand != rxMessage.at(6)) {
              switch(rxMessage.at(6)) {
                case BAND_UNDEFINED:
                  comboBoxBand->setCurrentIndex(0);
                  rotatorWindow->loadBand(BAND_UNDEFINED);
                  break;
                case BAND_160M:
                  comboBoxBand->setCurrentIndex(1);
                  rotatorWindow->loadBand(BAND_160M);
                  break;
                case BAND_80M:
                  comboBoxBand->setCurrentIndex(2);
                  rotatorWindow->loadBand(BAND_80M);
                  break;
                case BAND_40M:
                  comboBoxBand->setCurrentIndex(3);
                  rotatorWindow->loadBand(BAND_40M);
                  break;
                case BAND_30M:
                  comboBoxBand->setCurrentIndex(4);
                  rotatorWindow->loadBand(BAND_30M);
                  break;
                case BAND_20M:
                  comboBoxBand->setCurrentIndex(5);
                  rotatorWindow->loadBand(BAND_20M);
                  break;
                case BAND_17M:
                  comboBoxBand->setCurrentIndex(6);
                  rotatorWindow->loadBand(BAND_17M);
                  break;
                case BAND_15M:
                  comboBoxBand->setCurrentIndex(7);
                  rotatorWindow->loadBand(BAND_15M);
                  break;
                case BAND_12M:
                  comboBoxBand->setCurrentIndex(8);
                  rotatorWindow->loadBand(BAND_12M);
                  break;
                case BAND_10M:
                  comboBoxBand->setCurrentIndex(9);
                  rotatorWindow->loadBand(BAND_10M);
                  break;
                default:
                  comboBoxBand->setCurrentIndex(0);
                  rotatorWindow->loadBand(BAND_UNDEFINED);
                  break;
              }

              currBand = rxMessage.at(6);
            }

            comboBoxBand->blockSignals(false);

            if (rxMessage.size() > 8) { //This is just so we are backwards compatible
              currRXAntenna = rxMessage.at(8);
              rotatorWindow->updateRXAntenna(currRXAntenna);

              for (int i=0;i<12;i++) {
                if ((currRXAntenna > 0) && ((currRXAntenna-1) == i)) {
                  if (pushButtonRXAnt[i] != nullptr)
                    pushButtonRXAnt[i]->setChecked(true);

                }
                else {
                  if (pushButtonRXAnt[i] != nullptr)
                    pushButtonRXAnt[i]->setChecked(false);
                }
              }
            }
          }
        }
        else if (cmd == REMOTE_COMMAND_TERMINAL_DATA) {
          terminalWindow->addNewMessage(rxMessage);
        }
        else if (cmd == REMOTE_COMMAND_CONNECTED) {
          labelLEDRemote->setPixmap(QPixmap(PIXMAP_GREEN_ON));
          actionConnect->setEnabled(false);
          actionDisconnect->setEnabled(true);
        }
        else if (cmd == REMOTE_COMMAND_ROTATOR_SET_HEADING) {
          unsigned char temp[2];
          temp[0] = rxMessage.at(4);
          temp[1] = rxMessage.at(5);
          rotatorWindow->setRotatorAngle(rxMessage.at(3),(temp[0] << 8) + temp[1]);
          rotatorWindow->setRotatorStatusText(rxMessage.at(3),rxMessage.at(6));
        }
        else if (cmd == REMOTE_COMMAND_BAND_INFO) {
          bool bUpdateBandInfo = false;
          bool bUpdateAntInfo = false;

          for (unsigned char i=0;i<6;i++) {
            unsigned char temp = currBandInfoAddr[i];
            currBandInfoAddr[i] = rxMessage.at(3+i);

            if (temp != currBandInfoAddr[i])
              bUpdateBandInfo = true;
          }

          for (unsigned char i=0;i<6;i++) {
            unsigned char temp1 = currBandInfoBand[i];
            unsigned char temp2 = currBandInfoAntComb[i];
           // unsigned char temp3 = currBandInfoRXAnt[i];
            //unsigned char temp4 = currBandInfoPTTState[i];

            currBandInfoBand[i] = rxMessage.at(9+i);

            if (temp1 != currBandInfoBand[i])
                bUpdateBandInfo = true;

            //This check is so we are backwards compatible, so the program at least does not crash
            //when not receiving a message that is not long enough
            if (rxMessage.length() > 15) {
                currBandInfoAntComb[i] = rxMessage.at(15+i);

                currBandInfoRXAnt[i] = rxMessage.at(21+i) & 0x7F;

                //Check the PTT state flag
                if (rxMessage.at(21+i) & 0x80)
                    currBandInfoPTTState[i] = true;
                else
                    currBandInfoPTTState[i] = false;

                if (temp2 != currBandInfoAntComb[i])
                    bUpdateAntInfo = true;

               // if (temp3 != currBandInfoRXAnt[i])
              //      bUpdateRxAntInfo = true;

                //if (temp4 != currBandInfoPTTState[i])
                //    bUpdatePTTInfo = true;
            }
          }

          if (bUpdateBandInfo) {
            updateBandInfo();
			updateAntInfo();
		  }

          if (bUpdateAntInfo)
            updateAntInfo();
        }
      }
    }
  }
}

QString MainWindowImpl::translateBoxName(int radioIndex) {
    for (unsigned char i=0;i<6;i++) {
        if (settingsDialog->getBandInfoBoxAddr(i) == currBandInfoAddr[radioIndex]) {
            return(settingsDialog->getBandInfoBoxName(i));
        }
    }

    return(" ");
}

void MainWindowImpl::updateAntInfo() {
    rotatorWindow->setAntennaUsedBy(0," ");
    rotatorWindow->setAntennaUsedBy(1," ");
    rotatorWindow->setAntennaUsedBy(2," ");
    rotatorWindow->setAntennaUsedBy(3," ");

    unsigned char myRotatorIndexes[4];
    //Get the current antenna selection on this box from

    unsigned char myAntennaSelection = 0;

    if (currStatus & (1<<LED_STATUS_TX_ANT1))
        myAntennaSelection |= 1<<0;
    if (currStatus & (1<<LED_STATUS_TX_ANT2))
        myAntennaSelection |= 1<<1;
    if (currStatus & (1<<LED_STATUS_TX_ANT3))
        myAntennaSelection |= 1<<2;
    if (currStatus & (1<<LED_STATUS_TX_ANT4))
        myAntennaSelection |= 1<<3;

    //qDebug() << "ANT SEL: " << myAntennaSelection;

    if (currBand != 0) {
        myRotatorIndexes[0] = rotatorIndexList[currBand-1][0];
        myRotatorIndexes[1] = rotatorIndexList[currBand-1][1];
        myRotatorIndexes[2] = rotatorIndexList[currBand-1][2];
        myRotatorIndexes[3] = rotatorIndexList[currBand-1][3];

//        qDebug() << "ROT INDEX1: " << myRotatorIndexes[0];
       // qDebug() << "ROT INDEX2: " << myRotatorIndexes[1];
       // qDebug() << "ROT INDEX3: " << myRotatorIndexes[2];
        //qDebug() << "ROT INDEX4: " << myRotatorIndexes[3];


        QString usedByString[4] = {"","","",""};

        for (int radioIndex=5;radioIndex>=0;radioIndex--) {
           // qDebug() << "currBandInfoBand " << currBandInfoBand[radioIndex] << " Box Name " << translateBoxName(radioIndex);

            if (currBandInfoBand[radioIndex] != 0) {
                //If any antenna is selected, go through and see if it is some rotator we have
                if (currBandInfoAntComb[radioIndex] != 0) {
                    if (currBandInfoAntComb[radioIndex] & (1<<0)) {
                        for (int myAntIndex=0;myAntIndex<4;myAntIndex++)
                            if ((myRotatorIndexes[myAntIndex] != 0) && (myRotatorIndexes[myAntIndex] == rotatorIndexList[currBandInfoBand[radioIndex]-1][0]))
                                usedByString[myAntIndex].append(translateBoxName(radioIndex)+" ");
                    }

                    if (currBandInfoAntComb[radioIndex] & (1<<1)) {
                        for (int myAntIndex=0;myAntIndex<4;myAntIndex++)
                            if ((myRotatorIndexes[myAntIndex] != 0) && (myRotatorIndexes[myAntIndex] == rotatorIndexList[currBandInfoBand[radioIndex]-1][1]))
                                usedByString[myAntIndex].append(translateBoxName(radioIndex)+" ");
                    }

                    if (currBandInfoAntComb[radioIndex] & (1<<2)) {
                        for (int myAntIndex=0;myAntIndex<4;myAntIndex++)
                            if ((myRotatorIndexes[myAntIndex] != 0) && (myRotatorIndexes[myAntIndex] == rotatorIndexList[currBandInfoBand[radioIndex]-1][2]))
                                usedByString[myAntIndex].append(translateBoxName(radioIndex)+" ");
                    }

                    if (currBandInfoAntComb[radioIndex] & (1<<3)) {
                        for (int myAntIndex=0;myAntIndex<4;myAntIndex++)
                            if ((myRotatorIndexes[myAntIndex] != 0) && (myRotatorIndexes[myAntIndex] == rotatorIndexList[currBandInfoBand[radioIndex]-1][3]))
                                usedByString[myAntIndex].append(translateBoxName(radioIndex)+" ");
                    }



                    //if (myRotatorIndexes[0] == rotatorIndexList[currBandInfoBand[radioIndex]-1][0])
                }
            }
        }

        for (int antIndex=0;antIndex<4;antIndex++) {
            rotatorWindow->setAntennaUsedBy(antIndex,usedByString[antIndex]);
            //qDebug() << "STR -> " << usedByString[antIndex];
        }
    }
    else {
        rotatorWindow->setAntennaUsedBy(0," ");
        rotatorWindow->setAntennaUsedBy(1," ");
        rotatorWindow->setAntennaUsedBy(2," ");
        rotatorWindow->setAntennaUsedBy(3," ");
    }
}

void MainWindowImpl::updateBandInfo() {
    QPalette plt1;
    plt1.setColor(QPalette::WindowText, Qt::green);
    QPalette plt2;
    plt2.setColor(QPalette::WindowText, Qt::lightGray);

    for (unsigned char i=0;i<6;i++) {
        if ((settingsDialog->getBandInfoBoxAddr(0) != 0) && (settingsDialog->getBandInfoBoxAddr(0) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxABand->setPalette(plt1);
            else
                labelBoxABand->setPalette(plt2);

            labelBoxABand->setText(getBandName(currBandInfoBand[i]));
        }
        else if ((settingsDialog->getBandInfoBoxAddr(1) != 0) && (settingsDialog->getBandInfoBoxAddr(1) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxBBand->setPalette(plt1);
            else
                labelBoxBBand->setPalette(plt2);
            labelBoxBBand->setText(getBandName(currBandInfoBand[i]));
        }
        else if ((settingsDialog->getBandInfoBoxAddr(2) != 0) && (settingsDialog->getBandInfoBoxAddr(2) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxCBand->setPalette(plt1);
            else
                labelBoxCBand->setPalette(plt2);

          labelBoxCBand->setText(getBandName(currBandInfoBand[i]));
        }
        else if ((settingsDialog->getBandInfoBoxAddr(3) != 0) && (settingsDialog->getBandInfoBoxAddr(3) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxDBand->setPalette(plt1);
            else
                labelBoxDBand->setPalette(plt2);

          labelBoxDBand->setText(getBandName(currBandInfoBand[i]));
        }
        else if ((settingsDialog->getBandInfoBoxAddr(4) != 0) && (settingsDialog->getBandInfoBoxAddr(4) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxEBand->setPalette(plt1);
            else
                labelBoxEBand->setPalette(plt2);

          labelBoxEBand->setText(getBandName(currBandInfoBand[i]));
        }
        else if ((settingsDialog->getBandInfoBoxAddr(5) != 0) && (settingsDialog->getBandInfoBoxAddr(5) == currBandInfoAddr[i])) {
            if (currBandInfoBand[i] != 0)
                labelBoxFBand->setPalette(plt1);
            else
                labelBoxFBand->setPalette(plt2);

          labelBoxFBand->setText(getBandName(currBandInfoBand[i]));
        }
    }
}

void MainWindowImpl::quitProgram() {
    qDebug() << "Quitting";
    settingsDialog->setPosMainWindowX(geometry().x());
    settingsDialog->setPosMainWindowY(geometry().y());
    settingsDialog->setPosKeypadWindowX(keypadWindow->geometry().x());
    settingsDialog->setPosKeypadWindowY(keypadWindow->geometry().y());
    settingsDialog->setPosRotatorWindowX(rotatorWindow->geometry().x());
    settingsDialog->setPosRotatorWindowY(rotatorWindow->geometry().y());
    settingsDialog->setPosTerminalWindowX(terminalWindow->geometry().x());
    settingsDialog->setPosTerminalWindowY(terminalWindow->geometry().y());
    settingsDialog->setPosPowerMeterWindowX(powerMeterWindow->geometry().x());
    settingsDialog->setPosPowerMeterWindowY(powerMeterWindow->geometry().y());

    settingsDialog->setPowerMeterWindowOpen(powerMeterWindow->isVisible());
    settingsDialog->setRotatorWindowOpen(rotatorWindow->isVisible());
    settingsDialog->setTerminalWindowOpen(terminalWindow->isVisible());
    settingsDialog->setKeypadWindowOpen(keypadWindow->isVisible());

    settingsDialog->saveSettings();

    TCPComm->stopConnection();

    QApplication::quit();
}

void MainWindowImpl::closeEvent ( QCloseEvent * event ) {
	event->ignore();

    quitProgram();

	event->accept();
};

void MainWindowImpl::actionReconnect() {
    QTimer::singleShot(0, this, SLOT(actionDisconnectTriggered()));
    QTimer::singleShot(1000, this, SLOT(actionConnectTriggered()));
}

void MainWindowImpl::actionRebootTriggered() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_FORCE_RESET);

		QTimer::singleShot(1000, this, SLOT(actionDisconnectTriggered()));
		QTimer::singleShot(5000, this, SLOT(actionConnectTriggered()));
	}
}

void MainWindowImpl::updateDisplay() {

}

void MainWindowImpl::actionPowerMeterTriggered() {
  powerMeterWindow->show();
}

void MainWindowImpl::actionKeypadTriggered() {
  keypadWindow->show();
}

void MainWindowImpl::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setViewport(glcdPosX+settingsDialog->getGlcdPosXOffset(),glcdPosY+settingsDialog->getGlcdPosYOffset(),512,256);

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

void MainWindowImpl::resetGUI() {
  labelLEDPTT->setPixmap(QPixmap(PIXMAP_BLANK));
  labelLEDError->setPixmap(QPixmap(PIXMAP_RED_OFF));
  labelLEDTX1->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDTX2->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDTX3->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDTX4->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDRX1->setPixmap(QPixmap(PIXMAP_RED_OFF));
  labelLEDRX2->setPixmap(QPixmap(PIXMAP_RED_OFF));
  labelLEDRX3->setPixmap(QPixmap(PIXMAP_RED_OFF));
  labelLEDRX4->setPixmap(QPixmap(PIXMAP_RED_OFF));
  labelLEDAUX->setPixmap(QPixmap(PIXMAP_YELLOW_OFF));
  labelLEDRotating->setPixmap(QPixmap(PIXMAP_YELLOW_OFF));
  labelLEDRotate->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDTXRX->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDRXMode->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDSub->setPixmap(QPixmap(PIXMAP_GREEN_OFF));
  labelLEDRemote->setPixmap(QPixmap(PIXMAP_GREEN_OFF));



  if (rxAntennaLayout != nullptr) {
      QLayoutItem *child;
      while ((child = rxAntennaLayout->takeAt(0)) != nullptr) {
          delete child->widget(); // delete the widget
          delete child;   // delete the layout item
      }

      rxAntennaLayout = nullptr;
      frameRXAntennas->layout()->deleteLater();
  }

  for (unsigned int y=0;y<8;y++)
    for (unsigned int x=0;x<128;x++)
      glcd_buffer[y][x] = 255;

  QPalette plt;
  plt.setColor(QPalette::WindowText, Qt::red);

  for (unsigned char i=0;i<6;i++) {
      if (settingsDialog->getBandInfoBoxAddr(i) != 0) {
      if (i == 0) {
        labelBoxAName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxABand->setPalette(plt);
        labelBoxABand->setText("N/A");
      }
      else if (i == 1) {
        labelBoxBName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxBBand->setPalette(plt);
        labelBoxBBand->setText("N/A");
      }
      else if (i == 2) {
        labelBoxCName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxCBand->setPalette(plt);
        labelBoxCBand->setText("N/A");
      }
      else if (i == 3) {
        labelBoxDName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxDBand->setPalette(plt);
        labelBoxDBand->setText("N/A");
      }
      else if (i == 4) {
        labelBoxEName->setText(settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxEBand->setPalette(plt);
        labelBoxEBand->setText("N/A");
      }
      else if (i == 5) {
        labelBoxFName->setText(settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxFBand->setPalette(plt);
        labelBoxFBand->setText("N/A");
      }
    }
  }


  repaint();

  rotatorWindow->loadBand(0);
}

void MainWindowImpl::timerActivityUpdate() {
  activityTimeoutCounter++;

  if (activityTimeoutCounter > settingsDialog->getActivityTimerTimeoutLimit()) {
    actionDisconnectTriggered();
    activityTimeoutCounter = 0;

    QMessageBox::warning(
      this,
      tr("Disconnected"),
      tr("You were disconnected because of inactivity")
    );
  }
}

void MainWindowImpl::selectRXAntenna(int index) {
  switch (index) {
    case 1:
      pushButtonRXAnt1Clicked();

      break;
    case 2:
      pushButtonRXAnt2Clicked();
      break;
    case 3:
      pushButtonRXAnt3Clicked();
      break;
    case 4:
      pushButtonRXAnt4Clicked();
      break;
    case 5:
      pushButtonRXAnt5Clicked();
      break;
    case 6:
      pushButtonRXAnt6Clicked();
      break;
    case 7:
      pushButtonRXAnt7Clicked();
      break;
    case 8:
      pushButtonRXAnt8Clicked();
      break;
    case 9:
      pushButtonRXAnt9Clicked();
      break;
    case 10:
      pushButtonRXAnt10Clicked();
      break;
    case 11:
      pushButtonRXAnt11Clicked();
      break;
    case 12:
      pushButtonRXAnt12Clicked();
      break;
    default:
      break;
  }
}

void MainWindowImpl::enableRXAntenna(void) {
  if ((currStatus & (1<<LED_STATUS_RXANT)) == 0) {
    pushButtonRXAntEnableClicked();
  }
}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WindowFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

  settingsDialog = new SettingsDialog(this);
  settingsDialog->hide();

  terminalWindow = new terminalDialog(this);
  terminalWindow->hide();

  if (settingsDialog->getFrameRotatorWindow())
    rotatorWindow = new RotatorDialog(this);
  else
    rotatorWindow = new RotatorDialog(this,Qt::FramelessWindowHint);

	rotatorWindow->hide();

  rotatorWindow->setAntennaUsedBy(0," ");
  rotatorWindow->setAntennaUsedBy(1," ");
  rotatorWindow->setAntennaUsedBy(2," ");
  rotatorWindow->setAntennaUsedBy(3," ");

  powerMeterWindow = new PowerMeterDialog(this);
  powerMeterWindow->hide();

  keypadWindow = new Keypad(this);
  keypadWindow->hide();

  TCPComm = new TCPClass(this);

  busClient = new WMVBusClient();

  keypadWindow->setCOMMPtr(TCPComm);

  terminalWindow->setCOMMPtr(TCPComm);
  rotatorWindow->setCOMMPtr(TCPComm);

  if (settingsDialog->getShowMousePointer() == false) {
    rotatorWindow->setCursor(QCursor( Qt::BlankCursor ));
    this->setCursor(QCursor( Qt::BlankCursor));
  }

  for (int i=0;i<12;i++)
    pushButtonRXAnt[i] = nullptr;

  rxAntennaLayout = nullptr;

  timerActivity = new QTimer(this);
  connect(timerActivity, SIGNAL(timeout()), this, SLOT(timerActivityUpdate()));


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
  connect(actionPowerMeter, SIGNAL(triggered()), this, SLOT(actionPowerMeterTriggered()));

	connect(comboBoxBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxBandIndexChanged(int)));

	connect(pushButtonTX1, SIGNAL(clicked()), this, SLOT(pushButtonTX1Clicked()));
	connect(pushButtonTX2, SIGNAL(clicked()), this, SLOT(pushButtonTX2Clicked()));
	connect(pushButtonTX3, SIGNAL(clicked()), this, SLOT(pushButtonTX3Clicked()));
	connect(pushButtonTX4, SIGNAL(clicked()), this, SLOT(pushButtonTX4Clicked()));
  connect(pushButtonRXAntEnable, SIGNAL(clicked()), this, SLOT(pushButtonRXAntEnableClicked()));

	connect(pushButtonMenu, SIGNAL(clicked()), this, SLOT(pushButtonMenuClicked()));
	connect(pushButtonMenuEnter, SIGNAL(clicked()), this, SLOT(pushButtonMenuEnterClicked()));
	connect(pushButtonMenuLeft, SIGNAL(clicked()), this, SLOT(pushButtonMenuLeftClicked()));
	connect(pushButtonMenuRight, SIGNAL(clicked()), this, SLOT(pushButtonMenuRightClicked()));
  connect(pushButtonSub, SIGNAL(clicked()), this, SLOT(pushButtonSubClicked()));

  connect(rotatorWindow, SIGNAL(quitProgram()), this, SLOT(quitProgram()));
  connect(rotatorWindow, SIGNAL(signalSelectRXAntenna(int)), this, SLOT(selectRXAntenna(int)));
  connect(rotatorWindow, SIGNAL(signalEnableRXAntenna()), this, SLOT(enableRXAntenna()));

  //Set the default pixmaps
  resetGUI();

	currBand = BAND_UNDEFINED;


//    pushButtonRXAnt1->hide();
//    pushButtonRXAnt2->hide();
//    pushButtonRXAnt3->hide();
//    pushButtonRXAnt4->hide();
//    pushButtonRXAnt5->hide();
//    pushButtonRXAnt6->hide();
//    pushButtonRXAnt7->hide();
//    pushButtonRXAnt8->hide();
//    pushButtonRXAnt9->hide();
//    pushButtonRXAnt10->hide();
//    pushButtonRXAnt11->hide();
//    pushButtonRXAnt12->hide();


    setGeometry(QRect(settingsDialog->getPosMainWindowX(),settingsDialog->getPosMainWindowY(),566,600));
	keypadWindow->setGeometry(QRect(settingsDialog->getPosKeypadWindowX(),settingsDialog->getPosKeypadWindowY(),keypadWindow->width(),keypadWindow->height()));
	rotatorWindow->setGeometry(QRect(settingsDialog->getPosRotatorWindowX(),settingsDialog->getPosRotatorWindowY(),rotatorWindow->width(),rotatorWindow->height()));
	terminalWindow->setGeometry(QRect(settingsDialog->getPosTerminalWindowX(),settingsDialog->getPosTerminalWindowY(),terminalWindow->width(),terminalWindow->height()));
    powerMeterWindow->setGeometry(QRect(settingsDialog->getPosPowerMeterWindowX(),settingsDialog->getPosPowerMeterWindowY(),powerMeterWindow->width(),powerMeterWindow->height()));

	if (settingsDialog->getRotatorWindowOpen())
		rotatorWindow->show();
	else
		rotatorWindow->hide();

	if (settingsDialog->getTerminalWindowOpen())
		terminalWindow->show();
	else
		terminalWindow->hide();

  if (settingsDialog->getPowerMeterWindowOpen())
    powerMeterWindow->show();
  else
    powerMeterWindow->hide();

	if (settingsDialog->getKeypadWindowOpen())
		keypadWindow->show();
	else
		keypadWindow->hide();

  if (settingsDialog->getConnectOnStart())
    actionConnectTriggered();

  if (settingsDialog->getFrameRotatorWindowStartOnTop()) {
    rotatorWindow->show();
  }

  for (unsigned char i=0;i<6;i++) {
      QPalette plt;
      plt.setColor(QPalette::WindowText, Qt::red);

      if (settingsDialog->getBandInfoBoxAddr(i) != 0) {
      if (i == 0) {
        labelBoxAName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxABand->setPalette(plt);
        labelBoxABand->setText("N/A");
      }
      else if (i == 1) {
        labelBoxBName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxBBand->setPalette(plt);
        labelBoxBBand->setText("N/A");
      }
      else if (i == 2) {
        labelBoxCName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxCBand->setPalette(plt);
        labelBoxCBand->setText("N/A");
      }
      else if (i == 3) {
        labelBoxDName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxDBand->setPalette(plt);
        labelBoxDBand->setText("N/A");
      }
      else if (i == 4) {
        labelBoxEName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxEBand->setPalette(plt);
        labelBoxEBand->setText("N/A");
      }
      else if (i == 5) {
        labelBoxFName->setText("Radio " + settingsDialog->getBandInfoBoxName(i)+":");
        labelBoxFBand->setPalette(plt);
        labelBoxFBand->setText("N/A");
      }
    }
  }

  //Creating an index of which rotators are used for which antennas
  //so that we are able to write what radio is using what dependent antennas
  QSettings settings("rotator_settings.ini",QSettings::IniFormat,0);

  QStringList groups = settings.childGroups();

  for (int bandIndex=0;bandIndex<9;bandIndex++) {
      settings.beginGroup(getBandName(bandIndex+1));

      int size = settings.beginReadArray("Ant");

      for (int i=0;i<size;i++) {
        settings.setArrayIndex(i);

        rotatorIndexList[bandIndex][i] = settings.value("RotatorIndex").toInt();
      }

      settings.endArray();

      settings.endGroup();
  }

  updateBandInfo();

  repaint();
}
//

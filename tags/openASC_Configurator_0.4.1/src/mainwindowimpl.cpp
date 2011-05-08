#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QStringList>
#include <QDateTime>

#include "mainwindowimpl.h"
#include "submenuimpl.h"
#include "generic.h"
#include "bandclass.h"
#include "sequencerclass.h"
#include "radiointerfaceclass.h"
#include "commclass.h"
#include "rxantennaclass.h"
#include "settings.h"

#define STATUSBAR_MESSAGE_TIME 10000

BandClass bandData[9];
RXAntennaClass rxAntennas;

using namespace std;

void MainWindowImpl::listExtButtonFunctionsLoad() {
		listExtButtonFunctions << "None" << "Select RX Antenna 1" << "Select RX Antenna 2" << "Select RX Antenna 3" << "Select RX Antenna 4" << "Select RX Antenna 5"
						<< "Select RX Antenna 6" << "Select RX Antenna 7" << "Select RX Antenna 8" << "Select RX Antenna 9" << "Select RX Antenna 10";

		listExtButtonFunctions << "Toggle TX antenna 1" << "Toggle TX antenna 2" << "Toggle TX antenna 3" << "Toggle TX antenna 4" << "Toggle RX antenna 1"
						<< "Toggle RX antenna 2" << "Toggle RX antenna 3" << "Toggle RX antenna 4";

		listExtButtonFunctions << "Toggle RX Ant mode" << "Toggle TX/RX Ant mode";

		listExtButtonFunctions << "Set Array dir 1" << "Set Array dir 2" << "Set Array dir 3" << "Set Array dir 4" << "Set Array dir 5" << "Set Array dir 6" << "Set Array dir 7" << "Set Array dir 8";
		listExtButtonFunctions << "Set Stack comb 1" << "Set Stack comb 2" << "Set Stack comb 3" << "Set Stack comb 4" << "Set Stack comb 5" << "Set Stack comb 6";


		listExtButtonFunctions << "Amplifier toggle ON/OFF" << "Amplifier toggle standby" << "Amplifier tune" << "Amplifier reset";

		listExtButtonFunctions << "Rotate last antenna CW" << "Rotate last antenna CCW";

		listExtButtonFunctions << "Select band portion" << "Change band up" << "Change band down";

		comboBoxExtKeypadFunc->addItems(listExtButtonFunctions);
		comboBoxButtonAUX1->addItems(listExtButtonFunctions);
		comboBoxButtonAUX2->addItems(listExtButtonFunctions);
}

void MainWindowImpl::reloadCurrentRotatorProperties() {
	unsigned char index = comboBoxBand->currentIndex();
	
	unsigned char found = 0;
	
	//Go through all rotators and see if any index matches the one stored in the bandData class
	for (int i=0;i<rotators.getRotatorCount();i++) {
		if (rotators.getRotatorIndex(i) == bandData[index].getRotatorIndex(0)) {
			found = 1;
			comboBoxAntennaRotator1->setCurrentIndex(i+1);
		}
	}	
	
	if (found == 0)
		comboBoxAntennaRotator1->setCurrentIndex(0);
	
	//Go through all rotators and see if any index matches the one stored in the bandData class
	found = 0;
	for (int i=0;i<rotators.getRotatorCount();i++) {
		if (rotators.getRotatorIndex(i) == bandData[index].getRotatorIndex(1)) {
			found = 1;
			comboBoxAntennaRotator2->setCurrentIndex(i+1);
		}
	}
	
	if (found == 0)
		comboBoxAntennaRotator2->setCurrentIndex(0);
	
	//Go through all rotators and see if any index matches the one stored in the bandData class
	found = 0;
	for (int i=0;i<rotators.getRotatorCount();i++) {
		if (rotators.getRotatorIndex(i) == bandData[index].getRotatorIndex(2)) {
			found = 1;
			comboBoxAntennaRotator3->setCurrentIndex(i+1);
		}
	}
	
	if (found == 0)
		comboBoxAntennaRotator3->setCurrentIndex(0);
	
		//Go through all rotators and see if any index matches the one stored in the bandData class
	found = 0;
	for (int i=0;i<rotators.getRotatorCount();i++) {
		if (rotators.getRotatorIndex(i) == bandData[index].getRotatorIndex(3)) {
			found = 1;
			comboBoxAntennaRotator4->setCurrentIndex(i+1);
		}
	}
	
	if (found == 0)
		comboBoxAntennaRotator4->setCurrentIndex(0);
}

void MainWindowImpl::loadInitialGUIValues() {
	lineEditBandLimitHighSegHighLimit->setText(QString::number(bandData[0].getBandLimitHighSegHighLimit()));
	lineEditBandLimitHighSegLowLimit->setText(QString::number(bandData[0].getBandLimitHighSegLowLimit()));
	lineEditBandLimitLowSegHighLimit->setText(QString::number(bandData[0].getBandLimitLowSegHighLimit()));
	lineEditBandLimitLowSegLowLimit->setText(QString::number(bandData[0].getBandLimitLowSegLowLimit()));

	comboBoxBandCurrentIndexChanged(comboBoxBand->currentIndex());
	
	comboBoxSequencerSelectCurrentIndexChanged(comboBoxSequencerSelect->currentIndex());
	
	/* RADIO INTERFACE */
	comboBoxRadioType->setCurrentIndex(radioInterface.getRadioType());
	comboBoxRadioMode->setCurrentIndex(radioInterface.getRadioMode());
	comboBoxRadioBaudRate->setCurrentIndex(radioInterface.getBaudrate());
	comboBoxRadioStopbits->setCurrentIndex(radioInterface.getStopbits());
	spinBoxCIVAddress->setValue(radioInterface.getCIVAddress());
	sliderPollIntervalChanged(radioInterface.getPollInterval());
	groupBoxCAT->setChecked(radioInterface.getCATEnabled());
	comboBoxRadioSenseInput->setCurrentIndex(radioInterface.getSenseInput());
	
	checkBoxCombinationAllowed->setChecked(bandData[comboBoxBand->currentIndex()].getCombinationAllowed(comboBoxAntennaCombination->currentIndex()));	
	
	lineEditRXAntennaName1->setText(rxAntennas.getAntennaName(0));
	lineEditRXAntennaName2->setText(rxAntennas.getAntennaName(1));
	lineEditRXAntennaName3->setText(rxAntennas.getAntennaName(2));
	lineEditRXAntennaName4->setText(rxAntennas.getAntennaName(3));
	lineEditRXAntennaName5->setText(rxAntennas.getAntennaName(4));
	lineEditRXAntennaName6->setText(rxAntennas.getAntennaName(5));
	lineEditRXAntennaName7->setText(rxAntennas.getAntennaName(6));
	lineEditRXAntennaName8->setText(rxAntennas.getAntennaName(7));
	lineEditRXAntennaName9->setText(rxAntennas.getAntennaName(8));
	lineEditRXAntennaName10->setText(rxAntennas.getAntennaName(9));
	
	lineEditRXAntennaOutputs1->setText(rxAntennas.getAntennaName(0));
	lineEditRXAntennaOutputs2->setText(rxAntennas.getAntennaName(1));
	lineEditRXAntennaOutputs3->setText(rxAntennas.getAntennaName(2));
	lineEditRXAntennaOutputs4->setText(rxAntennas.getAntennaName(3));
	lineEditRXAntennaOutputs5->setText(rxAntennas.getAntennaName(4));
	lineEditRXAntennaOutputs6->setText(rxAntennas.getAntennaName(5));
	lineEditRXAntennaOutputs7->setText(rxAntennas.getAntennaName(6));
	lineEditRXAntennaOutputs8->setText(rxAntennas.getAntennaName(7));
	lineEditRXAntennaOutputs9->setText(rxAntennas.getAntennaName(8));
	lineEditRXAntennaOutputs10->setText(rxAntennas.getAntennaName(9));
	
	lineEditRXAntennaOutputStr160M->setText(rxAntennas.getBandOutputString(0));
	lineEditRXAntennaOutputStr80M->setText(rxAntennas.getBandOutputString(1));
	lineEditRXAntennaOutputStr40M->setText(rxAntennas.getBandOutputString(2));
	lineEditRXAntennaOutputStr30M->setText(rxAntennas.getBandOutputString(3));
	
	spinBoxNetworkAddress->setValue(settingsClass.getNetworkAddress());
	checkBoxNetworkIsMaster->setChecked(settingsClass.getDeviceIsMaster());
	spinBoxNetworkNrOfDevices->setValue(settingsClass.getNumberOfDevices());
	
	comboBoxPowerMeterBand->setCurrentIndex(0);
	spinBoxPowerMeterAddress->setValue(settingsClass.getPowerMeterAddress(0));

	doubleSpinBoxPowerMeterSWR->setValue(settingsClass.getPowerMeterVSWRAlarm());
	spinBoxPowerMeterUpdateRateText->setValue(settingsClass.getPowerMeterUpdateRateText());
	spinBoxPowerMeterUpdateRateBargraph->setValue(settingsClass.getPowerMeterUpdateRateBargraph());

	if (settingsClass.getPTTInterlockInput() == 0)
			radioButtonPTTInterlockNone->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 1)
			radioButtonPTTInterlockInput1->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 2)
			radioButtonPTTInterlockInput2->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 3)
			radioButtonPTTInterlockInput3->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 4)
			radioButtonPTTInterlockInput4->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 5)
			radioButtonPTTInterlockInput5->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 6)
			radioButtonPTTInterlockInput6->setChecked(true);
	else if (settingsClass.getPTTInterlockInput() == 7)
			radioButtonPTTInterlockInput7->setChecked(true);

	comboBoxButtonAUX1->setCurrentIndex(extInput.getAUXFunc(1));
	comboBoxButtonAUX2->setCurrentIndex(extInput.getAUXFunc(2));

	comboBoxRotatorsReload();
}

void MainWindowImpl::pushButtonDefaultBandLimitsPressed() {
	for (int i=0;i<9;i++)
		bandData[i].setDefaultBandLimits();
	
	lineEditBandLimitHighSegHighLimit->setText(QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitHighSegHighLimit()));
	lineEditBandLimitHighSegLowLimit->setText(QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitHighSegLowLimit()));
	lineEditBandLimitLowSegHighLimit->setText(QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitLowSegHighLimit()));
	lineEditBandLimitLowSegLowLimit->setText(QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitLowSegLowLimit()));
}

void MainWindowImpl::lineEditBandLimitLowSegLowLimitChanged(QString str) {
	if (strIsNumeric(str)) {
		bandData[comboBoxBandLimitBand->currentIndex()].setBandLimitLowSegLowLimit(str.toInt(0,10));
	}
	else {
		QString tmp = QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitLowSegLowLimit());
		lineEditBandLimitLowSegLowLimit->setText(tmp);
	}
}

void MainWindowImpl::lineEditBandLimitLowSegHighLimitChanged(QString str) {
	if (strIsNumeric(str)) {
		bandData[comboBoxBandLimitBand->currentIndex()].setBandLimitLowSegHighLimit(str.toInt(0,10));
	}
	else {
		QString tmp = QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitLowSegHighLimit());
		lineEditBandLimitLowSegHighLimit->setText(tmp);
	}
}

void MainWindowImpl::lineEditBandLimitHighSegLowLimitChanged(QString str) {
	if (strIsNumeric(str)) {
		bandData[comboBoxBandLimitBand->currentIndex()].setBandLimitHighSegLowLimit(str.toInt(0,10));
	}
	else {
		QString tmp = QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitHighSegLowLimit());
		lineEditBandLimitHighSegLowLimit->setText(tmp);
	}
}

void MainWindowImpl::lineEditBandLimitHighSegHighLimitChanged(QString str) {
	if (strIsNumeric(str)) {
		bandData[comboBoxBandLimitBand->currentIndex()].setBandLimitHighSegHighLimit(str.toInt(0,10));
	}
	else {
		QString tmp = QString::number(bandData[comboBoxBandLimitBand->currentIndex()].getBandLimitHighSegHighLimit());
		lineEditBandLimitHighSegHighLimit->setText(tmp);
	}
}

void MainWindowImpl::lineEditBandOutputStrHighChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		bandData[comboBoxBand->currentIndex()].setBandOutputStrHigh(str);
	}
	else {		
		lineEditBandOutputStrHigh->setText(bandData[comboBoxBand->currentIndex()].getBandOutputStrHigh());
	}	
}

void MainWindowImpl::lineEditBandOutputStrLowChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		bandData[comboBoxBand->currentIndex()].setBandOutputStrLow(str);
	}
	else {		
		lineEditBandOutputStrLow->setText(bandData[comboBoxBand->currentIndex()].getBandOutputStrLow());
	}	
}

void MainWindowImpl::comboBoxBandLimitBandCurrentIndexChanged(int index) {
	lineEditBandLimitHighSegHighLimit->setText(QString::number(bandData[index].getBandLimitHighSegHighLimit()));
	lineEditBandLimitHighSegLowLimit->setText(QString::number(bandData[index].getBandLimitHighSegLowLimit()));
	lineEditBandLimitLowSegHighLimit->setText(QString::number(bandData[index].getBandLimitLowSegHighLimit()));
	lineEditBandLimitLowSegLowLimit->setText(QString::number(bandData[index].getBandLimitLowSegLowLimit()));
}

void MainWindowImpl::comboBoxBandCurrentIndexChanged(int index) {
	comboBoxAntennaCombination->setCurrentIndex(0);

	lineEditBandOutputStrHigh->setText(bandData[index].getBandOutputStrHigh());
	lineEditBandOutputStrLow->setText(bandData[index].getBandOutputStrLow());
	
	lineEditAntennaName1->setText(bandData[index].getAntennaName(0));
	lineEditAntennaName2->setText(bandData[index].getAntennaName(1));
	lineEditAntennaName3->setText(bandData[index].getAntennaName(2));
	lineEditAntennaName4->setText(bandData[index].getAntennaName(3));
	
	lineEditAntennaOutputStr->setText(bandData[comboBoxBand->currentIndex()].getAntennaOutputStr(comboBoxAntennaCombination->currentIndex()));

	if (bandData[index].getCombinationAllowed(comboBoxAntennaCombination->currentIndex()))
		checkBoxCombinationAllowed->setChecked(true);
	else
		checkBoxCombinationAllowed->setChecked(false);

	checkBoxMultiband1->setChecked(bandData[comboBoxBand->currentIndex()].getAntennaMultiband(0));
	checkBoxMultiband2->setChecked(bandData[comboBoxBand->currentIndex()].getAntennaMultiband(1));
	checkBoxMultiband3->setChecked(bandData[comboBoxBand->currentIndex()].getAntennaMultiband(2));
	checkBoxMultiband4->setChecked(bandData[comboBoxBand->currentIndex()].getAntennaMultiband(3));
	
	int defaultAntennaIndex = bandData[comboBoxBand->currentIndex()].getDefaultAntenna();
	
	if (defaultAntennaIndex == 0)
		radioButtonAntennaDefault1->setChecked(true);
	else if (defaultAntennaIndex == 1)
		radioButtonAntennaDefault2->setChecked(true);
	else if (defaultAntennaIndex == 2)
		radioButtonAntennaDefault3->setChecked(true);
	else if (defaultAntennaIndex == 3)
		radioButtonAntennaDefault4->setChecked(true);
	
	if (bandData[index].getSubMenuType(0) == SUBMENU_NONE)
		checkBoxSubMenuEnabledAnt1->setChecked(false);
	else
		checkBoxSubMenuEnabledAnt1->setChecked(true);

	if (bandData[index].getSubMenuType(1) == SUBMENU_NONE)
		checkBoxSubMenuEnabledAnt2->setChecked(false);
	else
		checkBoxSubMenuEnabledAnt2->setChecked(true);

	if (bandData[index].getSubMenuType(2) == SUBMENU_NONE)
		checkBoxSubMenuEnabledAnt3->setChecked(false);
	else
		checkBoxSubMenuEnabledAnt3->setChecked(true);

	if (bandData[index].getSubMenuType(3) == SUBMENU_NONE)
		checkBoxSubMenuEnabledAnt4->setChecked(false);
	else
		checkBoxSubMenuEnabledAnt4->setChecked(true);

	reloadCurrentRotatorProperties();
}

void MainWindowImpl::comboBoxAntennaCombinationCurrentIndexChanged(int index) {
	lineEditAntennaOutputStr->setText(bandData[comboBoxBand->currentIndex()].getAntennaOutputStr(index));
	checkBoxCombinationAllowed->setChecked(bandData[comboBoxBand->currentIndex()].getCombinationAllowed(comboBoxAntennaCombination->currentIndex()));
}

void MainWindowImpl::lineEditAntennaName1Changed(QString str) {
	if (str.length() <= ANTENNA_TEXT_SIZE)
		bandData[comboBoxBand->currentIndex()].setAntennaName(0,str);
	else {
		QMessageBox::warning(0,"Warning!","Too many characters entered!");
		lineEditAntennaName1->setText(bandData[comboBoxBand->currentIndex()].getAntennaName(0));
	}
}

void MainWindowImpl::lineEditAntennaName2Changed(QString str) {
	if (str.length() <= ANTENNA_TEXT_SIZE)
		bandData[comboBoxBand->currentIndex()].setAntennaName(1,str);
	else {
		QMessageBox::warning(0,"Warning!","Too many characters entered!");
		lineEditAntennaName2->setText(bandData[comboBoxBand->currentIndex()].getAntennaName(0));
	}
}

void MainWindowImpl::lineEditAntennaName3Changed(QString str) {
	if (str.length() <= ANTENNA_TEXT_SIZE)
		bandData[comboBoxBand->currentIndex()].setAntennaName(2,str);
	else {
		QMessageBox::warning(0,"Warning!","Too many characters entered!");
		lineEditAntennaName3->setText(bandData[comboBoxBand->currentIndex()].getAntennaName(0));
	}
}

void MainWindowImpl::lineEditAntennaName4Changed(QString str) {
	if (str.length() <= ANTENNA_TEXT_SIZE)
		bandData[comboBoxBand->currentIndex()].setAntennaName(3,str);
	else {
		QMessageBox::warning(0,"Warning!","Too many characters entered!");
		lineEditAntennaName4->setText(bandData[comboBoxBand->currentIndex()].getAntennaName(0));
	}
}

void MainWindowImpl::checkBoxCombinationAllowedClicked(bool state) {
	bandData[comboBoxBand->currentIndex()].setCombinationAllowed(comboBoxAntennaCombination->currentIndex(),state);
}

void MainWindowImpl::checkBoxMultiband1Clicked(bool state) {
	bandData[comboBoxBand->currentIndex()].setAntennaMultiband(0,state);
}

void MainWindowImpl::checkBoxMultiband2Clicked(bool state) {
	bandData[comboBoxBand->currentIndex()].setAntennaMultiband(1,state);
}

void MainWindowImpl::checkBoxMultiband3Clicked(bool state) {
	bandData[comboBoxBand->currentIndex()].setAntennaMultiband(2,state);
}

void MainWindowImpl::checkBoxMultiband4Clicked(bool state) {
	bandData[comboBoxBand->currentIndex()].setAntennaMultiband(3,state);
}

void MainWindowImpl::lineEditAntennaOutputStrChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		bandData[comboBoxBand->currentIndex()].setAntennaOutputStr(comboBoxAntennaCombination->currentIndex(),str);
	}
	else {		
		lineEditAntennaOutputStr->setText(bandData[comboBoxBand->currentIndex()].getAntennaOutputStr(comboBoxAntennaCombination->currentIndex()));
	}	
}

void MainWindowImpl::actionSaveTriggered() {
   QString s = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename",
                    "/home",
                    "Config file (*.ini *.INI)");

	//Check if there is a file ending, if not we add one
	if (!s.contains("ini",Qt::CaseInsensitive))
		s.append(".ini");

	QSettings settings(s,QSettings::IniFormat,0);
	
	for (int i=0;i<9;i++)
		bandData[i].writeSettings(settings);
	
	rxAntennas.writeSettings(settings);
		
	sequencerFootswitch.writeSettings(settings);
	sequencerRadioSense.writeSettings(settings);
	sequencerComputer.writeSettings(settings);
	
	radioInterface.writeSettings(settings);
	
	extInput.writeSettings(settings);
	settingsClass.writeSettings(settings);
	
	rotators.writeSettings(settings);
	
	statusbar->showMessage("Saved configuration to: "+s,STATUSBAR_MESSAGE_TIME);
}

void MainWindowImpl::actionOpenTriggered() {
    QString s = QFileDialog::getOpenFileName(
                    this,
                    "Choose a file",
                    "/home",
                    "Config file (*.ini *.INI)");
                    
	QSettings settings(s,QSettings::IniFormat,0);
	
	for (int i=0;i<9;i++)
		bandData[i].loadSettings(settings);
	
	rxAntennas.loadSettings(settings);
	
	sequencerFootswitch.loadSettings(settings);
	sequencerRadioSense.loadSettings(settings);
	sequencerComputer.loadSettings(settings);
	
	radioInterface.loadSettings(settings);
		
	extInput.loadSettings(settings);
	settingsClass.loadSettings(settings);
	
	rotators.loadSettings(settings);
	
	loadInitialGUIValues();
	statusbar->showMessage("Loaded configuration from: "+s,STATUSBAR_MESSAGE_TIME);
}

void MainWindowImpl::actionQuitTriggered() {
	printf("Bye bye de SM2WMV (SJ2W)\n");
	printf("http://www.sj2w.se/\n");
	exit(0);
}

void MainWindowImpl::actionExitConfModeTriggered() {
	QByteArray res = strConvertToOutputStr(lineEditBandOutputStrHigh->text());
	
	for (int i=0;i<res.size();i++)
		qDebug("0x%02X",(unsigned char)res.at(i));
		if (serialPort.isOpen()) {
		serialPort.addTXMessage(CTRL_REBOOT,0,0);
		statusbar->showMessage("Exit configuration mode",STATUSBAR_MESSAGE_TIME);

		serialPort.closePort();
	}
}

void MainWindowImpl::actionConnectTriggered() {
	if (serialPort.openPort(lineEditDevicePort->text())) {
		QMessageBox::critical(0,"Serial port error!","Could not open the specified port!");
		addDebugLine("ERROR: Serial port error! Could not open the specified port!");
	}
	else {
		serialPort.start();
		
		addDebugLine("Serial port opened: " + lineEditDevicePort->text());
	}
}

void MainWindowImpl::actionDisconnectTriggered() {
	serialPort.closePort();
	serialPort.stopProcess();
	
	statusbar->showMessage("Serial port connection closed",STATUSBAR_MESSAGE_TIME);
	
	addDebugLine("Serial port closed: " + lineEditDevicePort->text());
}

void MainWindowImpl::actionSendSettingsTriggered() {
		addDebugLine("Sending settings to device");
		
	serialPort.addTXMessage(CTRL_CREATE_EEPROM_TABLE,0,0);

	for (int i=0;i<9;i++) {
		bandData[i].sendSettings(serialPort);
		
		addDebugLine("Sending bandData");
	}
	
	addDebugLine("Sending RX Antenna settings");
	rxAntennas.sendSettings(serialPort);
	
	addDebugLine("Sending Ext input settings");
	extInput.sendSettings(serialPort);

	addDebugLine("Sending main settings");
	settingsClass.sendSettings(serialPort);
	
	addDebugLine("Sending radio settings");
	radioInterface.sendSettings(serialPort);
	
	/* SEQUENCER -> THESE MUST BE SENT TOGETHER */
	addDebugLine("Sending sequencer settings");
	sequencerFootswitch.sendSettings(serialPort);
	sequencerComputer.sendSettings(serialPort);
	sequencerRadioSense.sendSettings(serialPort);

	//Save the sequencer settings
	serialPort.addTXMessage(CTRL_SET_SEQUENCER_SETTINGS,CTRL_SET_SEQUENCER_SAVE);

	/* SEQUENCER -> THESE MUST BE SENT TOGETHER END */

	unsigned char tx_buf[7];

	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();	
	
	tx_buf[0] = time.second();
	tx_buf[1] = time.minute();
	tx_buf[2] = time.hour();
	tx_buf[3] = date.dayOfWeek();
	tx_buf[4] = date.day();
	tx_buf[5] = date.month();
	//This need to be changed, when we cross over to year 3xxx ;-)
	tx_buf[6] = (date.year()-2000);

	serialPort.addTXMessage(CTRL_SET_TIME,7,tx_buf);

	serialPort.addTXMessage(CTRL_DONE,0,0);
		
	statusbar->showMessage("Sending settings to device",STATUSBAR_MESSAGE_TIME);
}

void MainWindowImpl::actionGetFirmwareRevTriggered() {
	serialPort.addTXMessage(CTRL_GET_FIRMWARE_REV,0,0);
	addDebugLine("Trying to retrieve firmware information");
}

void MainWindowImpl::addDebugLine(QString str) {
	if (pushButtonDebug->isChecked())
		listWidgetDebug->addItem(str);
}

void MainWindowImpl::groupBoxSequencerPTTRadioClicked(bool state) {
	if (comboBoxSequencerSelect->currentIndex() == 0) {
		sequencerFootswitch.setRadioPTTEnabled(state);
	}
	else if (comboBoxSequencerSelect->currentIndex() == 1) {
		sequencerComputer.setRadioPTTEnabled(state);
	}
	else if (comboBoxSequencerSelect->currentIndex() == 2) {
		sequencerRadioSense.setRadioPTTEnabled(state);
	}
}

void MainWindowImpl::groupBoxSequencerPTTAmpClicked(bool state) {
	if (comboBoxSequencerSelect->currentIndex() == 0) {
		sequencerFootswitch.setAmpPTTEnabled(state);
	}
	else if (comboBoxSequencerSelect->currentIndex() == 1) {
		sequencerComputer.setAmpPTTEnabled(state);
	}
	else if (comboBoxSequencerSelect->currentIndex() == 2) {
		sequencerRadioSense.setAmpPTTEnabled(state);
	}
}

void MainWindowImpl::groupBoxSequencerInhibitClicked(bool state) {
	if (comboBoxSequencerSelect->currentIndex() == 0) {
		sequencerFootswitch.setInhibitEnabled(state);
	}
	else if (comboBoxSequencerSelect->currentIndex() == 1) {
		sequencerComputer.setInhibitEnabled(state);
	}
}

void MainWindowImpl::comboBoxSequencerSelectCurrentIndexChanged(int index) {
	if (index == 0) {
		groupBoxSequencer->setTitle("Footswitch");
		
		spinBoxSequencerPTTRadioPreDelay->setValue(sequencerFootswitch.getRadioPreDelay());
		spinBoxSequencerPTTRadioPostDelay->setValue(sequencerFootswitch.getRadioPostDelay());
		spinBoxSequencerPTTAmpPreDelay->setValue(sequencerFootswitch.getAmpPreDelay());
		spinBoxSequencerPTTAmpPostDelay->setValue(sequencerFootswitch.getAmpPostDelay());
		spinBoxSequencerInhibitPredelay->setValue(sequencerFootswitch.getInhibitPreDelay());
		spinBoxSequencerInhibitPostdelay->setValue(sequencerFootswitch.getInhibitPostDelay());
		spinBoxSequencerAntennaPostDelay->setValue(sequencerFootswitch.getAntennaPostDelay());
		
		groupBoxSequencerPTTRadio->setChecked(sequencerFootswitch.getRadioPTTEnabled());
		groupBoxSequencerPTTAmp->setChecked(sequencerFootswitch.getAmpPTTEnabled());
		groupBoxSequencerInhibitEnabled->setChecked(sequencerFootswitch.getInhibitEnabled());
		
		groupBoxSequencerInhibitEnabled->setEnabled(true);
	}
	else if (index == 1) {
		groupBoxSequencer->setTitle("Computer");
		
		spinBoxSequencerPTTRadioPreDelay->setValue(sequencerComputer.getRadioPreDelay());
		spinBoxSequencerPTTRadioPostDelay->setValue(sequencerComputer.getRadioPostDelay());
		spinBoxSequencerPTTAmpPreDelay->setValue(sequencerComputer.getAmpPreDelay());
		spinBoxSequencerPTTAmpPostDelay->setValue(sequencerComputer.getAmpPostDelay());
		spinBoxSequencerInhibitPredelay->setValue(sequencerComputer.getInhibitPreDelay());
		spinBoxSequencerInhibitPostdelay->setValue(sequencerComputer.getInhibitPostDelay());
		spinBoxSequencerAntennaPostDelay->setValue(sequencerComputer.getAntennaPostDelay());
		
		groupBoxSequencerPTTRadio->setChecked(sequencerComputer.getRadioPTTEnabled());
		groupBoxSequencerPTTAmp->setChecked(sequencerComputer.getAmpPTTEnabled());
		groupBoxSequencerInhibitEnabled->setChecked(sequencerComputer.getInhibitEnabled());		
		
		groupBoxSequencerInhibitEnabled->setEnabled(true);
	}
	else if (index == 2) {
		groupBoxSequencer->setTitle("Radio sense");

		spinBoxSequencerPTTRadioPreDelay->setValue(sequencerRadioSense.getRadioPreDelay());
		spinBoxSequencerPTTRadioPostDelay->setValue(sequencerRadioSense.getRadioPostDelay());
		spinBoxSequencerPTTAmpPreDelay->setValue(sequencerRadioSense.getAmpPreDelay());
		spinBoxSequencerPTTAmpPostDelay->setValue(sequencerRadioSense.getAmpPostDelay());
		spinBoxSequencerAntennaPostDelay->setValue(sequencerRadioSense.getAntennaPostDelay());
		
		groupBoxSequencerPTTRadio->setChecked(sequencerRadioSense.getRadioPTTEnabled());
		groupBoxSequencerPTTAmp->setChecked(sequencerRadioSense.getAmpPTTEnabled());
			
		spinBoxSequencerInhibitPredelay->setValue(0);
		spinBoxSequencerInhibitPostdelay->setValue(0);
		groupBoxSequencerInhibitEnabled->setChecked(false);
		
		groupBoxSequencerInhibitEnabled->setEnabled(false);
	}
	
	checkBoxPTTInputFootswitch->setChecked(sequencerFootswitch.getInputEnabled());
	checkBoxPTTInputRadioSense->setChecked(sequencerRadioSense.getInputEnabled());
	checkBoxPTTInputComputerRTS->setChecked(sequencerComputer.getInputEnabled());
	checkBoxPTTInputRadioSenseInverted->setChecked(sequencerRadioSense.getInputInverted());
	checkBoxPTTInputComputerRTSInverted->setChecked(sequencerComputer.getInputInverted());	
}

void MainWindowImpl::spinBoxSequencerPTTRadioPreDelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setRadioPreDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setRadioPreDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 2)
		sequencerRadioSense.setRadioPreDelay(value);
}

void MainWindowImpl::spinBoxSequencerPTTRadioPostDelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setRadioPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setRadioPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 2)
		sequencerRadioSense.setRadioPostDelay(value);
}

void MainWindowImpl::spinBoxSequencerPTTAmpPreDelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setAmpPreDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setAmpPreDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 2)
		sequencerRadioSense.setAmpPreDelay(value);
}

void MainWindowImpl::spinBoxSequencerPTTAmpPostDelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setAmpPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setAmpPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 2)
		sequencerRadioSense.setAmpPostDelay(value);
}

void MainWindowImpl::spinBoxSequencerInhibitPredelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setInhibitPreDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setInhibitPreDelay(value);
}

void MainWindowImpl::spinBoxSequencerInhibitPostdelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setInhibitPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setInhibitPostDelay(value);
}

void MainWindowImpl::spinBoxSequencerAntennaPostDelayValueChanged(int value) {
	if (comboBoxSequencerSelect->currentIndex() == 0)
		sequencerFootswitch.setAntennaPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 1)
		sequencerComputer.setAntennaPostDelay(value);
	else if (comboBoxSequencerSelect->currentIndex() == 2)
		sequencerRadioSense.setAntennaPostDelay(value);
}

void MainWindowImpl::checkBoxPTTInputFootswitchClicked(bool state) {
	sequencerFootswitch.setInputEnabled(state);
}

void MainWindowImpl::checkBoxPTTInputComputerRTSClicked(bool state) {
	sequencerComputer.setInputEnabled(state);
}

void MainWindowImpl::checkBoxPTTInputRadioSenseClicked(bool state) {
	sequencerRadioSense.setInputEnabled(state);
}

void MainWindowImpl::checkBoxPTTInputComputerRTSInvertedClicked(bool state) {
	sequencerComputer.setInputInverted(state);
}

void MainWindowImpl::checkBoxPTTInputRadioSenseInvertedClicked(bool state) {
	sequencerRadioSense.setInputInverted(state);
}

void MainWindowImpl::radioButtonInhibitPolarityActiveLowClicked(bool state) {
	//TODO: FINISH THIS
}

void MainWindowImpl::radioButtonInhibitPolarityActiveHighClicked(bool state) {
	//TODO: FINISH THIS
}

void MainWindowImpl::comboBoxRadioTypeCurrentIndexChanged(int index) {
	if (index == RADIO_TYPE_ICOM) {
		comboBoxRadioMode->setCurrentIndex(0);
	}
	else {
		comboBoxRadioMode->setCurrentIndex(0);
	}
	
	radioInterface.setRadioType(index);
}

void MainWindowImpl::groupBoxCATClicked(bool state) {
		radioInterface.setCATEnable(state);
}

void MainWindowImpl::comboBoxRadioSenseInputCurrentIndexChanged(int index) {
		radioInterface.setSenseInput(index);
}

void MainWindowImpl::comboBoxRadioModeCurrentIndexChanged(int index) {
	radioInterface.setRadioMode(index);
}

void MainWindowImpl::comboBoxRadioBaudRateCurrentIndexChanged(int index) {
	radioInterface.setBaudrate(index);
}

void MainWindowImpl::comboBoxRadioStopbitsCurrentIndexChanged(int index) {
	radioInterface.setStopbits(index);
}

void MainWindowImpl::spinBoxCIVAddressValueChanged(int value) {
	radioInterface.setCIVAddress(value);
}

void MainWindowImpl::sliderPollIntervalChanged(int value){
	char buf[6];
	sprintf(buf, "%ims",value*10);

	labelRadioInterfacePoll->setText(buf);
	
	radioInterface.setPollInterval(value);
}

void MainWindowImpl::lineEditRXAntennaOutputs(QString str, int index) {
	if (strIsValidOutputStr(str)) {
		rxAntennas.setAntennaOutputStr(index-1,str);
	}
	else {
		if (index == 1)
			lineEditRXAntennaOutputs1->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 2)
			lineEditRXAntennaOutputs2->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 3)
			lineEditRXAntennaOutputs3->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 4)
			lineEditRXAntennaOutputs4->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 5)
			lineEditRXAntennaOutputs5->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 6)
			lineEditRXAntennaOutputs6->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 7)
			lineEditRXAntennaOutputs7->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 8)
			lineEditRXAntennaOutputs8->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 9)
			lineEditRXAntennaOutputs9->setText(rxAntennas.getAntennaOutputStr(index-1));
		else if (index == 10)
			lineEditRXAntennaOutputs10->setText(rxAntennas.getAntennaOutputStr(index-1));
	}
}

void MainWindowImpl::lineEditRXAntennaOutputs1Changed(QString str) {
	lineEditRXAntennaOutputs(str,1);
}

void MainWindowImpl::lineEditRXAntennaOutputs2Changed(QString str) {
	lineEditRXAntennaOutputs(str,2);
}

void MainWindowImpl::lineEditRXAntennaOutputs3Changed(QString str) {
	lineEditRXAntennaOutputs(str,3);
}

void MainWindowImpl::lineEditRXAntennaOutputs4Changed(QString str) {
	lineEditRXAntennaOutputs(str,4);
}

void MainWindowImpl::lineEditRXAntennaOutputs5Changed(QString str) {
	lineEditRXAntennaOutputs(str,5);
}

void MainWindowImpl::lineEditRXAntennaOutputs6Changed(QString str) {
	lineEditRXAntennaOutputs(str,6);
}

void MainWindowImpl::lineEditRXAntennaOutputs7Changed(QString str) {
	lineEditRXAntennaOutputs(str,7);
}

void MainWindowImpl::lineEditRXAntennaOutputs8Changed(QString str) {
	lineEditRXAntennaOutputs(str,8);
}

void MainWindowImpl::lineEditRXAntennaOutputs9Changed(QString str) {
	lineEditRXAntennaOutputs(str,9);
}

void MainWindowImpl::lineEditRXAntennaOutputs10Changed(QString str) {
	lineEditRXAntennaOutputs(str,10);
}

void MainWindowImpl::lineEditRXAntennaName1Changed(QString str) {
	rxAntennas.setAntennaName(0,str);
}

void MainWindowImpl::lineEditRXAntennaName2Changed(QString str) {
	rxAntennas.setAntennaName(1,str);
}

void MainWindowImpl::lineEditRXAntennaName3Changed(QString str) {
	rxAntennas.setAntennaName(2,str);
}

void MainWindowImpl::lineEditRXAntennaName4Changed(QString str) {
	rxAntennas.setAntennaName(3,str);
}

void MainWindowImpl::lineEditRXAntennaName5Changed(QString str) {
	rxAntennas.setAntennaName(4,str);
}

void MainWindowImpl::lineEditRXAntennaName6Changed(QString str) {
	rxAntennas.setAntennaName(5,str);
}

void MainWindowImpl::lineEditRXAntennaName7Changed(QString str) {
	rxAntennas.setAntennaName(6,str);
}

void MainWindowImpl::lineEditRXAntennaName8Changed(QString str) {
	rxAntennas.setAntennaName(7,str);
}

void MainWindowImpl::lineEditRXAntennaName9Changed(QString str) {
	rxAntennas.setAntennaName(8,str);
}

void MainWindowImpl::lineEditRXAntennaName10Changed(QString str) {
	rxAntennas.setAntennaName(9,str);
}

void MainWindowImpl::lineEditRXAntennaOutputStr160MChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		rxAntennas.setBandOutputString(0,str);
	}
	else {
		lineEditRXAntennaOutputStr160M->setText(rxAntennas.getBandOutputString(0));
	}
}

void MainWindowImpl::lineEditRXAntennaOutputStr80MChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		rxAntennas.setBandOutputString(1,str);
	}
	else {
		lineEditRXAntennaOutputStr80M->setText(rxAntennas.getBandOutputString(1));
	}
}

void MainWindowImpl::lineEditRXAntennaOutputStr40MChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		rxAntennas.setBandOutputString(2,str);
	}
	else {
		lineEditRXAntennaOutputStr40M->setText(rxAntennas.getBandOutputString(2));
	}
}

void MainWindowImpl::lineEditRXAntennaOutputStr30MChanged(QString str) {
	if (strIsValidOutputStr(str)) {
		rxAntennas.setBandOutputString(3,str);
	}
	else {
		lineEditRXAntennaOutputStr30M->setText(rxAntennas.getBandOutputString(3));
	}
}

void MainWindowImpl::pushButtonSubMenuAnt1Pressed() {
	checkBoxSubMenuEnabledAnt1->setChecked(true);
	
	SubMenuImpl subMenu(this);
	subMenu.setAntenna(bandData[comboBoxBand->currentIndex()],0);
	subMenu.exec();
}

void MainWindowImpl::pushButtonSubMenuAnt2Pressed() {
	checkBoxSubMenuEnabledAnt2->setChecked(true);
	
	SubMenuImpl subMenu(this);
	subMenu.setAntenna(bandData[comboBoxBand->currentIndex()],1);
	subMenu.exec();
}

void MainWindowImpl::pushButtonSubMenuAnt3Pressed() {
	checkBoxSubMenuEnabledAnt3->setChecked(true);
	
	SubMenuImpl subMenu(this);
	subMenu.setAntenna(bandData[comboBoxBand->currentIndex()],2);
	subMenu.exec();
}

void MainWindowImpl::pushButtonSubMenuAnt4Pressed() {
	checkBoxSubMenuEnabledAnt4->setChecked(true);
	
	SubMenuImpl subMenu(this);
	subMenu.setAntenna(bandData[comboBoxBand->currentIndex()],3);
	subMenu.exec();
}

void MainWindowImpl::checkBoxSubMenuEnabledAnt1Clicked(bool state) {
	if (state == false) {
		bandData[comboBoxBand->currentIndex()].setSubMenuType(0,SUBMENU_NONE);
	}
}

void MainWindowImpl::checkBoxSubMenuEnabledAnt2Clicked(bool state) {
	if (state == false) {
		bandData[comboBoxBand->currentIndex()].setSubMenuType(1,SUBMENU_NONE);
	}
}

void MainWindowImpl::checkBoxSubMenuEnabledAnt3Clicked(bool state) {
	if (state == false) {
		bandData[comboBoxBand->currentIndex()].setSubMenuType(2,SUBMENU_NONE);
	}
}

void MainWindowImpl::checkBoxSubMenuEnabledAnt4Clicked(bool state) {
	if (state == false) {
		bandData[comboBoxBand->currentIndex()].setSubMenuType(3,SUBMENU_NONE);
	}
}

void MainWindowImpl::radioButtonAntennaDefault1Clicked(bool state) {
	if (state == true) {
		bandData[comboBoxBand->currentIndex()].setDefaultAntenna(0);
	}
}

void MainWindowImpl::radioButtonAntennaDefault2Clicked(bool state) {
	if (state == true) {
		bandData[comboBoxBand->currentIndex()].setDefaultAntenna(1);
	}
}

void MainWindowImpl::radioButtonAntennaDefault3Clicked(bool state) {
	if (state == true) {
		bandData[comboBoxBand->currentIndex()].setDefaultAntenna(2);
	}	
}

void MainWindowImpl::radioButtonAntennaDefault4Clicked(bool state) {
	if (state == true) {
		bandData[comboBoxBand->currentIndex()].setDefaultAntenna(3);
	}
}

void MainWindowImpl::pushButtonExt0Pressed() {
	extInput.setCurrSelectedButton(0);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 0 function");
}

void MainWindowImpl::pushButtonExt1Pressed() {
	extInput.setCurrSelectedButton(1);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 1 function");
}

void MainWindowImpl::pushButtonExt2Pressed() {
	extInput.setCurrSelectedButton(2);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 2 function");
}

void MainWindowImpl::pushButtonExt3Pressed() {
	extInput.setCurrSelectedButton(3);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 3 function");
}

void MainWindowImpl::pushButtonExt4Pressed() {
	extInput.setCurrSelectedButton(4);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 4 function");
}

void MainWindowImpl::pushButtonExt5Pressed() {
	extInput.setCurrSelectedButton(5);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 5 function");
}

void MainWindowImpl::pushButtonExt6Pressed() {
	extInput.setCurrSelectedButton(6);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 6 function");
}

void MainWindowImpl::pushButtonExt7Pressed() {
	extInput.setCurrSelectedButton(7);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 7 function");
}

void MainWindowImpl::pushButtonExt8Pressed() {
	extInput.setCurrSelectedButton(8);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 8 function");
}

void MainWindowImpl::pushButtonExt9Pressed() {
	extInput.setCurrSelectedButton(9);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key 9 function");
}


void MainWindowImpl::pushButtonExtAPressed() {
	extInput.setCurrSelectedButton(10);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key A function");
}

void MainWindowImpl::pushButtonExtBPressed() {
	extInput.setCurrSelectedButton(11);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key B function");
}

void MainWindowImpl::pushButtonExtCPressed() {
	extInput.setCurrSelectedButton(12);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key C function");
}

void MainWindowImpl::pushButtonExtDPressed() {
	extInput.setCurrSelectedButton(13);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key D function");
}

void MainWindowImpl::pushButtonExtEPressed() {
	extInput.setCurrSelectedButton(14);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key E function");
}

void MainWindowImpl::pushButtonExtFPressed() {
	extInput.setCurrSelectedButton(15);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key F function");
}

void MainWindowImpl::pushButtonExtGPressed() {
	extInput.setCurrSelectedButton(16);
	
	comboBoxExtKeypadFunc->setCurrentIndex(extInput.getButtonFunction());
	labelExtKeyPadFunc->setText("Key G function");
}

void MainWindowImpl::comboBoxExtKeypadFuncIndexChanged(int funcIndex) {
		extInput.setButtonFunction(funcIndex);
}

void MainWindowImpl::spinBoxNetworkAddressValueChanged(int newValue) {
	settingsClass.setNetworkAddress(newValue);
}

void MainWindowImpl::checkBoxNetworkIsMasterClicked(bool state) {
	settingsClass.setDeviceIsMaster(state);
}
		
void MainWindowImpl::spinBoxNetworkNrOfDevicesValueChanged(int newValue) {
	settingsClass.setNumberOfDevices(newValue);
}

void MainWindowImpl::comboBoxRotatorsIndexChanged(int index) {
	if ((index > 0) && (index <= rotators.getRotatorCount())) {
		lineEditRotatorName->setText(rotators.getRotatorName(index-1));
		spinBoxRotatorSubAddress->setValue(rotators.getRotatorSubAddress(index-1));
		spinBoxRotatorAddress->setValue(rotators.getRotatorAddress(index-1));
		spinBoxRotatorStartAzimuth->setValue(rotators.getRotatorStartHeading(index-1));
		spinBoxRotatorDegreesAzimuth->setValue(rotators.getRotatorDegrees(index-1));
	}
	else if (index == 0) {
		lineEditRotatorName->setText("");
		spinBoxRotatorAddress->setValue(spinBoxRotatorAddress->value()+1);
		spinBoxRotatorSubAddress->setValue(0);
		spinBoxRotatorStartAzimuth->setValue(0);
		spinBoxRotatorDegreesAzimuth->setValue(360);
	}
}

void MainWindowImpl::comboBoxRotatorsReload() {
	comboBoxRotators->blockSignals(true);
	
	comboBoxRotators->clear();
	
	comboBoxRotators->addItem("None");
	comboBoxRotators->addItems(rotators.getNameList());
	
	comboBoxRotators->blockSignals(false);
	
	
	comboBoxAntennaRotator1->blockSignals(true);
	comboBoxAntennaRotator2->blockSignals(true);	
	comboBoxAntennaRotator3->blockSignals(true);	
	comboBoxAntennaRotator4->blockSignals(true);	
	
	comboBoxAntennaRotator1->clear();
	comboBoxAntennaRotator2->clear();
	comboBoxAntennaRotator3->clear();
	comboBoxAntennaRotator4->clear();
	
	comboBoxAntennaRotator1->addItem("None");
	comboBoxAntennaRotator2->addItem("None");
	comboBoxAntennaRotator3->addItem("None");
	comboBoxAntennaRotator4->addItem("None");
	
	comboBoxAntennaRotator1->addItems(rotators.getNameList());
	comboBoxAntennaRotator2->addItems(rotators.getNameList());
	comboBoxAntennaRotator3->addItems(rotators.getNameList());
	comboBoxAntennaRotator4->addItems(rotators.getNameList());
	
	comboBoxAntennaRotator1->blockSignals(false);
	comboBoxAntennaRotator2->blockSignals(false);
	comboBoxAntennaRotator3->blockSignals(false);
	comboBoxAntennaRotator4->blockSignals(false);
	
	reloadCurrentRotatorProperties();
}

void MainWindowImpl::pushButtonRotatorAddPressed() {
	rotators.addRotator(lineEditRotatorName->text(), spinBoxRotatorAddress->value(), spinBoxRotatorSubAddress->value(), spinBoxRotatorStartAzimuth->value(), spinBoxRotatorDegreesAzimuth->value());

	comboBoxRotatorsReload();
	comboBoxRotators->setCurrentIndex(comboBoxRotators->count()-1);
}

void MainWindowImpl::pushButtonRotatorSavePressed() {
	int tempIndex = comboBoxRotators->currentIndex();
	
	rotators.changeRotatorProperties(tempIndex-1, lineEditRotatorName->text(), spinBoxRotatorAddress->value(), spinBoxRotatorSubAddress->value(), spinBoxRotatorStartAzimuth->value(), spinBoxRotatorDegreesAzimuth->value());
	
	comboBoxRotatorsReload();
	comboBoxRotators->setCurrentIndex(tempIndex);
	
	reloadCurrentRotatorProperties();
}
	
void MainWindowImpl::pushButtonRotatorDeletePressed() {
	rotators.deleteRotator(comboBoxRotators->currentIndex()-1);
	
	comboBoxRotatorsReload();
}

void MainWindowImpl::comboBoxAntennaRotator1IndexChanged(int newIndex) {
	if (newIndex > 0) {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(0,rotators.getRotatorIndex(newIndex-1), rotators.getRotatorAddress(newIndex-1), rotators.getRotatorSubAddress(newIndex-1), rotators.getRotatorStartHeading(newIndex-1), rotators.getRotatorDegrees(newIndex-1));
		bandData[comboBoxBand->currentIndex()].setHasRotator(0,true);
	}
	else {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(0,-1,0,0,0,0);
		bandData[comboBoxBand->currentIndex()].setHasRotator(0,false);
	}
}

void MainWindowImpl::comboBoxAntennaRotator2IndexChanged(int newIndex) {
	if (newIndex > 0) {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(1,rotators.getRotatorIndex(newIndex-1), rotators.getRotatorAddress(newIndex-1), rotators.getRotatorSubAddress(newIndex-1), rotators.getRotatorStartHeading(newIndex-1), rotators.getRotatorDegrees(newIndex-1));
		bandData[comboBoxBand->currentIndex()].setHasRotator(1,true);
	}
	else {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(1,-1,0,0,0,0);
		bandData[comboBoxBand->currentIndex()].setHasRotator(1,false);
	}
}

void MainWindowImpl::comboBoxAntennaRotator3IndexChanged(int newIndex) {
	if (newIndex > 0) {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(2,rotators.getRotatorIndex(newIndex-1), rotators.getRotatorAddress(newIndex-1), rotators.getRotatorSubAddress(newIndex-1), rotators.getRotatorStartHeading(newIndex-1), rotators.getRotatorDegrees(newIndex-1));
		bandData[comboBoxBand->currentIndex()].setHasRotator(2,true);
	}
	else {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(2,-1,0,0,0,0);
		bandData[comboBoxBand->currentIndex()].setHasRotator(2,false);
	}
}
	
void MainWindowImpl::comboBoxAntennaRotator4IndexChanged(int newIndex) {
	if (newIndex > 0) {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(3,rotators.getRotatorIndex(newIndex-1), rotators.getRotatorAddress(newIndex-1), rotators.getRotatorSubAddress(newIndex-1), rotators.getRotatorStartHeading(newIndex-1), rotators.getRotatorDegrees(newIndex-1));
		bandData[comboBoxBand->currentIndex()].setHasRotator(3,true);
	}
	else {
		bandData[comboBoxBand->currentIndex()].setRotatorProperties(3,-1,0,0,0,0);
		bandData[comboBoxBand->currentIndex()].setHasRotator(3,false);
	}
}

void MainWindowImpl::spinBoxPowerMeterAdressValueChanged(int value) {
	if (checkBoxPowerMeterAllBands->isChecked())
		settingsClass.setPowerMeterAddress(99,value);
	else
		settingsClass.setPowerMeterAddress(comboBoxPowerMeterBand->currentIndex(),value);
}

void MainWindowImpl::spinBoxPowerMeterSWRValueChanged(double value) {
		settingsClass.setPowerMeterVSWRAlarm(value);
}

void MainWindowImpl::spinBoxPowerMeterUpdateRateTextValueChanged(int rate) {
		settingsClass.setPowerMeterUpdateRateText(rate);
}

void MainWindowImpl::spinBoxPowerMeterUpdateRateBargraphValueChanged(int rate) {
		settingsClass.setPowerMeterUpdateRateBargraph(rate);
}

void MainWindowImpl::radioButtonPTTInterlockNoneClicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(0);
}

void MainWindowImpl::radioButtonPTTInterlockInput1Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(1);
}

void MainWindowImpl::radioButtonPTTInterlockInput2Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(2);
}

void MainWindowImpl::radioButtonPTTInterlockInput3Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(3);
}

void MainWindowImpl::radioButtonPTTInterlockInput4Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(4);
}

void MainWindowImpl::radioButtonPTTInterlockInput5Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(5);
}

void MainWindowImpl::radioButtonPTTInterlockInput6Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(6);
}

void MainWindowImpl::radioButtonPTTInterlockInput7Clicked(bool state) {
		if (state == true)
				settingsClass.setPTTInterlockInput(7);
}

void MainWindowImpl::comboBoxButtonAUX1IndexChanged(int funcIndex) {
	extInput.setAUXFunc(1,funcIndex);
}

void MainWindowImpl::comboBoxButtonAUX2IndexChanged(int funcIndex) {
	extInput.setAUXFunc(2,funcIndex);
}

void MainWindowImpl::checkBoxPowerMeterAllBandsClicked(bool state) {
	if (state == true) {
		settingsClass.setPowerMeterAddress(99,spinBoxPowerMeterAddress->value());
	}
}

void MainWindowImpl::comboBoxPowerMeterBandChanged(int index) {
	spinBoxPowerMeterAddress->setValue(settingsClass.getPowerMeterAddress(index));
}

void MainWindowImpl::setupConnections() {
	/* RADIO INTERFACE START */
	connect(comboBoxRadioType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadioTypeCurrentIndexChanged(int)));
	connect(comboBoxRadioMode, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadioModeCurrentIndexChanged(int)));
	connect(comboBoxRadioBaudRate, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadioBaudRateCurrentIndexChanged(int)));
	connect(comboBoxRadioStopbits, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadioStopbitsCurrentIndexChanged(int)));
	connect(spinBoxCIVAddress, SIGNAL(valueChanged(int)), this, SLOT(spinBoxCIVAddressValueChanged(int)));
	connect(sliderPollInterval, SIGNAL(valueChanged(int)), this, SLOT(sliderPollIntervalChanged(int)));
	connect(groupBoxCAT, SIGNAL(clicked(bool)), this, SLOT(groupBoxCATClicked(bool)));
	connect(comboBoxRadioSenseInput, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadioSenseInputCurrentIndexChanged(int)));
	/* RADIO INTERFACE END */
	
	/* BAND LIMITS START */
	connect(lineEditBandLimitHighSegHighLimit, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandLimitHighSegHighLimitChanged(QString)));
	connect(lineEditBandLimitHighSegLowLimit, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandLimitHighSegLowLimitChanged(QString)));
	connect(lineEditBandLimitLowSegHighLimit, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandLimitLowSegHighLimitChanged(QString)));
	connect(lineEditBandLimitLowSegLowLimit, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandLimitLowSegLowLimitChanged(QString)));
	connect(comboBoxBandLimitBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxBandLimitBandCurrentIndexChanged(int)));
	connect(pushButtonDefaultBandLimits, SIGNAL(pressed()), this, SLOT(pushButtonDefaultBandLimitsPressed()));
	/* BAND LIMITS END */
	
	/* TX ANTENNAS START */
	connect(lineEditAntennaName1, SIGNAL(textChanged(QString)), this, SLOT(lineEditAntennaName1Changed(QString)));
	connect(lineEditAntennaName2, SIGNAL(textChanged(QString)), this, SLOT(lineEditAntennaName2Changed(QString)));
	connect(lineEditAntennaName3, SIGNAL(textChanged(QString)), this, SLOT(lineEditAntennaName3Changed(QString)));
	connect(lineEditAntennaName4, SIGNAL(textChanged(QString)), this, SLOT(lineEditAntennaName4Changed(QString)));
	connect(lineEditAntennaOutputStr, SIGNAL(textChanged(QString)), this, SLOT(lineEditAntennaOutputStrChanged(QString)));	
	connect(comboBoxAntennaCombination, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxAntennaCombinationCurrentIndexChanged(int)));	
	connect(checkBoxCombinationAllowed, SIGNAL(clicked(bool)), this, SLOT(checkBoxCombinationAllowedClicked(bool)));
	connect(checkBoxMultiband1, SIGNAL(clicked(bool)), this, SLOT(checkBoxMultiband1Clicked(bool)));
	connect(checkBoxMultiband2, SIGNAL(clicked(bool)), this, SLOT(checkBoxMultiband2Clicked(bool)));
	connect(checkBoxMultiband3, SIGNAL(clicked(bool)), this, SLOT(checkBoxMultiband3Clicked(bool)));
	connect(checkBoxMultiband4, SIGNAL(clicked(bool)), this, SLOT(checkBoxMultiband4Clicked(bool)));
	
	connect(lineEditBandOutputStrHigh, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandOutputStrHighChanged(QString)));
	connect(lineEditBandOutputStrLow, SIGNAL(textChanged(QString)), this, SLOT(lineEditBandOutputStrLowChanged(QString)));
	connect(comboBoxBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxBandCurrentIndexChanged(int)));
	
	connect(lineEditRXAntennaName1, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName1Changed(QString)));
	connect(lineEditRXAntennaName2, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName2Changed(QString)));
	connect(lineEditRXAntennaName3, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName3Changed(QString)));
	connect(lineEditRXAntennaName4, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName4Changed(QString)));
	connect(lineEditRXAntennaName5, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName5Changed(QString)));
	connect(lineEditRXAntennaName6, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName6Changed(QString)));
	connect(lineEditRXAntennaName7, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName7Changed(QString)));
	connect(lineEditRXAntennaName8, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName8Changed(QString)));
	connect(lineEditRXAntennaName9, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName9Changed(QString)));
	connect(lineEditRXAntennaName10, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaName10Changed(QString)));
	
	connect(lineEditRXAntennaOutputs1, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs1Changed(QString)));
	connect(lineEditRXAntennaOutputs2, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs2Changed(QString)));
	connect(lineEditRXAntennaOutputs3, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs3Changed(QString)));
	connect(lineEditRXAntennaOutputs4, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs4Changed(QString)));
	connect(lineEditRXAntennaOutputs5, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs5Changed(QString)));
	connect(lineEditRXAntennaOutputs6, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs6Changed(QString)));
	connect(lineEditRXAntennaOutputs7, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs7Changed(QString)));
	connect(lineEditRXAntennaOutputs8, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs8Changed(QString)));
	connect(lineEditRXAntennaOutputs9, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs9Changed(QString)));
	connect(lineEditRXAntennaOutputs10, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputs10Changed(QString)));
	
	connect(lineEditRXAntennaOutputStr160M, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputStr160MChanged(QString)));
	connect(lineEditRXAntennaOutputStr80M, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputStr80MChanged(QString)));
	connect(lineEditRXAntennaOutputStr40M, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputStr40MChanged(QString)));
	connect(lineEditRXAntennaOutputStr30M, SIGNAL(textChanged(QString)), this, SLOT(lineEditRXAntennaOutputStr30MChanged(QString)));
	
	connect(radioButtonAntennaDefault1, SIGNAL(clicked(bool)), this, SLOT(radioButtonAntennaDefault1Clicked(bool)));
	connect(radioButtonAntennaDefault2, SIGNAL(clicked(bool)), this, SLOT(radioButtonAntennaDefault2Clicked(bool)));
	connect(radioButtonAntennaDefault3, SIGNAL(clicked(bool)), this, SLOT(radioButtonAntennaDefault3Clicked(bool)));
	connect(radioButtonAntennaDefault4, SIGNAL(clicked(bool)), this, SLOT(radioButtonAntennaDefault4Clicked(bool)));
	
	/* TX ANTENNAS END */
	
	/* MENU START */
	connect(actionSave, SIGNAL(triggered()), this, SLOT(actionSaveTriggered()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(actionOpenTriggered()));	
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(actionQuitTriggered()));
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
	connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));
	connect(actionGetFirmwareRev, SIGNAL(triggered()), this, SLOT(actionGetFirmwareRevTriggered()));
	connect(actionExitConfMode, SIGNAL(triggered()), this, SLOT(actionExitConfModeTriggered()));
	connect(actionSendSettings, SIGNAL(triggered()), this, SLOT(actionSendSettingsTriggered()));
	/* MENU END */
	
	/* SEQUENCER START */
	connect(groupBoxSequencerPTTRadio, SIGNAL(clicked(bool)), this, SLOT(groupBoxSequencerPTTRadioClicked(bool)));
	connect(groupBoxSequencerPTTAmp, SIGNAL(clicked(bool)), this, SLOT(groupBoxSequencerPTTAmpClicked(bool)));
	connect(groupBoxSequencerInhibitEnabled, SIGNAL(clicked(bool)), this, SLOT(groupBoxSequencerInhibitClicked(bool)));
	connect(spinBoxSequencerPTTRadioPreDelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerPTTRadioPreDelayValueChanged(int)));
	connect(spinBoxSequencerPTTRadioPostDelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerPTTRadioPostDelayValueChanged(int)));
	connect(spinBoxSequencerPTTAmpPreDelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerPTTAmpPreDelayValueChanged(int)));
	connect(spinBoxSequencerPTTAmpPostDelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerPTTAmpPostDelayValueChanged(int)));
	connect(spinBoxSequencerInhibitPredelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerInhibitPredelayValueChanged(int)));
	connect(spinBoxSequencerInhibitPostdelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerInhibitPostdelayValueChanged(int)));
	connect(spinBoxSequencerAntennaPostDelay, SIGNAL(valueChanged(int)), this, SLOT(spinBoxSequencerAntennaPostDelayValueChanged(int)));
	connect(checkBoxPTTInputFootswitch, SIGNAL(clicked(bool)), this, SLOT(checkBoxPTTInputFootswitchClicked(bool)));
	connect(checkBoxPTTInputComputerRTS, SIGNAL(clicked(bool)), this, SLOT(checkBoxPTTInputComputerRTSClicked(bool)));
	connect(checkBoxPTTInputRadioSense, SIGNAL(clicked(bool)), this, SLOT(checkBoxPTTInputRadioSenseClicked(bool)));
	connect(checkBoxPTTInputComputerRTSInverted, SIGNAL(clicked(bool)), this, SLOT(checkBoxPTTInputComputerRTSInvertedClicked(bool)));
	connect(checkBoxPTTInputRadioSenseInverted, SIGNAL(clicked(bool)), this, SLOT(checkBoxPTTInputRadioSenseInvertedClicked(bool)));
	connect(radioButtonInhibitPolarityActiveLow, SIGNAL(clicked(bool)), this, SLOT(radioButtonInhibitPolarityActiveLowClicked(bool)));
	connect(radioButtonInhibitPolarityActiveHigh, SIGNAL(clicked(bool)), this, SLOT(radioButtonInhibitPolarityActiveHighClicked(bool)));
	connect(comboBoxSequencerSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSequencerSelectCurrentIndexChanged(int)));
	/* SEQUENCER END */	
	
	/* SUB MENUS */
	connect(pushButtonSubMenuAnt1, SIGNAL(pressed()), this, SLOT(pushButtonSubMenuAnt1Pressed()));
	connect(pushButtonSubMenuAnt2, SIGNAL(pressed()), this, SLOT(pushButtonSubMenuAnt2Pressed()));
	connect(pushButtonSubMenuAnt3, SIGNAL(pressed()), this, SLOT(pushButtonSubMenuAnt3Pressed()));
	connect(pushButtonSubMenuAnt4, SIGNAL(pressed()), this, SLOT(pushButtonSubMenuAnt4Pressed()));	
	connect(checkBoxSubMenuEnabledAnt1, SIGNAL(clicked(bool)), this, SLOT(checkBoxSubMenuEnabledAnt1Clicked(bool)));
	connect(checkBoxSubMenuEnabledAnt2, SIGNAL(clicked(bool)), this, SLOT(checkBoxSubMenuEnabledAnt2Clicked(bool)));
	connect(checkBoxSubMenuEnabledAnt3, SIGNAL(clicked(bool)), this, SLOT(checkBoxSubMenuEnabledAnt3Clicked(bool)));
	connect(checkBoxSubMenuEnabledAnt4, SIGNAL(clicked(bool)), this, SLOT(checkBoxSubMenuEnabledAnt4Clicked(bool)));
	
	/* External keypad */
	connect(pushButtonExt0, SIGNAL(pressed()), this, SLOT(pushButtonExt0Pressed()));
	connect(pushButtonExt1, SIGNAL(pressed()), this, SLOT(pushButtonExt1Pressed()));
	connect(pushButtonExt2, SIGNAL(pressed()), this, SLOT(pushButtonExt2Pressed()));
	connect(pushButtonExt3, SIGNAL(pressed()), this, SLOT(pushButtonExt3Pressed()));
	connect(pushButtonExt4, SIGNAL(pressed()), this, SLOT(pushButtonExt4Pressed()));
	connect(pushButtonExt5, SIGNAL(pressed()), this, SLOT(pushButtonExt5Pressed()));
	connect(pushButtonExt6, SIGNAL(pressed()), this, SLOT(pushButtonExt6Pressed()));
	connect(pushButtonExt7, SIGNAL(pressed()), this, SLOT(pushButtonExt7Pressed()));
	connect(pushButtonExt8, SIGNAL(pressed()), this, SLOT(pushButtonExt8Pressed()));
	connect(pushButtonExt9, SIGNAL(pressed()), this, SLOT(pushButtonExt9Pressed()));
	
	connect(pushButtonExtA, SIGNAL(pressed()), this, SLOT(pushButtonExtAPressed()));
	connect(pushButtonExtB, SIGNAL(pressed()), this, SLOT(pushButtonExtBPressed()));
	connect(pushButtonExtC, SIGNAL(pressed()), this, SLOT(pushButtonExtCPressed()));
	connect(pushButtonExtD, SIGNAL(pressed()), this, SLOT(pushButtonExtDPressed()));
	connect(pushButtonExtE, SIGNAL(pressed()), this, SLOT(pushButtonExtEPressed()));
	connect(pushButtonExtF, SIGNAL(pressed()), this, SLOT(pushButtonExtFPressed()));
	connect(pushButtonExtG, SIGNAL(pressed()), this, SLOT(pushButtonExtGPressed()));
	
	connect(comboBoxExtKeypadFunc, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxExtKeypadFuncIndexChanged(int)));
	
	connect(comboBoxButtonAUX1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxButtonAUX1IndexChanged(int)));
	connect(comboBoxButtonAUX2, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxButtonAUX2IndexChanged(int)));

	connect(spinBoxNetworkAddress, SIGNAL(valueChanged(int)), this, SLOT(spinBoxNetworkAddressValueChanged(int)));
	connect(checkBoxNetworkIsMaster, SIGNAL(clicked(bool)), this, SLOT(checkBoxNetworkIsMasterClicked(bool)));
	connect(spinBoxNetworkNrOfDevices, SIGNAL(valueChanged(int)), this, SLOT(spinBoxNetworkNrOfDevicesValueChanged(int)));
	
	connect(checkBoxPowerMeterAllBands, SIGNAL(clicked(bool)), this, SLOT(checkBoxPowerMeterAllBandsClicked(bool)));
	connect(comboBoxPowerMeterBand, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxPowerMeterBandChanged(int)));
	connect(spinBoxPowerMeterAddress, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPowerMeterAdressValueChanged(int)));
	connect(doubleSpinBoxPowerMeterSWR, SIGNAL(valueChanged(double)), this, SLOT(spinBoxPowerMeterSWRValueChanged(double)));
	connect(spinBoxPowerMeterUpdateRateText, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPowerMeterUpdateRateTextValueChanged(int)));
	connect(spinBoxPowerMeterUpdateRateBargraph, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPowerMeterUpdateRateBargraphValueChanged(int)));

	connect(radioButtonPTTInterlockNone, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockNoneClicked(bool)));
	connect(radioButtonPTTInterlockInput1, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput1Clicked(bool)));
	connect(radioButtonPTTInterlockInput2, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput2Clicked(bool)));
	connect(radioButtonPTTInterlockInput3, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput3Clicked(bool)));
	connect(radioButtonPTTInterlockInput4, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput4Clicked(bool)));
	connect(radioButtonPTTInterlockInput5, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput5Clicked(bool)));
	connect(radioButtonPTTInterlockInput6, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput6Clicked(bool)));
	connect(radioButtonPTTInterlockInput7, SIGNAL(clicked(bool)), this, SLOT(radioButtonPTTInterlockInput7Clicked(bool)));


	connect(comboBoxRotators, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRotatorsIndexChanged(int)));
	connect(pushButtonRotatorAdd, SIGNAL(pressed()), this, SLOT(pushButtonRotatorAddPressed()));
	connect(pushButtonRotatorSave, SIGNAL(pressed()), this, SLOT(pushButtonRotatorSavePressed()));
	connect(pushButtonRotatorDelete, SIGNAL(pressed()), this, SLOT(pushButtonRotatorDeletePressed()));
	
	connect(comboBoxAntennaRotator1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxAntennaRotator1IndexChanged(int)));
	connect(comboBoxAntennaRotator2, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxAntennaRotator2IndexChanged(int)));
	connect(comboBoxAntennaRotator3, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxAntennaRotator3IndexChanged(int)));
	connect(comboBoxAntennaRotator4, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxAntennaRotator4IndexChanged(int)));
}

void MainWindowImpl::showInformationDialog(QString caption, QString text) {
	qDebug("poop");
}

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f) {
	setupUi(this);

	listExtButtonFunctionsLoad();

	//! Set the sequencer names, used for saving settings etc */
	sequencerComputer.setName("SequencerComputer");
	sequencerRadioSense.setName("SequencerRadioSense");
	sequencerFootswitch.setName("SequencerFootswitch");
	
	setupConnections();

	//Set the index of the class	
	for (int i=0;i<9;i++) {
		bandData[i].setIndex(i);
		bandData[i].setDefaultBandLimits();
	}

	loadInitialGUIValues();
}

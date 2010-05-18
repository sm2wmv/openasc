#include "external_input.h"

ExternalInputClass::ExternalInputClass() {
	//Set all the buttons to have no function assigned to them at startup
	for (int i=0;i<17;i++)
		buttonFunction[i] = 0;

	currSelectedButtonIndex = 10;
}

void ExternalInputClass::writeSettings(QSettings& settings) {
	settings.beginGroup("ExternalInput");
	settings.beginWriteArray("ButtonFunction");

	for (int i=0;i<17;i++){
		settings.setArrayIndex(i);
		settings.setValue("buttonFunctionArray",buttonFunction[i]);
	}
		
	settings.endArray();

	settings.endGroup();
}

void ExternalInputClass::loadSettings(QSettings& settings) {
	settings.beginGroup("ExternalInput");
	
	int size = settings.beginReadArray("ButtonFunction");
	
	for (int i=0;i<size;i++) {
		settings.setArrayIndex(i);
		buttonFunction[i] = settings.value("buttonFunctionArray").toInt();
	}
	
	settings.endArray();

	settings.endGroup();
}

void ExternalInputClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[18];
		
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS;
	
	for (int x=0;x<17;x++)
		tx_buff[1+x] = buttonFunction[x];
	
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, sizeof(tx_buff), tx_buff);
}

void ExternalInputClass::setCurrSelectedButton(int index) {
	currSelectedButtonIndex = index;
}

int ExternalInputClass::getCurrSelectedButton() {
	return(currSelectedButtonIndex);
}

void ExternalInputClass::setButtonFunction(int funcIndex) {
	buttonFunction[currSelectedButtonIndex] = funcIndex;
}

int ExternalInputClass::getButtonFunction() {
	return(buttonFunction[currSelectedButtonIndex]);
}

#include "external_input.h"

ExternalInputClass::ExternalInputClass() {
	//Set all the buttons to have no function assigned to them at startup
	for (int i=0;i<17;i++)
		buttonFunction[i] = 0;

	buttonAUX1Func = 0;
	buttonAUX2Func = 0;

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

	settings.setValue("buttonAUX1Func",buttonAUX1Func);
	settings.setValue("buttonAUX2Func",buttonAUX2Func);

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

	buttonAUX1Func = settings.value("buttonAUX1Func").toInt();
	buttonAUX2Func = settings.value("buttonAUX2Func").toInt();

	settings.endGroup();
}

void ExternalInputClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[20];
		
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS;
	
	for (int x=0;x<17;x++)
		tx_buff[1+x] = buttonFunction[x];

	tx_buff[18] = buttonAUX1Func;
	tx_buff[19] = buttonAUX2Func;

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

void ExternalInputClass::setAUXFunc(unsigned char index, unsigned char funcIndex) {
	if (index == 1)
		buttonAUX1Func = funcIndex;
	else if (index == 2)
			buttonAUX2Func = funcIndex;
}

int ExternalInputClass::getAUXFunc(unsigned char index) {
	if (index == 1)
		return(buttonAUX1Func);
	else if (index == 2)
		return(buttonAUX2Func);

	return(0);
}

#include "settings.h"

SettingsClass::SettingsClass() {
	//Set the default values
	networkAddress = 0x01;
	deviceIsMaster = false;
	nrOfDevices = 10;
}

void SettingsClass::writeSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	settings.setValue("networkAddress",networkAddress);
	
	if (deviceIsMaster == true)
		settings.setValue("deviceIsMaster",1);
	else
		settings.setValue("deviceIsMaster",0);
	
	settings.setValue("nrOfDevices",nrOfDevices);
	
	settings.endGroup();
}

void SettingsClass::loadSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	networkAddress = settings.value("networkAddress").toInt();
	
	if (settings.value("deviceIsMaster").toInt() == 1)
		deviceIsMaster = true;
	else
		deviceIsMaster = false;
	
	nrOfDevices = settings.value("nrOfDevices").toInt();
	
	settings.endGroup();
}

void SettingsClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[2];
		
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_ADDRESS;
	tx_buff[1] = networkAddress;
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, sizeof(tx_buff), tx_buff);
	
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_NR_NODES;
	tx_buff[1] = nrOfDevices;
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, sizeof(tx_buff), tx_buff);
	
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_DEVICE_IS_MASTER;
	if (deviceIsMaster)
		tx_buff[1] = 1;
	else
		tx_buff[1] = 0;
	
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, sizeof(tx_buff), tx_buff);
	
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_SAVE;
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 1, tx_buff);
}

void SettingsClass::setNetworkAddress(int value) {
	networkAddress = value;
}

int SettingsClass::getNetworkAddress() {
	return(networkAddress);
}

void SettingsClass::setDeviceIsMaster(bool state) {
	deviceIsMaster = state;
}

bool SettingsClass::getDeviceIsMaster() {
	return(deviceIsMaster);
}

void SettingsClass::setNumberOfDevices(int deviceCount) {
	nrOfDevices = deviceCount;
}

int SettingsClass::getNumberOfDevices() {
	return(nrOfDevices);
}
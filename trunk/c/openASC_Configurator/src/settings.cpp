#include "settings.h"

SettingsClass::SettingsClass() {
	//Set the default values
	networkAddress = 0x01;
	deviceIsMaster = false;
	nrOfDevices = 10;

	for (int i=0;i<9;i++)
		powerMeterAddress[i] = 0;

	powerMeterVSWRAlarmValue = 0;
	powerMeterUpdateRateText = 500;
	powerMeterUpdateRateBargraph = 200;

	pttInterlockInput = 0;

	//Default amplifier status
	ampControlEnabled = false;
	ampAddr = 1;
	ampSubAddr = 0;
	ampFuncStatus = 0;
	ampBandSegmentCount = 1;
}

void SettingsClass::writeSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	settings.setValue("networkAddress",networkAddress);

	if (deviceIsMaster == true)
		settings.setValue("deviceIsMaster",1);
	else
		settings.setValue("deviceIsMaster",0);
	
	settings.setValue("nrOfDevices",nrOfDevices);
	
	settings.setValue("PowerMeterAddress[0]",powerMeterAddress[0]);
	settings.setValue("PowerMeterAddress[1]",powerMeterAddress[1]);
	settings.setValue("PowerMeterAddress[2]",powerMeterAddress[2]);
	settings.setValue("PowerMeterAddress[3]",powerMeterAddress[3]);
	settings.setValue("PowerMeterAddress[4]",powerMeterAddress[4]);
	settings.setValue("PowerMeterAddress[5]",powerMeterAddress[5]);
	settings.setValue("PowerMeterAddress[6]",powerMeterAddress[6]);
	settings.setValue("PowerMeterAddress[7]",powerMeterAddress[7]);
	settings.setValue("PowerMeterAddress[8]",powerMeterAddress[8]);

	settings.setValue("PowerMeterVSWRAlarmValue",powerMeterVSWRAlarmValue);
	settings.setValue("PowerMeterDisplayUpdateRateText",powerMeterUpdateRateText);
	settings.setValue("PowerMeterDisplayUpdateRateBargraph",powerMeterUpdateRateBargraph);

	settings.setValue("PTTInterlockInput",pttInterlockInput);

	settings.setValue("AmpControlEnabled",ampControlEnabled);
	settings.setValue("AmpAddr",ampAddr);
	settings.setValue("AmpSubAddr",ampSubAddr);
	settings.setValue("AmpFuncStatus",ampFuncStatus);
	settings.setValue("AmpBandSegmentCount",ampBandSegmentCount);

	settings.endGroup();
}

void SettingsClass::loadSettings(QSettings& settings) {
	settings.beginGroup("Settings");

	networkAddress = settings.value("networkAddress").toInt();
	
	if (settings.value("deviceIsMaster").toInt() == 1)
		deviceIsMaster = true;
	else
		deviceIsMaster = false;
	
	powerMeterAddress[0] = settings.value("PowerMeterAddress[0]").toInt();
	powerMeterAddress[1] = settings.value("PowerMeterAddress[1]").toInt();
	powerMeterAddress[2] = settings.value("PowerMeterAddress[2]").toInt();
	powerMeterAddress[3] = settings.value("PowerMeterAddress[3]").toInt();
	powerMeterAddress[4] = settings.value("PowerMeterAddress[4]").toInt();
	powerMeterAddress[5] = settings.value("PowerMeterAddress[5]").toInt();
	powerMeterAddress[6] = settings.value("PowerMeterAddress[6]").toInt();
	powerMeterAddress[7] = settings.value("PowerMeterAddress[7]").toInt();
	powerMeterAddress[8] = settings.value("PowerMeterAddress[8]").toInt();

	powerMeterVSWRAlarmValue = settings.value("PowerMeterVSWRAlarmValue").toDouble();
	powerMeterUpdateRateText = settings.value("PowerMeterDisplayUpdateRateText").toInt();
	powerMeterUpdateRateBargraph = settings.value("PowerMeterDisplayUpdateRateBargraph").toInt();

	pttInterlockInput = settings.value("PTTInterlockInput").toInt();

	nrOfDevices = settings.value("nrOfDevices").toInt();
	
	ampControlEnabled = settings.value("AmpControlEnabled").toBool();
	ampAddr = settings.value("AmpAddr").toInt();
	ampSubAddr = settings.value("AmpSubAddr").toInt();
	ampFuncStatus = settings.value("AmpFuncStatus").toInt();
	ampBandSegmentCount = settings.value("AmpBandSegmentCount").toInt();

	settings.endGroup();
}

void SettingsClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[30];
		
	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_NETWORK;
	tx_buff[1] = networkAddress;
	tx_buff[2] = nrOfDevices;

	if (deviceIsMaster)
		tx_buff[3] = 1;
	else
		tx_buff[3] = 0;
	
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 4, tx_buff);

	tx_buff[0] = CTRL_SET_POWERMETER_SETTINGS;
	tx_buff[1] = powerMeterAddress[0];
	tx_buff[2] = powerMeterAddress[1];
	tx_buff[3] = powerMeterAddress[2];
	tx_buff[4] = powerMeterAddress[3];
	tx_buff[5] = powerMeterAddress[4];
	tx_buff[6] = powerMeterAddress[5];
	tx_buff[7] = powerMeterAddress[6];
	tx_buff[8] = powerMeterAddress[7];
	tx_buff[9] = powerMeterAddress[8];
	tx_buff[10] = ((unsigned int)(powerMeterVSWRAlarmValue * 100) & 0xFF00) >> 8;
	tx_buff[11] = (unsigned int)(powerMeterVSWRAlarmValue * 100) & 0x00FF;
	tx_buff[12] = (powerMeterUpdateRateText >> 8);
	tx_buff[13] = (powerMeterUpdateRateText & 0x00FF);
	tx_buff[14] = (powerMeterUpdateRateBargraph >> 8);
	tx_buff[15] = (powerMeterUpdateRateBargraph & 0x00FF);	

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 16, tx_buff);

	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_OTHER;
	tx_buff[1] = pttInterlockInput;

	if (ampControlEnabled)
		tx_buff[2] = 1;
	else
		tx_buff[2] = 0;

	tx_buff[3] = ampAddr;	// Amp addr
	tx_buff[4] = ampSubAddr; // Amp sub addr
	tx_buff[5] = ampFuncStatus & 0x00FF; // Amp functions lower bits
	tx_buff[6] = (ampFuncStatus & 0xFF00) >> 8; // Amp functions higher bits
	tx_buff[7] = ampBandSegmentCount; // The number of band segments the amplifier has got

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 8, tx_buff);

	tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_SAVE;
	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 1, tx_buff);
}

void SettingsClass::setPTTInterlockInput(unsigned char value) {
		pttInterlockInput = value;
}

unsigned char SettingsClass::getPTTInterlockInput(void) {
		return(pttInterlockInput);
}

void SettingsClass::setPowerMeterAddress(unsigned char band, unsigned char address) {
	if (band == 99) {
		for (int i=0;i<9;i++)
			powerMeterAddress[i] = address;
	}
	else
		powerMeterAddress[band] = address;
}

unsigned char SettingsClass::getPowerMeterAddress(unsigned char band) {
		return(powerMeterAddress[band]);
}

void SettingsClass::setPowerMeterVSWRAlarm(double swr) {
		powerMeterVSWRAlarmValue = swr;
}

double SettingsClass::getPowerMeterVSWRAlarm(void) {
		return(powerMeterVSWRAlarmValue);
}

void SettingsClass::setPowerMeterUpdateRateText(unsigned int rate) {
		powerMeterUpdateRateText = rate;
}

void SettingsClass::setPowerMeterUpdateRateBargraph(unsigned int rate) {
		powerMeterUpdateRateBargraph = rate;
}

unsigned int SettingsClass::getPowerMeterUpdateRateText(void) {
		return(powerMeterUpdateRateText);
}

unsigned int SettingsClass::getPowerMeterUpdateRateBargraph(void) {
		return(powerMeterUpdateRateBargraph);
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

void SettingsClass::setAmpFuncStatus(unsigned char index, bool state) {
	if (state)
		ampFuncStatus |= (1<<index);
	else
		ampFuncStatus &= ~(1<<index);
}

bool SettingsClass::getAmpFuncStatus(unsigned char index) {
	if (ampFuncStatus & (1<<index))
		return(true);
	else
		return(false);
}

void SettingsClass::setAmpControlEnabled(bool state) {
	ampControlEnabled = state;
}

bool SettingsClass::getAmpControlEnabled() {
	return(ampControlEnabled);
}

void SettingsClass::setAmpAddr(unsigned char addr) {
	ampAddr = addr;
}

void SettingsClass::setAmpSubAddr(unsigned char addr) {
	ampSubAddr = addr;
}

unsigned char SettingsClass::getAmpAddr() {
	return(ampAddr);
}

unsigned char SettingsClass::getAmpSubAddr() {
	return(ampSubAddr);
}

unsigned char SettingsClass::getAmpBandSegmentCount() {
	return(ampBandSegmentCount);
}

void SettingsClass::setAmpBandSegmentCount(unsigned char segments) {
	ampBandSegmentCount = segments;
}

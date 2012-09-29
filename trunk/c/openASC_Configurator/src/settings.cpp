#include "settings.h"

SettingsClass::SettingsClass() {
	//Set the default values
	networkAddress = 0x01;
	deviceIsMaster = false;
	nrOfDevices = 10;

	for (int i=0;i<9;i++) {
		powerMeterAddress[i] = 0;
		powerMeterVSWRAlarmValue[i] = 0;
		powerMeterRefPowerValue[i] = 0;
	}


	powerMeterUpdateRateText = 500;
	powerMeterUpdateRateBargraph = 200;

	pttInterlockInput = 0;

	//Default amplifier status
	ampControlEnabled = false;
	ampAddr = 1;
	ampSubAddr = 0;
	ampFuncStatus = 0;
	ampBandSegmentCount = 1;

  /*! Ethernet */
  ethernetPort = 4750;
  ethernetIPAddr = "192.168.1.130";
	ethernetSubmask = "255.255.255.0";
  ethernetEnabled = false;
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

	settings.setValue("PowerMeterVSWRAlarmValue[0]",powerMeterVSWRAlarmValue[0]);
	settings.setValue("PowerMeterVSWRAlarmValue[1]",powerMeterVSWRAlarmValue[1]);
	settings.setValue("PowerMeterVSWRAlarmValue[2]",powerMeterVSWRAlarmValue[2]);
	settings.setValue("PowerMeterVSWRAlarmValue[3]",powerMeterVSWRAlarmValue[3]);
	settings.setValue("PowerMeterVSWRAlarmValue[4]",powerMeterVSWRAlarmValue[4]);
	settings.setValue("PowerMeterVSWRAlarmValue[5]",powerMeterVSWRAlarmValue[5]);
	settings.setValue("PowerMeterVSWRAlarmValue[6]",powerMeterVSWRAlarmValue[6]);
	settings.setValue("PowerMeterVSWRAlarmValue[7]",powerMeterVSWRAlarmValue[7]);
	settings.setValue("PowerMeterVSWRAlarmValue[8]",powerMeterVSWRAlarmValue[8]);

	settings.setValue("PowerMeterRefPowerValue[0]",powerMeterRefPowerValue[0]);
	settings.setValue("PowerMeterRefPowerValue[1]",powerMeterRefPowerValue[1]);
	settings.setValue("PowerMeterRefPowerValue[2]",powerMeterRefPowerValue[2]);
	settings.setValue("PowerMeterRefPowerValue[3]",powerMeterRefPowerValue[3]);
	settings.setValue("PowerMeterRefPowerValue[4]",powerMeterRefPowerValue[4]);
	settings.setValue("PowerMeterRefPowerValue[5]",powerMeterRefPowerValue[5]);
	settings.setValue("PowerMeterRefPowerValue[6]",powerMeterRefPowerValue[6]);
	settings.setValue("PowerMeterRefPowerValue[7]",powerMeterRefPowerValue[7]);
	settings.setValue("PowerMeterRefPowerValue[8]",powerMeterRefPowerValue[8]);


	settings.setValue("PowerMeterDisplayUpdateRateText",powerMeterUpdateRateText);
	settings.setValue("PowerMeterDisplayUpdateRateBargraph",powerMeterUpdateRateBargraph);

	settings.setValue("PTTInterlockInput",pttInterlockInput);

	settings.setValue("AmpControlEnabled",ampControlEnabled);
	settings.setValue("AmpAddr",ampAddr);
	settings.setValue("AmpSubAddr",ampSubAddr);
	settings.setValue("AmpFuncStatus",ampFuncStatus);
	settings.setValue("AmpBandSegmentCount",ampBandSegmentCount);

  settings.setValue("EthernetEnabled",ethernetEnabled);
  settings.setValue("EthernetIPAddr",ethernetIPAddr);
	settings.setValue("EthernetSubmask",ethernetSubmask);
  settings.setValue("EthernetPort",ethernetPort);

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

	powerMeterVSWRAlarmValue[0] = settings.value("PowerMeterVSWRAlarmValue[0]").toDouble();
	powerMeterVSWRAlarmValue[1] = settings.value("PowerMeterVSWRAlarmValue[1]").toDouble();
	powerMeterVSWRAlarmValue[2] = settings.value("PowerMeterVSWRAlarmValue[2]").toDouble();
	powerMeterVSWRAlarmValue[3] = settings.value("PowerMeterVSWRAlarmValue[3]").toDouble();
	powerMeterVSWRAlarmValue[4] = settings.value("PowerMeterVSWRAlarmValue[4]").toDouble();
	powerMeterVSWRAlarmValue[5] = settings.value("PowerMeterVSWRAlarmValue[5]").toDouble();
	powerMeterVSWRAlarmValue[6] = settings.value("PowerMeterVSWRAlarmValue[6]").toDouble();
	powerMeterVSWRAlarmValue[7] = settings.value("PowerMeterVSWRAlarmValue[7]").toDouble();
	powerMeterVSWRAlarmValue[8] = settings.value("PowerMeterVSWRAlarmValue[8]").toDouble();

	powerMeterRefPowerValue[0] = settings.value("PowerMeterRefPowerValue[0]").toInt();
	powerMeterRefPowerValue[1] = settings.value("PowerMeterRefPowerValue[1]").toInt();
	powerMeterRefPowerValue[2] = settings.value("PowerMeterRefPowerValue[2]").toInt();
	powerMeterRefPowerValue[3] = settings.value("PowerMeterRefPowerValue[3]").toInt();
	powerMeterRefPowerValue[4] = settings.value("PowerMeterRefPowerValue[4]").toInt();
	powerMeterRefPowerValue[5] = settings.value("PowerMeterRefPowerValue[5]").toInt();
	powerMeterRefPowerValue[6] = settings.value("PowerMeterRefPowerValue[6]").toInt();
	powerMeterRefPowerValue[7] = settings.value("PowerMeterRefPowerValue[7]").toInt();
	powerMeterRefPowerValue[8] = settings.value("PowerMeterRefPowerValue[8]").toInt();


	powerMeterUpdateRateText = settings.value("PowerMeterDisplayUpdateRateText").toInt();
	powerMeterUpdateRateBargraph = settings.value("PowerMeterDisplayUpdateRateBargraph").toInt();

	pttInterlockInput = settings.value("PTTInterlockInput").toInt();

	nrOfDevices = settings.value("nrOfDevices").toInt();
	
	ampControlEnabled = settings.value("AmpControlEnabled").toBool();
	ampAddr = settings.value("AmpAddr").toInt();
	ampSubAddr = settings.value("AmpSubAddr").toInt();
	ampFuncStatus = settings.value("AmpFuncStatus").toInt();
	ampBandSegmentCount = settings.value("AmpBandSegmentCount").toInt();

  ethernetEnabled = settings.value("EthernetEnabled").toBool();
  ethernetIPAddr = settings.value("EthernetIPAddr").toString();
	ethernetSubmask = settings.value("EthernetSubmask").toString();
  ethernetPort = settings.value("EthernetPort").toInt();

	settings.endGroup();
}

void SettingsClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[60];
		
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

	tx_buff[10] = ((unsigned int)(powerMeterVSWRAlarmValue[0] * 100) & 0xFF00) >> 8;
	tx_buff[11] = (unsigned int)(powerMeterVSWRAlarmValue[0] * 100) & 0x00FF;
	tx_buff[12] = ((unsigned int)(powerMeterVSWRAlarmValue[1] * 100) & 0xFF00) >> 8;
	tx_buff[13] = (unsigned int)(powerMeterVSWRAlarmValue[1] * 100) & 0x00FF;
	tx_buff[14] = ((unsigned int)(powerMeterVSWRAlarmValue[2] * 100) & 0xFF00) >> 8;
	tx_buff[15] = (unsigned int)(powerMeterVSWRAlarmValue[2] * 100) & 0x00FF;
	tx_buff[16] = ((unsigned int)(powerMeterVSWRAlarmValue[3] * 100) & 0xFF00) >> 8;
	tx_buff[17] = (unsigned int)(powerMeterVSWRAlarmValue[3] * 100) & 0x00FF;
	tx_buff[18] = ((unsigned int)(powerMeterVSWRAlarmValue[4] * 100) & 0xFF00) >> 8;
	tx_buff[19] = (unsigned int)(powerMeterVSWRAlarmValue[4] * 100) & 0x00FF;
	tx_buff[20] = ((unsigned int)(powerMeterVSWRAlarmValue[5] * 100) & 0xFF00) >> 8;
	tx_buff[21] = (unsigned int)(powerMeterVSWRAlarmValue[5] * 100) & 0x00FF;
	tx_buff[22] = ((unsigned int)(powerMeterVSWRAlarmValue[6] * 100) & 0xFF00) >> 8;
	tx_buff[23] = (unsigned int)(powerMeterVSWRAlarmValue[6] * 100) & 0x00FF;
	tx_buff[24] = ((unsigned int)(powerMeterVSWRAlarmValue[7] * 100) & 0xFF00) >> 8;
	tx_buff[25] = (unsigned int)(powerMeterVSWRAlarmValue[7] * 100) & 0x00FF;
	tx_buff[26] = ((unsigned int)(powerMeterVSWRAlarmValue[8] * 100) & 0xFF00) >> 8;
	tx_buff[27] = (unsigned int)(powerMeterVSWRAlarmValue[8] * 100) & 0x00FF;

	tx_buff[28] = (powerMeterRefPowerValue[0] >> 8) & 0x00FF;
	tx_buff[29] = (powerMeterRefPowerValue[0] & 0x00FF);
	tx_buff[30] = (powerMeterRefPowerValue[1] >> 8) & 0x00FF;
	tx_buff[31] = (powerMeterRefPowerValue[1] & 0x00FF);
	tx_buff[32] = (powerMeterRefPowerValue[2] >> 8) & 0x00FF;
	tx_buff[33] = (powerMeterRefPowerValue[2] & 0x00FF);
	tx_buff[34] = (powerMeterRefPowerValue[3] >> 8) & 0x00FF;
	tx_buff[35] = (powerMeterRefPowerValue[3] & 0x00FF);
	tx_buff[36] = (powerMeterRefPowerValue[4] >> 8) & 0x00FF;
	tx_buff[37] = (powerMeterRefPowerValue[4] & 0x00FF);
	tx_buff[38] = (powerMeterRefPowerValue[5] >> 8) & 0x00FF;
	tx_buff[39] = (powerMeterRefPowerValue[5] & 0x00FF);
	tx_buff[40] = (powerMeterRefPowerValue[6] >> 8) & 0x00FF;
	tx_buff[41] = (powerMeterRefPowerValue[6] & 0x00FF);
	tx_buff[42] = (powerMeterRefPowerValue[7] >> 8) & 0x00FF;
	tx_buff[43] = (powerMeterRefPowerValue[7] & 0x00FF);
	tx_buff[44] = (powerMeterRefPowerValue[8] >> 8) & 0x00FF;
	tx_buff[45] = (powerMeterRefPowerValue[8] & 0x00FF);

	tx_buff[46] = (powerMeterUpdateRateText >> 8);
	tx_buff[47] = (powerMeterUpdateRateText & 0x00FF);
	tx_buff[48] = (powerMeterUpdateRateBargraph >> 8);
	tx_buff[49] = (powerMeterUpdateRateBargraph & 0x00FF);

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 50, tx_buff);

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


  QStringList listIP = ethernetIPAddr.split(QRegExp("[.]"));
	QStringList listSubmask = ethernetSubmask.split(QRegExp("[.]"));

  tx_buff[0] = CTRL_SET_DEVICE_SETTINGS_ETHERNET;

  if (listIP.size() == 4) {
    tx_buff[1] = listIP.takeFirst().toInt();
    tx_buff[2] = listIP.takeFirst().toInt();
    tx_buff[3] = listIP.takeFirst().toInt();
    tx_buff[4] = listIP.takeFirst().toInt();
  }
  else
  {
    tx_buff[1] = 0;
    tx_buff[2] = 0;
    tx_buff[3] = 0;
    tx_buff[4] = 0;
  }

	if (listSubmask.size() == 4) {
		tx_buff[5] = listSubmask.takeFirst().toInt();
		tx_buff[6] = listSubmask.takeFirst().toInt();
		tx_buff[7] = listSubmask.takeFirst().toInt();
		tx_buff[8] = listSubmask.takeFirst().toInt();
  }
  else
  {
    tx_buff[5] = 0;
    tx_buff[6] = 0;
    tx_buff[7] = 0;
    tx_buff[8] = 0;
  }

  tx_buff[9] = (ethernetPort >> 8) & 0xFF; //Upper byte
  tx_buff[10] = ethernetPort & 0xFF;       //Lower byte

	serialPort.addTXMessage(CTRL_SET_DEVICE_SETTINGS, 11, tx_buff);

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

void SettingsClass::setPowerMeterVSWRAlarm(unsigned char band, float swr) {
	if (band == 99)	{
		for (unsigned char i=0;i<9;i++)
			powerMeterVSWRAlarmValue[i] = swr;
	}
	else
		powerMeterVSWRAlarmValue[band] = swr;
}

void SettingsClass::setPowerMeterRefPower(unsigned char band, unsigned int power) {
	if (band == 99)	{
		for (unsigned char i=0;i<9;i++)
			powerMeterRefPowerValue[i] = power;
	}
	else
		powerMeterRefPowerValue[band] = power;
}

unsigned int SettingsClass::getPowerMeterRefPower(unsigned char band) {
	return(powerMeterRefPowerValue[band]);
}

float SettingsClass::getPowerMeterVSWRAlarm(unsigned char band) {
		return(powerMeterVSWRAlarmValue[band]);
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

void SettingsClass::setEthernetIPAddr(QString address) {
  ethernetIPAddr = address;
}

void SettingsClass::setEthernetSubmask(QString address) {
	ethernetSubmask = address;
}

void SettingsClass::setEthernetPort(unsigned int port) {
  ethernetPort = port;
}

QString SettingsClass::getEthernetIPAddr(void) {
  return(ethernetIPAddr);
}

QString SettingsClass::getEthernetSubmask(void) {
	return(ethernetSubmask);
}

unsigned int SettingsClass::getEthernetPort(void) {
  return(ethernetPort);
}

bool SettingsClass::getEthernetEnabled(void) {
  return(ethernetEnabled);
}

void SettingsClass::setEthernetEnabled(bool state) {
  ethernetEnabled = state;
}

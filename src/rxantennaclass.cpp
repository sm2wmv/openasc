#include "rxantennaclass.h"
#include "generic.h"

RXAntennaClass::RXAntennaClass() {

}

void RXAntennaClass::setAntennaOutputStr(int index, QString str) {
	antennaOutputStr[index] = str;
}

QString RXAntennaClass::getAntennaOutputStr(int index) {
	return(antennaOutputStr[index]);
}

void RXAntennaClass::setAntennaName(int index, QString str) {
	antennaName[index] = str;
}

QString RXAntennaClass::getAntennaName(int index) {
	return(antennaName[index]);
}

void RXAntennaClass::setBandOutputString(int band, QString str) {
	bandOutputStr[band] = str;
}

QString RXAntennaClass::getBandOutputString(int band) {
	return(bandOutputStr[band]);
}

void RXAntennaClass::writeSettings(QSettings& settings) {
	settings.beginGroup("RXAntennas");
		
	settings.setValue("AntennaName1",antennaName[0]);
	settings.setValue("AntennaName2",antennaName[1]);
	settings.setValue("AntennaName3",antennaName[2]);
	settings.setValue("AntennaName4",antennaName[3]);
	settings.setValue("AntennaName5",antennaName[4]);
	settings.setValue("AntennaName6",antennaName[5]);
	settings.setValue("AntennaName7",antennaName[6]);
	settings.setValue("AntennaName8",antennaName[7]);
	settings.setValue("AntennaName9",antennaName[8]);
	settings.setValue("AntennaName10",antennaName[9]);
	
	settings.setValue("AntennaOutputStr1",antennaOutputStr[0]);
	settings.setValue("AntennaOutputStr2",antennaOutputStr[1]);
	settings.setValue("AntennaOutputStr3",antennaOutputStr[2]);
	settings.setValue("AntennaOutputStr4",antennaOutputStr[3]);
	settings.setValue("AntennaOutputStr5",antennaOutputStr[4]);
	settings.setValue("AntennaOutputStr6",antennaOutputStr[5]);
	settings.setValue("AntennaOutputStr7",antennaOutputStr[6]);
	settings.setValue("AntennaOutputStr8",antennaOutputStr[7]);
	settings.setValue("AntennaOutputStr9",antennaOutputStr[8]);
	settings.setValue("AntennaOutputStr10",antennaOutputStr[9]);
	
	settings.setValue("BandOutputStr1",bandOutputStr[0]);
	settings.setValue("BandOutputStr2",bandOutputStr[1]);
	settings.setValue("BandOutputStr3",bandOutputStr[2]);
	settings.setValue("BandOutputStr4",bandOutputStr[3]);
	
	settings.endGroup();
}

void RXAntennaClass::loadSettings(QSettings& settings) {
	settings.beginGroup("RXAntennas");
	
	antennaName[0] = settings.value("AntennaName1").toString();
	antennaName[1] = settings.value("AntennaName2").toString();
	antennaName[2] = settings.value("AntennaName3").toString();
	antennaName[3] = settings.value("AntennaName4").toString();
	antennaName[4] = settings.value("AntennaName5").toString();
	antennaName[5] = settings.value("AntennaName6").toString();
	antennaName[6] = settings.value("AntennaName7").toString();
	antennaName[7] = settings.value("AntennaName8").toString();
	antennaName[8] = settings.value("AntennaName9").toString();
	antennaName[9] = settings.value("AntennaName10").toString();

	antennaOutputStr[0] = settings.value("AntennaOutputStr1").toString();
	antennaOutputStr[1] = settings.value("AntennaOutputStr2").toString();
	antennaOutputStr[2] = settings.value("AntennaOutputStr3").toString();
	antennaOutputStr[3] = settings.value("AntennaOutputStr4").toString();
	antennaOutputStr[4] = settings.value("AntennaOutputStr5").toString();
	antennaOutputStr[5] = settings.value("AntennaOutputStr6").toString();
	antennaOutputStr[6] = settings.value("AntennaOutputStr7").toString();
	antennaOutputStr[7] = settings.value("AntennaOutputStr8").toString();
	antennaOutputStr[8] = settings.value("AntennaOutputStr9").toString();
	antennaOutputStr[9] = settings.value("AntennaOutputStr10").toString();
	
	bandOutputStr[0] = settings.value("BandOutputStr1").toString();
	bandOutputStr[1] = settings.value("BandOutputStr2").toString();
	bandOutputStr[2] = settings.value("BandOutputStr3").toString();
	bandOutputStr[3] = settings.value("BandOutputStr4").toString();
	
	settings.endGroup();
}

void RXAntennaClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[20];
	
	for (int i=0;i<10;i++) {
		tx_buff[0] = CTRL_SET_RX_ANT_DATA_TEXT;
		tx_buff[1] = 0;
		tx_buff[2] = i;
		tx_buff[3] = antennaName[i].length();
	
		for (int x=0;x<antennaName[i].length();x++)
			tx_buff[4+x] = antennaName[i].at(x).toAscii();
		
		serialPort.addTXMessage(CTRL_SET_RX_ANT_DATA,tx_buff[3]+4,tx_buff);
	}
	
	for (int i=0;i<10;i++) {
		QByteArray temp = strConvertToOutputStr(antennaOutputStr[i]);
		int len = temp.count();
		temp.insert(0,CTRL_SET_RX_ANT_DATA_ANT_OUT_STR);
		temp.insert(1,i);
		temp.insert(2,len);
		
		for (int x=0;x<temp.count();x++)
			tx_buff[x] = temp.at(x);
		
		serialPort.addTXMessage(CTRL_SET_RX_ANT_DATA,3+tx_buff[2],tx_buff);	
	}
	
	for (int i=0;i<4;i++) {
		QByteArray temp = strConvertToOutputStr(bandOutputStr[i]);
		int len = temp.count();
		temp.insert(0,CTRL_SET_RX_ANT_DATA_BAND_OUT_STR);
		temp.insert(1,i);
		temp.insert(2,len);
		
		for (int x=0;x<temp.count();x++)
			tx_buff[x] = temp.at(x);
		
		serialPort.addTXMessage(CTRL_SET_RX_ANT_DATA,3+tx_buff[2],tx_buff);
	}

	//Save the settings to the EEPROM
	tx_buff[0] = CTRL_SET_RX_ANT_DATA_SAVE;
	tx_buff[1] = 0;
	serialPort.addTXMessage(CTRL_SET_RX_ANT_DATA,2,tx_buff);
}
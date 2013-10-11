#include <QtDebug>

#include "bandclass.h"
#include "generic.h"

BandClass::BandClass() {
	for (int i=0;i<4;i++) {
		hasRotator[i] = false;
		subMenuType[i] = 0;
	}

	antennaOutputCombAllowed[15] = true;
}

void BandClass::setIndex(int new_index) {
	bandIndex = new_index;
	
	switch(bandIndex) {
		case 0: bandName = "160M";
				break;
		case 1: bandName = "80M";
				break;
		case 2: bandName = "40M";
				break;
		case 3: bandName = "30M";
				break;
		case 4: bandName = "20M";
				break;
		case 5: bandName = "17M";
				break;
		case 6: bandName = "15M";
				break;
		case 7: bandName = "12M";
				break;
		case 8: bandName = "10M";
                break;
	}

    bandLockConf = (1<<bandIndex);
}

QString BandClass::getBandName() {
	return(bandName);
}

int BandClass::getIndex(void) {
	return(bandIndex);
}

void BandClass::setCombinationAllowed(int combNr, bool state) {
	antennaOutputCombAllowed[combNr] = state;
}

bool BandClass::getCombinationAllowed(int combNr) {
	return(antennaOutputCombAllowed[combNr]);
}

void BandClass::setBandLimitHighSegHighLimit(int limit) {
	band_data.high_portion_high_limit = limit;
}

int BandClass::getBandLimitHighSegHighLimit(void) {
	return(band_data.high_portion_high_limit);
}

void BandClass::setBandLimitHighSegLowLimit(int limit) {
	band_data.high_portion_low_limit = limit;
}

int BandClass::getBandLimitHighSegLowLimit(void) {
	return(band_data.high_portion_low_limit);
}

void BandClass::setBandLimitLowSegHighLimit(int limit) {
	band_data.low_portion_high_limit = limit;
}

int BandClass::getBandLimitLowSegHighLimit(void) {
	return(band_data.low_portion_high_limit);
}

void BandClass::setBandLimitLowSegLowLimit(int limit) {
	band_data.low_portion_low_limit = limit;
}

int BandClass::getBandLimitLowSegLowLimit(void) {
	return(band_data.low_portion_low_limit);
}

void BandClass::setBandOutputStrHigh(QString str) {	
	bandOutputStrHigh = str;
}

void BandClass::setBandOutputStrLow(QString str) {	
	bandOutputStrLow = str;
}

QString BandClass::getBandOutputStrHigh() {	
	return(bandOutputStrHigh);
}

QString BandClass::getBandOutputStrLow() {	
	return(bandOutputStrLow);
}

QString BandClass::getAntennaName(int index) {
	return(antennaName[index]);
}

char BandClass::setAntennaName(int index, QString str) {
	antennaName[index] = str;
	
	return(0);
}

void BandClass::setAntennaMultiband(int index, bool state) {
	multiband[index] = state;
}

bool BandClass::getAntennaMultiband(int index) {
	return(multiband[index]);
}

int BandClass::getFlagStatus(int index) {
	int temp = 0;
	
	if (getAntennaMultiband(index))
		temp |= (1<<2);
			
	if (antennaName[index].length() > 0)
		temp |= (1<<0);
	
	if (hasRotator[index] == true)
		temp |= (1<<1);
	
	return(temp);
}


void BandClass::setAntennaOutputStr(int index, QString str) {
	if (index < 15) {
		antennaOutputStr[index] = str;
		antennaOutputStrLength[index] = str.length();
	}
}

QString BandClass::getAntennaOutputStr(int index) {
	if (index < 15) {
		return(antennaOutputStr[index]);
	}
	else
		return("");
}

void BandClass::setDefaultBandLimits() {
	if (bandIndex == 0) {
		band_data.low_portion_low_limit = 1800;
		band_data.high_portion_low_limit = 1800;
		band_data.low_portion_high_limit = 2000;
		band_data.high_portion_high_limit = 2000;
	}
	else if (bandIndex == 1) {
		band_data.low_portion_low_limit = 3500;
		band_data.high_portion_low_limit = 3500;
		band_data.low_portion_high_limit = 3900;
		band_data.high_portion_high_limit = 3900;
	}
	else if (bandIndex == 2) {
		band_data.low_portion_low_limit = 7000;
		band_data.high_portion_low_limit = 7000;
		band_data.low_portion_high_limit = 7300;
		band_data.high_portion_high_limit = 7300;
	}
	else if (bandIndex == 3) {
		band_data.low_portion_low_limit = 10100;
		band_data.high_portion_low_limit = 10100;
		band_data.low_portion_high_limit = 10200;
		band_data.high_portion_high_limit = 10200;
	}
	else if (bandIndex == 4) {
		band_data.low_portion_low_limit = 14000;
		band_data.high_portion_low_limit = 14000;
		band_data.low_portion_high_limit = 14350;
		band_data.high_portion_high_limit = 14350;
	}
	else if (bandIndex == 5) {
		band_data.low_portion_low_limit = 18068;
		band_data.high_portion_low_limit = 18068;
		band_data.low_portion_high_limit = 18180;
		band_data.high_portion_high_limit = 18180;
	}
	else if (bandIndex == 6) {
		band_data.low_portion_low_limit = 21000;
		band_data.high_portion_low_limit = 21000;
		band_data.low_portion_high_limit = 21450;
		band_data.high_portion_high_limit = 21450;
	}
	else if (bandIndex == 7) {
		band_data.low_portion_low_limit = 24800;
		band_data.high_portion_low_limit = 24800;
		band_data.low_portion_high_limit = 25000;
		band_data.high_portion_high_limit = 25000;
	}
	else if (bandIndex == 8) {
		band_data.low_portion_low_limit = 28000;
		band_data.high_portion_low_limit = 28000;
		band_data.low_portion_high_limit = 30000;
		band_data.high_portion_high_limit = 30000;
	}	
}

void BandClass::setSubMenuType(int ant_index, int type) {
	subMenuType[ant_index] = type;
}

int BandClass::getSubMenuType(int ant_index) {
	return(subMenuType[ant_index]);
}

void BandClass::setSubMenu4SQdirectionCount(int ant_index, int directions) {
	subMenu4SQ[ant_index].directionCount = directions;
}

int BandClass::getSubMenu4SQdirectionCount(int ant_index) {
	return(subMenu4SQ[ant_index].directionCount);
}

void BandClass::setSubMenu4SQdirectionName(int ant_index, int index, QString str) {
	subMenu4SQ[ant_index].directionName[index] = str;
}

QString BandClass::getSubMenu4SQdirectionName(int ant_index, int index) {
	return(subMenu4SQ[ant_index].directionName[index]);
}

void BandClass::setSubMenu4SQdirectionOutputStr(int ant_index, int index, QString str) {
	subMenu4SQ[ant_index].directionOutputStr[index] = str;
}

QString BandClass::getSubMenu4SQdirectionOutputStr(int ant_index, int index) {
	return(subMenu4SQ[ant_index].directionOutputStr[index]);
}

void BandClass::setSubMenuStackCombinationCount(int ant_index, int combos) {
	subMenuStack[ant_index].combinationCount = combos;
}

int BandClass::getSubMenuStackCombinationCount(int ant_index) {
	return(subMenuStack[ant_index].combinationCount);
}

void BandClass::setSubMenuStackName(int ant_index, QString str) {
	subMenuStack[ant_index].stackName = str;
}

QString BandClass::getSubMenuStackName(int ant_index) {
	return(subMenuStack[ant_index].stackName);
}

void BandClass::setHasRotator(unsigned char ant_index, bool state) {
	hasRotator[ant_index] = state;
}

void BandClass::setSubMenuStackCombinationName(int ant_index, int index, QString str) {
	subMenuStack[ant_index].combinationName[index] = str;
}

QString BandClass::getSubMenuStackCombinationName(int ant_index, int index) {
	return(subMenuStack[ant_index].combinationName[index]);
}

void BandClass::setSubMenuStackCombinationOutputStr(int ant_index, int index, QString str) {
	subMenuStack[ant_index].combinationOutputStr[index] = str;
}

QString BandClass::getSubMenuStackCombinationOutputStr(int ant_index, int index) {
	return(subMenuStack[ant_index].combinationOutputStr[index]);
}

void BandClass::writeSettings(QSettings& settings) {
	settings.beginGroup(bandName);
		settings.setValue("BandLimitHighSegHighLimit",band_data.high_portion_high_limit);
		settings.setValue("BandLimitHighSegLowLimit",band_data.high_portion_low_limit);
		settings.setValue("BandLimitLowSegHighLimit",band_data.low_portion_high_limit);
		settings.setValue("BandLimitLowSegLowLimit",band_data.low_portion_low_limit);
		
		settings.setValue("AntennaName1",antennaName[0]);
		settings.setValue("AntennaName2",antennaName[1]);
		settings.setValue("AntennaName3",antennaName[2]);
		settings.setValue("AntennaName4",antennaName[3]);
		
		antennaFlags[0] = getFlagStatus(0);
		antennaFlags[1] = getFlagStatus(1);
		antennaFlags[2] = getFlagStatus(2);
		antennaFlags[3] = getFlagStatus(3);
		
		settings.setValue("AntennaFlags1",antennaFlags[0]);
		settings.setValue("AntennaFlags2",antennaFlags[1]);
		settings.setValue("AntennaFlags3",antennaFlags[2]);
		settings.setValue("AntennaFlags4",antennaFlags[3]);
	
		settings.setValue("defaultAntennaIndex",defaultAntennaIndex);
		
		settings.setValue("subMenuTypeAnt1",subMenuType[0]);
		settings.setValue("subMenuTypeAnt2",subMenuType[1]);
		settings.setValue("subMenuTypeAnt3",subMenuType[2]);
		settings.setValue("subMenuTypeAnt4",subMenuType[3]);

		settings.beginWriteArray("SubMenu");

		for (int i=0;i<4;i++) {
				settings.setArrayIndex(i);

				settings.setValue("subMenu4SQdirectionCount",subMenu4SQ[i].directionCount);

				settings.setValue("subMenu4SQdirectionName1",subMenu4SQ[i].directionName[0]);
				settings.setValue("subMenu4SQdirectionName2",subMenu4SQ[i].directionName[1]);
				settings.setValue("subMenu4SQdirectionName3",subMenu4SQ[i].directionName[2]);
				settings.setValue("subMenu4SQdirectionName4",subMenu4SQ[i].directionName[3]);
				settings.setValue("subMenu4SQdirectionName5",subMenu4SQ[i].directionName[4]);
				settings.setValue("subMenu4SQdirectionName6",subMenu4SQ[i].directionName[5]);
				settings.setValue("subMenu4SQdirectionName7",subMenu4SQ[i].directionName[6]);
				settings.setValue("subMenu4SQdirectionName8",subMenu4SQ[i].directionName[7]);
				settings.setValue("subMenu4SQdirectionOutputStr1",subMenu4SQ[i].directionOutputStr[0]);
				settings.setValue("subMenu4SQdirectionOutputStr2",subMenu4SQ[i].directionOutputStr[1]);
				settings.setValue("subMenu4SQdirectionOutputStr3",subMenu4SQ[i].directionOutputStr[2]);
				settings.setValue("subMenu4SQdirectionOutputStr4",subMenu4SQ[i].directionOutputStr[3]);
				settings.setValue("subMenu4SQdirectionOutputStr5",subMenu4SQ[i].directionOutputStr[4]);
				settings.setValue("subMenu4SQdirectionOutputStr6",subMenu4SQ[i].directionOutputStr[5]);
				settings.setValue("subMenu4SQdirectionOutputStr7",subMenu4SQ[i].directionOutputStr[6]);
				settings.setValue("subMenu4SQdirectionOutputStr8",subMenu4SQ[i].directionOutputStr[7]);

				settings.setValue("subMenuStackName",subMenuStack[i].stackName);
				settings.setValue("subMenuStackCombinationCount",subMenuStack[i].combinationCount);

				settings.setValue("subMenuStackCombinationName1",subMenuStack[i].combinationName[0]);
				settings.setValue("subMenuStackCombinationName2",subMenuStack[i].combinationName[1]);
				settings.setValue("subMenuStackCombinationName3",subMenuStack[i].combinationName[2]);
				settings.setValue("subMenuStackCombinationName4",subMenuStack[i].combinationName[3]);
				settings.setValue("subMenuStackCombinationName5",subMenuStack[i].combinationName[4]);
				settings.setValue("subMenuStackCombinationName6",subMenuStack[i].combinationName[5]);

				settings.setValue("subMenuStackCombinationOutputStr1",subMenuStack[i].combinationOutputStr[0]);
				settings.setValue("subMenuStackCombinationOutputStr2",subMenuStack[i].combinationOutputStr[1]);
				settings.setValue("subMenuStackCombinationOutputStr3",subMenuStack[i].combinationOutputStr[2]);
				settings.setValue("subMenuStackCombinationOutputStr4",subMenuStack[i].combinationOutputStr[3]);
				settings.setValue("subMenuStackCombinationOutputStr5",subMenuStack[i].combinationOutputStr[4]);
				settings.setValue("subMenuStackCombinationOutputStr6",subMenuStack[i].combinationOutputStr[5]);
		}

		settings.endArray();

		settings.setValue("RotatorIndex1",rotatorIndex[0]);
		settings.setValue("RotatorIndex2",rotatorIndex[1]);
		settings.setValue("RotatorIndex3",rotatorIndex[2]);
		settings.setValue("RotatorIndex4",rotatorIndex[3]);
	
		settings.setValue("RotatorAddress1",rotatorAddress[0]);
		settings.setValue("RotatorAddress2",rotatorAddress[1]);
		settings.setValue("RotatorAddress3",rotatorAddress[2]);
		settings.setValue("RotatorAddress4",rotatorAddress[3]);

		settings.setValue("RotatorSubAddress1",rotatorSubAddress[0]);
		settings.setValue("RotatorSubAddress2",rotatorSubAddress[1]);
		settings.setValue("RotatorSubAddress3",rotatorSubAddress[2]);
		settings.setValue("RotatorSubAddress4",rotatorSubAddress[3]);
	
		settings.setValue("RotatorStartHeading1",rotatorStartHeading[0]);
		settings.setValue("RotatorStartHeading2",rotatorStartHeading[1]);
		settings.setValue("RotatorStartHeading3",rotatorStartHeading[2]);
		settings.setValue("RotatorStartHeading4",rotatorStartHeading[3]);
	
		settings.setValue("rotatorDegrees1",rotatorDegrees[0]);
		settings.setValue("rotatorDegrees2",rotatorDegrees[1]);
		settings.setValue("rotatorDegrees3",rotatorDegrees[2]);
		settings.setValue("rotatorDegrees4",rotatorDegrees[3]);
	
		settings.setValue("RotatorDelay1",rotatorDelay[0]);
		settings.setValue("RotatorDelay2",rotatorDelay[1]);
		settings.setValue("RotatorDelay3",rotatorDelay[2]);
		settings.setValue("RotatorDelay4",rotatorDelay[3]);
		
		settings.beginWriteArray("AntennaOutputStr");

		for (int i=0;i<15;i++){
			settings.setArrayIndex(i);

			settings.setValue("OutputStr",antennaOutputStr[i]);
			settings.setValue("OutputStrLength",antennaOutputStrLength[i]);
			settings.setValue("OutputCombAllowed",antennaOutputCombAllowed[i]);
		}
		
		settings.endArray();

		settings.setValue("OutputCombAllowedNoAntenna",antennaOutputCombAllowed[15]);

		settings.setValue("BandOutputStrHigh",bandOutputStrHigh);
		settings.setValue("BandOutputStrLow",bandOutputStrLow);
		
        settings.setValue("BandLockConf",bandLockConf);

	settings.endGroup();
}

void BandClass::setDefaultAntenna(int index) {
	defaultAntennaIndex = index;
}

int BandClass::getDefaultAntenna() {
	return(defaultAntennaIndex);
}

void BandClass::loadSettings(QSettings& settings) {
	settings.beginGroup(bandName);
	
	band_data.high_portion_high_limit = settings.value("BandLimitHighSegHighLimit").toInt();
	band_data.high_portion_low_limit = settings.value("BandLimitHighSegLowLimit").toInt();
	band_data.low_portion_high_limit = settings.value("BandLimitLowSegHighLimit").toInt();
	band_data.low_portion_low_limit = settings.value("BandLimitLowSegLowLimit").toInt();

	antennaName[0] = settings.value("AntennaName1").toString();
	antennaName[1] = settings.value("AntennaName2").toString();
	antennaName[2] = settings.value("AntennaName3").toString();
	antennaName[3] = settings.value("AntennaName4").toString();

	antennaFlags[0] = settings.value("AntennaFlags1").toUInt(0);
	antennaFlags[1] = settings.value("AntennaFlags2").toUInt(0);
	antennaFlags[2] = settings.value("AntennaFlags3").toUInt(0);
	antennaFlags[3] = settings.value("AntennaFlags4").toUInt(0);
		
	subMenuType[0] = settings.value("subMenuTypeAnt1").toUInt(0);
	subMenuType[1] = settings.value("subMenuTypeAnt2").toUInt(0);
	subMenuType[2] = settings.value("subMenuTypeAnt3").toUInt(0);
	subMenuType[3] = settings.value("subMenuTypeAnt4").toUInt(0);
	
	defaultAntennaIndex = settings.value("defaultAntennaIndex").toUInt(0);
	

	int size = settings.beginReadArray("SubMenu");

		for (int i=0;i<size;i++) {
				settings.setArrayIndex(i);

				subMenu4SQ[i].directionCount = settings.value("subMenu4SQdirectionCount").toUInt(0);
				subMenu4SQ[i].directionName[0] = settings.value("subMenu4SQdirectionName1").toString();
				subMenu4SQ[i].directionName[1] = settings.value("subMenu4SQdirectionName2").toString();
				subMenu4SQ[i].directionName[2] = settings.value("subMenu4SQdirectionName3").toString();
				subMenu4SQ[i].directionName[3] = settings.value("subMenu4SQdirectionName4").toString();
				subMenu4SQ[i].directionName[4] = settings.value("subMenu4SQdirectionName5").toString();
				subMenu4SQ[i].directionName[5] = settings.value("subMenu4SQdirectionName6").toString();
				subMenu4SQ[i].directionName[6] = settings.value("subMenu4SQdirectionName7").toString();
				subMenu4SQ[i].directionName[7] = settings.value("subMenu4SQdirectionName8").toString();
	
				subMenu4SQ[i].directionOutputStr[0] = settings.value("subMenu4SQdirectionOutputStr1").toString();
				subMenu4SQ[i].directionOutputStr[1] = settings.value("subMenu4SQdirectionOutputStr2").toString();
				subMenu4SQ[i].directionOutputStr[2] = settings.value("subMenu4SQdirectionOutputStr3").toString();
				subMenu4SQ[i].directionOutputStr[3] = settings.value("subMenu4SQdirectionOutputStr4").toString();
				subMenu4SQ[i].directionOutputStr[4] = settings.value("subMenu4SQdirectionOutputStr5").toString();
				subMenu4SQ[i].directionOutputStr[5] = settings.value("subMenu4SQdirectionOutputStr6").toString();
				subMenu4SQ[i].directionOutputStr[6] = settings.value("subMenu4SQdirectionOutputStr7").toString();
				subMenu4SQ[i].directionOutputStr[7] = settings.value("subMenu4SQdirectionOutputStr8").toString();
				
				subMenuStack[i].stackName = settings.value("subMenuStackName").toString();
				subMenuStack[i].combinationCount = settings.value("subMenuStackCombinationCount").toUInt(0);

				subMenuStack[i].combinationName[0] = settings.value("subMenuStackCombinationName1").toString();
				subMenuStack[i].combinationName[1] = settings.value("subMenuStackCombinationName2").toString();
				subMenuStack[i].combinationName[2] = settings.value("subMenuStackCombinationName3").toString();
				subMenuStack[i].combinationName[3] = settings.value("subMenuStackCombinationName4").toString();
				subMenuStack[i].combinationName[4] = settings.value("subMenuStackCombinationName5").toString();
				subMenuStack[i].combinationName[5] = settings.value("subMenuStackCombinationName6").toString();
	
				subMenuStack[i].combinationOutputStr[0] = settings.value("subMenuStackCombinationOutputStr1").toString();
				subMenuStack[i].combinationOutputStr[1] = settings.value("subMenuStackCombinationOutputStr2").toString();
				subMenuStack[i].combinationOutputStr[2] = settings.value("subMenuStackCombinationOutputStr3").toString();
				subMenuStack[i].combinationOutputStr[3] = settings.value("subMenuStackCombinationOutputStr4").toString();
				subMenuStack[i].combinationOutputStr[4] = settings.value("subMenuStackCombinationOutputStr5").toString();
				subMenuStack[i].combinationOutputStr[5] = settings.value("subMenuStackCombinationOutputStr6").toString();
		}

	settings.endArray();

	rotatorIndex[0] = settings.value("RotatorIndex1").toInt();
	rotatorIndex[1] = settings.value("RotatorIndex2").toInt();
	rotatorIndex[2] = settings.value("RotatorIndex3").toInt();
	rotatorIndex[3] = settings.value("RotatorIndex4").toInt();
	
	rotatorAddress[0] = settings.value("RotatorAddress1").toInt();
	rotatorAddress[1] = settings.value("RotatorAddress2").toInt();
	rotatorAddress[2] = settings.value("RotatorAddress3").toInt();
	rotatorAddress[3] = settings.value("RotatorAddress4").toInt();

	rotatorSubAddress[0] = settings.value("RotatorSubAddress1").toInt();
	rotatorSubAddress[1] = settings.value("RotatorSubAddress2").toInt();
	rotatorSubAddress[2] = settings.value("RotatorSubAddress3").toInt();
	rotatorSubAddress[3] = settings.value("RotatorSubAddress4").toInt();
	
	rotatorStartHeading[0] = settings.value("RotatorStartHeading1").toInt();
	rotatorStartHeading[1] = settings.value("RotatorStartHeading2").toInt();
	rotatorStartHeading[2] = settings.value("RotatorStartHeading3").toInt();
	rotatorStartHeading[3] = settings.value("RotatorStartHeading4").toInt();
	
	rotatorDegrees[0] = settings.value("RotatorDegrees1").toInt();
	rotatorDegrees[1] = settings.value("RotatorDegrees2").toInt();
	rotatorDegrees[2] = settings.value("RotatorDegrees3").toInt();
	rotatorDegrees[3] = settings.value("RotatorDegrees4").toInt();
	
	rotatorDelay[0] = settings.value("RotatorDelay1").toInt();
	rotatorDelay[1] = settings.value("RotatorDelay2").toInt();
	rotatorDelay[2] = settings.value("RotatorDelay3").toInt();
	rotatorDelay[3] = settings.value("RotatorDelay4").toInt();
	
    if (settings.contains("BandLockConf")) {
        bandLockConf = settings.value("BandLockConf").toInt();
    }
    else {
        bandLockConf = (1<<bandIndex);
    }

	for (int i=0;i<4;i++) {
		if (antennaFlags[i] & (1<<2))
			multiband[i] = true;
		else
			multiband[i] = false;
		
		if (antennaFlags[i] & (1<<1))
			hasRotator[i] = true;
		else
			hasRotator[i] = false;
	}
			
	size = settings.beginReadArray("AntennaOutputStr");
	
	for (int i=0;i<size;i++) {
		settings.setArrayIndex(i);
		antennaOutputStr[i] = settings.value("OutputStr").toString();
		antennaOutputStrLength[i] = settings.value("OutputStrLength").toInt();
		antennaOutputCombAllowed[i] = settings.value("OutputCombAllowed").toBool();
	}

	settings.endArray();

	antennaOutputCombAllowed[15] = settings.value("OutputCombAllowedNoAntenna").toBool();

	bandOutputStrHigh = settings.value("BandOutputStrHigh").toString();
	bandOutputStrLow  = settings.value("BandOutputStrLow").toString();

	settings.endGroup();
}

void BandClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[35];
	
	for (int i=0;i<4;i++) {
		tx_buff[0] = CTRL_SET_ANT_DATA_TEXT;
		tx_buff[1] = bandIndex;
		tx_buff[2] = i;
		tx_buff[3] = antennaName[i].length();
	
		for (int x=0;x<antennaName[i].length();x++)
            tx_buff[4+x] = antennaName[i].at(x).toLatin1();
		
		serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
	}
	
	tx_buff[0] = CTRL_SET_ANT_DATA_SUB_MENU_TYPE;
	tx_buff[1] = subMenuType[0];
	tx_buff[2] = subMenuType[1];
	tx_buff[3] = subMenuType[2];
	tx_buff[4] = subMenuType[3];
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,5,tx_buff);

	tx_buff[0] = CTRL_SET_ANT_DATA_ANT_FLAGS;
	tx_buff[1] = bandIndex;
	tx_buff[2] = getFlagStatus(0);
	tx_buff[3] = getFlagStatus(1);
	tx_buff[4] = getFlagStatus(2);
	tx_buff[5] = getFlagStatus(3);
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,6,tx_buff);
	
	tx_buff[0] = CTRL_SET_ANT_DATA_COMB_ALLOWED;
	tx_buff[1] = 0;
	tx_buff[2] = 0;
	
	for (int i=0;i<8;i++) {
		if (antennaOutputCombAllowed[i+8])
			tx_buff[1] |= (1<<i);
		
		if (antennaOutputCombAllowed[i])
			tx_buff[2] |= (1<<i);
	}
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,3,tx_buff);
	
	for (int i=0;i<15;i++) {
		QByteArray temp = strConvertToOutputStr(antennaOutputStr[i]);
		int len = temp.count();
		temp.insert(0,CTRL_SET_ANT_DATA_ANT_OUT_STR);
		temp.insert(1,i);
		temp.insert(2,len);

		for (int x=0;x<temp.count();x++)
			tx_buff[x] = temp.at(x);

		serialPort.addTXMessage(CTRL_SET_ANT_DATA,3+tx_buff[2],tx_buff);
	}
	
	tx_buff[0] = CTRL_SET_ANT_DEFAULT_INDEX;
	tx_buff[1] = bandIndex;
	tx_buff[2] = defaultAntennaIndex;
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,3,tx_buff);
	
	/* SEND ROTATOR DATA */
	tx_buff[0] = CTRL_SET_ANT_ROTATOR_DATA;
	tx_buff[1] = bandIndex;
	
	unsigned char posCount = 2;

	for (unsigned char i=0;i<4;i++) {
		tx_buff[posCount++]	= rotatorAddress[i];
		tx_buff[posCount++] = rotatorSubAddress[i];
		tx_buff[posCount++] = (rotatorDegrees[i] >> 8);
		tx_buff[posCount++] = (rotatorDegrees[i] & 0xFF);
		tx_buff[posCount++] = (rotatorStartHeading[i] >> 8);
		tx_buff[posCount++] = (rotatorStartHeading[i] & 0xFF);
	}
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,26,tx_buff);

	for (int i=0;i<4;i++) {
		if (subMenuType[i] == SUBMENU_VERT_ARRAY) {
				tx_buff[0] = CTRL_SET_ANT_SUB_MENU_DATA;
				tx_buff[1] = i;	//Antenna index
				tx_buff[2] = subMenu4SQ[i].directionCount;
		}

		if (subMenuType[i] == SUBMENU_STACK) {
			tx_buff[0] = CTRL_SET_ANT_SUB_MENU_DATA;
			tx_buff[1] = i;
			tx_buff[2] = subMenuStack[i].combinationCount;
		}

		serialPort.addTXMessage(CTRL_SET_ANT_DATA,3,tx_buff);
	}

	for (int i=0;i<4;i++) {
		if (subMenuType[i] == SUBMENU_VERT_ARRAY) {
				for (int dirIndex=0;dirIndex<8;dirIndex++) {
						tx_buff[0] = CTRL_SET_ANT_SUB_MENU_TEXT;
						tx_buff[1] = i; //Antenna index
						tx_buff[2] = dirIndex;
						tx_buff[3] = subMenu4SQ[i].directionName[dirIndex].length();

						for (int x=0;x<subMenu4SQ[i].directionName[dirIndex].length();x++) {
                                tx_buff[4+x] = subMenu4SQ[i].directionName[dirIndex].at(x).toLatin1();
						}

						//qDebug() << subMenu4SQ[i].directionName[dirIndex];

						serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
				}

				for (int dirIndex=0;dirIndex<8;dirIndex++) {
						QByteArray temp = strConvertToOutputStr(subMenu4SQ[i].directionOutputStr[dirIndex]);
						int len = temp.count();
						temp.insert(0,CTRL_SET_ANT_SUB_MENU_OUTPUT_STR);
						temp.insert(1,i);
						temp.insert(2,dirIndex);
						temp.insert(3,len);

						for (int x=0;x<temp.count();x++)
								tx_buff[x] = temp.at(x);

						serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
				}
		}

		if (subMenuType[i] == SUBMENU_STACK) {
				for (int combIndex=0;combIndex<6;combIndex++) {
						tx_buff[0] = CTRL_SET_ANT_SUB_MENU_TEXT;
						tx_buff[1] = i; //Antenna index
						tx_buff[2] = combIndex;
						tx_buff[3] = subMenuStack[i].combinationName[combIndex].length();

						for (int x=0;x<subMenuStack[i].combinationName[combIndex].length();x++) {
                                tx_buff[4+x] = subMenuStack[i].combinationName[combIndex].at(x).toLatin1();
						}

						serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
				}

				for (int combIndex=0;combIndex<6;combIndex++) {
						QByteArray temp = strConvertToOutputStr(subMenuStack[i].combinationOutputStr[combIndex]);
						int len = temp.count();
						temp.insert(0,CTRL_SET_ANT_SUB_MENU_OUTPUT_STR);
						temp.insert(1,i);
						temp.insert(2,combIndex);
						temp.insert(3,len);

						for (int x=0;x<temp.count();x++)
								tx_buff[x] = temp.at(x);

						serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
				}
		}
	}

	tx_buff[0] = CTRL_SET_ANT_DATA_SAVE;
	tx_buff[1] = bandIndex;
			
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,2,tx_buff);
	
	tx_buff[0] = CTRL_SET_BAND_DATA_LIMITS;
	tx_buff[1] = bandIndex;
	tx_buff[2] = ((band_data.low_portion_low_limit & 0xFF00)>>8);
	tx_buff[3] = (band_data.low_portion_low_limit & 0x00FF);
	tx_buff[4] = ((band_data.low_portion_high_limit & 0xFF00)>>8);
	tx_buff[5] = (band_data.low_portion_high_limit & 0x00FF);
	tx_buff[6] = ((band_data.high_portion_low_limit & 0xFF00)>>8);
	tx_buff[7] = (band_data.high_portion_low_limit & 0x00FF);
	tx_buff[8] = ((band_data.high_portion_high_limit & 0xFF00)>>8);
	tx_buff[9] = (band_data.high_portion_high_limit & 0x00FF);
	
	serialPort.addTXMessage(CTRL_SET_BAND_DATA,10,tx_buff);
		
	QByteArray temp = strConvertToOutputStr(bandOutputStrHigh);
	int len = temp.count();
	temp.insert(0,CTRL_SET_BAND_DATA_HIGH_OUT_STR);
	temp.insert(1,bandIndex);
	temp.insert(2,len);
	
	for (int x=0;x<temp.count();x++)
		tx_buff[x] = temp.at(x);
	
	serialPort.addTXMessage(CTRL_SET_BAND_DATA,3+tx_buff[2],tx_buff);
	
	temp.clear();
	temp = strConvertToOutputStr(bandOutputStrLow);
	len = temp.count();
	temp.insert(0,CTRL_SET_BAND_DATA_LOW_OUT_STR);
	temp.insert(1,bandIndex);
	temp.insert(2,len);

	for (int x=0;x<temp.count();x++)
		tx_buff[x] = temp.at(x);

	serialPort.addTXMessage(CTRL_SET_BAND_DATA,3+tx_buff[2],tx_buff);	
	
    tx_buff[0] = CTRL_SET_BAND_LOCK_CONF;
    tx_buff[1] = bandIndex;
    tx_buff[2] = (bandLockConf >> 8) & 0x00FF;
    tx_buff[3] = bandLockConf & 0x00FF;

    serialPort.addTXMessage(CTRL_SET_BAND_DATA,4,tx_buff);

	//Save the settings to the EEPROM
	tx_buff[0] = CTRL_SET_BAND_DATA_SAVE;
	tx_buff[1] = bandIndex;
	serialPort.addTXMessage(CTRL_SET_BAND_DATA,2,tx_buff);
}

void BandClass::setRotatorProperties(unsigned char antIndex, int index, unsigned char addr, unsigned char subAddr, int startHeading, unsigned int degrees) {
	rotatorSubAddress[antIndex] = subAddr;
	rotatorAddress[antIndex] = addr;
	rotatorIndex[antIndex] = index;
	rotatorStartHeading[antIndex] = startHeading;
	rotatorDegrees[antIndex] = degrees;
}

int BandClass::getRotatorIndex(unsigned char antIndex) {
	return(rotatorIndex[antIndex]);
}

int BandClass::getBandLockConf() {
    return(bandLockConf);
}

void BandClass::setBandLockConf(int conf) {
    bandLockConf = conf;
}

#include "bandclass.h"
#include "generic.h"

BandClass::BandClass() {
	for (int i=0;i<4;i++)
		hasRotator[i] = false;
}

void BandClass::setIndex(int new_index) {
	index = new_index;
	
	switch(index) {
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
}

QString BandClass::getBandName() {
	return(bandName);
}

int BandClass::getIndex(void) {
	return(index);
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
	antennaOutputStr[index] = str;
	antennaOutputStrLength[index] = str.length();
}

QString BandClass::getAntennaOutputStr(int index) {
	return(antennaOutputStr[index]);
}

void BandClass::setDefaultBandLimits() {
	if (index == 0) {
		band_data.low_portion_low_limit = 1800;
		band_data.high_portion_low_limit = 1800;
		band_data.low_portion_high_limit = 2000;
		band_data.high_portion_high_limit = 2000;
	}
	else if (index == 1) {
		band_data.low_portion_low_limit = 3500;
		band_data.high_portion_low_limit = 3500;
		band_data.low_portion_high_limit = 3900;
		band_data.high_portion_high_limit = 3900;
	}
	else if (index == 2) {
		band_data.low_portion_low_limit = 7000;
		band_data.high_portion_low_limit = 7000;
		band_data.low_portion_high_limit = 7300;
		band_data.high_portion_high_limit = 7300;
	}
	else if (index == 3) {
		band_data.low_portion_low_limit = 10100;
		band_data.high_portion_low_limit = 10100;
		band_data.low_portion_high_limit = 10200;
		band_data.high_portion_high_limit = 10200;
	}
	else if (index == 4) {
		band_data.low_portion_low_limit = 14000;
		band_data.high_portion_low_limit = 14000;
		band_data.low_portion_high_limit = 14350;
		band_data.high_portion_high_limit = 14350;
	}
	else if (index == 5) {
		band_data.low_portion_low_limit = 18068;
		band_data.high_portion_low_limit = 18068;
		band_data.low_portion_high_limit = 18180;
		band_data.high_portion_high_limit = 18180;
	}
	else if (index == 6) {
		band_data.low_portion_low_limit = 21000;
		band_data.high_portion_low_limit = 21000;
		band_data.low_portion_high_limit = 21450;
		band_data.high_portion_high_limit = 21450;
	}
	else if (index == 7) {
		band_data.low_portion_low_limit = 24800;
		band_data.high_portion_low_limit = 24800;
		band_data.low_portion_high_limit = 25000;
		band_data.high_portion_high_limit = 25000;
	}
	else if (index == 8) {
		band_data.low_portion_low_limit = 28000;
		band_data.high_portion_low_limit = 28000;
		band_data.low_portion_high_limit = 30000;
		band_data.high_portion_high_limit = 30000;
	}	
}

void BandClass::setSubMenuType(int index, int type) {
	subMenuType[index] = type;
}

int BandClass::getSubMenuType(int index) {
	return(subMenuType[index]);
}

void BandClass::setSubMenu4SQdirectionCount(int directions) {
	subMenu4SQ.directionCount = directions;
}

int BandClass::getSubMenu4SQdirectionCount() {
	return(subMenu4SQ.directionCount);
}

void BandClass::setSubMenu4SQdirectionName(int index, QString str) {
	subMenu4SQ.directionName[index] = str;	
}

QString BandClass::getSubMenu4SQdirectionName(int index) {
	return(subMenu4SQ.directionName[index]);	
}

void BandClass::setSubMenu4SQdirectionOutputStr(int index, QString str) {
	subMenu4SQ.directionOutputStr[index] = str;
}

QString BandClass::getSubMenu4SQdirectionOutputStr(int index) {
	return(subMenu4SQ.directionOutputStr[index]);
}

void BandClass::setSubMenuStackCombinationCount(int combos) {
	subMenuStack.combinationCount = combos;
}

int BandClass::getSubMenuStackCombinationCount() {
	return(subMenuStack.combinationCount);
}

void BandClass::setSubMenuStackName(QString str) {
	subMenuStack.stackName = str;
}

QString BandClass::getSubMenuStackName() {
	return(subMenuStack.stackName);
}

void BandClass::setHasRotator(unsigned char ant_index, bool state) {
	hasRotator[ant_index] = state;
}

void BandClass::setSubMenuStackCombinationName(int index, QString str) {
	subMenuStack.combinationName[index] = str;
}

QString BandClass::getSubMenuStackCombinationName(int index) {
	return(subMenuStack.combinationName[index]);
}

void BandClass::setSubMenuStackCombinationOutputStr(int index, QString str) {
	subMenuStack.combinationOutputStr[index] = str;
}

QString BandClass::getSubMenuStackCombinationOutputStr(int index) {
	return(subMenuStack.combinationOutputStr[index]);
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
		settings.setValue("subMenu4SQdirectionCount",subMenu4SQ.directionCount);
		settings.setValue("subMenu4SQdirectionName1",subMenu4SQ.directionName[0]);
		settings.setValue("subMenu4SQdirectionName2",subMenu4SQ.directionName[1]);
		settings.setValue("subMenu4SQdirectionName3",subMenu4SQ.directionName[2]);
		settings.setValue("subMenu4SQdirectionName4",subMenu4SQ.directionName[3]);
		settings.setValue("subMenu4SQdirectionName5",subMenu4SQ.directionName[4]);
		settings.setValue("subMenu4SQdirectionName6",subMenu4SQ.directionName[5]);
		settings.setValue("subMenu4SQdirectionName7",subMenu4SQ.directionName[6]);
		settings.setValue("subMenu4SQdirectionName8",subMenu4SQ.directionName[7]);
		settings.setValue("subMenu4SQdirectionOutputStr1",subMenu4SQ.directionOutputStr[0]);
		settings.setValue("subMenu4SQdirectionOutputStr2",subMenu4SQ.directionOutputStr[1]);
		settings.setValue("subMenu4SQdirectionOutputStr3",subMenu4SQ.directionOutputStr[2]);
		settings.setValue("subMenu4SQdirectionOutputStr4",subMenu4SQ.directionOutputStr[3]);
		settings.setValue("subMenu4SQdirectionOutputStr5",subMenu4SQ.directionOutputStr[4]);
		settings.setValue("subMenu4SQdirectionOutputStr6",subMenu4SQ.directionOutputStr[5]);
		settings.setValue("subMenu4SQdirectionOutputStr7",subMenu4SQ.directionOutputStr[6]);
		settings.setValue("subMenu4SQdirectionOutputStr8",subMenu4SQ.directionOutputStr[7]);
				
		settings.setValue("subMenuStackName",subMenuStack.stackName);
		settings.setValue("subMenuStackCombinationCount",subMenuStack.combinationCount);
		
		settings.setValue("subMenuStackCombinationName1",subMenuStack.combinationName[0]);
		settings.setValue("subMenuStackCombinationName2",subMenuStack.combinationName[1]);
		settings.setValue("subMenuStackCombinationName3",subMenuStack.combinationName[2]);
		settings.setValue("subMenuStackCombinationName4",subMenuStack.combinationName[3]);
		settings.setValue("subMenuStackCombinationName5",subMenuStack.combinationName[4]);
		settings.setValue("subMenuStackCombinationName6",subMenuStack.combinationName[5]);
		
		settings.setValue("subMenuStackCombinationOutputStr1",subMenuStack.combinationOutputStr[0]);
		settings.setValue("subMenuStackCombinationOutputStr2",subMenuStack.combinationOutputStr[1]);
		settings.setValue("subMenuStackCombinationOutputStr3",subMenuStack.combinationOutputStr[2]);
		settings.setValue("subMenuStackCombinationOutputStr4",subMenuStack.combinationOutputStr[3]);
		settings.setValue("subMenuStackCombinationOutputStr5",subMenuStack.combinationOutputStr[4]);
		settings.setValue("subMenuStackCombinationOutputStr6",subMenuStack.combinationOutputStr[5]);
		
		settings.setValue("RotatorIndex1",rotatorIndex[0]);
		settings.setValue("RotatorIndex2",rotatorIndex[1]);
		settings.setValue("RotatorIndex3",rotatorIndex[2]);
		settings.setValue("RotatorIndex4",rotatorIndex[3]);
	
		settings.setValue("RotatorAddress1",rotatorAddress[0]);
		settings.setValue("RotatorAddress2",rotatorAddress[1]);
		settings.setValue("RotatorAddress3",rotatorAddress[2]);
		settings.setValue("RotatorAddress4",rotatorAddress[3]);
	
		settings.setValue("RotatorMinHeading1",rotatorMinHeading[0]);
		settings.setValue("RotatorMinHeading2",rotatorMinHeading[1]);
		settings.setValue("RotatorMinHeading3",rotatorMinHeading[2]);
		settings.setValue("RotatorMinHeading4",rotatorMinHeading[3]);
	
		settings.setValue("RotatorMaxHeading1",rotatorMaxHeading[0]);
		settings.setValue("RotatorMaxHeading2",rotatorMaxHeading[1]);
		settings.setValue("RotatorMaxHeading3",rotatorMaxHeading[2]);
		settings.setValue("RotatorMaxHeading4",rotatorMaxHeading[3]);
	
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
		
		settings.setValue("BandOutputStrHigh",bandOutputStrHigh);
		settings.setValue("BandOutputStrLow",bandOutputStrLow);
		
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
	subMenu4SQ.directionCount = settings.value("subMenu4SQdirectionCount").toUInt(0);
	
	defaultAntennaIndex = settings.value("defaultAntennaIndex").toUInt(0);
	
	subMenu4SQ.directionName[0] = settings.value("subMenu4SQdirectionName1").toString();
	subMenu4SQ.directionName[1] = settings.value("subMenu4SQdirectionName2").toString();
	subMenu4SQ.directionName[2] = settings.value("subMenu4SQdirectionName3").toString();
	subMenu4SQ.directionName[3] = settings.value("subMenu4SQdirectionName4").toString();
	subMenu4SQ.directionName[4] = settings.value("subMenu4SQdirectionName5").toString();
	subMenu4SQ.directionName[5] = settings.value("subMenu4SQdirectionName6").toString();
	subMenu4SQ.directionName[6] = settings.value("subMenu4SQdirectionName7").toString();
	subMenu4SQ.directionName[7] = settings.value("subMenu4SQdirectionName8").toString();
	
	subMenu4SQ.directionOutputStr[0] = settings.value("subMenu4SQdirectionOutputStr1").toString();
	subMenu4SQ.directionOutputStr[1] = settings.value("subMenu4SQdirectionOutputStr2").toString();
	subMenu4SQ.directionOutputStr[2] = settings.value("subMenu4SQdirectionOutputStr3").toString();
	subMenu4SQ.directionOutputStr[3] = settings.value("subMenu4SQdirectionOutputStr4").toString();
	subMenu4SQ.directionOutputStr[4] = settings.value("subMenu4SQdirectionOutputStr5").toString();
	subMenu4SQ.directionOutputStr[5] = settings.value("subMenu4SQdirectionOutputStr6").toString();
	subMenu4SQ.directionOutputStr[6] = settings.value("subMenu4SQdirectionOutputStr7").toString();
	subMenu4SQ.directionOutputStr[7] = settings.value("subMenu4SQdirectionOutputStr8").toString();
				
	subMenuStack.stackName = settings.value("subMenuStackName").toString();
	subMenuStack.combinationCount = settings.value("subMenuStackCombinationCount").toUInt(0);
	
	subMenuStack.combinationName[0] = settings.value("subMenuStackCombinationName1").toString();
	subMenuStack.combinationName[1] = settings.value("subMenuStackCombinationName2").toString();
	subMenuStack.combinationName[2] = settings.value("subMenuStackCombinationName3").toString();
	subMenuStack.combinationName[3] = settings.value("subMenuStackCombinationName4").toString();
	subMenuStack.combinationName[4] = settings.value("subMenuStackCombinationName5").toString();
	subMenuStack.combinationName[5] = settings.value("subMenuStackCombinationName6").toString();
	
	subMenuStack.combinationOutputStr[0] = settings.value("subMenuStackCombinationOutputStr1").toString();
	subMenuStack.combinationOutputStr[1] = settings.value("subMenuStackCombinationOutputStr2").toString();
	subMenuStack.combinationOutputStr[2] = settings.value("subMenuStackCombinationOutputStr3").toString();
	subMenuStack.combinationOutputStr[3] = settings.value("subMenuStackCombinationOutputStr4").toString();
	subMenuStack.combinationOutputStr[4] = settings.value("subMenuStackCombinationOutputStr5").toString();
	subMenuStack.combinationOutputStr[5] = settings.value("subMenuStackCombinationOutputStr6").toString();		
	
	rotatorIndex[0] = settings.value("RotatorIndex1").toInt();
	rotatorIndex[1] = settings.value("RotatorIndex2").toInt();
	rotatorIndex[2] = settings.value("RotatorIndex3").toInt();
	rotatorIndex[3] = settings.value("RotatorIndex4").toInt();
	
	rotatorAddress[0] = settings.value("RotatorAddress1").toInt();
	rotatorAddress[1] = settings.value("RotatorAddress2").toInt();
	rotatorAddress[2] = settings.value("RotatorAddress3").toInt();
	rotatorAddress[3] = settings.value("RotatorAddress4").toInt();
	
	rotatorMinHeading[0] = settings.value("RotatorMinHeading1").toInt();
	rotatorMinHeading[1] = settings.value("RotatorMinHeading2").toInt();
	rotatorMinHeading[2] = settings.value("RotatorMinHeading3").toInt();
	rotatorMinHeading[3] = settings.value("RotatorMinHeading4").toInt();
	
	rotatorMaxHeading[0] = settings.value("RotatorMaxHeading1").toInt();
	rotatorMaxHeading[1] = settings.value("RotatorMaxHeading2").toInt();
	rotatorMaxHeading[2] = settings.value("RotatorMaxHeading3").toInt();
	rotatorMaxHeading[3] = settings.value("RotatorMaxHeading4").toInt();
	
	rotatorDelay[0] = settings.value("RotatorDelay1").toInt();
	rotatorDelay[1] = settings.value("RotatorDelay2").toInt();
	rotatorDelay[2] = settings.value("RotatorDelay3").toInt();
	rotatorDelay[3] = settings.value("RotatorDelay4").toInt();
	
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
			
	int size = settings.beginReadArray("AntennaOutputStr");
	
	for (int i=0;i<size;i++) {
		settings.setArrayIndex(i);
		
		antennaOutputStr[i] = settings.value("OutputStr").toString();
		antennaOutputStrLength[i] = settings.value("OutputStrLength").toInt();
		antennaOutputCombAllowed[i] = settings.value("OutputCombAllowed").toBool();
	}
	
	settings.endArray();

	bandOutputStrHigh = settings.value("BandOutputStrHigh").toString();
	bandOutputStrLow  = settings.value("BandOutputStrLow").toString();

	settings.endGroup();
}

void BandClass::sendSettings(CommClass& serialPort) {
	unsigned char tx_buff[30];
	
	for (int i=0;i<4;i++) {
		tx_buff[0] = CTRL_SET_ANT_DATA_TEXT;
		tx_buff[1] = index;
		tx_buff[2] = i;
		tx_buff[3] = antennaName[i].length();
	
		for (int x=0;x<antennaName[i].length();x++)
			tx_buff[4+x] = antennaName[i].at(x).toAscii();
		
		serialPort.addTXMessage(CTRL_SET_ANT_DATA,tx_buff[3]+4,tx_buff);
	}
	
	//TODO: tx_buff[0] = CTRL_SET_ANT_DATA_SUB_MENU_TYPE;
	
	tx_buff[0] = CTRL_SET_ANT_DATA_ANT_FLAGS;
	tx_buff[1] = index;
	tx_buff[2] = antennaFlags[0];
	tx_buff[3] = antennaFlags[1];
	tx_buff[4] = antennaFlags[2];
	tx_buff[5] = antennaFlags[3];
	
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
	tx_buff[1] = index;
	tx_buff[2] = defaultAntennaIndex;
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,3,tx_buff);
	
	/* SEND ROTATOR DATA */
	tx_buff[0] = CTRL_SET_ANT_ROTATOR_DATA;
	tx_buff[1] = index;
	
	unsigned char posCount = 2;
	for (unsigned char i=0;i<4;i++) {
		tx_buff[posCount++]	= rotatorAddress[i];
		tx_buff[posCount++] = (rotatorMaxHeading[i] >> 8);
		tx_buff[posCount++] = (rotatorMaxHeading[i] & 0xFF);
		tx_buff[posCount++] = (rotatorMinHeading[i] >> 8);
		tx_buff[posCount++] = (rotatorMinHeading[i] & 0xFF);
		tx_buff[posCount++] = rotatorDelay[i];
	}
	
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,26,tx_buff);
	
	tx_buff[0] = CTRL_SET_ANT_DATA_SAVE;
	tx_buff[1] = index;
			
	serialPort.addTXMessage(CTRL_SET_ANT_DATA,2,tx_buff);
	
	tx_buff[0] = CTRL_SET_BAND_DATA_LIMITS;
	tx_buff[1] = index;
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
	temp.insert(1,index);
	temp.insert(2,len);
	
	for (int x=0;x<temp.count();x++)
		tx_buff[x] = temp.at(x);
	
	serialPort.addTXMessage(CTRL_SET_BAND_DATA,3+tx_buff[2],tx_buff);
	
	temp.clear();
	temp = strConvertToOutputStr(bandOutputStrLow);
	len = temp.count();
	temp.insert(0,CTRL_SET_BAND_DATA_LOW_OUT_STR);
	temp.insert(1,index);
	temp.insert(2,len);

	for (int x=0;x<temp.count();x++)
		tx_buff[x] = temp.at(x);

	serialPort.addTXMessage(CTRL_SET_BAND_DATA,3+tx_buff[2],tx_buff);	
	
	//Save the settings to the EEPROM
	tx_buff[0] = CTRL_SET_BAND_DATA_SAVE;
	tx_buff[1] = index;
	serialPort.addTXMessage(CTRL_SET_BAND_DATA,2,tx_buff);
}

void BandClass::setRotatorProperties(unsigned char antIndex, int index, unsigned char addr, int minHeading, int maxHeading, unsigned char delay) {
	rotatorAddress[antIndex] = addr;
	rotatorIndex[antIndex] = index;
	rotatorMinHeading[antIndex] = minHeading;
	rotatorMaxHeading[antIndex] = maxHeading;
	rotatorDelay[antIndex] = delay;
}

int BandClass::getRotatorIndex(unsigned char antIndex) {
	return(rotatorIndex[antIndex]);
}
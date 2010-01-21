#include "generic.h"
#include "mainwindowimpl.h"
#include "commclass.h"

char strIsValidOutputStr(QString str) {
	for (int i=0;i<str.length();i++) {
		//Check if the output string contains any invalid characters
		if (((str[i] < '0') || (str[i] > '9')) && (str[i] != ' ') && (str[i] != '@') && (str[i] != ','))
			return(0);
	}
	
	return(1);
}



QByteArray strConvertToOutputStr(QString str) {
	QStringList listMain;
	QByteArray byteArray;
	
	if (str.count() == 0)
		return(0);
	
	listMain = str.split(" ");
	
	for (int i=0;i<listMain.size();i++) {
		QStringList tempList = listMain.at(i).split("@");
		
		QStringList tempList2 = tempList.first().split(",");
		unsigned char addr = tempList.last().toUInt(0,10);
		
		for (int x=0;x<tempList2.size();x++)
			byteArray.append(tempList2.at(x).toUInt(0,10));
		
		byteArray.append(OUTPUT_ADDR_DELIMITER);
		byteArray.append(addr);
	}
	
	return(byteArray);
}

char strIsNumeric(QString str) {	
	if (str == "")
		return(0);	
	
	for (int i=0;i<str.length();i++) {
		if ((str[i] < '0') || (str[i] > '9'))
			return(0);
	}

	return(1);
}
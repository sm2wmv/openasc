#include "generic.h"
#include "mainwindowimpl.h"

char strIsValidOutputStr(QString str) {
	for (int i=0;i<str.length();i++) {
		//Check if the output string contains any invalid characters
		if (((str[i] < '0') || (str[i] > '9')) && (str[i] != ' ') && (str[i] != '@') && (str[i] != ','))
			return(0);
	}
	
	return(1);
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
#ifndef __RXANTENNACLASS_H__
#define __RXANTENNACLASS_H__

#include <qsettings.h>

#include "main.h"
#include "mainwindowimpl.h"
#include "commclass.h"

class RXAntennaClass
{
	public:
		RXAntennaClass();
		void setAntennaOutputStr(int index, QString str);
		QString getAntennaOutputStr(int index);
		void setAntennaName(int index, QString str);
		QString getAntennaName(int index);
		void setBandOutputString(int band, QString str);
		QString getBandOutputString(int band);
		
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void sendSettings(CommClass& serialPort);
	private:

	protected:
        QString antennaOutputStr[12];
        QString antennaName[12];
		QString bandOutputStr[4];
};

#endif // __RXAntennaCLASS_H__

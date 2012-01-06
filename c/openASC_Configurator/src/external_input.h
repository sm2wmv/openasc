#ifndef __EXTERNAL_INPUTCLASS_H__
#define __EXTERNAL_INPUTCLASS_H__

#include <qsettings.h>

#include "commclass.h"

class ExternalInputClass {
	public:
		ExternalInputClass();
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void sendSettings(CommClass& serialPort);
		
		void setCurrSelectedButton(int index);
		int getCurrSelectedButton();
		
		void setButtonFunction(int funcIndex);
		int getButtonFunction();

		void setAUXFunc(unsigned char index, unsigned char funcIndex);
		int getAUXFunc(unsigned char index);
	private:

	protected:
		int currSelectedButtonIndex;
		int buttonFunction[21];
		int buttonAUX1Func;
		int buttonAUX2Func;
};

#endif // __EXTERNAL_INPUTCLASS_H__

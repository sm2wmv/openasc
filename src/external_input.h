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
	private:

	protected:
		int currSelectedButtonIndex;
		int buttonFunction[17];
};

#endif // __EXTERNAL_INPUTCLASS_H__

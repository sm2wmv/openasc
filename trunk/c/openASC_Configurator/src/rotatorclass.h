#ifndef __ROTATOR_CLASS_H__
#define __ROTATOR_CLASS_H__

#include <qsettings.h>

class RotatorClass {
	public:
		RotatorClass();
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void addRotator(QString rotatorName, unsigned char rotatorAddress, unsigned char subAddress, unsigned int rotatorStartHeading, unsigned int rotatorDegrees, unsigned char rotatorDelay, bool view360deg);
		unsigned char changeRotatorProperties(int index, QString rotatorName, unsigned char rotatorAddress, unsigned char subAddress, unsigned int rotatorStartHeading, unsigned int rotatorDegrees, unsigned char rotatorDelay, bool view360deg);
		QString getRotatorName(int index);
		unsigned char getRotatorAddress(int index);
		unsigned char getRotatorSubAddress(int index);
		unsigned int getRotatorStartHeading(int index);
		unsigned int getRotatorDegrees(int index);
		unsigned char getRotatorDelay(int index);
		int getRotatorIndex(int index);
		QStringList getNameList();
		int getRotatorCount();
		int deleteRotator(int index);
		bool getRotator360degView(int index);
	private:
		//The last index that was used, this is just increased as time goes by 
		//so that each rotator has got an unique index
		int lastIndex;
		QList<unsigned char> addressList;
		QList<unsigned char> subAddressList;
		QStringList nameList;
		QList<unsigned int> startHeadingList;
		QList<unsigned int> degreesList;
		QList<unsigned char> delayList;
		QList<bool> view360degList;
		QList<int> indexList;
	protected:
};

#endif // __ROTATOR_CLASS_H__

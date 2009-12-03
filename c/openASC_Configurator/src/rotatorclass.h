#ifndef __ROTATOR_CLASS_H__
#define __ROTATOR_CLASS_H__

#include <qsettings.h>

class RotatorClass {
	public:
		RotatorClass();
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void addRotator(QString rotatorName, unsigned char rotatorAddress, int rotatorMinHeading, int rotatorMaxHeading, unsigned char rotatorDelay);
		unsigned char changeRotatorProperties(int index, QString rotatorName, unsigned char rotatorAddress, int rotatorMinHeading, int rotatorMaxHeading, unsigned char rotatorDelay);
		QString getRotatorName(int index);
		unsigned char getRotatorAddress(int index);
		int getRotatorMinHeading(int index);
		int getRotatorMaxHeading(int index);
		unsigned char getRotatorDelay(int index);
		int getRotatorIndex(int index);
		QStringList getNameList();
		int getRotatorCount();
		int deleteRotator(int index);
	private:
		//The last index that was used, this is just increased as time goes by 
		//so that each rotator has got an unique index
		int lastIndex;
		QList<unsigned char> addressList;
		QStringList nameList;
		QList<int> minHeadingList;
		QList<int> maxHeadingList;
		QList<unsigned char> delayList;
		QList<int> indexList;
	protected:
};

#endif // __ROTATOR_CLASS_H__

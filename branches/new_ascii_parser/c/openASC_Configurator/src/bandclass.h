#ifndef __BANDCLASS_H__
#define __BANDCLASS_H__

#include <qsettings.h>

#include "main.h"
#include "mainwindowimpl.h"
#include "commclass.h"

#define SUBMENU_NONE				0
#define SUBMENU_VERT_ARRAY	1
#define SUBMENU_STACK				2

typedef struct {
	int directionCount;
	QString directionName[8];
	QString directionOutputStr[8];
} structSubMenu4SQ;

typedef struct {
	int combinationCount;
	QString stackName;
	QString combinationName[6];
	QString combinationOutputStr[6];
} structSubMenuStack;

typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! The low limit of the lower portion of the band
	unsigned int low_portion_low_limit;
	//! The high limit of the lower portion of the band
	unsigned int low_portion_high_limit;
	//! The low limit of the higher portion of the band
	unsigned int high_portion_low_limit;
	//! The high limit of the higher portion of the band
	unsigned int high_portion_high_limit;
	//! The length of the high output str
	unsigned char band_high_output_str_length;
	//! The length of the low output str
	unsigned char band_low_output_str_length;
	//! These outputs are activated when you enter the high area of a band
	unsigned char band_high_output_str[BAND_OUTPUT_STR_SIZE];
	//! These outputs are activated when you enter the low area of a band
	unsigned char band_low_output_str[BAND_OUTPUT_STR_SIZE];
} struct_band;

class BandClass
{
	public:
		BandClass();
		void setIndex ( int new_index );
		int getIndex ( void );
		QString getBandName();
		
		void setCombinationAllowed(int combNr, bool state);
		bool getCombinationAllowed(int combNr);
				
		void setDefaultBandLimits();

		void setBandOutputStrHigh ( QString str );
		void setBandOutputStrLow ( QString str );
		QString getBandOutputStrHigh();
		QString getBandOutputStrLow();

		void setAntennaMultiband(int index, bool state);
		bool getAntennaMultiband(int index);
		int getFlagStatus(int index);
		
		QString getAntennaName ( int index );
		char setAntennaName ( int index, QString str );
		void setAntennaOutputStr ( int index, QString str );
		QString getAntennaOutputStr ( int index );

		void setBandLimitHighSegHighLimit ( int limit );
		int getBandLimitHighSegHighLimit ( void );
		void setBandLimitHighSegLowLimit ( int limit );
		int getBandLimitHighSegLowLimit ( void );
		void setBandLimitLowSegHighLimit ( int limit );
		int getBandLimitLowSegHighLimit ( void );
		void setBandLimitLowSegLowLimit ( int limit );
		int getBandLimitLowSegLowLimit ( void );

		void setSubMenuType(int ant_index, int type);
		int getSubMenuType(int ant_index);
		void setSubMenu4SQdirectionCount(int ant_index, int directions);
		int getSubMenu4SQdirectionCount(int ant_index);
		void setSubMenu4SQdirectionName(int ant_index, int index, QString str);
		QString getSubMenu4SQdirectionName(int ant_index, int index);
		void setSubMenu4SQdirectionOutputStr(int ant_index, int index, QString str);
		QString getSubMenu4SQdirectionOutputStr(int ant_index, int index);
		
		void setSubMenuStackCombinationCount(int ant_index, int combos);
		int getSubMenuStackCombinationCount(int ant_index);
		void setSubMenuStackName(int ant_index, QString str);
		QString getSubMenuStackName(int ant_index);
		void setSubMenuStackCombinationName(int ant_index, int index, QString str);
		QString getSubMenuStackCombinationName(int ant_index, int index);
		void setSubMenuStackCombinationOutputStr(int ant_index, int index, QString str);
		QString getSubMenuStackCombinationOutputStr(int ant_index, int index);
		
		void writeSettings ( QSettings& settings );
		void loadSettings ( QSettings& settings );
		void sendSettings(CommClass& serialPort);
		
		void setDefaultAntenna(int index);
		int getDefaultAntenna();
		void setRotatorProperties(unsigned char antIndex, int index, unsigned char addr, unsigned char subAddr, int startHeading, unsigned int degrees);
		int getRotatorIndex(unsigned char antIndex);
		
		void setHasRotator(unsigned char ant_index, bool state);
	private:
		unsigned char rotatorAddress[4];
		unsigned char rotatorSubAddress[4];
		int rotatorIndex[4];
		int rotatorStartHeading[4];
		unsigned int rotatorDegrees[4];
		unsigned char rotatorDelay[4];
	protected:
		int bandIndex;
		struct_band band_data;
		structSubMenu4SQ subMenu4SQ[4];
		structSubMenuStack subMenuStack[4];
//		struct_antenna antenna_data;
		QString bandOutputStrHigh;
		QString bandOutputStrLow;
		QString antennaOutputStr[15];
		QString antennaName[4];
		int defaultAntennaIndex;
		int antennaOutputStrLength[15];
		QString bandName;
		unsigned char antennaFlags[4];
		bool antennaOutputCombAllowed[16];
		bool multiband[4];
		bool hasRotator[4];
		//0 = none
		//1 = 4-SQ
		//2 = STACK
		int subMenuType[4];
};

#endif // __BANDCLASS_H__

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "sequencerclass.h"
#include "radiointerfaceclass.h"
#include "commclass.h"
#include "external_input.h"
#include "settings.h"
#include "rotatorclass.h"

//! Radio type is KENWOOD
#define RADIO_TYPE_KENWOOD		0
//! Radio type is ICOM
#define RADIO_TYPE_ICOM 			1

#define RADIO_MODE_MANUAL 	0
#define RADIO_MODE_CAT_POLL	1
#define RADIO_MODE_CAT_MON	2
#define RADIO_MODE_BCD			3

//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	void loadInitialGUIValues();
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void addDebugLine(QString str);
private:
	void setupConnections();
	void lineEditRXAntennaOutputs(QString str, int index);
	void comboBoxRotatorsReload();
	void reloadCurrentRotatorProperties();
protected:
	SequencerClass sequencerFootswitch;
	SequencerClass sequencerComputer;
	SequencerClass sequencerRadioSense;
	ExternalInputClass extInput;
	RadioInterfaceClass radioInterface;
	SettingsClass settingsClass;
	CommClass serialPort;
	RotatorClass rotators;
private slots:
	void comboBoxBandCurrentIndexChanged(int index);
	void comboBoxRadioTypeCurrentIndexChanged(int index);
	void comboBoxRadioModeCurrentIndexChanged(int index);
	void comboBoxRadioBaudRateCurrentIndexChanged(int index);
	void comboBoxRadioStopbitsCurrentIndexChanged(int index);
	void spinBoxCIVAddressValueChanged(int value);
	void sliderPollIntervalChanged(int value);
	void groupBoxCATClicked(bool);
	void comboBoxRadioSenseInputCurrentIndexChanged(int);
	
	void lineEditBandLimitHighSegHighLimitChanged(QString str);
	void lineEditBandLimitHighSegLowLimitChanged(QString str);
	void lineEditBandLimitLowSegHighLimitChanged(QString str);
	void lineEditBandLimitLowSegLowLimitChanged(QString str);
	void comboBoxBandLimitBandCurrentIndexChanged(int index);
	void pushButtonDefaultBandLimitsPressed();	
	
	void lineEditAntennaName1Changed(QString str);
	void lineEditAntennaName2Changed(QString str);
	void lineEditAntennaName3Changed(QString str);
	void lineEditAntennaName4Changed(QString str);
	void lineEditAntennaOutputStrChanged(QString str);
	void checkBoxCombinationAllowedClicked(bool state);
	void comboBoxAntennaCombinationCurrentIndexChanged(int index);
	void lineEditBandOutputStrHighChanged(QString str);
	void lineEditBandOutputStrLowChanged(QString str);
	void checkBoxMultiband1Clicked(bool state);
	void checkBoxMultiband2Clicked(bool state);
	void checkBoxMultiband3Clicked(bool state);
	void checkBoxMultiband4Clicked(bool state);
	void radioButtonAntennaDefault1Clicked(bool state);
	void radioButtonAntennaDefault2Clicked(bool state);
	void radioButtonAntennaDefault3Clicked(bool state);
	void radioButtonAntennaDefault4Clicked(bool state);
	
	void lineEditRXAntennaName1Changed(QString str);
	void lineEditRXAntennaName2Changed(QString str);
	void lineEditRXAntennaName3Changed(QString str);
	void lineEditRXAntennaName4Changed(QString str);
	void lineEditRXAntennaName5Changed(QString str);
	void lineEditRXAntennaName6Changed(QString str);
	void lineEditRXAntennaName7Changed(QString str);
	void lineEditRXAntennaName8Changed(QString str);
	void lineEditRXAntennaName9Changed(QString str);
	void lineEditRXAntennaName10Changed(QString str);
	
	void lineEditRXAntennaOutputs1Changed(QString str);
	void lineEditRXAntennaOutputs2Changed(QString str);
	void lineEditRXAntennaOutputs3Changed(QString str);
	void lineEditRXAntennaOutputs4Changed(QString str);
	void lineEditRXAntennaOutputs5Changed(QString str);
	void lineEditRXAntennaOutputs6Changed(QString str);
	void lineEditRXAntennaOutputs7Changed(QString str);
	void lineEditRXAntennaOutputs8Changed(QString str);
	void lineEditRXAntennaOutputs9Changed(QString str);
	void lineEditRXAntennaOutputs10Changed(QString str);
	
	void lineEditRXAntennaOutputStr160MChanged(QString str);
	void lineEditRXAntennaOutputStr80MChanged(QString str);
	void lineEditRXAntennaOutputStr40MChanged(QString str);
	void lineEditRXAntennaOutputStr30MChanged(QString str);
	
	void actionSaveTriggered();
	void actionOpenTriggered();
	void actionQuitTriggered();
	void actionConnectTriggered();
	void actionDisconnectTriggered();
	void actionExitConfModeTriggered();
	void actionGetFirmwareRevTriggered();
	void actionSendSettingsTriggered();
	
	void groupBoxSequencerPTTRadioClicked(bool state);
	void groupBoxSequencerPTTAmpClicked(bool state);
	void groupBoxSequencerInhibitClicked(bool state);
	void comboBoxSequencerSelectCurrentIndexChanged(int index);
	
	void spinBoxSequencerPTTRadioPreDelayValueChanged(int value);
	void spinBoxSequencerPTTRadioPostDelayValueChanged(int value);
	void spinBoxSequencerPTTAmpPreDelayValueChanged(int value);
	void spinBoxSequencerPTTAmpPostDelayValueChanged(int value);
	void spinBoxSequencerInhibitPredelayValueChanged(int value);
	void spinBoxSequencerInhibitPostdelayValueChanged(int value);
	void spinBoxSequencerAntennaPostDelayValueChanged(int value);
	void checkBoxPTTInputFootswitchClicked(bool state);
	void checkBoxPTTInputComputerRTSClicked(bool state);
	void checkBoxPTTInputRadioSenseClicked(bool state);
	void checkBoxPTTInputComputerRTSInvertedClicked(bool state);
	void checkBoxPTTInputRadioSenseInvertedClicked(bool state);
	void radioButtonInhibitPolarityActiveLowClicked(bool state);
	void radioButtonInhibitPolarityActiveHighClicked(bool state);
	
	void pushButtonSubMenuAnt1Pressed();
	void pushButtonSubMenuAnt2Pressed();
	void pushButtonSubMenuAnt3Pressed();
	void pushButtonSubMenuAnt4Pressed();
	void checkBoxSubMenuEnabledAnt1Clicked(bool state);
	void checkBoxSubMenuEnabledAnt2Clicked(bool state);
	void checkBoxSubMenuEnabledAnt3Clicked(bool state);
	void checkBoxSubMenuEnabledAnt4Clicked(bool state);
	
	/* EXTERNAL KEYPAD */	
	void pushButtonExt0Pressed();
	void pushButtonExt1Pressed();
	void pushButtonExt2Pressed();
	void pushButtonExt3Pressed();
	void pushButtonExt4Pressed();
	void pushButtonExt5Pressed();
	void pushButtonExt6Pressed();
	void pushButtonExt7Pressed();
	void pushButtonExt8Pressed();
	void pushButtonExt9Pressed();
	
	void pushButtonExtAPressed();
	void pushButtonExtBPressed();
	void pushButtonExtCPressed();
	void pushButtonExtDPressed();
	void pushButtonExtEPressed();
	void pushButtonExtFPressed();
	void pushButtonExtGPressed();
	
	void comboBoxExtKeypadFuncIndexChanged(int funcIndex);
	
	void spinBoxNetworkAddressValueChanged(int newValue);
	void checkBoxNetworkIsMasterClicked(bool state);
	void spinBoxNetworkNrOfDevicesValueChanged(int newValue);
	
	void spinBoxPowerMeterAdressValueChanged(int);
	void spinBoxPowerMeterSWRValueChanged(double);
	void spinBoxPowerMeterUpdateRateTextValueChanged(int);
	void spinBoxPowerMeterUpdateRateBargraphValueChanged(int);

	void radioButtonPTTInterlockNoneClicked(bool);
	void radioButtonPTTInterlockInput1Clicked(bool);
	void radioButtonPTTInterlockInput2Clicked(bool);
	void radioButtonPTTInterlockInput3Clicked(bool);
	void radioButtonPTTInterlockInput4Clicked(bool);
	void radioButtonPTTInterlockInput5Clicked(bool);
	void radioButtonPTTInterlockInput6Clicked(bool);
	void radioButtonPTTInterlockInput7Clicked(bool);

	/* The rotator functions */
	void comboBoxRotatorsIndexChanged(int index);
	void pushButtonRotatorAddPressed();
	void pushButtonRotatorSavePressed();
	void pushButtonRotatorDeletePressed();
	
	void comboBoxAntennaRotator1IndexChanged(int newIndex);
	void comboBoxAntennaRotator2IndexChanged(int newIndex);
	void comboBoxAntennaRotator3IndexChanged(int newIndex);
	void comboBoxAntennaRotator4IndexChanged(int newIndex);
public slots:
 
};
#endif





#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rotatordialog.h"
#include "settingsdialog.h"
#include "terminaldialog.h"
#include "tcpclass.h"
#include "errordialog.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QTimer>
#include <QImage>
#include <QProgressBar>
#include <QCheckBox>

#define INTERFACE_TYPE_SERIAL 0
#define INTERFACE_TYPE_TCP    1

static unsigned char openASC_logo[] = {
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,127,
127,127,127,127,127,255,255,255,255,255,255,255,255,255,255,255,
127,127,127,127,127,127,127,127,255,255,255,255,255,255,255,255,
255,255,255,127,127,127,127,127,127,127,255,255,255,255,255,255,
255,255,255,255,255,255,127,63,31,31,15,143,143,143,15,15,
31,31,127,255,255,255,63,15,15,15,15,15,159,143,143,15,
15,15,31,63,255,255,255,127,63,31,31,15,143,143,143,15,
15,15,31,63,255,255,255,63,15,15,15,15,15,159,143,15,
15,15,15,15,31,255,255,255,255,255,255,63,15,7,1,128,
224,0,0,0,0,7,255,255,255,255,255,135,1,0,0,0,
24,60,60,60,124,124,120,248,240,248,255,127,15,7,3,1,
192,240,248,248,252,252,252,252,248,240,224,248,255,255,255,255,
255,255,255,255,255,193,0,0,0,0,62,63,63,15,0,0,
128,192,240,255,127,7,0,0,0,0,56,63,63,31,15,0,
0,128,192,248,255,255,193,128,0,0,0,25,57,57,57,56,
56,24,152,248,255,127,3,0,0,0,128,248,255,255,63,3,
0,0,0,128,252,255,63,31,7,1,0,192,224,224,227,227,
227,224,0,0,0,0,15,255,255,31,7,15,31,30,62,62,
60,60,60,24,0,0,0,128,225,255,255,224,128,0,0,0,
15,31,63,63,63,63,31,31,15,231,255,255,255,255,255,255,
255,255,255,255,239,235,235,235,234,234,234,234,234,234,235,235,
235,251,255,207,192,192,192,192,240,255,238,234,234,234,234,235,
235,235,235,235,235,235,235,235,235,234,234,234,234,234,234,234,
234,235,235,235,235,234,234,234,234,234,235,235,235,235,234,234,
234,234,234,235,235,234,234,234,234,234,235,235,235,235,235,235,
235,235,234,234,234,234,234,235,235,235,235,235,234,234,234,234,
234,234,234,234,235,235,235,235,235,235,235,235,235,235,235,234,
234,234,234,234,234,234,235,235,251,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,63,49,224,230,206,14,24,249,63,199,248,240,131,207,247,
59,5,192,248,255,125,60,158,206,230,240,249,255,255,0,0,
127,159,231,251,0,0,63,207,227,248,252,63,199,248,240,131,
207,247,59,5,192,248,255,0,0,127,31,199,227,248,252,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,254,254,254,254,254,255,255,254,255,255,255,255,255,255,
254,254,255,255,254,254,254,254,254,254,254,255,255,255,254,254,
255,255,255,255,254,254,255,255,255,255,255,254,255,255,255,255,
255,255,254,254,255,255,255,254,254,254,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

typedef struct {
	unsigned char currentBand;
	unsigned char currentAntennas;
	unsigned char currentRXAntennas;
	unsigned int currentErrors;
	unsigned char subMenuType[4];
	unsigned char antennaFlags[4];
	unsigned char antSubOptSelected[4];
//struct_sub_menu_array subMenuArray;
//	struct_sub_menu_stack subMenuStack;
} status_struct;

typedef struct {
	unsigned char combCount;
	QString combNames[6];
} struct_sub_menu_stack;

class MainWindowImpl : public QMainWindow, public Ui::MainWindowImpl {
Q_OBJECT
public:
		MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
		RotatorDialog *rotatorWindow;
		SettingsDialog *settingsDialog;
		terminalDialog *terminalWindow;
		ErrorDialog *errorDialog;
		void pushButtonPressed(unsigned char button);
		QString getBandName(int bandIndex);
		unsigned char glcd_buffer[8][128];
		TCPClass *TCPComm;
		void updateDisplay();
private:
		int interfaceType;
		QTimer *timerPollRXQueue;
		QTimer *timerPollStatus;
		void getRXAntennaInfo();
		void getTXAntennaInfo();
		status_struct status;
		unsigned char bandChangedFlag;
protected:
		void paintEvent(QPaintEvent *event);
private slots:
public slots:
		void actionRebootTriggered();
		void WindowRotatorsTriggered();
		void actionConnectTriggered();
		void actionDisconnectTriggered();
		void actionSettingsEditTriggered();
		void actionErrorDialogTriggered();
		void actionTerminalTriggered();

		void pushButtonTX1Clicked();
		void pushButtonTX2Clicked();
		void pushButtonTX3Clicked();
		void pushButtonTX4Clicked();

		void comboBoxBandIndexChanged(int index);

		void pushButtonRXAnt1Clicked();
		void pushButtonRXAnt2Clicked();
		void pushButtonRXAnt3Clicked();
		void pushButtonRXAnt4Clicked();
		void pushButtonRXAnt5Clicked();
		void pushButtonRXAnt6Clicked();
		void pushButtonRXAnt7Clicked();
		void pushButtonRXAnt8Clicked();
		void pushButtonRXAnt9Clicked();
		void pushButtonRXAnt10Clicked();

		void pushButtonRXAntClicked();

		void timerPollRXQueueUpdate();
		void timerPollStatusUpdate();

		void pushButtonSubMenu1Clicked();
		void pushButtonSubMenu2Clicked();
		void pushButtonSubMenu3Clicked();
		void pushButtonSubMenu4Clicked();

		void pushButtonSubMenuArray1Clicked();
		void pushButtonSubMenuArray2Clicked();
		void pushButtonSubMenuArray3Clicked();
		void pushButtonSubMenuArray4Clicked();
};
#endif






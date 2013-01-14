#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include <QTimer>
#include <QMutex>
#include <QMessageBox>
#include <QDebug>

#include "setupdialog.h"
#include "ui_setupdialog.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

//#define DEBUG

class MainWindow;

#include "wmvbusclient.h"

/*! PowerMeter message structure
 *  ------------------------------------------------------FWD_PWR-----REF_PWR------VSWR---------
 *  |0xFE|0xFE|fm_addr|to_addr|chksum|flags|0xA0|7|TYPE|byte0|byte1|byte0|byte1|byte0|byte1|0xFD
 *  --------------------------------------------------------------------------------------------
 *  TYPE is a specification what type of pickup it is, what the power limits etc is.
 *
 *  VSWR is represented by an int, for example 311 means 3.11:1, 121 means 1.21:1 etc.
 *
 *  FWD_PWR and REF_PWR is represented by an int as well. So for example 1245 would mean 1245 watts.
 *
 *  The PowerMeter does broadcast these messages at a certain interval which is configured in the pickup. That way
 *  we can read whatever PowerMeters values on the bus just by changing which address we wish to listen to. The type variable
 *  is meant to be used for auto-scaling the power meter limits.
 */

/*! 150w pickup configuration */
#define PICKUP_TYPE_150W		0
/*! 1000w pickup configuration */
#define PICKUP_TYPE_1000W		1
/*! 1500w pickup configuration */
#define PICKUP_TYPE_1500W		2
/*! 2000w pickup configuration */
#define PICKUP_TYPE_2000W		3
/*! 3000w pickup configuration */
#define PICKUP_TYPE_3000W		4
/*! 5000w pickup configuration */
#define PICKUP_TYPE_5000W		5
/*! 10000w pickup configuration */
#define PICKUP_TYPE_10000W	6
/*! 15000w pickup configuration */
#define PICKUP_TYPE_15000W	7

namespace Ui {
  class MainWindowClass;
}

typedef struct {
  unsigned char pickupAddr;
  unsigned int vswrAlarm;
  unsigned int textUpdateRate;
  unsigned int bargraphUpdateRate;
} settingsStruct;

typedef struct {
  unsigned char type;
  unsigned int fwdPower;
  unsigned int refPower;
  unsigned int vswr;
  unsigned int maxPower;
} pickupStatusStruct;

class MainWindow : public QMainWindow, public BusMessageListener
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void recvBusMessage(BUS_MESSAGE message);
private:
  SetupDialog *setupDialog;
  Ui::MainWindowClass *ui;
  void setupConnections();
  WMVBusClient client;
  settingsStruct settings;
  pickupStatusStruct pickupStatus;
  void rescaleMeters(unsigned char type);
  QTimer *timerTextUpdate;
  QTimer *timerMetersUpdate;
private slots:
  void menuActionSetupTriggered();
  void menuActionSettingsAboutPressed();
  void actionConnectTriggered();
  void actionDisconnectTriggered();
  void timerTextUpdateTimeout();
  void timerMetersUpdateTimeout();
  };

#endif // MAINWINDOW_H

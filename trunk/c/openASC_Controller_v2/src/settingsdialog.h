#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include <QMainWindow>
#include <qsettings.h>

#include "ui_settingsdialog.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsDialog)
public:
	explicit SettingsDialog(QWidget *parent = 0);
	virtual ~SettingsDialog();

  QString getNetworkIPAddress(void);
  QString getPowerMeterNetworkIPAddress(void);
  unsigned int getNetworkPort(void);
  unsigned int getPowerMeterNetworkPort(void);

	int getPosMainWindowX();
	int getPosMainWindowY();
	int getPosKeypadWindowX();
	int getPosKeypadWindowY();
	int getPosRotatorWindowX();
	int getPosRotatorWindowY();
	int getPosTerminalWindowX();
	int getPosTerminalWindowY();
  int getPosPowerMeterWindowX();
  int getPosPowerMeterWindowY();

	void setPosMainWindowX(int pos);
	void setPosMainWindowY(int pos);
	void setPosKeypadWindowX(int pos);
	void setPosKeypadWindowY(int pos);
	void setPosRotatorWindowX(int pos);
	void setPosRotatorWindowY(int pos);
	void setPosTerminalWindowX(int pos);
	void setPosTerminalWindowY(int pos);
  void setPosPowerMeterWindowX(int pos);
  void setPosPowerMeterWindowY(int pos);

	void setKeypadWindowOpen(bool state);
	void setRotatorWindowOpen(bool state);
	void setTerminalWindowOpen(bool state);
  void setPowerMeterWindowOpen(bool state);
	bool getTerminalWindowOpen();
	bool getRotatorWindowOpen();
	bool getKeypadWindowOpen();
  bool getPowerMeterWindowOpen();

  bool getPowerMeterEnabled();

  bool getFrameRotatorWindow();
  bool getConnectOnStart();
  bool getFrameRotatorWindowStartOnTop();

  bool getActivityTimer();
  int  getActivityTimerTimeoutLimit();

  bool getShowMousePointer();
  unsigned char getPowerMeterPickupAddr(unsigned char band);

  QString getBandInfoBoxName(int boxIndex);
  int getBandInfoBoxAddr(int boxIndex);

  unsigned int getPowerMeterBargraphsUpdateRate();
  unsigned int getPowerMeterTextUpdateRate();

  unsigned char getPowerMeterPickupType(unsigned char band);

  void saveSettings();
protected:
	virtual void changeEvent(QEvent *e);
private:
	Ui::SettingsDialog *m_ui;
	QString strNetworkIPAddress;
  QString strPowerMeterNetworkIPAddress;
  unsigned int iNetworkPort;
  unsigned int iPowerMeterNetworkPort;
  bool bPowerMeterEnabled;

  unsigned int iBargraphsUpdateRate;
  unsigned int iTextUpdateRate;

  int mainWindowPosX;
	int mainWindowPosY;
	int keypadWindowPosX;
	int keypadWindowPosY;
	int rotatorWindowPosX;
	int rotatorWindowPosY;
	int terminalWindowPosX;
	int terminalWindowPosY;
  int powerMeterWindowPosX;
  int powerMeterWindowPosY;

	bool keypadWindowOpen;
	bool rotatorWindowOpen;
	bool terminalWindowOpen;
  bool powerMeterWindowOpen;

  bool frameRotatorWindow;
  bool frameRotatorWindowStartOnTop;
  bool connectOnStart;

  bool activityTimer;
  int activityTimerTimeoutLimit;
  bool showMousePointer;

  unsigned char cPickupAddr[9];
  unsigned char cPickupType[9];

  QString bandInfoBoxName[6];
  int bandInfoBoxAddr[6];
public slots:
	void groupboxNetworkClicked(bool state);
	void btnOKClicked();
	void btnCancelClicked();
private slots:
  void on_pushButtonOK_clicked();
  void on_pushButtonCancel_clicked();
  void on_comboBoxPowerMeterBand_currentIndexChanged(int index);
  void on_spinBoxPowerMeterAddress_valueChanged(int arg1);
  void on_comboBoxPowerMeterScaling_currentIndexChanged(int index);
  void on_checkBoxPowerMeterAllBands_clicked(bool checked);
};

#endif // SETTINGSDIALOG_H

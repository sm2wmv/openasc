#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets/QDialog>

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
	int getNetworkPort(void);
	int getPosMainWindowX();
	int getPosMainWindowY();
	int getPosKeypadWindowX();
	int getPosKeypadWindowY();
	int getPosRotatorWindowX();
	int getPosRotatorWindowY();
	int getPosTerminalWindowX();
	int getPosTerminalWindowY();

	void setPosMainWindowX(int pos);
	void setPosMainWindowY(int pos);
	void setPosKeypadWindowX(int pos);
	void setPosKeypadWindowY(int pos);
	void setPosRotatorWindowX(int pos);
	void setPosRotatorWindowY(int pos);
	void setPosTerminalWindowX(int pos);
	void setPosTerminalWindowY(int pos);

	void setKeypadWindowOpen(bool state);
	void setRotatorWindowOpen(bool state);
	void setTerminalWindowOpen(bool state);
	bool getTerminalWindowOpen();
	bool getRotatorWindowOpen();
	bool getKeypadWindowOpen();

  bool getFrameRotatorWindow();
  bool getConnectOnStart();
  bool getFrameRotatorWindowStartOnTop();

  bool getActivityTimer();
  int  getActivityTimerTimeoutLimit();

  bool getShowMousePointer();

	void saveSettings();
protected:
	virtual void changeEvent(QEvent *e);
private:
	Ui::SettingsDialog *m_ui;
	QString strNetworkIPAddress;
	int iNetworkPort;
	int mainWindowPosX;
	int mainWindowPosY;
	int keypadWindowPosX;
	int keypadWindowPosY;
	int rotatorWindowPosX;
	int rotatorWindowPosY;
	int terminalWindowPosX;
	int terminalWindowPosY;

	bool keypadWindowOpen;
	bool rotatorWindowOpen;
	bool terminalWindowOpen;

  bool frameRotatorWindow;
  bool frameRotatorWindowStartOnTop;
  bool connectOnStart;

  bool activityTimer;
  int activityTimerTimeoutLimit;
  bool showMousePointer;
public slots:
	void groupboxNetworkClicked(bool state);
	void btnOKClicked();
	void btnCancelClicked();
};

#endif // SETTINGSDIALOG_H

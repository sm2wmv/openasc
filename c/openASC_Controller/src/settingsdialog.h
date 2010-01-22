#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>

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

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *m_ui;
		QString strNetworkIPAddress;
		QString strCOMDeviceName;
		int iNetworkPort;
		int iDeviceInterfaceType;
public slots:
		void groupboxNetworkClicked(bool state);
		void groupBoxCOMClicked(bool state);
		void btnOKClicked();
		void btnCancelClicked();
};

#endif // SETTINGSDIALOG_H

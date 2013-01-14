#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QtGui/QDialog>
#include <QSettings>

#define DEVICE_TYPE_USB  0
#define DEVICE_TYPE_TCP  1

namespace Ui {
    class SetupDialog;
}

class SetupDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SetupDialog)
public:
    explicit SetupDialog(QWidget *parent = 0);
    virtual ~SetupDialog();
  QString getUSBDeviceName();
  unsigned char getDeviceConnectionType();
  unsigned char getPowerMeterAddress();
  unsigned int getPowerMeterTextUpdateRate();
  unsigned int getPowerMeterBargraphUpdateRate();
protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SetupDialog *m_ui;
		void readSettings();
		void saveSettings();

private slots:
    void pushButtonOKClicked();
    void pushButtonCancelClicked();
};

#endif // SETUPDIALOG_H

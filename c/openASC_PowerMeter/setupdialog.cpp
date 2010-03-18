#include "setupdialog.h"
#include "ui_setupdialog.h"

/*! Load all settings from settings.ini */
void SetupDialog::readSettings() {
		QSettings settings("settings.ini",QSettings::IniFormat,0);

		settings.beginGroup("Settings");

		//strNetworkIPAddress = settings.value("networkIPAddress").toString();
		//iNetworkPort = settings.value("networkPort").toInt();
		//strCOMDeviceName = settings.value("COMDeviceName").toString();
		//iDeviceInterfaceType = settings.value("DeviceInterfaceType").toInt();

		settings.endGroup();
}

void SetupDialog::saveSettings() {

}

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SetupDialog)
{
    m_ui->setupUi(this);

		readSettings();
}

SetupDialog::~SetupDialog()
{
    delete m_ui;
}

void SetupDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

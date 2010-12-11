#include "src/settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindowimpl.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog) {
		m_ui->setupUi(this);
		QSettings settings("settings.ini",QSettings::IniFormat,0);

		settings.beginGroup("Settings");

		strNetworkIPAddress = settings.value("networkIPAddress").toString();
		iNetworkPort = settings.value("networkPort").toInt();
		strCOMDeviceName = settings.value("COMDeviceName").toString();
		iDeviceInterfaceType = settings.value("DeviceInterfaceType").toInt();

		settings.endGroup();

		if (iDeviceInterfaceType == INTERFACE_TYPE_TCP) {
				m_ui->groupBoxNetwork->setChecked(true);
				m_ui->groupBoxCOM->setChecked(false);
		}
		else if (iDeviceInterfaceType == INTERFACE_TYPE_SERIAL) {
				m_ui->groupBoxCOM->setChecked(true);
				m_ui->groupBoxNetwork->setChecked(false);
		}

		m_ui->lineEditIP->setText(strNetworkIPAddress);
		m_ui->spinBoxPort->setValue(iNetworkPort);
		m_ui->lineEditDeviceName->setText(strCOMDeviceName);
}

SettingsDialog::~SettingsDialog() {
    delete m_ui;
}

void SettingsDialog::changeEvent(QEvent *e) {
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::groupboxNetworkClicked(bool state) {
		if (state == true) {
				m_ui->groupBoxCOM->setChecked(false);
				iDeviceInterfaceType = INTERFACE_TYPE_TCP;
		}
}

void SettingsDialog::groupBoxCOMClicked(bool state) {
		if (state == true) {
				m_ui->groupBoxNetwork->setChecked(false);
				iDeviceInterfaceType = INTERFACE_TYPE_SERIAL;
		}
}

void SettingsDialog::btnOKClicked() {
		QSettings settings("settings.ini",QSettings::IniFormat,0);

		strNetworkIPAddress = m_ui->lineEditIP->text();
		iNetworkPort = m_ui->spinBoxPort->value();
		strCOMDeviceName = m_ui->lineEditDeviceName->text();


		settings.beginGroup("Settings");

		settings.setValue("networkIPAddress",strNetworkIPAddress);
		settings.setValue("networkPort",iNetworkPort);
		settings.setValue("COMDeviceName",strCOMDeviceName);
		settings.setValue("DeviceInterfaceType",iDeviceInterfaceType);

		settings.endGroup();

		this->hide();
}

void SettingsDialog::btnCancelClicked() {
		this->hide();
}

QString SettingsDialog::getCOMDeviceName(void) {
	return(strCOMDeviceName);
}

QString SettingsDialog::getNetworkIPAddress(void) {
	return(strNetworkIPAddress);
}

int SettingsDialog::getNetworkPort(void) {
	return(iNetworkPort);
}

int SettingsDialog::getDeviceInterfaceType(void) {
	return(iDeviceInterfaceType);
}


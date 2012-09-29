#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindowimpl.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog) {
		m_ui->setupUi(this);

		QSettings settings("settings.ini",QSettings::IniFormat,0);

		settings.beginGroup("Settings");

		strNetworkIPAddress = settings.value("networkIPAddress").toString();
		iNetworkPort = settings.value("networkPort").toInt();

		settings.endGroup();

		m_ui->groupBoxNetwork->setChecked(true);

		m_ui->lineEditIP->setText(strNetworkIPAddress);
		m_ui->spinBoxPort->setValue(iNetworkPort);
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
}

void SettingsDialog::btnOKClicked() {
		QSettings settings("settings.ini",QSettings::IniFormat,0);

		strNetworkIPAddress = m_ui->lineEditIP->text();
		iNetworkPort = m_ui->spinBoxPort->value();

		settings.beginGroup("Settings");

		settings.setValue("networkIPAddress",strNetworkIPAddress);
		settings.setValue("networkPort",iNetworkPort);

		settings.endGroup();

		this->hide();
}

void SettingsDialog::btnCancelClicked() {
		this->hide();
}

QString SettingsDialog::getNetworkIPAddress(void) {
	return(strNetworkIPAddress);
}

int SettingsDialog::getNetworkPort(void) {
	return(iNetworkPort);
}


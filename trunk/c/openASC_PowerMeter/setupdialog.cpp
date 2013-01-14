#include "setupdialog.h"
#include "ui_setupdialog.h"

/*! Load all settings from settings.ini */
void SetupDialog::readSettings() {
    QSettings settings("settings.ini",QSettings::IniFormat,0);

		settings.beginGroup("Settings");

    m_ui->lineEditUSBDeviceName->setText(settings.value("USBDeviceName").toString());
    m_ui->spinBoxPickupAddress->setValue(settings.value("PickupAddress").toInt());
    m_ui->spinBoxDisplayTextUpdateRate->setValue(settings.value("DisplayTextUpdateRate").toInt());
    m_ui->spinBoxDisplayProgressBarUpdateRate->setValue(settings.value("DisplayProgressBarUpdateRate").toInt());

    settings.endGroup();
}

void SetupDialog::saveSettings() {
  QSettings settings("settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("Settings");

  settings.setValue("USBDeviceName",m_ui->lineEditUSBDeviceName->text());
  settings.setValue("PickupAddress",m_ui->spinBoxPickupAddress->value());
  settings.setValue("DisplayTextUpdateRate",m_ui->spinBoxDisplayTextUpdateRate->value());
  settings.setValue("DisplayProgressBarUpdateRate",m_ui->spinBoxDisplayProgressBarUpdateRate->value());

  settings.endGroup();
}

SetupDialog::SetupDialog(QWidget *parent) : QDialog(parent),m_ui(new Ui::SetupDialog) {
  m_ui->setupUi(this);

  readSettings();

  connect(m_ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOKClicked()));
  connect(m_ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancelClicked()));
}

void SetupDialog::pushButtonOKClicked() {
  saveSettings();

  this->hide();
}

void SetupDialog::pushButtonCancelClicked() {
  this->hide();
}

QString SetupDialog::getUSBDeviceName() {
  return(m_ui->lineEditUSBDeviceName->text());
}

unsigned char SetupDialog::getDeviceConnectionType() {
  return(DEVICE_TYPE_USB);
}

unsigned char SetupDialog::getPowerMeterAddress() {
  return(m_ui->spinBoxPickupAddress->value());
}

unsigned int SetupDialog::getPowerMeterTextUpdateRate() {
  return(m_ui->spinBoxDisplayTextUpdateRate->value());
}


unsigned int SetupDialog::getPowerMeterBargraphUpdateRate() {
  return(m_ui->spinBoxDisplayProgressBarUpdateRate->value());
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

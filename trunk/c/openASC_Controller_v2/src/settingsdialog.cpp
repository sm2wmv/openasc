#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindowimpl.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog) {
  m_ui->setupUi(this);

  m_ui->tabWidget->setCurrentIndex(0);

  for (unsigned char i=0;i<9;i++)
    cPickupAddr[i] = 0;

  bool bSaveSettings = false;

  QSettings settings("settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("Settings");

  strNetworkIPAddress = settings.value("networkIPAddress").toString();
  iNetworkPort = settings.value("networkPort").toInt();
  strPowerMeterNetworkIPAddress = settings.value("powerMeterNetworkIPAddress").toString();
  iPowerMeterNetworkPort = settings.value("powerMeterNetworkPort").toInt();

  if (settings.contains("mainWindowPosX")) {
    mainWindowPosX = settings.value("mainWindowPosX").toInt();
    mainWindowPosY = settings.value("mainWindowPosY").toInt();
    keypadWindowPosX = settings.value("keypadWindowPosX").toInt();
    keypadWindowPosY = settings.value("keypadWindowPosY").toInt();
    rotatorWindowPosX = settings.value("rotatorWindowPosX").toInt();
    rotatorWindowPosY = settings.value("rotatorWindowPosY").toInt();
    terminalWindowPosX = settings.value("terminalWindowPosX").toInt();
    terminalWindowPosY = settings.value("terminalWindowPosY").toInt();
    powerMeterWindowPosX = settings.value("powerMeterWindowPosX").toInt();
    powerMeterWindowPosY = settings.value("powerMeterWindowPosY").toInt();

    terminalWindowOpen = settings.value("terminalWindowOpen").toBool();
    rotatorWindowOpen = settings.value("rotatorWindowOpen").toBool();
    keypadWindowOpen = settings.value("keypadWindowOpen").toBool();
    powerMeterWindowOpen = settings.value("powerMeterWindowOpen").toBool();

    frameRotatorWindow = settings.value("frameRotatorWindow").toBool();
    frameRotatorWindowStartOnTop = settings.value("frameRotatorWindowStartOnTop").toBool();
    connectOnStart = settings.value("connectOnStart").toBool();
    activityTimer = settings.value("activityTimer").toBool();
    activityTimerTimeoutLimit = settings.value("activityTimerTimeoutLimit").toInt();
    showMousePointer = settings.value("showMousePointer").toBool();
    bPowerMeterEnabled = settings.value("powerMeterEnabled").toBool();

    bandInfoBoxName[0] = settings.value("BoxAName").toString();
    bandInfoBoxName[1] = settings.value("BoxBName").toString();
    bandInfoBoxName[2] = settings.value("BoxCName").toString();
    bandInfoBoxName[3] = settings.value("BoxDName").toString();
    bandInfoBoxName[4] = settings.value("BoxEName").toString();
    bandInfoBoxName[5] = settings.value("BoxFName").toString();
    bandInfoBoxAddr[0] = settings.value("BoxANetworkAddr").toInt();
    bandInfoBoxAddr[1] = settings.value("BoxBNetworkAddr").toInt();
    bandInfoBoxAddr[2] = settings.value("BoxCNetworkAddr").toInt();
    bandInfoBoxAddr[3] = settings.value("BoxDNetworkAddr").toInt();
    bandInfoBoxAddr[4] = settings.value("BoxENetworkAddr").toInt();
    bandInfoBoxAddr[5] = settings.value("BoxFNetworkAddr").toInt();

    cPickupAddr[0] = settings.value("PickupAddr0").toInt();
    cPickupAddr[1] = settings.value("PickupAddr1").toInt();
    cPickupAddr[2] = settings.value("PickupAddr2").toInt();
    cPickupAddr[3] = settings.value("PickupAddr3").toInt();
    cPickupAddr[4] = settings.value("PickupAddr4").toInt();
    cPickupAddr[5] = settings.value("PickupAddr5").toInt();
    cPickupAddr[6] = settings.value("PickupAddr6").toInt();
    cPickupAddr[7] = settings.value("PickupAddr7").toInt();
    cPickupAddr[8] = settings.value("PickupAddr8").toInt();

    cPickupType[0] = settings.value("PickupType0").toInt();
    cPickupType[1] = settings.value("PickupType1").toInt();
    cPickupType[2] = settings.value("PickupType2").toInt();
    cPickupType[3] = settings.value("PickupType3").toInt();
    cPickupType[4] = settings.value("PickupType4").toInt();
    cPickupType[5] = settings.value("PickupType5").toInt();
    cPickupType[6] = settings.value("PickupType6").toInt();
    cPickupType[7] = settings.value("PickupType7").toInt();
    cPickupType[8] = settings.value("PickupType8").toInt();

    iBargraphsUpdateRate = settings.value("BargraphsUpdateRate").toInt();
    iTextUpdateRate = settings.value("TextUpdateRate").toInt();
  }
  else {
    strNetworkIPAddress = "127.0.0.1";
    strPowerMeterNetworkIPAddress = "127.0.0.1";
    iNetworkPort = 34554;
    iPowerMeterNetworkPort = 34555;
    mainWindowPosX = 100;
    mainWindowPosY = 100;
    keypadWindowPosX = 100;
    keypadWindowPosY = 100;
    rotatorWindowPosX = 100;
    rotatorWindowPosY = 100;
    terminalWindowPosX = 100;
    terminalWindowPosY = 100;
    powerMeterWindowPosX = 100;
    powerMeterWindowPosY = 100;

    powerMeterWindowOpen = false;
    terminalWindowOpen = false;
    rotatorWindowOpen = false;
    keypadWindowOpen = false;
    frameRotatorWindow = true;
    frameRotatorWindowStartOnTop = false;
    connectOnStart = false;
    activityTimer = true;

    activityTimerTimeoutLimit = 1800;
    showMousePointer = true;

    for (unsigned char i=0;i<6;i++)
      bandInfoBoxAddr[i] = 0;

    bandInfoBoxName[0] = "Radio 1";
    bandInfoBoxName[1] = "Radio 2";
    bandInfoBoxName[2] = "Radio 3";
    bandInfoBoxName[3] = "Radio 4";
    bandInfoBoxName[4] = "Radio 5";
    bandInfoBoxName[5] = "Radio 6";

    for (unsigned char i=0;i<9;i++) {
      cPickupAddr[i] = 0;
      cPickupType[i] = 0;
    }

    iBargraphsUpdateRate = 500;
    iTextUpdateRate = 200;

    bSaveSettings = true;
  }

  settings.endGroup();

  if (activityTimerTimeoutLimit == 0)
    activityTimerTimeoutLimit = 1800;

  m_ui->groupBoxNetwork->setChecked(true);
  m_ui->groupBoxPowerMeterNetwork->setChecked(bPowerMeterEnabled);
  m_ui->lineEditIP->setText(strNetworkIPAddress);
  m_ui->lineEditPowerMeterIP->setText(strPowerMeterNetworkIPAddress);
  m_ui->spinBoxPort->setValue(iNetworkPort);
  m_ui->spinBoxPowerMeterPort->setValue(iPowerMeterNetworkPort);
  m_ui->checkBoxRotatorWindowFrame->setChecked(frameRotatorWindow);
  m_ui->checkBoxRotatorWindowStartOnStop->setChecked(frameRotatorWindowStartOnTop);
  m_ui->checkBoxConnectOnStartup->setChecked(connectOnStart);
  m_ui->checkBoxActivityTimer->setChecked(activityTimer);
  m_ui->checkBoxRotatorShowMousePointer->setChecked(showMousePointer);

  m_ui->spinBoxPowerMeterUpdateRateBargraph->setValue(iBargraphsUpdateRate);
  m_ui->spinBoxPowerMeterUpdateRateText->setValue(iTextUpdateRate);

  //Set combobox to show 160m
  m_ui->comboBoxPowerMeterBand->setCurrentIndex(0);
  m_ui->spinBoxPowerMeterAddress->setValue(cPickupAddr[0]);
  m_ui->comboBoxPowerMeterScaling->setCurrentIndex(cPickupType[0]);

  if (bSaveSettings)
    saveSettings();
}

unsigned int SettingsDialog::getPowerMeterBargraphsUpdateRate() {
  return(iBargraphsUpdateRate);
}

unsigned int SettingsDialog::getPowerMeterTextUpdateRate() {
  return(iTextUpdateRate);
}

int SettingsDialog::getActivityTimerTimeoutLimit() {
  return(activityTimerTimeoutLimit);
}

SettingsDialog::~SettingsDialog() {
  delete m_ui;
}

bool SettingsDialog::getFrameRotatorWindow() {
  return(frameRotatorWindow);
}

bool SettingsDialog::getActivityTimer() {
  return(activityTimer);
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

void SettingsDialog::saveSettings() {
	QSettings settings("settings.ini",QSettings::IniFormat,0);

  bPowerMeterEnabled = m_ui->groupBoxPowerMeterNetwork->isChecked();
  strPowerMeterNetworkIPAddress = m_ui->lineEditPowerMeterIP->text();
  iPowerMeterNetworkPort = m_ui->spinBoxPowerMeterPort->value();
  strNetworkIPAddress = m_ui->lineEditIP->text();
  iNetworkPort = m_ui->spinBoxPort->value();
  frameRotatorWindow = m_ui->checkBoxRotatorWindowFrame->isChecked();
  frameRotatorWindowStartOnTop = m_ui->checkBoxRotatorWindowStartOnStop->isChecked();
  connectOnStart = m_ui->checkBoxConnectOnStartup->isChecked();
  activityTimer = m_ui->checkBoxActivityTimer->isChecked();
  showMousePointer = m_ui->checkBoxRotatorShowMousePointer->isChecked();
  iBargraphsUpdateRate = m_ui->spinBoxPowerMeterUpdateRateBargraph->value();
  iTextUpdateRate = m_ui->spinBoxPowerMeterUpdateRateText->value();

	settings.beginGroup("Settings");

	settings.setValue("networkIPAddress",strNetworkIPAddress);
	settings.setValue("networkPort",iNetworkPort);
  settings.setValue("powerMeterNetworkIPAddress",strPowerMeterNetworkIPAddress);
  settings.setValue("powerMeterNetworkPort",iPowerMeterNetworkPort);
  settings.setValue("powerMeterEnabled",bPowerMeterEnabled);

	settings.setValue("mainWindowPosX",mainWindowPosX);
	settings.setValue("mainWindowPosY",mainWindowPosY);
	settings.setValue("keypadWindowPosX",keypadWindowPosX);
	settings.setValue("keypadWindowPosY",keypadWindowPosY);
	settings.setValue("rotatorWindowPosX",rotatorWindowPosX);
	settings.setValue("rotatorWindowPosY",rotatorWindowPosY);
	settings.setValue("terminalWindowPosX",terminalWindowPosX);
	settings.setValue("terminalWindowPosY",terminalWindowPosY);
  settings.setValue("powerMeterWindowPosX",powerMeterWindowPosX);
  settings.setValue("powerMeterWindowPosY",powerMeterWindowPosY);

	settings.setValue("keypadWindowOpen",keypadWindowOpen);
	settings.setValue("rotatorWindowOpen",rotatorWindowOpen);
	settings.setValue("terminalWindowOpen",terminalWindowOpen);
  settings.setValue("powerMeterWindowOpen",powerMeterWindowOpen);

  settings.setValue("frameRotatorWindow",frameRotatorWindow);
  settings.setValue("frameRotatorWindowStartOnTop",frameRotatorWindowStartOnTop);
  settings.setValue("connectOnStart",connectOnStart);
  settings.setValue("activityTimer",activityTimer);
  settings.setValue("activityTimerTimeoutLimit",activityTimerTimeoutLimit);
  settings.setValue("showMousePointer",showMousePointer);

  settings.setValue("BoxAName",bandInfoBoxName[0]);
  settings.setValue("BoxBName",bandInfoBoxName[1]);
  settings.setValue("BoxCName",bandInfoBoxName[2]);
  settings.setValue("BoxDName",bandInfoBoxName[3]);
  settings.setValue("BoxEName",bandInfoBoxName[4]);
  settings.setValue("BoxFName",bandInfoBoxName[5]);

  settings.setValue("BoxANetworkAddr",bandInfoBoxAddr[0]);
  settings.setValue("BoxBNetworkAddr",bandInfoBoxAddr[1]);
  settings.setValue("BoxCNetworkAddr",bandInfoBoxAddr[2]);
  settings.setValue("BoxDNetworkAddr",bandInfoBoxAddr[3]);
  settings.setValue("BoxENetworkAddr",bandInfoBoxAddr[4]);
  settings.setValue("BoxFNetworkAddr",bandInfoBoxAddr[5]);

  settings.setValue("PickupAddr0",cPickupAddr[0]);
  settings.setValue("PickupAddr1",cPickupAddr[1]);
  settings.setValue("PickupAddr2",cPickupAddr[2]);
  settings.setValue("PickupAddr3",cPickupAddr[3]);
  settings.setValue("PickupAddr4",cPickupAddr[4]);
  settings.setValue("PickupAddr5",cPickupAddr[5]);
  settings.setValue("PickupAddr6",cPickupAddr[6]);
  settings.setValue("PickupAddr7",cPickupAddr[7]);
  settings.setValue("PickupAddr8",cPickupAddr[8]);

  settings.setValue("PickupType0",cPickupType[0]);
  settings.setValue("PickupType1",cPickupType[1]);
  settings.setValue("PickupType2",cPickupType[2]);
  settings.setValue("PickupType3",cPickupType[3]);
  settings.setValue("PickupType4",cPickupType[4]);
  settings.setValue("PickupType5",cPickupType[5]);
  settings.setValue("PickupType6",cPickupType[6]);
  settings.setValue("PickupType7",cPickupType[7]);
  settings.setValue("PickupType8",cPickupType[8]);

  settings.setValue("BargraphsUpdateRate",iBargraphsUpdateRate);
  settings.setValue("TextUpdateRate",iTextUpdateRate);

  settings.endGroup();
}

bool SettingsDialog::getConnectOnStart() {
  return(connectOnStart);
}

bool SettingsDialog::getFrameRotatorWindowStartOnTop() {
  return(frameRotatorWindowStartOnTop);
}

void SettingsDialog::btnOKClicked() {
  saveSettings();

  this->hide();
}

void SettingsDialog::btnCancelClicked() {
  this->hide();
}

QString SettingsDialog::getNetworkIPAddress(void) {
	return(strNetworkIPAddress);
}

unsigned int SettingsDialog::getNetworkPort(void) {
	return(iNetworkPort);
}

int SettingsDialog::getPosMainWindowX() {
	return(mainWindowPosX);
}

int SettingsDialog::getPosMainWindowY() {
	return(mainWindowPosY);
}

int SettingsDialog::getPosKeypadWindowX() {
	return(keypadWindowPosX);
}

int SettingsDialog::getPosKeypadWindowY() {
	return(keypadWindowPosY);
}

int SettingsDialog::getPosRotatorWindowX() {
	return(rotatorWindowPosX);
}

int SettingsDialog::getPosRotatorWindowY() {
	return(rotatorWindowPosY);
}

int SettingsDialog::getPosTerminalWindowX() {
	return(terminalWindowPosX);
}

int SettingsDialog::getPosTerminalWindowY() {
	return(terminalWindowPosY);
}

int SettingsDialog::getPosPowerMeterWindowX() {
  return(powerMeterWindowPosX);
}

int SettingsDialog::getPosPowerMeterWindowY() {
  return(powerMeterWindowPosY);
}

void SettingsDialog::setPosMainWindowX(int pos) {
	mainWindowPosX = pos;
}

void SettingsDialog::setPosMainWindowY(int pos) {
	mainWindowPosY = pos;
}

void SettingsDialog::setPosKeypadWindowX(int pos) {
	keypadWindowPosX = pos;
}

void SettingsDialog::setPosKeypadWindowY(int pos) {
	keypadWindowPosY = pos;
}

void SettingsDialog::setPosRotatorWindowX(int pos) {
	rotatorWindowPosX = pos;
}

void SettingsDialog::setPosRotatorWindowY(int pos) {
	rotatorWindowPosY = pos;
}

void SettingsDialog::setPosTerminalWindowX(int pos) {
	terminalWindowPosX = pos;
}

void SettingsDialog::setPosTerminalWindowY(int pos) {
	terminalWindowPosY = pos;
}

void SettingsDialog::setPosPowerMeterWindowX(int pos) {
  powerMeterWindowPosX = pos;
}

void SettingsDialog::setPosPowerMeterWindowY(int pos) {
  powerMeterWindowPosY = pos;
}

void SettingsDialog::setKeypadWindowOpen(bool state) {
	keypadWindowOpen = state;
}

void SettingsDialog::setRotatorWindowOpen(bool state) {
	rotatorWindowOpen = state;
}

void SettingsDialog::setTerminalWindowOpen(bool state) {
	terminalWindowOpen = state;
}

void SettingsDialog::setPowerMeterWindowOpen(bool state) {
  powerMeterWindowOpen = state;
}

bool SettingsDialog::getTerminalWindowOpen() {
	return(terminalWindowOpen);
}

bool SettingsDialog::getRotatorWindowOpen() {
	return(rotatorWindowOpen);
}

bool SettingsDialog::getKeypadWindowOpen() {
	return(keypadWindowOpen);
}

QString SettingsDialog::getPowerMeterNetworkIPAddress(void) {
  return(strPowerMeterNetworkIPAddress);
}

unsigned int SettingsDialog::getPowerMeterNetworkPort(void) {
  return(iPowerMeterNetworkPort);
}

bool SettingsDialog::getPowerMeterWindowOpen() {
  return(powerMeterWindowOpen);
}

bool SettingsDialog::getShowMousePointer()  {
  return(showMousePointer);
}

QString SettingsDialog::getBandInfoBoxName(int boxIndex) {
    return(bandInfoBoxName[boxIndex]);
}

int SettingsDialog::getBandInfoBoxAddr(int boxIndex) {
    return(bandInfoBoxAddr[boxIndex]);
}

bool SettingsDialog::getPowerMeterEnabled() {
  return(bPowerMeterEnabled);
}

unsigned char SettingsDialog::getPowerMeterPickupType(unsigned char band) {
  return(cPickupType[band]);
}

unsigned char SettingsDialog::getPowerMeterPickupAddr(unsigned char band) {
  return(cPickupAddr[band]);
}

//

void SettingsDialog::on_pushButtonOK_clicked() {
  saveSettings();
  this->hide();
}

void SettingsDialog::on_pushButtonCancel_clicked() {
  this->hide();
}

void SettingsDialog::on_comboBoxPowerMeterBand_currentIndexChanged(int index) {
  m_ui->spinBoxPowerMeterAddress->setValue(cPickupAddr[index]);
  m_ui->comboBoxPowerMeterScaling->setCurrentIndex(cPickupType[index]);
}

void SettingsDialog::on_spinBoxPowerMeterAddress_valueChanged(int arg1) {
  if (m_ui->checkBoxPowerMeterAllBands->isChecked()) {
    for(unsigned char i=0;i<9;i++)
      cPickupAddr[i] = arg1;
  }
  else
    cPickupAddr[m_ui->comboBoxPowerMeterBand->currentIndex()] = arg1;
}

void SettingsDialog::on_comboBoxPowerMeterScaling_currentIndexChanged(int index) {
  if (m_ui->checkBoxPowerMeterAllBands->isChecked()) {
    for(unsigned char i=0;i<9;i++)
      cPickupType[i] = index;
  }
  else
    cPickupType[m_ui->comboBoxPowerMeterBand->currentIndex()] = index;
}

void SettingsDialog::on_checkBoxPowerMeterAllBands_clicked(bool checked) {
  if(checked) {
    for (unsigned char i=0;i<9;i++) {
      cPickupAddr[i] = m_ui->spinBoxPowerMeterAddress->value();
      cPickupType[i] = m_ui->comboBoxPowerMeterScaling->currentIndex();
    }
  }
}

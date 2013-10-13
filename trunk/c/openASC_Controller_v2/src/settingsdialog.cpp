#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindowimpl.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::SettingsDialog) {
		m_ui->setupUi(this);

    bool bSaveSettings = false;

		QSettings settings("settings.ini",QSettings::IniFormat,0);

		settings.beginGroup("Settings");

		strNetworkIPAddress = settings.value("networkIPAddress").toString();
		iNetworkPort = settings.value("networkPort").toInt();

		if (settings.contains("mainWindowPosX")) {
            mainWindowPosX = settings.value("mainWindowPosX").toInt();
            mainWindowPosY = settings.value("mainWindowPosY").toInt();
            keypadWindowPosX = settings.value("keypadWindowPosX").toInt();
            keypadWindowPosY = settings.value("keypadWindowPosY").toInt();
            rotatorWindowPosX = settings.value("rotatorWindowPosX").toInt();
            rotatorWindowPosY = settings.value("rotatorWindowPosY").toInt();
            terminalWindowPosX = settings.value("terminalWindowPosX").toInt();
            terminalWindowPosY = settings.value("terminalWindowPosY").toInt();

            terminalWindowOpen = settings.value("terminalWindowOpen").toBool();
            rotatorWindowOpen = settings.value("rotatorWindowOpen").toBool();
            keypadWindowOpen = settings.value("keypadWindowOpen").toBool();

            frameRotatorWindow = settings.value("frameRotatorWindow").toBool();
            frameRotatorWindowStartOnTop = settings.value("frameRotatorWindowStartOnTop").toBool();
            connectOnStart = settings.value("connectOnStart").toBool();
            activityTimer = settings.value("activityTimer").toBool();
            activityTimerTimeoutLimit = settings.value("activityTimerTimeoutLimit").toInt();
            showMousePointer = settings.value("showMousePointer").toBool();

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
        }
		else {
            strNetworkIPAddress = "127.0.0.1";
            iNetworkPort = 34554;
			mainWindowPosX = 100;
			mainWindowPosY = 100;
			keypadWindowPosX = 100;
			keypadWindowPosY = 100;
			rotatorWindowPosX = 100;
			rotatorWindowPosY = 100;
			terminalWindowPosX = 100;
			terminalWindowPosY = 100;

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

            bSaveSettings = true;
		}

		settings.endGroup();

    if (activityTimerTimeoutLimit == 0)
        activityTimerTimeoutLimit = 1800;

    m_ui->groupBoxNetwork->setChecked(true);

    m_ui->lineEditIP->setText(strNetworkIPAddress);
    m_ui->spinBoxPort->setValue(iNetworkPort);
    m_ui->checkBoxRotatorWindowFrame->setChecked(frameRotatorWindow);
    m_ui->checkBoxRotatorWindowStartOnStop->setChecked(frameRotatorWindowStartOnTop);
    m_ui->checkBoxConnectOnStartup->setChecked(connectOnStart);
    m_ui->checkBoxActivityTimer->setChecked(activityTimer);
    m_ui->checkBoxRotatorShowMousePointer->setChecked(showMousePointer);

    if (bSaveSettings)
      saveSettings();
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

    strNetworkIPAddress = m_ui->lineEditIP->text();
    iNetworkPort = m_ui->spinBoxPort->value();
    frameRotatorWindow = m_ui->checkBoxRotatorWindowFrame->isChecked();
    frameRotatorWindowStartOnTop = m_ui->checkBoxRotatorWindowStartOnStop->isChecked();
    connectOnStart = m_ui->checkBoxConnectOnStartup->isChecked();
    activityTimer = m_ui->checkBoxActivityTimer->isChecked();
    showMousePointer = m_ui->checkBoxRotatorShowMousePointer->isChecked();

	settings.beginGroup("Settings");

	settings.setValue("networkIPAddress",strNetworkIPAddress);
	settings.setValue("networkPort",iNetworkPort);
	settings.setValue("mainWindowPosX",mainWindowPosX);
	settings.setValue("mainWindowPosY",mainWindowPosY);
	settings.setValue("keypadWindowPosX",keypadWindowPosX);
	settings.setValue("keypadWindowPosY",keypadWindowPosY);
	settings.setValue("rotatorWindowPosX",rotatorWindowPosX);
	settings.setValue("rotatorWindowPosY",rotatorWindowPosY);
	settings.setValue("terminalWindowPosX",terminalWindowPosX);
	settings.setValue("terminalWindowPosY",terminalWindowPosY);

	settings.setValue("keypadWindowOpen",keypadWindowOpen);
	settings.setValue("rotatorWindowOpen",rotatorWindowOpen);
	settings.setValue("terminalWindowOpen",terminalWindowOpen);

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

int SettingsDialog::getNetworkPort(void) {
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

void SettingsDialog::setKeypadWindowOpen(bool state) {
	keypadWindowOpen = state;
}

void SettingsDialog::setRotatorWindowOpen(bool state) {
	rotatorWindowOpen = state;
}

void SettingsDialog::setTerminalWindowOpen(bool state) {
	terminalWindowOpen = state;
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

bool SettingsDialog::getShowMousePointer()  {
  return(showMousePointer);
}

QString SettingsDialog::getBandInfoBoxName(int boxIndex) {
    return(bandInfoBoxName[boxIndex]);
}

int SettingsDialog::getBandInfoBoxAddr(int boxIndex) {
    return(bandInfoBoxAddr[boxIndex]);
}

//

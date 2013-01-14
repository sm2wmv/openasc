#include "mainwindow.h"
#include "ui_mainwindow.h"

QMutex mutex;

void MainWindow::menuActionSetupTriggered() {
	setupDialog->show();
}

void MainWindow::rescaleMeters(unsigned char type) {
  if (type == PICKUP_TYPE_150W) {
    ui->progressBarFwd->setMaximum(150);
    ui->progressBarRef->setMaximum(15);
  }
  else if (type == PICKUP_TYPE_1000W) {
    ui->progressBarFwd->setMaximum(1000);
    ui->progressBarRef->setMaximum(100);
  }
  else if (type == PICKUP_TYPE_1500W) {
    ui->progressBarFwd->setMaximum(1500);
    ui->progressBarRef->setMaximum(150);
  }
  else if (type == PICKUP_TYPE_2000W) {
    ui->progressBarFwd->setMaximum(2000);
    ui->progressBarRef->setMaximum(200);
  }
  else if (type == PICKUP_TYPE_3000W) {
    ui->progressBarFwd->setMaximum(3000);
    ui->progressBarRef->setMaximum(300);
  }
  else if (type == PICKUP_TYPE_5000W) {
    ui->progressBarFwd->setMaximum(5000);
    ui->progressBarRef->setMaximum(500);
  }
  else if (type == PICKUP_TYPE_10000W) {
    ui->progressBarFwd->setMaximum(10000);
    ui->progressBarRef->setMaximum(1000);
  }
  else if (type == PICKUP_TYPE_15000W) {
    ui->progressBarFwd->setMaximum(15000);
    ui->progressBarRef->setMaximum(1500);
  }
}

void MainWindow::recvBusMessage(BUS_MESSAGE message) {
	#ifdef DEBUG
		qDebug("----------------------------");
		qDebug("from_addr: \t\t0x%02X",message.from_addr);
		qDebug("to_addr: \t\t0x%02X",message.to_addr);
		qDebug("checksum: \t\t0x%02X",message.checksum);
		qDebug("flags: \t\t0x%02X",message.flags);
		qDebug("cmd: \t\t0x%02X",message.cmd);
		qDebug("length: \t\t0x%02X",message.length);

		for (int i=0;i<message.length;i++)
				qDebug("data[%i]: \t\t0x%02X",i,message.data[i]);

		qDebug(" ");
	#endif

	if (message.from_addr == settings.pickupAddr) {
    if (message.cmd == BUS_CMD_POWERMETER_STATUS) {
      mutex.lock();
      if (message.data[0] != pickupStatus.type) {
        pickupStatus.type = message.data[0];
        rescaleMeters(pickupStatus.type);
      }

      pickupStatus.fwdPower = message.data[2] + (message.data[1]>>8);
      pickupStatus.refPower = message.data[4] + (message.data[3]>>8);
      pickupStatus.vswr = message.data[6] + (message.data[5]>>8);

      mutex.unlock();
    }
  }
}

void MainWindow::menuActionSettingsAboutPressed() {
		AboutDialog *aboutDialog = new AboutDialog();
		aboutDialog->show();
}

void MainWindow::actionConnectTriggered() {
  if (setupDialog->getDeviceConnectionType() == DEVICE_TYPE_USB) {
    settings.pickupAddr = setupDialog->getPowerMeterAddress();
    settings.textUpdateRate = setupDialog->getPowerMeterTextUpdateRate();
    settings.bargraphUpdateRate = setupDialog->getPowerMeterBargraphUpdateRate();

    if (!client.initClient(settings.pickupAddr, setupDialog->getUSBDeviceName(), this)) {
      client.start();
      client.receiveMsg();

      timerTextUpdate->start(settings.textUpdateRate);
      timerMetersUpdate->start(settings.bargraphUpdateRate);
    }
    else {
      QMessageBox::critical(0,"Serial port error!","Could not open the specified port!");
      qDebug() << "Failed to open: " << setupDialog->getUSBDeviceName();
    }
  }
}

void MainWindow::timerTextUpdateTimeout() {
  mutex.lock();

  ui->labelFwdValue->setText(QString::number(pickupStatus.fwdPower)+"W");
  ui->labelRefValue->setText(QString::number(pickupStatus.refPower)+"W");

  char str[10];
  sprintf((char *)str,"%i.%02i:1",(pickupStatus.vswr/100),(pickupStatus.vswr%100));

  ui->labelSWRValue->setText(QString(str));
  mutex.unlock();
}

void MainWindow::timerMetersUpdateTimeout() {
  mutex.lock();

  ui->progressBarFwd->setValue(pickupStatus.fwdPower);
  ui->progressBarRef->setValue(pickupStatus.refPower);

  mutex.unlock();
}

void MainWindow::actionDisconnectTriggered() {
  client.stopProcess();

  timerTextUpdate->stop();
  timerMetersUpdate->stop();
}

void MainWindow::setupConnections() {
  connect(ui->actionSettingsSetup, SIGNAL(triggered()), this, SLOT(menuActionSetupTriggered()));
  connect(ui->actionSettingsAbout, SIGNAL(triggered()), this, SLOT(menuActionSettingsAboutPressed()));
  connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
  connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));

  connect(timerTextUpdate, SIGNAL(timeout()), this, SLOT(timerTextUpdateTimeout()));
  connect(timerMetersUpdate, SIGNAL(timeout()), this, SLOT(timerMetersUpdateTimeout()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
  ui->setupUi(this);
  setupDialog = new SetupDialog();

  timerTextUpdate = new QTimer(this);
  timerMetersUpdate = new QTimer(this);

  setupConnections();

  pickupStatus.fwdPower = 0;
  pickupStatus.refPower = 0;

  rescaleMeters(PICKUP_TYPE_150W);
}

MainWindow::~MainWindow()
{
    delete ui;
}

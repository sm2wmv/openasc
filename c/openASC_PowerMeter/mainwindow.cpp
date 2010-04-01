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
		mutex.lock();
			if (message.data[0] != pickupStatus.type) {
				pickupStatus.type = message.data[0];
				rescaleMeters(pickupStatus.type);
		}

		pickupStatus.fwdPower = message.data[1] + (message.data[2]>>8);
		pickupStatus.refPower = message.data[3] + (message.data[4]>>8);
		pickupStatus.vswr = message.data[5] + (message.data[6]>>8);

		mutex.unlock();
	}
}

void MainWindow::menuActionSettingsAboutPressed() {
		AboutDialog *aboutDialog = new AboutDialog();
		aboutDialog->show();
}

void MainWindow::actionConnectTriggered() {
		client.start();
		client.receiveMsg();
}

void MainWindow::timerTextUpdateTimeout() {
		mutex.lock();
		ui->labelFwdValue->setText(QString::number(pickupStatus.fwdPower)+"W");
		ui->labelRefValue->setText(QString::number(pickupStatus.refPower)+"W");

		ui->labelSWRValue->setText("SWR: 1.21");
		mutex.unlock();
}

void MainWindow::timerMetersUpdateTimeout() {
		mutex.lock();

		ui->progressBarFwd->setValue(pickupStatus.fwdPower);
		ui->progressBarRef->setValue(pickupStatus.refPower);

		mutex.unlock();
}

void MainWindow::timerTestValueChangeTimeout() {
		mutex.lock();

		pickupStatus.fwdPower = rand() % 10000+2000;
		pickupStatus.refPower = pickupStatus.fwdPower / 10;

		mutex.unlock();
}

void MainWindow::actionDisconnectTriggered() {
		client.stopProcess();
}

void MainWindow::setupConnections() {
		connect(ui->actionSettingsSetup, SIGNAL(triggered()), this, SLOT(menuActionSetupTriggered()));
		connect(ui->actionSettingsAbout, SIGNAL(triggered()), this, SLOT(menuActionSettingsAboutPressed()));
		connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
		connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnectTriggered()));

		connect(timerTextUpdate, SIGNAL(timeout()), this, SLOT(timerTextUpdateTimeout()));
		connect(timerMetersUpdate, SIGNAL(timeout()), this, SLOT(timerMetersUpdateTimeout()));
		connect(timerTestValueChange, SIGNAL(timeout()), this, SLOT(timerTestValueChangeTimeout()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
		ui->setupUi(this);
		setupDialog = new SetupDialog();

		timerTextUpdate = new QTimer(this);
		timerMetersUpdate = new QTimer(this);

		//Just for testing
		timerTestValueChange = new QTimer(this);

		setupConnections();

		//Dummy variables, should be set from settings
		settings.textUpdateRate = 400;
		settings.metersUpdateRate = 100;

		rescaleMeters(PICKUP_TYPE_15000W);
		pickupStatus.fwdPower = 12500;
		pickupStatus.refPower = 250;

		//Just for testing
		timerTestValueChange->start(50);

		timerTextUpdate->start(settings.textUpdateRate);
		timerMetersUpdate->start(settings.metersUpdateRate);

		client.initClient(0x04, "/dev/ttyUSB0", this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

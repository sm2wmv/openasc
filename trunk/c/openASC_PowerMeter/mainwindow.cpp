#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::menuActionSetupTriggered() {
	setupDialog->show();
}

void MainWindow::recvBusMessage(BUS_MESSAGE message) {
	#ifdef DEBUG
		qDebug("----------------------------");
		qDebug("from_addr: \t\t0x%02X",message.from_addr);
		qDebug("to_addr: \t\t0x%02X",message.to_addr);
		qDebug("checksum: \t\t0x%02X",message.checksum);
		qDebug("flags: \t\t\t0x%02X",message.flags);
		qDebug("cmd: \t\t\t0x%02X",message.cmd);
		qDebug("length: \t\t0x%02X",message.length);

		for (int i=0;i<message.length;i++)
				qDebug("data[%i]: \t\t0x%02X",i,message.data[i]);

		qDebug(" ");
	#endif
}

void MainWindow::menuActionSettingsAboutPressed() {
		AboutDialog *aboutDialog = new AboutDialog();
		aboutDialog->show();
}

void MainWindow::actionConnectTriggered() {
		client.start();
		client.receiveMsg();
}

void MainWindow::actionDisconnectTriggered() {
		client.stopProcess();
}

void MainWindow::setupConnections() {
		connect(ui->actionSettingsSetup, SIGNAL(triggered()), this, SLOT(menuActionSetupTriggered()));
		connect(ui->actionSettingsAbout, SIGNAL(triggered()), this, SLOT(menuActionSettingsAboutPressed()));
		connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(actionConnectTriggered()));
		connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(actionDisonnectTriggered()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
		ui->setupUi(this);
		setupDialog = new SetupDialog();
		setupConnections();

		client.initClient(0x04, "/dev/ttyUSB0", this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

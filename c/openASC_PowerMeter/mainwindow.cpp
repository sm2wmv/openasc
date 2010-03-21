#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::menuActionSetupTriggered() {
	setupDialog->show();
}

void MainWindow::busParseMessage(BUS_MESSAGE message) {
		qDebug("PARSING MESSAGE");
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

		client.initClient(0x04, "/dev/ttyUSB0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::menuActionSetupTriggered() {
	setupDialog->show();
}

void MainWindow::menuActionSettingsAboutPressed() {
		AboutDialog *aboutDialog = new AboutDialog();
		aboutDialog->show();
}

void MainWindow::setupConnections() {
		connect(ui->actionSettingsSetup, SIGNAL(triggered()), this, SLOT(menuActionSetupTriggered()));
		connect(ui->actionSettingsAbout, SIGNAL(triggered()), this, SLOT(menuActionSettingsAboutPressed()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
		setupDialog = new SetupDialog();
		setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "setupdialog.h"
#include "ui_setupdialog.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "wmvbusclient.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
		void busParseMessage(BUS_MESSAGE message);
private:
		SetupDialog *setupDialog;
		Ui::MainWindowClass *ui;
		void setupConnections();
		WMVBusClient client;
private slots:
		void menuActionSetupTriggered();
		void menuActionSettingsAboutPressed();
		void actionConnectTriggered();
		void actionDisconnectTriggered();
};

#endif // MAINWINDOW_H

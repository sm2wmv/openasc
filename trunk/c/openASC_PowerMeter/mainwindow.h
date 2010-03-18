#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "setupdialog.h"
#include "ui_setupdialog.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

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

private:
		SetupDialog *setupDialog;
		Ui::MainWindowClass *ui;
		void setupConnections();

private slots:
		void menuActionSetupTriggered();
		void menuActionSettingsAboutPressed();
};

#endif // MAINWINDOW_H

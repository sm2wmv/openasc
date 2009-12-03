#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rotatordialog.h"

//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	RotatorDialog *rotatorWindow;
private:
protected:
private slots:
public slots:
	void WindowRotatorsClicked();
  void pushButtonTXComb1(bool state);
 
};
#endif






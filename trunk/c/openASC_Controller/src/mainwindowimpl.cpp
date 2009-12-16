#include <qmessagebox.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <QCleanlooksStyle>

#include "mainwindowimpl.h"
#include "generic.h"
	

void MainWindowImpl::WindowRotatorsClicked() {
	if (rotatorWindow->isVisible())
		rotatorWindow->hide();
	else
		rotatorWindow->show();
}
  
void MainWindowImpl::pushButtonTX1Clicked(bool state) {

}

void MainWindowImpl::pushButtonTX2Clicked(bool state) {

}

void MainWindowImpl::pushButtonTX3Clicked(bool state) {

}

void MainWindowImpl::pushButtonTX4Clicked(bool state) {

}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();
}

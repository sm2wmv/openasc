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
  
void MainWindowImpl::pushButtonTXComb1(bool state) {
	
}

void MainWindowImpl::pushButtonTXComb2(bool state) {

}

MainWindowImpl::MainWindowImpl ( QWidget * parent, Qt::WFlags f ) : QMainWindow ( parent, f ) {
	setupUi(this);

	rotatorWindow = new RotatorDialog(this);
	rotatorWindow->hide();
}

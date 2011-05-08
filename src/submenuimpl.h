#ifndef SUBMENUIMPL_H
#define SUBMENUIMPL_H
//
#include <QDialog>
#include "ui_submenu.h"
#include "bandclass.h"
//
class SubMenuImpl : public QDialog, public Ui::SubMenu
{
Q_OBJECT
public:
	SubMenuImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void setAntenna(BandClass& bandClass, int antIndex);
private:
	void setupConnections();
	int currAntIndex;
private slots:
	void radioButton4SQClicked(bool state);
	void radioButtonStackClicked(bool state);
	
	void spinBoxDirCountChanged(int currValue);
	void spinBoxStackPositionsValueChanged(int currValue);
	
	void pushButtonSavePressed();
	void pushButtonCancelPressed();
};
#endif






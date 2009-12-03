#ifndef ROTATORDIALOG_H
#define ROTATORDIALOG_H
//
#include <QDialog>
#include "ui_rotatorwindow.h"
#include "mapviewwidget.h"
//
class RotatorDialog : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	RotatorDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	MapViewWidget *mapLarge;
	MapViewWidget *mapSmall[5];
private:
	void loadBigMap(QString path);
	void loadSmallMap(QString path);
private slots:
};
#endif






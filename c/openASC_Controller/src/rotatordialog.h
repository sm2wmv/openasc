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
	MapViewWidget *map[4];
private:
	void loadMap(QString path);
	void mousePressEvent ( QMouseEvent * event );
private slots:
};
#endif






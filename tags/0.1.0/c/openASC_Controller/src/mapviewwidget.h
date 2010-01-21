#ifndef MAPVIEWWIDGET_H
#define MAPVIEWWIDGET_H
//
#include <QWidget>
#include "ui_mapviewwidget.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>

#include <QMouseEvent>

//2 degrees target beamwidth
#define TARGET_DIR_BEAMWIDTH	1
#define TARGET_DIR_BEAMWIDTH_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_COLOR	blue

//
class MapViewWidget : public QWidget, public Ui::Form
{
Q_OBJECT
public:
	MapViewWidget( QWidget * parent = 0, Qt::WFlags f = 0 );
	QPainter painter;
	void setImagePath(QString path);
	void setCurrentDir(int azimuthAngle, int beamWidth);
	void setTargetDir(int azimuthAngle);
	void setWidgetSize(int width, int height);
	void setMapTitle(QString title);
protected:
		void paintEvent(QPaintEvent *event);
		QString imagePath;
		int currAzimuthAngle;
		int currBeamWidth;
		int targetAzimuthAngle;
		int sizeWidth;
		int sizeHeight;
		QString mapTitle;
private:
		QImage image;
private slots:
};
#endif






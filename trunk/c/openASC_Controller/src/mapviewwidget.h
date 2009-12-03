#ifndef MAPVIEWWIDGET_H
#define MAPVIEWWIDGET_H
//
#include <QWidget>
#include "ui_mapviewwidget.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>

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
	void setCurrentDir(float azimuthAngle, float beamWidth);
	void setTargetDir(float azimuthAngle);
	void setWidgetSize(float width, float height);
	void setMapTitle(QString title);
protected:
		void paintEvent(QPaintEvent *event);
		QString imagePath;
		float currAzimuthAngle;
		float currBeamWidth;
		float targetAzimuthAngle;
		int sizeWidth;
		int sizeHeight;
		QString mapTitle;
private:
		QImage image;
private slots:
};
#endif






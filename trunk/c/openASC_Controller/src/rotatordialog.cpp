#include "rotatordialog.h"
#include "mapviewwidget.h"
#include <math.h>

#define PI 3.1456

void RotatorDialog::loadMap(QString path) {
	for (int i=0;i<4;i++) {
		map[i] = new MapViewWidget();
		map[i]->setWidgetSize(500,500);
	
		QVBoxLayout *mapLayout = new QVBoxLayout;
		mapLayout->addWidget(map[i]);
			
		if (i == 0)
			frameMap1->setLayout(mapLayout);
		if (i == 1)
			frameMap2->setLayout(mapLayout);
		if (i == 2)
			frameMap3->setLayout(mapLayout);
		if (i == 3)
			frameMap3->setLayout(mapLayout);		
		
		map[i]->setImagePath(path);
	}
}

void RotatorDialog::mousePressEvent ( QMouseEvent * event ) {
	printf("X: %i Y: %i\n",event->x(),event->y());
	
	if ((event->x() >= 124) && (event->y() >= 8) && (event->x() <= (620)) && (event->y() <= 506)) {
		double mapX = abs(372 - event->x());
		double mapY = 256 - event->y();
		
		if ((372 - event->x()) < 0)
			map[0]->setCurrentDir(90-atan(mapY/mapX)*(180/PI),50);
		else
			map[0]->setCurrentDir(270+atan(mapY/mapX)*(180/PI),50);
	}
}

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WFlags f) : QDialog(parent, f) {
	setupUi(this);
	
	this->resize(1280,1024);
	
	loadMap("../src/maps/map.jpg");
	
	map[0]->setMapTitle("6/6/6el");
	map[1]->setMapTitle("5/5el");
	map[2]->setMapTitle("4el");
	
	map[0]->setCurrentDir(325,50);
	map[0]->setTargetDir(325);
	map[1]->setCurrentDir(120,60);
	map[1]->setTargetDir(45);
	map[2]->setCurrentDir(180,75);
	map[2]->setTargetDir(180);	
}
//


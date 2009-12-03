#include "rotatordialog.h"
#include "mapviewwidget.h"

void RotatorDialog::loadBigMap(QString path) {
	mapLarge = new MapViewWidget();
	mapLarge->setWidgetSize(600,600);
	
	QVBoxLayout *mapLargeLayout = new QVBoxLayout;
	mapLargeLayout->addWidget(mapLarge);
	
	frameMapLarge->setLayout(mapLargeLayout);	
	
	mapLarge->setImagePath(path);
	
	mapLarge->setMapTitle("6/6/6el");
	
	mapLarge->setCurrentDir(225,45);
	mapLarge->repaint();
	
	mapLarge->setCurrentDir(305,65);
	mapLarge->setTargetDir(45);
}

void RotatorDialog::loadSmallMap(QString path) {
	for (int i=0;i<5;i++) {
		mapSmall[i] = new MapViewWidget();
		mapSmall[i]->setWidgetSize(300,300);
	
		QVBoxLayout *mapSmallLayout = new QVBoxLayout;
		mapSmallLayout->addWidget(mapSmall[i]);
			
		if (i == 0)
			frameMapSmall1->setLayout(mapSmallLayout);
		if (i == 1)
			frameMapSmall2->setLayout(mapSmallLayout);
		
		mapSmall[i]->setImagePath(path);
	}
	
	mapSmall[0]->setCurrentDir(45,60);
	mapSmall[0]->setTargetDir(45);
	mapSmall[1]->setCurrentDir(180,70);
	mapSmall[1]->setTargetDir(180);
}

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	
	loadBigMap("../src/maps/kp04hm_large.jpg");
	loadSmallMap("../src/maps/kp04hm_small.jpg");
	
	mapSmall[0]->setMapTitle("4/4el");
	mapSmall[1]->setMapTitle("4el @EU");
}
//


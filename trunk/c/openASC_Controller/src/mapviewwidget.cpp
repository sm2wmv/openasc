#include "mapviewwidget.h"

//

void MapViewWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	painter.setPen(Qt::SolidLine);
	painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_COLOR);
	painter.setBrush(Qt::FDiagPattern);
	//painter.setBrush(Qt::Dense7Pattern);

	image = QImage(imagePath);
	image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);
	
	painter.drawImage(0 , 0 , image);
	
	float rectWidth = sizeWidth - (sizeWidth * 0.08);
	float rectHeight = sizeHeight - (sizeHeight * 0.08);
	
	QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth, rectHeight);
	painter.drawPie(rectangle,(360-(-90+currAzimuthAngle + currBeamWidth/2))*16,currBeamWidth*16);

	painter.drawText(10,20,mapTitle);
	painter.drawText(10,35,QString::number(currAzimuthAngle)+176);
	
        painter.drawText(250,280,"Stopped");
	
	if (targetAzimuthAngle != currAzimuthAngle) {
		painter.setPen(Qt::SolidLine);
		painter.setPen(Qt::TARGET_DIR_BEAMWIDTH_COLOR);
		painter.setBrush(Qt::TARGET_DIR_BEAMWIDTH_COLOR);
		
		QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth, rectHeight);
		painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle + TARGET_DIR_BEAMWIDTH/2))*16,TARGET_DIR_BEAMWIDTH*16);
	}
}

void MapViewWidget::setCurrentDir(int azimuthAngle, int beamWidth) {
	currAzimuthAngle = azimuthAngle;
	currBeamWidth = beamWidth;
	
        painter.drawText(10,35,QString::number(currAzimuthAngle)+176);
	
	repaint();
}

void MapViewWidget::setTargetDir(int azimuthAngle) {
	targetAzimuthAngle = azimuthAngle;
	
	repaint();
}

void MapViewWidget::setWidgetSize(int width, int height) {
	sizeWidth = width;
	sizeHeight = height;
}

void MapViewWidget::setMapTitle(QString title) {
	mapTitle = title;
}

MapViewWidget::MapViewWidget( QWidget * parent, Qt::WFlags f) 
	: QWidget(parent, f) {
	currAzimuthAngle = 0;
	currBeamWidth = 0;
	targetAzimuthAngle = 0;
	
	setupUi(this);
}

void MapViewWidget::setImagePath(QString path) {
	imagePath = path;
/*	image.load(path);
	
	//image.setPixel(10,10,255);
	imageLabel->setPixmap(QPixmap::fromImage(image));	*/
}

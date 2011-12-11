#include "rotatordialog.h"
#include <math.h>

#define PI 3.1415

void RotatorDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //painter.setBrush(Qt::Dense7Pattern);

    image = QImage(imagePath);
    image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);

    painter.drawImage(0 , 0 , image);

		float rectWidth = sizeWidth - (sizeWidth * 0.06);
		float rectHeight = sizeHeight - (sizeHeight * 0.06);
		QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth-2, rectHeight-2);

    for (int i=0;i<4;i++) {
				if(antBeamWidth[i] > 0) {
            if (i==0) {
                painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
                painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR),Qt::FDiagPattern));
            }
            else if (i==1) {
                painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
                painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR),Qt::FDiagPattern));
            }
            else if (i==2) {
                painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
                painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR),Qt::FDiagPattern));
            }
            else if (i==3) {
                painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
                painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR),Qt::FDiagPattern));
            }

						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
        }
    }
}

void RotatorDialog::mousePressEvent ( QMouseEvent * event ) {	
    if ((event->x() >= 8) && (event->y() >= 8) && (event->x() <= (600)) && (event->y() <= 600)) {
				double mapX = abs(300 - event->x());
				double mapY = 300 - event->y();

				if ((300 - event->x()) < 0)
            setTargetDir(currAntIndex,90-atan(mapY/mapX)*(180/PI));
        else
            setTargetDir(currAntIndex,270+atan(mapY/mapX)*(180/PI));
    }
}

void RotatorDialog::setTargetDir(int antIndex, int targetAngle) {
	targetAzimuthAngle[antIndex] = targetAngle;

	qDebug("TARGET_DIR: %d",targetAngle);

	rotationEventStatus = 1;
}

char RotatorDialog::getRotationEventStatus(void) {
	unsigned char temp = rotationEventStatus;
	rotationEventStatus = 0;

	return(temp);
}

int RotatorDialog::getTargetDir(unsigned char antIndex) {
	return(targetAzimuthAngle[antIndex]);
}

int RotatorDialog::getActiveAntenna(){
	return(currAntIndex);
}

void RotatorDialog::setRotatorFlag(unsigned char antIndex, unsigned char flags) {
	if (flags & (1<<1)) {//No rotation
		if (antIndex == 0)
			labelAnt1Status->setText("Stopped");
		else if (antIndex == 1)
			labelAnt2Status->setText("Stopped");
		else if (antIndex == 1)
			labelAnt3Status->setText("Stopped");
		else if (antIndex == 1)
			labelAnt4Status->setText("Stopped");
	}

	if (flags & (1<<3)) {
		if (antIndex == 0)
			labelAnt1Status->setText("Rotating CW");
		else if (antIndex == 1)
			labelAnt2Status->setText("Rotating CW");
		else if (antIndex == 1)
			labelAnt3Status->setText("Rotating CW");
		else if (antIndex == 1)
			labelAnt4Status->setText("Rotating CW");
	}

	if (flags & (1<<4)) {
		if (antIndex == 0)
			labelAnt1Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt2Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt3Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt4Status->setText("Rotating CCW");
	}

}

void RotatorDialog::setAntName(int antIndex, QString name) {
    antName[antIndex] = name;
}

void RotatorDialog::loadBand(int bandIndex) {
	switch(bandIndex) {
		case 0: bandName="None";
			break;
		case 1: bandName = "160m";
			break;
		case 2: bandName = "80m";
			break;
		case 3: bandName = "40m";
			break;
		case 4: bandName = "30m";
			break;
		case 5: bandName = "20m";
			break;
		case 6: bandName = "17m";
			break;
		case 7: bandName = "15m";
			break;
		case 8: bandName = "12m";
			break;
		case 9: bandName = "10m";
			break;
	}

	if (bandIndex == 0) {
		for (int i=0;i<4;i++) {
			antName[i] = "";
			antExist[i] = false;
			antBeamWidth[i] = 0;
			antVerticalArray[i] = false;
		}
	}
	else {
		QSettings settings("rotator_settings.ini",QSettings::IniFormat,0);

		settings.beginGroup(bandName);

		int size = settings.beginReadArray("Ant");

		for (int i=0;i<size;i++) {
			settings.setArrayIndex(i);

			antName[i] = settings.value("Name").toString();
			antExist[i] = settings.value("Exist").toBool();
			antHasRotor[i] = settings.value("HasRotor").toBool();
			antBeamWidth[i] = settings.value("BeamWidth").toInt();
			antVerticalArray[i] = settings.value("VerticalArray").toBool();
		}

		settings.endArray();

		settings.endGroup();

		groupBoxAnt1->setVisible(false);
		groupBoxAnt2->setVisible(false);
		groupBoxAnt3->setVisible(false);
		groupBoxAnt4->setVisible(false);

		if (antExist[0])
			groupBoxAnt1->setVisible(true);
		if (antExist[1])
			groupBoxAnt2->setVisible(true);
		if (antExist[2])
			groupBoxAnt3->setVisible(true);
		if (antExist[3])
			groupBoxAnt4->setVisible(true);

		labelAnt1Title->setText(antName[0]);
		labelAnt2Title->setText(antName[1]);
		labelAnt3Title->setText(antName[2]);
		labelAnt4Title->setText(antName[3]);


	}
}

void RotatorDialog::setRotatorAngle(int antIndex, int angle) {
	if (antIndex < 4) {
		currAzimuthAngle[antIndex] = angle;

		if (antIndex == 0)
			labelAnt1Dir->setText(QString::number(angle)+'°');
		else if (antIndex == 1)
			labelAnt2Dir->setText(QString::number(angle)+'°');
		else if (antIndex == 2)
			labelAnt3Dir->setText(QString::number(angle)+'°');
		else if (antIndex == 3)
			labelAnt4Dir->setText(QString::number(angle)+'°');

		repaint();
	}
}

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WFlags f) : QDialog(parent, f) {
    setupUi(this);

    this->resize(800,600);
    imagePath = "../src/maps/map.jpg";

    sizeWidth = 600;
    sizeHeight = 600;

    currAzimuthAngle[0] = 305;
    targetAzimuthAngle[0] = 305;

    currAzimuthAngle[1] = 50;
    targetAzimuthAngle[1] = 60;

    currAzimuthAngle[2] = 180;
    targetAzimuthAngle[2] = 185;

    currAzimuthAngle[3] = 0;
    targetAzimuthAngle[3] = 0;

		rotationEventStatus = 0;

    currAntIndex = 0;

    QPalette plt;
    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
    groupBoxAnt1->setPalette(plt);

    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
    groupBoxAnt2->setPalette(plt);

    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
    groupBoxAnt3->setPalette(plt);
}
//

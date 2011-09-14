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

						if (antHasRotor[i])
							painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
        }
    }
}

void RotatorDialog::mousePressEvent ( QMouseEvent * event ) {
	if ((event->x() >= 8) && (event->y() >= 8) && (event->x() <= (600)) && (event->y() <= 600)) {
			double mapX = abs(300 - event->x());
			double mapY = 300 - event->y();

			if (currAntIndex != -1) {
				if ((300 - event->x()) < 0)
						setTargetDir(currAntIndex,90-atan(mapY/mapX)*(180/PI));
				else
						setTargetDir(currAntIndex,270+atan(mapY/mapX)*(180/PI));
			}
	}
	else {
		if ((event->x() >= 620) && (event->y() >= 0) && (event->x() <= (800)) && (event->y() <= 100)) {
			currAntIndex = 0;
		}
		else if ((event->x() >= 620) && (event->y() >= 100) && (event->x() <= (800)) && (event->y() <= 200)) {
			currAntIndex = 1;
		}
		else if ((event->x() >= 620) && (event->y() >= 200) && (event->x() <= (800)) && (event->y() <= 300)) {
			currAntIndex = 2;
		}
		else if ((event->x() >= 620) && (event->y() >= 300) && (event->x() <= (800)) && (event->y() <= 400)) {
			currAntIndex = 3;
		}
		else
			currAntIndex = -1;
	}

/*	QString str;

	str.append(QString("%1").arg(event->x()));

	labelX->setText(str);

	str.clear();
	str.append(QString("%1").arg(event->y()));
	labelY->setText(str);*/
}

void RotatorDialog::setTargetDir(int antIndex, int targetAngle) {
		targetAzimuthAngle[antIndex] = targetAngle;

		if (comm->isOpen()) {
			char msg[13];

			if (antIndex == 0) {
				sprintf(msg, "setdir 1 %i\r",targetAngle);
				QString var((char *)msg);

				comm->addTXMessage(var);
			}
		}

		currAntIndex = -1;
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

		labelAnt1Title->setVisible(false);
		labelAnt1Dir->setVisible(false);
		labelAnt1Status->setVisible(false);

		labelAnt2Title->setVisible(false);
		labelAnt2Dir->setVisible(false);
		labelAnt2Status->setVisible(false);

		labelAnt3Title->setVisible(false);
		labelAnt3Dir->setVisible(false);
		labelAnt3Status->setVisible(false);

		labelAnt4Title->setVisible(false);
		labelAnt4Dir->setVisible(false);
		labelAnt4Status->setVisible(false);


		if (antExist[0]) {
			labelAnt1Title->setVisible(true);

			if (antHasRotor[0]) {
				labelAnt1Dir->setVisible(true);
				labelAnt1Status->setVisible(true);
			}
		}

		if (antExist[1]) {
			labelAnt2Title->setVisible(true);

			if (antHasRotor[1]) {
				labelAnt2Dir->setVisible(true);
				labelAnt2Status->setVisible(true);
			}
		}

		if (antExist[2]) {
			labelAnt3Title->setVisible(true);

			if (antHasRotor[2]) {
				labelAnt3Dir->setVisible(true);
				labelAnt3Status->setVisible(true);
			}
		}

		if (antExist[3]) {
			labelAnt4Title->setVisible(true);

			if (antHasRotor[3]) {
				labelAnt4Dir->setVisible(true);
				labelAnt4Status->setVisible(true);
			}
		}

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

void RotatorDialog::setCommPtr(CommClass* commPtr) {
	comm = commPtr;
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

		currAntIndex = -1;

    QPalette plt;
    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
		labelAnt1Title->setPalette(plt);
		labelAnt1Dir->setPalette(plt);
		labelAnt1Status->setPalette(plt);

    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
		labelAnt2Title->setPalette(plt);
		labelAnt2Dir->setPalette(plt);
		labelAnt2Status->setPalette(plt);

    plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
		labelAnt3Title->setPalette(plt);
		labelAnt3Dir->setPalette(plt);
		labelAnt3Status->setPalette(plt);

		plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
		labelAnt4Title->setPalette(plt);
		labelAnt4Dir->setPalette(plt);
		labelAnt4Status->setPalette(plt);
}
//

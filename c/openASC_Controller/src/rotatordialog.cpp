#include "rotatordialog.h"
#include <math.h>

#define PI 3.1415

void RotatorDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //painter.setBrush(Qt::Dense7Pattern);

    image = QImage(imagePath);
    image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);

    painter.drawImage(0 , 0 , image);

    float rectWidth = sizeWidth - (sizeWidth * 0.08);
    float rectHeight = sizeHeight - (sizeHeight * 0.08);
    QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth, rectHeight);

    for (int i=0;i<4;i++) {
        if(hasRotor[i] == true) {
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

            painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + currBeamWidth[i]/2))*16,currBeamWidth[i]*16);
        }
    }
}

void RotatorDialog::mousePressEvent ( QMouseEvent * event ) {	
    if ((event->x() >= 8) && (event->y() >= 8) && (event->x() <= (600)) && (event->y() <= 600)) {
        double mapX = abs(300 - event->x());
        double mapY = 300 - event->y();

        if ((256 - event->x()) < 0)
            setTargetDir(currAntIndex,90-atan(mapY/mapX)*(180/PI));
        else
            setTargetDir(currAntIndex,270+atan(mapY/mapX)*(180/PI));
    }
}

void RotatorDialog::setTargetDir(int antIndex, int targetAngle) {
    targetAzimuthAngle[antIndex] = targetAngle;
}

void RotatorDialog::setAntName(int antIndex, QString name) {
    antName[antIndex] = name;


}

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WFlags f) : QDialog(parent, f) {
    setupUi(this);

    this->resize(800,600);
    imagePath = "../src/maps/map.jpg";

    sizeWidth = 600;
    sizeHeight = 600;

    currAzimuthAngle[0] = 305;
    currBeamWidth[0] = 10;
    targetAzimuthAngle[0] = 305;

    currAzimuthAngle[1] = 50;
    currBeamWidth[1] = 65;
    targetAzimuthAngle[1] = 60;

    currAzimuthAngle[2] = 180;
    currBeamWidth[2] = 65;
    targetAzimuthAngle[2] = 185;

    currAzimuthAngle[3] = 0;
    currBeamWidth[3] = 0;
    targetAzimuthAngle[3] = 0;

    hasRotor[0] = true;
    hasRotor[1] = true;
    hasRotor[2] = true;
    hasRotor[3] = false;

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

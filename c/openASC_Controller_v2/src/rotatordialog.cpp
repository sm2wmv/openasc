#include "rotatordialog.h"
#include <math.h>
#include <QtMath>
#include <QtDebug>
#include <QGraphicsDropShadowEffect>

#include "mainwindowimpl.h"

#define PI 3.1415

void RotatorDialog::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	image = QImage(imagePath);
	image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);

	painter.drawImage(0 , 0 , image);

	float rectWidth = sizeWidth - (sizeWidth * 0.06);
	float rectHeight = sizeHeight - (sizeHeight * 0.06);
	QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth-2, rectHeight-2);


    QPen linepen(Qt::red);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(5);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(linepen);
    painter.drawPoint(QPoint(sizeWidth/2-2,sizeHeight/2-2));

    QSize labelSize;

	for (int i=0;i<4;i++) {
		if ((antExist[i] == true) && (antBeamWidth[i] > 0)) {
			if (i==0) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR),Qt::FDiagPattern));

                    labelSize = labelMapNameAnt1->size();

                    if (antVerticalArray[i]) {
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt1->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),labelSize.width(),labelSize.height());
                    }
                    else {
                        labelMapNameAnt1->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),labelSize.width(),labelSize.height());

                        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                    }

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if ((rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CW)) || (rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CCW))) {
                        painter.setBrush(QBrush(QColor(Qt::TARGET_DIR_BEAMWIDTH_A1_COLOR),Qt::SolidPattern));
						painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle[i] + 0.5))*16,0.5*16);
					}

			}
			else if (i==1) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR),Qt::FDiagPattern));

                    labelSize = labelMapNameAnt2->size();

                    if (antVerticalArray[i]) {
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt2->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),labelSize.width(),labelSize.height());
                    }
                    else {
                        labelMapNameAnt2->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),labelSize.width(),labelSize.height());
                        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                    }

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

                    if ((rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CW)) || (rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CCW))) {
                        painter.setBrush(QBrush(QColor(Qt::TARGET_DIR_BEAMWIDTH_A2_COLOR),Qt::SolidPattern));
                        painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle[i] + 0.5))*16,0.5*16);
                    }
			}
			else if (i==2) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR),Qt::FDiagPattern));

                    labelSize = labelMapNameAnt3->size();

                    if (antVerticalArray[i]) {
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt3->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),labelSize.width(),labelSize.height());
                    }
                    else {
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt3->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),labelSize.width(),labelSize.height());
                    }

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

                    if ((rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CW)) || (rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CCW))) {
                        painter.setBrush(QBrush(QColor(Qt::TARGET_DIR_BEAMWIDTH_A3_COLOR),Qt::SolidPattern));
                        painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle[i] + 0.5))*16,0.5*16);
                    }
			}
			else if (i==3) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR),Qt::FDiagPattern));

                    labelSize = labelMapNameAnt4->size();

                    if (antVerticalArray[i]) {
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt4->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]-270))),labelSize.width(),labelSize.height());
                    }
                    else {
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
                        labelMapNameAnt4->setGeometry(sizeWidth/2 - labelSize.width()/2 - lengthMapLabel * qCos(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),sizeHeight/2-labelSize.height()/2-lengthMapLabel*qSin(qDegreesToRadians((float)(currAzimuthAngle[i]-270))),labelSize.width(),labelSize.height());
                    }

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

                    if ((rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CW)) || (rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CCW))) {
                        painter.setBrush(QBrush(QColor(Qt::TARGET_DIR_BEAMWIDTH_A4_COLOR),Qt::SolidPattern));
                        painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle[i] + 0.5))*16,0.5*16);
                    }
			}
		}
	}
}

void RotatorDialog::setRotatorStatusText(unsigned char index, unsigned char status) {
	rotatorStatus[index] = status;

	if (index == 0) {
        if (antHasRotor[index]) {
            if (status & (1<<FLAG_ROTATOR_ROTATION_CCW))
                labelAnt1Status->setText("Rotating CCW");
            else if (status & (1<<FLAG_ROTATOR_ROTATION_CW))
                labelAnt1Status->setText("Rotating CW");
            else {
                if (antVerticalArray[0])
                    labelAnt1Status->setText("");
                else
                    labelAnt1Status->setText("Stopped");
            }
        }
        else
            labelAnt1Status->setText("");
	}
	else if (index == 1) {
        if (antHasRotor[index]) {
            if (status & (1<<FLAG_ROTATOR_ROTATION_CCW))
                labelAnt2Status->setText("Rotating CCW");
            else if (status & (1<<FLAG_ROTATOR_ROTATION_CW))
                labelAnt2Status->setText("Rotating CW");
            else {
                if (antVerticalArray[1])
                    labelAnt2Status->setText("");
                else
                    labelAnt2Status->setText("Stopped");
            }
        }
        else
            labelAnt2Status->setText("");
	}
	else if (index == 2) {
        if (antHasRotor[index]) {
            if (status & (1<<FLAG_ROTATOR_ROTATION_CCW))
                labelAnt3Status->setText("Rotating CCW");
            else if (status & (1<<FLAG_ROTATOR_ROTATION_CW))
                labelAnt3Status->setText("Rotating CW");
            else {
                if (antVerticalArray[2])
                    labelAnt3Status->setText("");
                else
                    labelAnt3Status->setText("Stopped");

            }
        }
        else
            labelAnt3Status->setText("");

	}
	else if (index == 3) {
        if (antHasRotor[index]) {
            if (status & (1<<FLAG_ROTATOR_ROTATION_CCW))
                labelAnt4Status->setText("Rotating CCW");
            else if (status & (1<<FLAG_ROTATOR_ROTATION_CW))
                labelAnt4Status->setText("Rotating CW");
            else {
                if (antVerticalArray[3])
                    labelAnt4Status->setText("");
                else
                    labelAnt4Status->setText("Stopped");

            }
        }
        else
            labelAnt4Status->setText("");
	}
}

void RotatorDialog::mousePressEvent ( QMouseEvent * event ) {	
    if (currAntIndex != -1) {
      if ((event->x() >= 8) && (event->y() >= 8) && (event->x() <= (sizeWidth)) && (event->y() <= sizeHeight)) {
          double mapX = abs(sizeWidth/2 - event->x());
          double mapY = sizeHeight/2 - event->y();

          if ((sizeWidth/2 - event->x()) < 0)
                        setTargetDir(currAntIndex,90-atan(mapY/mapX)*(180/PI));
                else
                        setTargetDir(currAntIndex,270+atan(mapY/mapX)*(180/PI));
        }
    }
}

void RotatorDialog::setTargetDir(int antIndex, int targetAngle) {
	if (antIndex >= 0) {
		targetAzimuthAngle[antIndex] = targetAngle;

		if (TCPComm->isConnected()) {
			if (antHasRotor[antIndex]) {
				QByteArray temp;
				temp.append(antIndex);
				temp.append(targetAngle >> 8);
				temp.append(targetAngle & 0xFF);

				TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_SET_HEADING,temp.length(),temp);
			}
			else if (antVerticalArray[antIndex]) {
				for (unsigned char i=0;i<verticalArrayNrDirs[antIndex];i++) {
					if ((abs(verticalArrayDirAngle[antIndex][i]-targetAngle) < antBeamWidth[antIndex]/2)) {
						QByteArray temp;
						temp.append(antIndex);
						temp.append(i);
						TCPComm->addTXMessage(REMOTE_COMMAND_SET_ARRAY_DIR,temp.length(),temp);
						break;
					}
				}
			}
		}

		repaint();
		rotationEventStatus = 1;
	}
}

int RotatorDialog::getTargetDir(unsigned char antIndex) {
	return(targetAzimuthAngle[antIndex]);
}

void RotatorDialog::setRotatorFlag(unsigned char antIndex, unsigned char flags) {
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
	else if (flags & (1<<4)) {
		if (antIndex == 0)
			labelAnt1Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt2Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt3Status->setText("Rotating CCW");
		else if (antIndex == 1)
			labelAnt4Status->setText("Rotating CCW");
	}
	else {
		if (antIndex == 0) {
			if (antVerticalArray[0])
				labelAnt1Status->setText("");
			else
				labelAnt1Status->setText("Stopped");
		}
		else if (antIndex == 1) {
			if (antVerticalArray[1])
				labelAnt2Status->setText("");
			else
				labelAnt2Status->setText("Stopped");
		}
		else if (antIndex == 2) {
			if (antVerticalArray[2])
				labelAnt3Status->setText("");
			else
				labelAnt3Status->setText("Stopped");
		}
		else if (antIndex == 3) {
			if (antVerticalArray[3])
				labelAnt4Status->setText("");
			else
				labelAnt4Status->setText("Stopped");
		}
	}
}

void RotatorDialog::setAntName(int antIndex, QString name) {
    antName[antIndex] = name;
}

void RotatorDialog::setStatusPresetButtons() {
	bool temp_val = false;

	if (currAntIndex != -1) {
		if (antHasRotor[currAntIndex])
			temp_val = true;
	}
	else
		temp_val = false;

	pushButtonPreset1->setEnabled(temp_val);
	pushButtonPreset2->setEnabled(temp_val);
	pushButtonPreset3->setEnabled(temp_val);
	pushButtonPreset4->setEnabled(temp_val);
	pushButtonPreset5->setEnabled(temp_val);
	pushButtonSTOP->setEnabled(temp_val);

	pushButtonRotateCCW->setEnabled(temp_val);
	pushButtonRotateCW->setEnabled(temp_val);
}

void RotatorDialog::loadBand(int bandIndex) {
	currAntIndex = -1;

  pushButtonAnt1->setChecked(false);
  pushButtonAnt2->setChecked(false);
  pushButtonAnt3->setChecked(false);
  pushButtonAnt4->setChecked(false);

  labelAnt1Dir->setText("");
  labelAnt2Dir->setText("");
  labelAnt3Dir->setText("");
  labelAnt4Dir->setText("");

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
		default:
			bandName = "None";
			break;
	}

	currAzimuthAngle[0] = 0;
	currAzimuthAngle[1] = 0;
	currAzimuthAngle[2] = 0;
	currAzimuthAngle[3] = 0;

	if (bandIndex == 0) {
		for (int i=0;i<4;i++) {
			antName[i] = "";
			antExist[i] = false;
			antBeamWidth[i] = 0;
			antVerticalArray[i] = false;
		}

        antCount = 0;

        frameAnt1->setVisible(false);
        frameAnt2->setVisible(false);
        frameAnt3->setVisible(false);
        frameAnt4->setVisible(false);

		pushButtonAnt1->setVisible(false);
		pushButtonAnt2->setVisible(false);
		pushButtonAnt3->setVisible(false);
		pushButtonAnt4->setVisible(false);

		pushButtonRotateCCW->setEnabled(false);
		pushButtonRotateCW->setEnabled(false);

		pushButtonPreset1->setEnabled(false);
		pushButtonPreset2->setEnabled(false);
		pushButtonPreset3->setEnabled(false);
		pushButtonPreset4->setEnabled(false);
		pushButtonPreset5->setEnabled(false);
		pushButtonSTOP->setEnabled(false);
	}
	else {
		QSettings settings("rotator_settings.ini",QSettings::IniFormat,0);

		settings.beginGroup(bandName);

        antCount = 0;

		int size = settings.beginReadArray("Ant");

		for (int i=0;i<size;i++) {
			settings.setArrayIndex(i);

			antName[i] = settings.value("Name").toString();
			antExist[i] = settings.value("Exist").toBool();
			antHasRotor[i] = settings.value("HasRotor").toBool();
			antBeamWidth[i] = settings.value("BeamWidth").toInt();
			antVerticalArray[i] = settings.value("VerticalArray").toBool();
			antFixedAngle[i] = settings.value("AntennaFixedAngle").toInt();
			antFixed[i] = settings.value("AntennaFixed").toBool();
			antBiDirectional[i] = settings.value("AntennaBiDirectional").toBool();

            if (antExist[i])
                antCount++;

			if (antVerticalArray[i]) {
				verticalArrayNrDirs[i] = settings.value("VerticalArrayNrDirs").toInt();

				verticalArrayDirAngle[i][0] = settings.value("VerticalArrayDir1Angle").toInt();
				verticalArrayDirAngle[i][1] = settings.value("VerticalArrayDir2Angle").toInt();
				verticalArrayDirAngle[i][2] = settings.value("VerticalArrayDir3Angle").toInt();
				verticalArrayDirAngle[i][3] = settings.value("VerticalArrayDir4Angle").toInt();

				verticalArrayDirName[i][0] = settings.value("VerticalArrayDir1Name").toString();
				verticalArrayDirName[i][1] = settings.value("VerticalArrayDir2Name").toString();
				verticalArrayDirName[i][2] = settings.value("VerticalArrayDir3Name").toString();
				verticalArrayDirName[i][3] = settings.value("VerticalArrayDir4Name").toString();
			}

			if (antFixed[i]) {
				setRotatorAngle(i,antFixedAngle[i]);
			}
		}

		settings.endArray();

		settings.endGroup();

		settings.beginGroup("PresetButtons");

		pushButtonPreset1->setText(settings.value("Preset1Text").toString());
		pushButtonPreset2->setText(settings.value("Preset2Text").toString());
		pushButtonPreset3->setText(settings.value("Preset3Text").toString());
		pushButtonPreset4->setText(settings.value("Preset4Text").toString());
		pushButtonPreset5->setText(settings.value("Preset5Text").toString());

		presetButtonValue[0] = settings.value("Preset1Angle").toInt();
		presetButtonValue[1] = settings.value("Preset2Angle").toInt();
		presetButtonValue[2] = settings.value("Preset3Angle").toInt();
		presetButtonValue[3] = settings.value("Preset4Angle").toInt();
		presetButtonValue[4] = settings.value("Preset5Angle").toInt();

		settings.endGroup();

		setStatusPresetButtons();

        frameAnt1->setVisible(false);
        frameAnt2->setVisible(false);
        frameAnt3->setVisible(false);
        frameAnt4->setVisible(false);

		pushButtonAnt1->setVisible(false);
		pushButtonAnt2->setVisible(false);
		pushButtonAnt3->setVisible(false);
		pushButtonAnt4->setVisible(false);

        labelMapNameAnt1->setVisible(false);
        labelMapNameAnt2->setVisible(false);
        labelMapNameAnt3->setVisible(false);
        labelMapNameAnt4->setVisible(false);

		if (antExist[0]) {
            frameAnt1->setVisible(true);
			pushButtonAnt1->setVisible(true);
            labelMapNameAnt1->setVisible(true);

			if ((antHasRotor[0]) || (antVerticalArray[0])) {
				pushButtonAnt1->setEnabled(true);
                pushButtonAnt1->setChecked(false);
            }
			else {
				pushButtonAnt1->setEnabled(false);
				pushButtonAnt1->setChecked(false);
			}
		}

		if (antExist[1]) {
            frameAnt2->setVisible(true);
			pushButtonAnt2->setVisible(true);
            labelMapNameAnt2->setVisible(true);

			if ((antHasRotor[1]) || (antVerticalArray[1])) {
				pushButtonAnt2->setEnabled(true);
			}
			else {
				pushButtonAnt2->setEnabled(false);
                pushButtonAnt2->setChecked(false);
			}
		}

		if (antExist[2]) {
            frameAnt3->setVisible(true);
			pushButtonAnt3->setVisible(true);
            labelMapNameAnt3->setVisible(true);

			if ((antHasRotor[2]) || (antVerticalArray[2])) {
				pushButtonAnt3->setEnabled(true);
			}
            else {
				pushButtonAnt3->setEnabled(false);
                pushButtonAnt3->setChecked(false);
            }
		}

		if (antExist[3]) {
            frameAnt4->setVisible(true);
			pushButtonAnt4->setVisible(true);
            labelMapNameAnt4->setVisible(true);

			if ((antHasRotor[3]) || (antVerticalArray[3])) {
				pushButtonAnt4->setEnabled(true);
			}
            else {
                pushButtonAnt4->setEnabled(false);
                pushButtonAnt4->setChecked(false);
            }
        }

        setStatusPresetButtons();

		labelAnt1Title->setText(antName[0]);
		labelAnt2Title->setText(antName[1]);
		labelAnt3Title->setText(antName[2]);
		labelAnt4Title->setText(antName[3]);

        labelMapNameAnt1->setText(antName[0]);
        labelMapNameAnt1->adjustSize();

        labelMapNameAnt2->setText(antName[1]);
        labelMapNameAnt2->adjustSize();

        labelMapNameAnt3->setText(antName[2]);
        labelMapNameAnt3->adjustSize();

        labelMapNameAnt4->setText(antName[3]);
        labelMapNameAnt4->adjustSize();

        qDebug() << labelMapNameAnt1->size();

		if (antFixed[0]) {
			labelAnt1Status->setText("Fixed");
			pushButtonAnt1->setVisible(false);
		}
		else if (antVerticalArray[0])
			labelAnt1Status->setText("");
        else if (antHasRotor[0] == false)
            labelAnt1Status->setText("Stopped");
        else
            labelAnt1Status->setText("");

		if (antFixed[1]) {
			labelAnt2Status->setText("Fixed");
			pushButtonAnt2->setVisible(false);
		}
		else if (antVerticalArray[1])
			labelAnt2Status->setText("");
        else if (antHasRotor[1] == false)
            labelAnt2Status->setText("Stopped");
        else
            labelAnt2Status->setText("");

		if (antFixed[2]) {
			labelAnt3Status->setText("Fixed");
			pushButtonAnt3->setVisible(false);
		}
		else if (antVerticalArray[2])
			labelAnt3Status->setText("");
        else if (antHasRotor[2] == false)
            labelAnt3Status->setText("Stopped");
        else
            labelAnt3Status->setText("");

		if (antFixed[3]) {
			labelAnt4Status->setText("Fixed");
			pushButtonAnt4->setVisible(false);
		}
		else if (antVerticalArray[3])
			labelAnt4Status->setText("");
        else if (antHasRotor[3])
            labelAnt4Status->setText("Stopped");
        else
            labelAnt4Status->setText("");

    }

	for (unsigned char antIndex=0;antIndex<4;antIndex++) {
		if (antVerticalArray[antIndex]) {
			if (antIndex == 0) {
				labelAnt1Dir->setText(verticalArrayDirName[antIndex][0]);
				labelAnt1Status->setText("");
			}
			else if (antIndex == 1) {
				labelAnt2Dir->setText(verticalArrayDirName[antIndex][0]);
				labelAnt2Status->setText("");
			}
			else if (antIndex == 2) {
				labelAnt3Dir->setText(verticalArrayDirName[antIndex][0]);
				labelAnt3Status->setText("");
			}
			else if (antIndex == 3) {
				labelAnt4Dir->setText(verticalArrayDirName[antIndex][0]);
				labelAnt4Status->setText("");
			}
		}
	}

    if (antCount == 1) {
        for (int i=0;i<4;i++) {
            if (antExist[i])
                currAntIndex = i;
        }
    }

	repaint();
}

void RotatorDialog::setRotatorAngle(int antIndex, unsigned int angle) {
	if (antIndex < 4) {
		if (antVerticalArray[antIndex]) {
			if (angle < 4)
				currAzimuthAngle[antIndex] = angle;
			else
				currAzimuthAngle[antIndex] = 0;
		}
		else
			currAzimuthAngle[antIndex] = angle;

		if (antIndex == 0) {
			if (antVerticalArray[antIndex]) {
				labelAnt1Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
                labelAnt1Dir->setText(QString::number(angle) + "\u00B0");
		}
		else if (antIndex == 1) {
			if (antVerticalArray[antIndex]) {
				labelAnt2Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
                labelAnt2Dir->setText(QString::number(angle) + "\u00B0");
		}
		else if (antIndex == 2) {
			if (antVerticalArray[antIndex]) {
				labelAnt3Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
                labelAnt3Dir->setText(QString::number(angle) + "\u00B0");
		}
		else if (antIndex == 3) {
			if (antVerticalArray[antIndex]) {
				labelAnt4Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
                labelAnt4Dir->setText(QString::number(angle) + "\u00B0");
		}

		repaint();
	}
}

void RotatorDialog::pushButtonRotateCWPressed() {
	if (TCPComm->isConnected()) {
        TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_TURN_CW,lastAntIndex);
	}
}

void RotatorDialog::pushButtonRotateCCWPressed() {
	if (TCPComm->isConnected()) {
        TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_TURN_CCW,lastAntIndex);
	}
}

void RotatorDialog::pushButtonRotateCWReleased() {	
    if (TCPComm->isConnected()) {
        TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,lastAntIndex);
	}
    pushButtonRotateCW->setEnabled(false);
}

void RotatorDialog::pushButtonRotateCCWReleased() {
	if (TCPComm->isConnected()) {
        TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,lastAntIndex);
	}
    pushButtonRotateCCW->setEnabled(false);
}

void RotatorDialog::disableButtonAntSelection() {
    if (antCount != 1) {
        if (!antVerticalArray[currAntIndex]) {
            pushButtonAnt1->setChecked(false);
            pushButtonAnt2->setChecked(false);
            pushButtonAnt3->setChecked(false);
            pushButtonAnt4->setChecked(false);


            pushButtonPreset1->setEnabled(false);
            pushButtonPreset2->setEnabled(false);
            pushButtonPreset3->setEnabled(false);
            pushButtonPreset4->setEnabled(false);
            pushButtonPreset5->setEnabled(false);

            if (pushButtonRotateCW->isDown() != true)
                pushButtonRotateCW->setEnabled(false);

            if (pushButtonRotateCCW->isDown() != true)
                pushButtonRotateCCW->setEnabled(false);

            currAntIndex = -1;
        }
    }
}

void RotatorDialog::pushButtonAnt1Clicked() {
    //Make sure if the timer is already running that we stop it
    if (singleShotTimer->isActive()) {
        qDebug() << "Stopping timer";
        singleShotTimer->stop();
    }

	pushButtonAnt1->setChecked(true);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(false);

    lastAntIndex = currAntIndex;
	currAntIndex = 0;

    singleShotTimer->start();
    qDebug() << "Starting timer";

	setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt2Clicked() {
    //Make sure if the timer is already running that we stop it
    if (singleShotTimer->isActive())
        singleShotTimer->stop();

	pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(true);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(false);

	currAntIndex = 1;
    lastAntIndex = currAntIndex;

    singleShotTimer->start();

    setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt3Clicked() {
    //Make sure if the timer is already running that we stop it
    if (singleShotTimer->isActive())
        singleShotTimer->stop();

	pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(true);
	pushButtonAnt4->setChecked(false);

	currAntIndex = 2;
    lastAntIndex = currAntIndex;

    singleShotTimer->start();

	setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt4Clicked() {
    //Make sure if the timer is already running that we stop it
    if (singleShotTimer->isActive())
        singleShotTimer->stop();

    pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(true);

	currAntIndex = 3;
    lastAntIndex = currAntIndex;

    singleShotTimer->start();

	setStatusPresetButtons();
}

void RotatorDialog::setCOMMPtr(TCPClass *ptr) {
	TCPComm = ptr;
}

void RotatorDialog::pushButtonPreset1Clicked() {
    if (currAntIndex != -1)
        setTargetDir(currAntIndex,presetButtonValue[0]);
}

void RotatorDialog::pushButtonPreset2Clicked() {
    if (currAntIndex != -1)
        setTargetDir(currAntIndex,presetButtonValue[1]);
}

void RotatorDialog::pushButtonPreset3Clicked() {
    if (currAntIndex != -1)
        setTargetDir(currAntIndex,presetButtonValue[2]);
}

void RotatorDialog::pushButtonPreset4Clicked() {
    if (currAntIndex != -1)
        setTargetDir(currAntIndex,presetButtonValue[3]);
}

void RotatorDialog::pushButtonPreset5Clicked() {
    if (currAntIndex != -1)
        setTargetDir(currAntIndex,presetButtonValue[4]);
}

void RotatorDialog::pushButtonSTOPClicked() {
	if (TCPComm->isConnected()) {
        TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,lastAntIndex);
	}
}

void RotatorDialog::keyPressEvent(QKeyEvent *e) {
  if (e->type() == QEvent::KeyPress) {
    Qt::Key keyPressed = (Qt::Key)e->key();

    if(keyPressed == Qt::Key_Q) //Hide the window
      this->hide();
    else if (keyPressed == Qt::Key_S) //Set the position to the left upper corner of the monitor
      this->setGeometry(QRect(0,0,this->width(),this->height()));
  }
}

void RotatorDialog::setupLayout() {
  QSettings settings("rotator_settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("MapSettings");

  if (settings.contains("ImagePath")) {
    imagePath = settings.value("ImagePath").toString();
    sizeWidth = settings.value("MapWidth").toInt();
    sizeHeight = settings.value("MapHeight").toInt();
  }
  else {
    imagePath = "maps/map2.png";
    sizeWidth = 600;
    sizeHeight = 600;

    settings.setValue("ImagePath",imagePath);
    settings.setValue("MapWidth",sizeWidth);
    settings.setValue("MapHeight",sizeHeight);
  }

  settings.endGroup();

  this->setMinimumWidth(sizeWidth+200);
  this->setMinimumHeight(sizeHeight);
  this->resize(sizeWidth+200,sizeHeight);

  imageLabel->setGeometry(0,0,sizeWidth,sizeHeight);
  pushButtonAnt1->setGeometry(sizeWidth+5,10,55,96);
  pushButtonAnt2->setGeometry(sizeWidth+5,110,55,96);
  pushButtonAnt3->setGeometry(sizeWidth+5,210,55,96);
  pushButtonAnt4->setGeometry(sizeWidth+5,310,55,96);
  frameAnt1->setGeometry(sizeWidth+65,10,130,96);
  frameAnt2->setGeometry(sizeWidth+65,110,130,96);
  frameAnt3->setGeometry(sizeWidth+65,210,130,96);
  frameAnt4->setGeometry(sizeWidth+65,310,130,96);
  pushButtonRotateCCW->setGeometry(sizeWidth-120,sizeHeight-60,55,55);
  pushButtonRotateCW->setGeometry(sizeWidth-60,sizeHeight-60,55,55);
  pushButtonReconnect->setGeometry(5,sizeHeight-60,121,55);
  pushButtonPreset1->setGeometry(sizeWidth+5,sizeHeight-185,90,55);
  pushButtonPreset2->setGeometry(sizeWidth+5,sizeHeight-120,90,55);
  pushButtonPreset3->setGeometry(sizeWidth+5,sizeHeight-55,90,55);
  pushButtonPreset4->setGeometry(sizeWidth+105,sizeHeight-185,90,55);
  pushButtonPreset5->setGeometry(sizeWidth+105,sizeHeight-120,90,55);
  pushButtonSTOP->setGeometry(sizeWidth+105,sizeHeight-55,90,55);

  if ((sizeWidth < 600) || (sizeHeight < 600)) {
    pushButtonPreset1->setVisible(false);
    pushButtonPreset2->setVisible(false);
    pushButtonPreset3->setVisible(false);
    pushButtonPreset4->setVisible(false);
    pushButtonPreset5->setVisible(false);

    this->setMinimumWidth(sizeWidth+200);
    this->setMinimumHeight(sizeHeight+70);
    this->resize(sizeWidth+200,sizeHeight+70);

    pushButtonRotateCCW->setGeometry(sizeWidth-120,sizeHeight-60+70,55,55);
    pushButtonRotateCW->setGeometry(sizeWidth-60,sizeHeight-60+70,55,55);
    pushButtonSTOP->setGeometry(sizeWidth+105,sizeHeight-55+70,90,55);
    pushButtonReconnect->setGeometry(5,sizeHeight-60+70,121,55);
  }

  setRotatorAngle(0,0);
  targetAzimuthAngle[0] = 0;

  setRotatorAngle(1,0);
  targetAzimuthAngle[1] = 0;

  setRotatorAngle(2,0);
  targetAzimuthAngle[2] = 0;

  setRotatorAngle(3,0);
  targetAzimuthAngle[3] = 0;
}

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f) {
  setupUi(this);

  imagePath = "maps/map2.png";
  sizeWidth = 600;
  sizeHeight = 600;

  setupLayout();

  rotationEventStatus = 0;

  currAntIndex = -1;

  //This timer is used to disable the antenna selection for rotation. It does this to all
  //Antennas except if there is only one on the band or if the last selected antenna is a
  //vertical array. This is because we want to avoid having people rotating antenna towers
  //by mistake.
  singleShotTimer = new QTimer();
  singleShotTimer->setSingleShot(true);
  singleShotTimer->setInterval(ANT_SEL_ROTATE_TIMEOUT);
  connect(singleShotTimer,SIGNAL(timeout()),this,SLOT(disableButtonAntSelection()));

  QGraphicsDropShadowEffect* effect[4];

  for (int i=0;i<4;i++) {
      effect[i] = new QGraphicsDropShadowEffect();
      effect[i]->setColor(Qt::black);
      effect[i]->setBlurRadius(20);
      effect[i]->setXOffset(3);
      effect[i]->setYOffset(3);
  }

  labelMapNameAnt1->setVisible(false);
  labelMapNameAnt2->setVisible(false);
  labelMapNameAnt3->setVisible(false);
  labelMapNameAnt4->setVisible(false);

  labelMapNameAnt1->setGraphicsEffect(effect[0]);
  labelMapNameAnt2->setGraphicsEffect(effect[1]);
  labelMapNameAnt3->setGraphicsEffect(effect[2]);
  labelMapNameAnt4->setGraphicsEffect(effect[3]);

  QPalette plt;
  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  plt.setColor(QPalette::Button, Qt::lightGray);
  labelMapNameAnt1->setPalette(plt);
  labelAnt1Title->setPalette(plt);
  labelAnt1Dir->setPalette(plt);
  labelAnt1Status->setPalette(plt);
  pushButtonAnt1->setPalette(plt);
  plt.setColor(QPalette::Window, Qt::lightGray);
  frameAnt1->setAutoFillBackground(true);
  frameAnt1->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  plt.setColor(QPalette::Button, Qt::lightGray);
  labelMapNameAnt2->setPalette(plt);
  labelAnt2Title->setPalette(plt);
  labelAnt2Dir->setPalette(plt);
  labelAnt2Status->setPalette(plt);
  pushButtonAnt2->setPalette(plt);
  plt.setColor(QPalette::Window, Qt::lightGray);
  frameAnt2->setAutoFillBackground(true);
  frameAnt2->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  plt.setColor(QPalette::Button, Qt::lightGray);
  labelMapNameAnt3->setPalette(plt);
  labelAnt3Title->setPalette(plt);
  labelAnt3Dir->setPalette(plt);
  labelAnt3Status->setPalette(plt);
  pushButtonAnt3->setPalette(plt);
  plt.setColor(QPalette::Window, Qt::lightGray);
  frameAnt3->setAutoFillBackground(true);
  frameAnt3->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  plt.setColor(QPalette::Button, Qt::lightGray);
  labelMapNameAnt4->setPalette(plt);
  labelAnt4Title->setPalette(plt);
  labelAnt4Dir->setPalette(plt);
  labelAnt4Status->setPalette(plt);
  pushButtonAnt4->setPalette(plt);
  plt.setColor(QPalette::Background, Qt::lightGray);
  frameAnt4->setAutoFillBackground(true);
  frameAnt4->setPalette(plt);

  lengthMapLabel = 220;

/*  this->pushButtonAnt1->setStyleSheet("QPushButton {\nbackground-color:lightGray;\ncolor:"+QString(STRING_DIR_BEAMWIDTH_A1_COLOR)+";\n}");
  this->pushButtonAnt2->setStyleSheet("QPushButton {\nbackground-color:lightGray;\ncolor:"+QString(STRING_DIR_BEAMWIDTH_A2_COLOR)+";\n}");
  this->pushButtonAnt3->setStyleSheet("QPushButton {\nbackground-color:lightGray;\ncolor:"+QString(STRING_DIR_BEAMWIDTH_A3_COLOR)+";\n}");
  this->pushButtonAnt4->setStyleSheet("QPushButton {\nbackground-color:lightGray;\ncolor:"+QString(STRING_DIR_BEAMWIDTH_A4_COLOR)+";\n}");

  this->frameAnt1->setStyleSheet("QFrame {\nbackground-color:lightGray;\ncolor"+QString(STRING_DIR_BEAMWIDTH_A1_COLOR)+";\n}");
  this->frameAnt2->setStyleSheet("QFrame {\nbackground-color:lightGray;\ncolor"+QString(STRING_DIR_BEAMWIDTH_A2_COLOR)+";\n}");
  this->frameAnt3->setStyleSheet("QFrame {\nbackground-color:lightGray;\ncolor"+QString(STRING_DIR_BEAMWIDTH_A3_COLOR)+";\n}");
  this->frameAnt4->setStyleSheet("QFrame {\nbackground-color:lightGray;\ncolor"+QString(STRING_DIR_BEAMWIDTH_A4_COLOR)+";\n}");
*/

  connect(pushButtonAnt1, SIGNAL(clicked()), this, SLOT(pushButtonAnt1Clicked()));
  connect(pushButtonAnt2, SIGNAL(clicked()), this, SLOT(pushButtonAnt2Clicked()));
  connect(pushButtonAnt3, SIGNAL(clicked()), this, SLOT(pushButtonAnt3Clicked()));
  connect(pushButtonAnt4, SIGNAL(clicked()), this, SLOT(pushButtonAnt4Clicked()));

  connect(pushButtonPreset1, SIGNAL(clicked()), this, SLOT(pushButtonPreset1Clicked()));
  connect(pushButtonPreset2, SIGNAL(clicked()), this, SLOT(pushButtonPreset2Clicked()));
  connect(pushButtonPreset3, SIGNAL(clicked()), this, SLOT(pushButtonPreset3Clicked()));
  connect(pushButtonPreset4, SIGNAL(clicked()), this, SLOT(pushButtonPreset4Clicked()));
  connect(pushButtonPreset5, SIGNAL(clicked()), this, SLOT(pushButtonPreset5Clicked()));
  connect(pushButtonSTOP, SIGNAL(clicked()), this, SLOT(pushButtonSTOPClicked()));

  connect(pushButtonRotateCCW, SIGNAL(pressed()), this, SLOT(pushButtonRotateCCWPressed()));
  connect(pushButtonRotateCW, SIGNAL(pressed()), this, SLOT(pushButtonRotateCWPressed()));
  connect(pushButtonRotateCCW, SIGNAL(released()), this, SLOT(pushButtonRotateCCWReleased()));
  connect(pushButtonRotateCW, SIGNAL(released()), this, SLOT(pushButtonRotateCWReleased()));

  connect(pushButtonReconnect, SIGNAL(clicked()), parent, SLOT(actionReconnect()));

  pushButtonAnt1->setChecked(true);
  pushButtonAnt2->setChecked(false);
  pushButtonAnt3->setChecked(false);
  pushButtonAnt4->setChecked(false);

  loadBand(0);

  for (unsigned char i=0;i<4;i++)
    rotatorStatus[i] = 0;
}
//

void RotatorDialog::on_pushButtonRotateQuit_clicked()
{
    emit quitProgram();
}

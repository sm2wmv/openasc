#include "rotatordialog.h"
#include <math.h>

#define PI 3.1415

void RotatorDialog::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	image = QImage(imagePath);
	image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);

	painter.drawImage(0 , 0 , image);

	float rectWidth = sizeWidth - (sizeWidth * 0.06);
	float rectHeight = sizeHeight - (sizeHeight * 0.06);
	QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth-2, rectHeight-2);

	for (int i=0;i<4;i++) {
		if ((antExist[i] == true) && (antBeamWidth[i] > 0)) {
			if (i==0) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR),Qt::FDiagPattern));

					if (antVerticalArray[i])
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
					else
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if ((rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CW)) || (rotatorStatus[i] & (1<<FLAG_ROTATOR_ROTATION_CCW))) {
						painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR),Qt::SolidPattern));
						painter.drawPie(rectangle,(360-(-90+targetAzimuthAngle[i] + 0.5))*16,0.5*16);
					}
			}
			else if (i==1) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR),Qt::FDiagPattern));

					if (antVerticalArray[i])
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
					else
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
			}
			else if (i==2) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR),Qt::FDiagPattern));

					if (antVerticalArray[i])
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
					else
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
			}
			else if (i==3) {
					painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
					painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR),Qt::FDiagPattern));

					if (antVerticalArray[i])
						painter.drawPie(rectangle,(360-(-90+verticalArrayDirAngle[i][currAzimuthAngle[i]] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
					else
						painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);

					if (antBiDirectional[i])
						painter.drawPie(rectangle,(180-(-90+currAzimuthAngle[i] + antBeamWidth[i]/2))*16,antBeamWidth[i]*16);
			}
		}
	}
}

void RotatorDialog::setRotatorStatusText(unsigned char index, unsigned char status) {
	rotatorStatus[index] = status;

	if (index == 0) {
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
	else if (index == 1) {
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
	else if (index == 2) {
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
	else if (index == 3) {
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
	if (antIndex >= 0) {
		targetAzimuthAngle[antIndex] = targetAngle;

		qDebug("TARGET_DIR[%i]: %d",antIndex,targetAngle);

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

		groupBoxAnt1->setVisible(false);
		groupBoxAnt2->setVisible(false);
		groupBoxAnt3->setVisible(false);
		groupBoxAnt4->setVisible(false);

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

		groupBoxAnt1->setVisible(false);
		groupBoxAnt2->setVisible(false);
		groupBoxAnt3->setVisible(false);
		groupBoxAnt4->setVisible(false);

		pushButtonAnt1->setVisible(false);
		pushButtonAnt2->setVisible(false);
		pushButtonAnt3->setVisible(false);
		pushButtonAnt4->setVisible(false);

		if (antExist[0]) {
			groupBoxAnt1->setVisible(true);
			pushButtonAnt1->setVisible(true);

			if ((antHasRotor[0]) || (antVerticalArray[0])) {
				pushButtonAnt1->setEnabled(true);
				currAntIndex = 0;
				pushButtonAnt1->setChecked(true);
			}
			else {
				pushButtonAnt1->setEnabled(false);
				pushButtonAnt1->setChecked(false);
			}
		}

		if (antExist[1]) {
			groupBoxAnt2->setVisible(true);
			pushButtonAnt2->setVisible(true);

			if ((antHasRotor[1]) || (antVerticalArray[1])) {
				pushButtonAnt2->setEnabled(true);

				if (currAntIndex == -1) {
					currAntIndex = 1;
					pushButtonAnt2->setChecked(true);
				}
			}
			else {
				pushButtonAnt2->setEnabled(false);
			}
		}

		if (antExist[2]) {
			groupBoxAnt3->setVisible(true);
			pushButtonAnt3->setVisible(true);

			if ((antHasRotor[2]) || (antVerticalArray[2])) {
				pushButtonAnt3->setEnabled(true);

				if (currAntIndex == -1) {
					currAntIndex = 2;
					pushButtonAnt3->setChecked(true);
				}
			}
			else
				pushButtonAnt3->setEnabled(false);
		}

		if (antExist[3]) {
			groupBoxAnt4->setVisible(true);
			pushButtonAnt4->setVisible(true);

			if ((antHasRotor[3]) || (antVerticalArray[3])) {
				pushButtonAnt4->setEnabled(true);

				if (currAntIndex == -1) {
					currAntIndex = 3;
					pushButtonAnt4->setChecked(true);
				}
			}
			else
				pushButtonAnt4->setEnabled(false);
		}

                setStatusPresetButtons();

		labelAnt1Title->setText(antName[0]);
		labelAnt2Title->setText(antName[1]);
		labelAnt3Title->setText(antName[2]);
		labelAnt4Title->setText(antName[3]);

		if (antFixed[0]) {
			labelAnt1Status->setText("Fixed");
			pushButtonAnt1->setVisible(false);
		}
		else if (antVerticalArray[0])
			labelAnt1Status->setText("");
		else
			labelAnt1Status->setText("Stopped");

		if (antFixed[1]) {
			labelAnt2Status->setText("Fixed");
			pushButtonAnt2->setVisible(false);
		}
		else if (antVerticalArray[1])
			labelAnt2Status->setText("");
		else
			labelAnt2Status->setText("Stopped");

		if (antFixed[2]) {
			labelAnt3Status->setText("Fixed");
			pushButtonAnt3->setVisible(false);
		}
		else if (antVerticalArray[2])
			labelAnt3Status->setText("");
		else
			labelAnt3Status->setText("Stopped");

		if (antFixed[3]) {
			labelAnt4Status->setText("Fixed");
			pushButtonAnt4->setVisible(false);
		}
		else if (antVerticalArray[3])
			labelAnt4Status->setText("");
		else
			labelAnt4Status->setText("Stopped");
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
				labelAnt1Dir->setText(QString::number(angle) + '°');
		}
		else if (antIndex == 1) {
			if (antVerticalArray[antIndex]) {
				labelAnt2Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
				labelAnt2Dir->setText(QString::number(angle) + '°');
		}
		else if (antIndex == 2) {
			if (antVerticalArray[antIndex]) {
				labelAnt3Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
				labelAnt3Dir->setText(QString::number(angle) + '°');
		}
		else if (antIndex == 3) {
			if (antVerticalArray[antIndex]) {
				labelAnt4Dir->setText(verticalArrayDirName[antIndex][currAzimuthAngle[antIndex]]);
			}
			else
				labelAnt4Dir->setText(QString::number(angle) + '°');
		}

		repaint();
	}
}

void RotatorDialog::pushButtonRotateCWPressed() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_TURN_CW,currAntIndex);
	}
}

void RotatorDialog::pushButtonRotateCCWPressed() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_TURN_CCW,currAntIndex);
	}
}

void RotatorDialog::pushButtonRotateCWReleased() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,currAntIndex);
	}
}

void RotatorDialog::pushButtonRotateCCWReleased() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,currAntIndex);
	}
}

void RotatorDialog::pushButtonAnt1Clicked() {
	pushButtonAnt1->setChecked(true);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(false);

	currAntIndex = 0;

	setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt2Clicked() {
	pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(true);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(false);

	currAntIndex = 1;

	setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt3Clicked() {
	pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(true);
	pushButtonAnt4->setChecked(false);

	currAntIndex = 2;

	setStatusPresetButtons();
}

void RotatorDialog::pushButtonAnt4Clicked() {
	pushButtonAnt1->setChecked(false);
	pushButtonAnt2->setChecked(false);
	pushButtonAnt3->setChecked(false);
	pushButtonAnt4->setChecked(true);

	currAntIndex = 3;

	setStatusPresetButtons();
}

void RotatorDialog::setCOMMPtr(TCPClass *ptr) {
	TCPComm = ptr;
}

void RotatorDialog::pushButtonPreset1Clicked() {
	setTargetDir(currAntIndex,presetButtonValue[0]);
}

void RotatorDialog::pushButtonPreset2Clicked() {
	setTargetDir(currAntIndex,presetButtonValue[1]);
}

void RotatorDialog::pushButtonPreset3Clicked() {
	setTargetDir(currAntIndex,presetButtonValue[2]);
}

void RotatorDialog::pushButtonPreset4Clicked() {
	setTargetDir(currAntIndex,presetButtonValue[3]);
}

void RotatorDialog::pushButtonPreset5Clicked() {
	setTargetDir(currAntIndex,presetButtonValue[4]);
}

void RotatorDialog::pushButtonSTOPClicked() {
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_ROTATOR_STOP,currAntIndex);
	}

	qDebug("STOP");
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

RotatorDialog::RotatorDialog( QWidget * parent, Qt::WFlags f) : QDialog(parent, f) {
  setupUi(this);

  this->resize(800,600);
  imagePath = "maps/map.jpg";

  sizeWidth = 600;
  sizeHeight = 600;

  setRotatorAngle(0,0);
  targetAzimuthAngle[0] = 0;

  setRotatorAngle(1,0);
  targetAzimuthAngle[1] = 0;

  setRotatorAngle(2,0);
  targetAzimuthAngle[2] = 0;

  setRotatorAngle(3,0);
  targetAzimuthAngle[3] = 0;

  rotationEventStatus = 0;

  currAntIndex = 0;

  QPalette plt;
  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  groupBoxAnt1->setPalette(plt);
  pushButtonAnt1->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  groupBoxAnt2->setPalette(plt);
  pushButtonAnt2->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  groupBoxAnt3->setPalette(plt);
  pushButtonAnt3->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  groupBoxAnt4->setPalette(plt);
  pushButtonAnt4->setPalette(plt);

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

  pushButtonAnt1->setChecked(true);
  pushButtonAnt2->setChecked(false);
  pushButtonAnt3->setChecked(false);
  pushButtonAnt4->setChecked(false);

  loadBand(0);

  for (unsigned char i=0;i<4;i++)
    rotatorStatus[i] = 0;
}
//

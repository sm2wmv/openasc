#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#define PI 3.1415

void MainWindow::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  image = QImage(imagePath);
  image.convertToFormat(QImage::Format_ARGB32_Premultiplied ,Qt::ColorOnly);

  painter.drawImage(0 , 0 , image);

  float rectWidth = sizeWidth - (sizeWidth * 0.06);
  float rectHeight = sizeHeight - (sizeHeight * 0.06);
  QRectF rectangle(sizeWidth/2-rectWidth/2,sizeWidth/2-rectHeight/2, rectWidth-2, rectHeight-2);

  for (int i=0;i<4;i++) {
    if ((rotorExist[i] == true) && (beamWidth[i] > 0)) {
      if (i==0) {
        painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
        painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR),Qt::FDiagPattern));

        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + beamWidth[i]/2))*16,beamWidth[i]*16);
      }
      else if (i==1) {
        painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
        painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR),Qt::FDiagPattern));

        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + beamWidth[i]/2))*16,beamWidth[i]*16);
      }
      else if (i==2) {
        painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
        painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR),Qt::FDiagPattern));

        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + beamWidth[i]/2))*16,beamWidth[i]*16);
      }
      else if (i==3) {
        painter.setPen(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
        painter.setBrush(QBrush(QColor(Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR),Qt::FDiagPattern));

        painter.drawPie(rectangle,(360-(-90+currAzimuthAngle[i] + beamWidth[i]/2))*16,beamWidth[i]*16);
      }
    }
  }
}

void MainWindow::mousePressEvent ( QMouseEvent * event ) {
  if ((event->x() >= 8) && (event->y() >= 8) && (event->x() <= (600)) && (event->y() <= 600)) {
      double mapX = abs(300 - event->x());
      double mapY = 300 - event->y();

      if ((300 - event->x()) < 0)
          setTargetDir(currRotorIndex,90-atan(mapY/mapX)*(180/PI));
      else
          setTargetDir(currRotorIndex,270+atan(mapY/mapX)*(180/PI));
  }
}

void MainWindow::setTargetDir(int rotorIndex, int targetAngle) {

}

void MainWindow::setRotorAngle(int rotorIndex, unsigned int angle) {
  currAzimuthAngle[rotorIndex] = angle;
}

int MainWindow::getTargetDir(unsigned char rotorIndex) {

}

void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (e->type() == QEvent::KeyPress) {
    Qt::Key keyPressed = (Qt::Key)e->key();

    if(keyPressed == Qt::Key_Q) //Quit
      exit(0);
    else if (keyPressed == Qt::Key_S) //Set the position to the left upper corner of the monitor
      this->setGeometry(QRect(0,0,this->width(),this->height()));
  }
}

void MainWindow::pushButtonRotor1Clicked() {
  currRotorIndex = 0;

  ui->pushButtonRotor1->setChecked(true);
  ui->pushButtonRotor2->setChecked(false);
  ui->pushButtonRotor3->setChecked(false);
  ui->pushButtonRotor4->setChecked(false);
}

void MainWindow::pushButtonRotor2Clicked() {
  currRotorIndex = 1;

  ui->pushButtonRotor1->setChecked(false);
  ui->pushButtonRotor2->setChecked(true);
  ui->pushButtonRotor3->setChecked(false);
  ui->pushButtonRotor4->setChecked(false);
}

void MainWindow::pushButtonRotor3Clicked() {
  currRotorIndex = 2;

  ui->pushButtonRotor1->setChecked(false);
  ui->pushButtonRotor2->setChecked(false);
  ui->pushButtonRotor3->setChecked(true);
  ui->pushButtonRotor4->setChecked(false);
}

void MainWindow::pushButtonRotor4Clicked() {
  currRotorIndex = 3;

  ui->pushButtonRotor1->setChecked(false);
  ui->pushButtonRotor2->setChecked(false);
  ui->pushButtonRotor3->setChecked(false);
  ui->pushButtonRotor4->setChecked(true);
}

void MainWindow::pushButtonPreset1Clicked() {
  networkManager->get(QNetworkRequest(QUrl("http://sj2w.shacknet.nu:8083/rotatorcontrol/get")));
  //networkManager->get(QNetworkRequest(QUrl("http://sj2w.shacknet.nu:8083/relaycontrol/off/3$")));
}

void MainWindow::pushButtonPreset2Clicked() {

}

void MainWindow::pushButtonPreset3Clicked() {

}

void MainWindow::pushButtonPreset4Clicked() {

}

void MainWindow::pushButtonPreset5Clicked() {

}

void MainWindow::pushButtonSTOPClicked() {

}

void MainWindow::pushButtonRotateCWPressed() {
  //We don't have any way to control the webswitch CW/CCW so we just do like this instead
  setTargetDir(currRotorIndex,359);
}

void MainWindow::pushButtonRotateCCWPressed() {
  //We don't have any way to control the webswitch CW/CCW so we just do like this instead
  setTargetDir(currRotorIndex,0);
}

void MainWindow::pushButtonRotateCWReleased() {
  pushButtonSTOPClicked();
}

void MainWindow::pushButtonRotateCCWReleased() {
  pushButtonSTOPClicked();
}

void MainWindow::networkReplyFinished(QNetworkReply* reply) {
  qDebug(reply->readAll());
}

void MainWindow::loadSettings() {
  QSettings settings("settings.ini",QSettings::IniFormat,0);

  settings.beginGroup("Various");

  if (settings.contains("WindowHasFrame") == false) {
    settings.setValue("WindowHasFrame",true);
  }

  windowFrameVisible = settings.value("WindowHasFrame").toBool();

  settings.endGroup();

  settings.beginGroup("Network");

  if (settings.contains("Rotor1Url") == false) {
    settings.setValue("Rotor1Url","http://example.com:port");
    settings.setValue("Rotor2Url","http://example.com:port");
    settings.setValue("Rotor3Url","http://example.com:port");
    settings.setValue("Rotor4Url","http://example.com:port");
  }

  rotorURL[0] = settings.value("Rotor1Url").toString();
  rotorURL[1] = settings.value("Rotor2Url").toString();
  rotorURL[2] = settings.value("Rotor3Url").toString();
  rotorURL[3] = settings.value("Rotor4Url").toString();

  settings.endGroup();

  settings.beginGroup("RotorSettings");

  if (settings.contains("Rotor1Exist") == false) {
    settings.setValue("Rotor1Exist",true);
    settings.setValue("Rotor2Exist",false);
    settings.setValue("Rotor3Exist",false);
    settings.setValue("Rotor4Exist",false);

    settings.setValue("Rotor1Name","Rotor 1");
    settings.setValue("Rotor2Name","Rotor 2");
    settings.setValue("Rotor3Name","Rotor 3");
    settings.setValue("Rotor4Name","Rotor 4");

    settings.setValue("Rotor1BeamWidth",60);
    settings.setValue("Rotor2BeamWidth",60);
    settings.setValue("Rotor3BeamWidth",60);
    settings.setValue("Rotor4BeamWidth",60);
  }

  rotorExist[0] = settings.value("Rotor1Exist").toBool();
  rotorExist[1] = settings.value("Rotor2Exist").toBool();
  rotorExist[2] = settings.value("Rotor3Exist").toBool();
  rotorExist[3] = settings.value("Rotor4Exist").toBool();

  rotorName[0] = settings.value("Rotor1Name").toString();
  rotorName[1] = settings.value("Rotor2Name").toString();
  rotorName[2] = settings.value("Rotor3Name").toString();
  rotorName[3] = settings.value("Rotor4Name").toString();

  beamWidth[0] = settings.value("Rotor1BeamWidth").toInt();
  beamWidth[1] = settings.value("Rotor2BeamWidth").toInt();
  beamWidth[2] = settings.value("Rotor3BeamWidth").toInt();
  beamWidth[3] = settings.value("Rotor4BeamWidth").toInt();

  settings.endGroup();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->resize(800,600);
  imagePath = "maps/map.png";

  sizeWidth = 600;
  sizeHeight = 600;

  loadSettings();

  if (windowFrameVisible == false)
    setWindowFlags(Qt::FramelessWindowHint);

  setRotorAngle(0,90);
  targetAzimuthAngle[0] = 0;

  setRotorAngle(1,180);
  targetAzimuthAngle[1] = 0;

  setRotorAngle(2,270);
  targetAzimuthAngle[2] = 0;

  setRotorAngle(3,0);
  targetAzimuthAngle[3] = 0;

  QPalette plt;
  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A1_COLOR);
  ui->groupBoxRotor1->setPalette(plt);
  ui->pushButtonRotor1->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A2_COLOR);
  ui->groupBoxRotor2->setPalette(plt);
  ui->pushButtonRotor2->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A3_COLOR);
  ui->groupBoxRotor3->setPalette(plt);
  ui->pushButtonRotor3->setPalette(plt);

  plt.setColor(QPalette::WindowText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  plt.setColor(QPalette::ButtonText, Qt::CURRENT_DIR_BEAMWIDTH_A4_COLOR);
  ui->groupBoxRotor4->setPalette(plt);
  ui->pushButtonRotor4->setPalette(plt);

  ui->groupBoxRotor1->setVisible(rotorExist[0]);
  ui->groupBoxRotor2->setVisible(rotorExist[1]);
  ui->groupBoxRotor3->setVisible(rotorExist[2]);
  ui->groupBoxRotor4->setVisible(rotorExist[3]);

  ui->pushButtonRotor1->setVisible(rotorExist[0]);
  ui->pushButtonRotor2->setVisible(rotorExist[1]);
  ui->pushButtonRotor3->setVisible(rotorExist[2]);
  ui->pushButtonRotor4->setVisible(rotorExist[3]);

  currRotorIndex = -1;

  ui->labelRotor1Title->setText(rotorName[0]);
  ui->labelRotor2Title->setText(rotorName[1]);
  ui->labelRotor3Title->setText(rotorName[2]);
  ui->labelRotor4Title->setText(rotorName[3]);

  if (rotorExist[0]) {
    currRotorIndex = 0;
    ui->pushButtonRotor1->setChecked(true);
  }
  else if (rotorExist[1]) {
    currRotorIndex = 1;
    ui->pushButtonRotor2->setChecked(true);
  }
  else if (rotorExist[2]) {
    currRotorIndex = 2;
    ui->pushButtonRotor3->setChecked(true);
  }
  else if (rotorExist[3]) {
    currRotorIndex = 3;
    ui->pushButtonRotor4->setChecked(true);
  }

  connect(ui->pushButtonRotor1,SIGNAL(clicked()),this,SLOT(pushButtonRotor1Clicked()));
  connect(ui->pushButtonRotor2,SIGNAL(clicked()),this,SLOT(pushButtonRotor2Clicked()));
  connect(ui->pushButtonRotor3,SIGNAL(clicked()),this,SLOT(pushButtonRotor3Clicked()));
  connect(ui->pushButtonRotor4,SIGNAL(clicked()),this,SLOT(pushButtonRotor4Clicked()));

  connect(ui->pushButtonPreset1, SIGNAL(clicked()), this, SLOT(pushButtonPreset1Clicked()));
  connect(ui->pushButtonPreset2, SIGNAL(clicked()), this, SLOT(pushButtonPreset2Clicked()));
  connect(ui->pushButtonPreset3, SIGNAL(clicked()), this, SLOT(pushButtonPreset3Clicked()));
  connect(ui->pushButtonPreset4, SIGNAL(clicked()), this, SLOT(pushButtonPreset4Clicked()));
  connect(ui->pushButtonPreset5, SIGNAL(clicked()), this, SLOT(pushButtonPreset5Clicked()));

  connect(ui->pushButtonSTOP, SIGNAL(clicked()), this, SLOT(pushButtonSTOPClicked()));

  connect(ui->pushButtonRotateCCW, SIGNAL(pressed()), this, SLOT(pushButtonRotateCCWPressed()));
  connect(ui->pushButtonRotateCW, SIGNAL(pressed()), this, SLOT(pushButtonRotateCWPressed()));
  connect(ui->pushButtonRotateCCW, SIGNAL(released()), this, SLOT(pushButtonRotateCCWReleased()));
  connect(ui->pushButtonRotateCW, SIGNAL(released()), this, SLOT(pushButtonRotateCWReleased()));


  //Setup the network access manager
  networkManager = new QNetworkAccessManager(this);
  connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow() {
  delete ui;
}

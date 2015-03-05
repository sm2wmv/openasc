#include "powermeterdialog.h"
#include "ui_powermeterdialog.h"
#include "stdio.h"

QMutex mutex;

pickupStatusStruct pickupStatus;

PowerMeterDialog::PowerMeterDialog(QWidget *parent) : QDialog(parent),ui(new Ui::PowerMeterDialog) {
  ui->setupUi(this);

  pickupStatus.type = 255;

  timerUpdateBargraphs = new QTimer(this);
  timerUpdateText = new QTimer(this);

  connect(timerUpdateBargraphs, SIGNAL(timeout()), this, SLOT(timerUpdateBargraphsUpdate()));
  connect(timerUpdateText, SIGNAL(timeout()), this, SLOT(timerUpdateTextUpdate()));
}

PowerMeterDialog::~PowerMeterDialog() {
  delete ui;
}

void PowerMeterDialog::timerUpdateBargraphsUpdate() {
  mutex.lock();
  if (pickupStatus.fwdPower > (unsigned int)(ui->progressBarFwd->maximum()))
    ui->progressBarFwd->setValue(ui->progressBarFwd->maximum());
  else
    ui->progressBarFwd->setValue(pickupStatus.fwdPower);

  if (pickupStatus.refPower > (unsigned int)(ui->progressBarRef->maximum()))
    ui->progressBarRef->setValue(ui->progressBarRef->maximum());
  else
    ui->progressBarRef->setValue(pickupStatus.refPower);

  mutex.unlock();
}

void PowerMeterDialog::timerUpdateTextUpdate() {
  mutex.lock();
  ui->labelFwdValue->setText(QString::number(pickupStatus.fwdPower)+"W");
  ui->labelRefValue->setText(QString::number(pickupStatus.refPower)+"W");

  char str[10];
  sprintf((char *)str,"%i.%02i:1",(pickupStatus.vswr/100),(pickupStatus.vswr%100));

  ui->labelSWRValue->setText(QString(str));

  mutex.unlock();
}

void PowerMeterDialog::setTimers(unsigned int timerText, unsigned int timerBargraph) {
  iTimerTextUpdateRate = timerText;
  iTimerBargraphsUpdateRate = timerBargraph;
}

void PowerMeterDialog::startTimers() {
  timerUpdateBargraphs->setInterval(iTimerBargraphsUpdateRate);
  timerUpdateText->setInterval(iTimerTextUpdateRate);

  timerUpdateBargraphs->start();
  timerUpdateText->start();
}

void PowerMeterDialog::stopTimers() {
  timerUpdateBargraphs->stop();
  timerUpdateText->stop();
}

void PowerMeterDialog::setValues(unsigned char type, unsigned int fwdPower, unsigned int refPower, unsigned int vSWR) {
  mutex.lock();

  if (type != pickupStatus.type) {
    pickupStatus.type = type;

    rescaleMeters(type);
  }

  pickupStatus.fwdPower = fwdPower;
  pickupStatus.refPower = refPower;
  pickupStatus.vswr = vSWR;

  mutex.unlock();
}

void PowerMeterDialog::rescaleMeters(unsigned char type) {
  if (type == PICKUP_TYPE_150W) {
    ui->progressBarFwd->setMaximum(150);
    ui->progressBarRef->setMaximum(15);
  }
  else if (type == PICKUP_TYPE_1000W) {
    ui->progressBarFwd->setMaximum(1000);
    ui->progressBarRef->setMaximum(100);
  }
  else if (type == PICKUP_TYPE_1500W) {
    ui->progressBarFwd->setMaximum(1500);
    ui->progressBarRef->setMaximum(150);
  }
  else if (type == PICKUP_TYPE_2000W) {
    ui->progressBarFwd->setMaximum(2000);
    ui->progressBarRef->setMaximum(200);
  }
  else if (type == PICKUP_TYPE_3000W) {
    ui->progressBarFwd->setMaximum(3000);
    ui->progressBarRef->setMaximum(300);
  }
  else if (type == PICKUP_TYPE_5000W) {
    ui->progressBarFwd->setMaximum(5000);
    ui->progressBarRef->setMaximum(500);
  }
  else if (type == PICKUP_TYPE_10000W) {
    ui->progressBarFwd->setMaximum(10000);
    ui->progressBarRef->setMaximum(1000);
  }
  else if (type == PICKUP_TYPE_15000W) {
    ui->progressBarFwd->setMaximum(15000);
    ui->progressBarRef->setMaximum(1500);
  }
}

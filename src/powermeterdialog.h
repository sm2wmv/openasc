#ifndef POWERMETERDIALOG_H
#define POWERMETERDIALOG_H

#include <QDialog>
#include <QMutex>
#include <QTimer>

/*! 150w pickup configuration */
#define PICKUP_TYPE_150W		0
/*! 1000w pickup configuration */
#define PICKUP_TYPE_1000W		1
/*! 1500w pickup configuration */
#define PICKUP_TYPE_1500W		2
/*! 2000w pickup configuration */
#define PICKUP_TYPE_2000W		3
/*! 3000w pickup configuration */
#define PICKUP_TYPE_3000W		4
/*! 5000w pickup configuration */
#define PICKUP_TYPE_5000W		5
/*! 10000w pickup configuration */
#define PICKUP_TYPE_10000W	6
/*! 15000w pickup configuration */
#define PICKUP_TYPE_15000W	7

typedef struct {
  unsigned char type;
  unsigned int fwdPower;
  unsigned int refPower;
  unsigned int vswr;
  unsigned int maxPower;
} pickupStatusStruct;

namespace Ui {
class PowerMeterDialog;
}

class PowerMeterDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PowerMeterDialog(QWidget *parent = 0);
  ~PowerMeterDialog();
  void setValues(unsigned char type, unsigned int fwdPower, unsigned int refPower, unsigned int vSWR);
  void setTimers(unsigned int timerText, unsigned int timerBargraph);
  void stopTimers();
  void startTimers();
  void rescaleMeters(unsigned char type);
private:
  Ui::PowerMeterDialog *ui;
  unsigned int iTimerTextUpdateRate;
  unsigned int iTimerBargraphsUpdateRate;
  QTimer *timerUpdateBargraphs;
  QTimer *timerUpdateText;
public slots:
  void timerUpdateBargraphsUpdate();
  void timerUpdateTextUpdate();
};

#endif // POWERMETERDIALOG_H

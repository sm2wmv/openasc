#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>

#include <QMouseEvent>
#include <QColor>
#include <QBrush>
#include <QSettings>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#define TARGET_DIR_BEAMWIDTH	1

#define TARGET_DIR_BEAMWIDTH_A1_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A1_COLOR	black

#define TARGET_DIR_BEAMWIDTH_A2_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A2_COLOR	blue

#define TARGET_DIR_BEAMWIDTH_A3_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A3_COLOR	darkGreen

#define TARGET_DIR_BEAMWIDTH_A4_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A4_COLOR	darkYellow

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void setTargetDir(int rotorIndex, int targetAngle);
  void setRotorAngle(int rotorIndex, unsigned int angle);
  int getTargetDir(unsigned char rotorIndex);
  void loadSettings();
protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent ( QMouseEvent * event );
  void keyPressEvent(QKeyEvent *e);
private:
  Ui::MainWindow *ui;
  void loadMap(QString path);
  QString imagePath;
  int currAzimuthAngle[4];
  int targetAzimuthAngle[4];
  int sizeWidth;
  int sizeHeight;
  bool windowFrameVisible;

  int beamWidth[4];
  bool rotorExist[4];
  QString rotorURL[4];
  QString rotorName[4];

  char currRotorIndex;
  QImage image;
  QNetworkAccessManager *networkManager;
private slots:
  void networkReplyFinished(QNetworkReply*);

  void pushButtonRotor1Clicked();
  void pushButtonRotor2Clicked();
  void pushButtonRotor3Clicked();
  void pushButtonRotor4Clicked();
  void pushButtonPreset1Clicked();
  void pushButtonPreset2Clicked();
  void pushButtonPreset3Clicked();
  void pushButtonPreset4Clicked();
  void pushButtonPreset5Clicked();
  void pushButtonSTOPClicked();
  void pushButtonRotateCWPressed();
  void pushButtonRotateCCWPressed();
  void pushButtonRotateCWReleased();
  void pushButtonRotateCCWReleased();

};

#endif // MAINWINDOW_H

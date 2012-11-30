#ifndef ROTATORDIALOG_H
#define ROTATORDIALOG_H
//
#include <QDialog>
#include "ui_rotatorwindow.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>

#include <QMouseEvent>
#include <QColor>
#include <QBrush>
#include <QSettings>

#define TARGET_DIR_BEAMWIDTH	1
#define TARGET_DIR_BEAMWIDTH_A1_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A1_COLOR	black

#define TARGET_DIR_BEAMWIDTH_A2_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A2_COLOR	blue

#define TARGET_DIR_BEAMWIDTH_A3_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A3_COLOR	darkGreen

#define TARGET_DIR_BEAMWIDTH_A4_COLOR	red
#define CURRENT_DIR_BEAMWIDTH_A4_COLOR	darkYellow

class RotatorDialog : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	RotatorDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	QPainter painter;
	void setTargetDir(int antIndex, int targetAngle);
	void setAntName(int antIndex, QString name);
	void loadBand(int bandIndex);
	void setRotatorAngle(int antIndex, int angle);
	char getRotationEventStatus(void);
	int getTargetDir(unsigned char antIndex);
	int getActiveAntenna();
	void setRotatorFlag(unsigned char antIndex, unsigned char flags);
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent ( QMouseEvent * event );
	QSettings settings;
private:
	void loadMap(QString path);
	QString imagePath;
	int currAzimuthAngle[4];
	int targetAzimuthAngle[4];
	int currAntIndex;
	int sizeWidth;
	int sizeHeight;
	QImage image;
	unsigned char rotationEventStatus;

	QString bandName;
	QString antName[4];
	bool antExist[4];
	bool antHasRotor[4];
	bool antBiDirectional[4];
	bool antFixed[4];
	int antBeamWidth[4];
	int antFixedAngle[4];
	bool antVerticalArray[4];
	int presetButtonValue[5];
private slots:

public slots:
	void pushButtonAnt1Clicked();
	void pushButtonAnt2Clicked();
	void pushButtonAnt3Clicked();
	void pushButtonAnt4Clicked();
	void pushButtonPreset1Clicked();
	void pushButtonPreset2Clicked();
	void pushButtonPreset3Clicked();
	void pushButtonPreset4Clicked();
	void pushButtonPreset5Clicked();
	void pushButtonSTOPClicked();
};
#endif






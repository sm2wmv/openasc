#ifndef KEYPAD_H
#define KEYPAD_H

#include <QDialog>
#include <QSettings>
#include "tcpclass.h"

namespace Ui {
    class Keypad;
}

class Keypad : public QDialog
{
    Q_OBJECT

public:
    void setCOMMPtr(TCPClass *comm);
    explicit Keypad(QWidget *parent = 0);
    ~Keypad();

private:
    Ui::Keypad *ui;
    TCPClass *TCPComm;
public slots:
    void pushButtonExt0Pressed(void);
    void pushButtonExt1Pressed(void);
    void pushButtonExt2Pressed(void);
    void pushButtonExt3Pressed(void);
    void pushButtonExt4Pressed(void);
    void pushButtonExt5Pressed(void);
    void pushButtonExt6Pressed(void);
    void pushButtonExt7Pressed(void);
    void pushButtonExt8Pressed(void);
    void pushButtonExt9Pressed(void);
    void pushButtonExtAPressed(void);
    void pushButtonExtBPressed(void);
    void pushButtonExtCPressed(void);
    void pushButtonExtDPressed(void);
    void pushButtonExtEPressed(void);
    void pushButtonExtFPressed(void);
    void pushButtonExtGPressed(void);
    void pushButtonExtE1Pressed(void);
    void pushButtonExtE2Pressed(void);
    void pushButtonExtE3Pressed(void);
    void pushButtonExtE4Pressed(void);
};

#endif // KEYPAD_H

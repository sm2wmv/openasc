#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include "../../../uC/wmv_bus/bus_commands.h"
#include "../../../uC/internal_comm_commands.h"

#include <QDialog>
#include <QMainWindow>

namespace Ui {
    class terminalDialog;
}

class terminalDialog : public QDialog {
    Q_OBJECT
public:
		terminalDialog(QWidget *parent = 0);
    ~terminalDialog();
protected:
    void changeEvent(QEvent *e);
private:
		Ui::terminalDialog *ui;
public slots:
	void lineEditCommandReturnPressed();
	void pushButtonSetAddressClicked();
};

#endif // TERMINALDIALOG_H

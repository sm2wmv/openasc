#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include "../../../uC/wmv_bus/bus_commands.h"
#include "../../../uC/internal_comm_commands.h"

#include <QDialog>
#include <QMainWindow>
#include "commclass.h"

namespace Ui {
    class terminalDialog;
}

class terminalDialog : public QDialog {
    Q_OBJECT
public:
		terminalDialog(QWidget *parent = 0);
    ~terminalDialog();
		void setSerialPtr(CommClass *serialPtr);
		CommClass *serial;
		void addNewMessage(struct_message message);
protected:
    void changeEvent(QEvent *e);
private:
		Ui::terminalDialog *ui;
public slots:
	void lineEditCommandReturnPressed();
	void pushButtonSetAddressClicked();
	void pushButtonClearClicked();
};

#endif // TERMINALDIALOG_H

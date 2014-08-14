#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include "../../../uC/wmv_bus/bus_commands.h"
#include "../../../uC/internal_comm_commands.h"
#include "../../../uC/remote_commands.h"

#include <QDialog>
#include <QMainWindow>
#include "tcpclass.h"

namespace Ui {
    class terminalDialog;
}

class terminalDialog : public QDialog {
    Q_OBJECT
public:
		terminalDialog(QWidget *parent = 0);
    ~terminalDialog();
		void addNewMessage(QByteArray message);
		void setCOMMPtr(TCPClass *tcpPtr);
protected:
    void changeEvent(QEvent *e);
private:
		Ui::terminalDialog *ui;
		void appendText(const QString &text);
		TCPClass *TCPComm;
public slots:
	void lineEditCommandReturnPressed();
	void pushButtonSetAddressClicked();
	void pushButtonClearClicked();
};

#endif // TERMINALDIALOG_H

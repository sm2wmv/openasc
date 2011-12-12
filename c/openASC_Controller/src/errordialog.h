#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

#include "commclass.h"

namespace Ui {
    class ErrorDialog;
}

class ErrorDialog : public QDialog {
    Q_OBJECT
public:
    ErrorDialog(QWidget *parent = 0);
    ~ErrorDialog();
		void setErrorList(int errorStatus);
		void setSerialPtr(CommClass *serialPtr);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::ErrorDialog *ui;
		CommClass *serial;
	public slots:
		void pushButtonClearErrorsClicked();
};

#endif // ERRORDIALOG_H

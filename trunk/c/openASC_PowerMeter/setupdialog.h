#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class SetupDialog;
}

class SetupDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SetupDialog)
public:
    explicit SetupDialog(QWidget *parent = 0);
    virtual ~SetupDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SetupDialog *m_ui;
		void readSettings();
		void saveSettings();
};

#endif // SETUPDIALOG_H

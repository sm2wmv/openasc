#include "terminaldialog.h"
#include "ui_terminaldialog.h"

QString strTextEdit;
int setAddress = 0;

void terminalDialog::lineEditCommandReturnPressed(){
	if (ui->lineEditCommand->text().length() > 0) {
		if (setAddress != 0) {
			ui->textEdit->append("<< " + ui->lineEditCommand->text());

		}
		else
			ui->textEdit->append("ERROR: No address is set");
	}
}

void terminalDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void terminalDialog::pushButtonSetAddressClicked() {
}

terminalDialog::~terminalDialog()
{
		delete ui;
}

terminalDialog::terminalDialog(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::terminalDialog)
{
		ui->setupUi(this);

		strTextEdit.clear();

		connect(ui->lineEditCommand, SIGNAL(returnPressed()), this, SLOT(lineEditCommandReturnPressed()));
		connect(ui->pushButtonSetAddress, SIGNAL(clicked()), this, SLOT(pushButtonSetAddressClicked()));
}

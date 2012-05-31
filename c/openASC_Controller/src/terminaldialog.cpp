#include "terminaldialog.h"
#include "ui_terminaldialog.h"

QString strTextEdit;
int setAddress = 0;

void terminalDialog::lineEditCommandReturnPressed(){
	if (ui->lineEditCommand->text().length() > 0) {
		if (setAddress != 0) {
			ui->textEdit->setTextColor(Qt::darkGreen);
			ui->textEdit->append("<< " + ui->lineEditCommand->text());

			if (serial->isOpen()) {
				if (ui->lineEditCommand->text().length() < 15) {
					serial->addTXMessage(INT_COMM_PC_SEND_TO_ADDR,ui->lineEditCommand->text().length(), ui->lineEditCommand->text());
					ui->lineEditCommand->clear();
				}
				else {
					ui->textEdit->setTextColor(Qt::darkRed);
					ui->textEdit->append("ERROR: Too long argument");
				}
			}
		}
		else {
			ui->textEdit->setTextColor(Qt::darkRed);
			ui->textEdit->append("ERROR: No address is set");
		}
	}
}

void terminalDialog::addNewMessage(struct_message message) {
	for (unsigned char i=0;i<message.length;i++) {
		if (message.data[i] != 13) {
			if (message.data[i] != '\n')
				strTextEdit.append(message.data[i]);
		}
		else {
			ui->textEdit->setTextColor(Qt::black);
			ui->textEdit->append(">> " + strTextEdit);
			strTextEdit.clear();
		}
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
	if (serial->isOpen()) {
		serial->addTXMessage(INT_COMM_PC_CONNECT_TO_ADDR,ui->spinBoxAddress->value());

		ui->textEdit->setTextColor(Qt::darkCyan);
		ui->textEdit->append(">> Connecting to address: " + ui->spinBoxAddress->text());
		setAddress = ui->spinBoxAddress->value();
	}
	else {
		ui->textEdit->setTextColor(Qt::darkRed);
		ui->textEdit->append("ERROR: Serial device is not open");
		setAddress = 0;
	}
}

void terminalDialog::pushButtonClearClicked() {
	ui->textEdit->clear();
}

void terminalDialog::setSerialPtr(CommClass *serialPtr) {
	serial = serialPtr;
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
		connect(ui->pushButtonClear, SIGNAL(clicked()), this, SLOT(pushButtonClearClicked()));
}

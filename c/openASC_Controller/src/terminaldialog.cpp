#include "terminaldialog.h"
#include "ui_terminaldialog.h"

int setAddress = 0;

void terminalDialog::lineEditCommandReturnPressed(){
	if (ui->lineEditCommand->text().length() > 0) {
		if (setAddress != 0) {
			ui->textEdit->setTextColor(Qt::darkGreen);
			appendText(ui->lineEditCommand->text() + "\n");

			if (serial->isOpen()) {
				if (ui->lineEditCommand->text().length() < 15) {
					serial->addTXMessage(INT_COMM_PC_SEND_TO_ADDR,ui->lineEditCommand->text().length(), ui->lineEditCommand->text());
					ui->lineEditCommand->clear();
				}
				else {
					ui->textEdit->setTextColor(Qt::darkRed);
					appendText(">> ERROR: Too long argument\n");
				}
			}
		}
		else {
			ui->textEdit->setTextColor(Qt::darkRed);
			appendText(">> ERROR: No address is set\n");
		}
	}
}

void terminalDialog::addNewMessage(struct_message message) {
	QString strTextEdit;
	for (unsigned char i=0;i<message.length;i++) {
		strTextEdit.append(message.data[i]);
	}
	ui->textEdit->setTextColor(Qt::black);
	appendText(strTextEdit);
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
		appendText(">> Connecting to address: " + ui->spinBoxAddress->text() + "\n");
		setAddress = ui->spinBoxAddress->value();
                  // Send empty command to force the remote to send a prompt
		serial->addTXMessage(INT_COMM_PC_SEND_TO_ADDR, 0, "");
	}
	else {
		ui->textEdit->setTextColor(Qt::darkRed);
		appendText(">> ERROR: Serial device is not open\n");
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

		connect(ui->lineEditCommand, SIGNAL(returnPressed()), this, SLOT(lineEditCommandReturnPressed()));
		connect(ui->pushButtonSetAddress, SIGNAL(clicked()), this, SLOT(pushButtonSetAddressClicked()));
		connect(ui->pushButtonClear, SIGNAL(clicked()), this, SLOT(pushButtonClearClicked()));
}

void terminalDialog::appendText(const QString &text)
{
	ui->textEdit->insertPlainText(text);
	ui->textEdit->ensureCursorVisible();
}


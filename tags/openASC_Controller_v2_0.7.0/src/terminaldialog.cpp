#include "terminaldialog.h"
#include "ui_terminaldialog.h"

int setAddress = 0;

void terminalDialog::lineEditCommandReturnPressed(){
	if (ui->lineEditCommand->text().length() > 0) {
		if (setAddress != 0) {
			ui->textEdit->setTextColor(Qt::darkGreen);
			appendText(ui->lineEditCommand->text() + "\n");

			if (TCPComm->isConnected()) {
				if (ui->lineEditCommand->text().length() < 15) {
					TCPComm->addTXMessage(REMOTE_COMMAND_TERMINAL_DATA,ui->lineEditCommand->text().length(),ui->lineEditCommand->text());
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

void terminalDialog::addNewMessage(QByteArray message) {
	QString strTextEdit;
	for (unsigned char i=3;i<message.length();i++) {
		strTextEdit.append(message.at(i));
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
	if (TCPComm->isConnected()) {
		TCPComm->addTXMessage(REMOTE_COMMAND_TERMINAL_CONNECT,ui->spinBoxAddress->value());

		ui->textEdit->setTextColor(Qt::darkCyan);
		appendText(">> Connecting to address: " + ui->spinBoxAddress->text() + "\n");
		setAddress = ui->spinBoxAddress->value();

		// Send empty command to force the remote to send a prompt
		TCPComm->addTXMessage(REMOTE_COMMAND_TERMINAL_DATA,0);
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

void terminalDialog::setCOMMPtr(TCPClass *tcpPtr) {
	TCPComm = tcpPtr;
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
//

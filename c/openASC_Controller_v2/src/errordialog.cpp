#include "errordialog.h"
#include "ui_errordialog.h"

#include "../../../uC/remote_commands.h"
#include "../../../uC/front_panel/errors.h"

void ErrorDialog::setErrorList(int errorStatus) {
	ui->textEditErrors->clear();

	if (errorStatus & (1<<ERROR_TYPE_BUS_RESEND))
		ui->textEditErrors->append("BUS RESEND");

	if (errorStatus & (1<<ERROR_TYPE_BUS_SYNC))
		ui->textEditErrors->append("BUS SYNC TIMEOUT");

	if (errorStatus & (1<<ERROR_TYPE_BUS_TX_QUEUE_FULL))
		ui->textEditErrors->append("BUS TX QUEUE FULL");

	if (errorStatus & (1<<ERROR_TYPE_BUS_RX_QUEUE_FULL))
		ui->textEditErrors->append("BUS RX QUEUE FULL");

	if (errorStatus & (1<<ERROR_TYPE_INT_COMM_RESEND))
		ui->textEditErrors->append("INTERNAL COMM RESEND");

	if (errorStatus & (1<<ERROR_TYPE_ANT_PING_TIMEOUT))
		ui->textEditErrors->append("ANTENNA PING TIMEOUT");

	if (errorStatus & (1<<ERROR_TYPE_BAND_PING_TIMEOUT))
		ui->textEditErrors->append("BAND PING TIMEOUT");

	if (errorStatus & (1<<ERROR_TYPE_HIGH_VSWR))
		ui->textEditErrors->append("HIGH VSWR");

	if (errorStatus & (1<<ERROR_TYPE_BAND_IN_USE))
		ui->textEditErrors->append("BAND IS ALREADY IN USE");

	if (errorStatus & (1<<ERROR_TYPE_INT_COMM_TX_FULL))
		ui->textEditErrors->append("INTERNAL COMM TX QUEUE FULL");

	if (errorStatus & (1<<ERROR_TYPE_INT_COMM_RX_FULL))
		ui->textEditErrors->append("INTERNAL COMM RX QUEUE FULL");

	if (errorStatus & (1<<ERROR_TYPE_EVENT_QUEUE_FULL))
		ui->textEditErrors->append("EVENT QUEUE FULL");
}

void ErrorDialog::pushButtonClearErrorsClicked(){
}

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);

		connect(ui->pushButtonClearErrors,SIGNAL(clicked()),this,SLOT(pushButtonClearErrorsClicked()));
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::changeEvent(QEvent *e)
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

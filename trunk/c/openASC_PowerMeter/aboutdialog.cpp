#include "aboutdialog.h"
#include "ui_aboutdialog.h"

void AboutDialog::pushButtonClosePressed() {
		this->hide();
}

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutDialog)
{
    m_ui->setupUi(this);

	connect(m_ui->pushButtonClose, SIGNAL(pressed()), this, SLOT(pushButtonClosePressed()));
}

AboutDialog::~AboutDialog()
{
    delete m_ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

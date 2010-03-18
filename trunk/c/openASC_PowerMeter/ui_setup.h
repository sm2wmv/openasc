/********************************************************************************
** Form generated from reading ui file 'setup.ui'
**
** Created: Sat Mar 13 01:52:48 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_setupDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *setupDialog)
    {
        if (setupDialog->objectName().isEmpty())
            setupDialog->setObjectName(QString::fromUtf8("setupDialog"));
        setupDialog->resize(398, 298);
        buttonBox = new QDialogButtonBox(setupDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 260, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(setupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), setupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), setupDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(setupDialog);
    } // setupUi

    void retranslateUi(QDialog *setupDialog)
    {
        setupDialog->setWindowTitle(QApplication::translate("setupDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(setupDialog);
    } // retranslateUi

};

namespace Ui {
    class setupDialog: public Ui_setupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H

/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created: Mon Sep 12 12:23:49 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;
    QGroupBox *groupBoxNetwork;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *labelIP;
    QLineEdit *lineEditIP;
    QLabel *labelPort;
    QSpinBox *spinBoxPort;
    QGroupBox *groupBoxCOM;
    QWidget *layoutWidget1;
    QFormLayout *formLayoutCOM;
    QLabel *labelDeviceName;
    QLineEdit *lineEditDeviceName;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(303, 218);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SettingsDialog->sizePolicy().hasHeightForWidth());
        SettingsDialog->setSizePolicy(sizePolicy);
        pushButtonOK = new QPushButton(SettingsDialog);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(80, 180, 103, 30));
        pushButtonCancel = new QPushButton(SettingsDialog);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(190, 180, 103, 30));
        groupBoxNetwork = new QGroupBox(SettingsDialog);
        groupBoxNetwork->setObjectName(QString::fromUtf8("groupBoxNetwork"));
        groupBoxNetwork->setGeometry(QRect(10, 5, 285, 95));
        groupBoxNetwork->setCheckable(true);
        groupBoxNetwork->setChecked(false);
        layoutWidget = new QWidget(groupBoxNetwork);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(15, 20, 258, 63));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        labelIP = new QLabel(layoutWidget);
        labelIP->setObjectName(QString::fromUtf8("labelIP"));
        labelIP->setLayoutDirection(Qt::LeftToRight);
        labelIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, labelIP);

        lineEditIP = new QLineEdit(layoutWidget);
        lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditIP);

        labelPort = new QLabel(layoutWidget);
        labelPort->setObjectName(QString::fromUtf8("labelPort"));
        labelPort->setLayoutDirection(Qt::LeftToRight);
        labelPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPort);

        spinBoxPort = new QSpinBox(layoutWidget);
        spinBoxPort->setObjectName(QString::fromUtf8("spinBoxPort"));
        spinBoxPort->setMaximum(65000);
        spinBoxPort->setValue(5660);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinBoxPort);

        groupBoxCOM = new QGroupBox(SettingsDialog);
        groupBoxCOM->setObjectName(QString::fromUtf8("groupBoxCOM"));
        groupBoxCOM->setGeometry(QRect(10, 105, 285, 60));
        groupBoxCOM->setCheckable(true);
        groupBoxCOM->setChecked(false);
        layoutWidget1 = new QWidget(groupBoxCOM);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(15, 20, 260, 29));
        formLayoutCOM = new QFormLayout(layoutWidget1);
        formLayoutCOM->setObjectName(QString::fromUtf8("formLayoutCOM"));
        formLayoutCOM->setContentsMargins(0, 0, 0, 0);
        labelDeviceName = new QLabel(layoutWidget1);
        labelDeviceName->setObjectName(QString::fromUtf8("labelDeviceName"));
        labelDeviceName->setLayoutDirection(Qt::LeftToRight);
        labelDeviceName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayoutCOM->setWidget(0, QFormLayout::LabelRole, labelDeviceName);

        lineEditDeviceName = new QLineEdit(layoutWidget1);
        lineEditDeviceName->setObjectName(QString::fromUtf8("lineEditDeviceName"));

        formLayoutCOM->setWidget(0, QFormLayout::FieldRole, lineEditDeviceName);


        retranslateUi(SettingsDialog);
        QObject::connect(groupBoxNetwork, SIGNAL(clicked(bool)), SettingsDialog, SLOT(groupboxNetworkClicked(bool)));
        QObject::connect(groupBoxCOM, SIGNAL(clicked(bool)), SettingsDialog, SLOT(groupBoxCOMClicked(bool)));
        QObject::connect(pushButtonOK, SIGNAL(pressed()), SettingsDialog, SLOT(btnOKClicked()));
        QObject::connect(pushButtonCancel, SIGNAL(pressed()), SettingsDialog, SLOT(btnCancelClicked()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("SettingsDialog", "OK", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("SettingsDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        groupBoxNetwork->setTitle(QApplication::translate("SettingsDialog", " Network ", 0, QApplication::UnicodeUTF8));
        labelIP->setText(QApplication::translate("SettingsDialog", "Address:", 0, QApplication::UnicodeUTF8));
        labelPort->setText(QApplication::translate("SettingsDialog", "Port:", 0, QApplication::UnicodeUTF8));
        groupBoxCOM->setTitle(QApplication::translate("SettingsDialog", "COM", 0, QApplication::UnicodeUTF8));
        labelDeviceName->setText(QApplication::translate("SettingsDialog", "Device name:", 0, QApplication::UnicodeUTF8));
        lineEditDeviceName->setText(QApplication::translate("SettingsDialog", "/dev/ttyUSB1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H

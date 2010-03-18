/********************************************************************************
** Form generated from reading ui file 'setupdialog.ui'
**
** Created: Thu Mar 18 23:39:53 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETUPDIALOG_H
#define UI_SETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabConnection;
    QGroupBox *groupBoxNetwork;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout;
    QLabel *labelNetworkAddress;
    QLineEdit *lineEditNetworkAddress;
    QLabel *labelNetworkPort;
    QSpinBox *spinBoxNetworkPort;
    QGroupBox *groupBoxUSB;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QLabel *labelUSBDeviceName;
    QLineEdit *lineEditUSBDeviceName;
    QLabel *labelUSBBusPort;
    QSpinBox *spinBoxBusPort;
    QWidget *tabPickup;
    QGroupBox *groupBoxPickupNetwork;
    QLabel *labelPickupAddress;
    QSpinBox *spinBoxPickupAddress;
    QWidget *tabMisc;
    QGroupBox *groupBoxDisplay;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout_3;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBoxDisplayTextUpdateRate;
    QSpinBox *spinBoxDisplayProgressBarUpdateRate;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBoxAlarmSWR;
    QLabel *label_4;
    QSpinBox *spinBoxAlarmFwdPower;
    QLabel *label_5;
    QSpinBox *spinBoxAlarmRefPower;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SetupDialog)
    {
        if (SetupDialog->objectName().isEmpty())
            SetupDialog->setObjectName(QString::fromUtf8("SetupDialog"));
        SetupDialog->resize(449, 427);
        verticalLayout = new QVBoxLayout(SetupDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(SetupDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabConnection = new QWidget();
        tabConnection->setObjectName(QString::fromUtf8("tabConnection"));
        groupBoxNetwork = new QGroupBox(tabConnection);
        groupBoxNetwork->setObjectName(QString::fromUtf8("groupBoxNetwork"));
        groupBoxNetwork->setGeometry(QRect(10, 10, 208, 157));
        groupBoxNetwork->setCheckable(true);
        verticalLayout_3 = new QVBoxLayout(groupBoxNetwork);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelNetworkAddress = new QLabel(groupBoxNetwork);
        labelNetworkAddress->setObjectName(QString::fromUtf8("labelNetworkAddress"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelNetworkAddress);

        lineEditNetworkAddress = new QLineEdit(groupBoxNetwork);
        lineEditNetworkAddress->setObjectName(QString::fromUtf8("lineEditNetworkAddress"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lineEditNetworkAddress);

        labelNetworkPort = new QLabel(groupBoxNetwork);
        labelNetworkPort->setObjectName(QString::fromUtf8("labelNetworkPort"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelNetworkPort);

        spinBoxNetworkPort = new QSpinBox(groupBoxNetwork);
        spinBoxNetworkPort->setObjectName(QString::fromUtf8("spinBoxNetworkPort"));

        formLayout->setWidget(3, QFormLayout::LabelRole, spinBoxNetworkPort);


        verticalLayout_3->addLayout(formLayout);

        groupBoxUSB = new QGroupBox(tabConnection);
        groupBoxUSB->setObjectName(QString::fromUtf8("groupBoxUSB"));
        groupBoxUSB->setGeometry(QRect(10, 175, 208, 157));
        groupBoxUSB->setCheckable(true);
        verticalLayout_2 = new QVBoxLayout(groupBoxUSB);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        labelUSBDeviceName = new QLabel(groupBoxUSB);
        labelUSBDeviceName->setObjectName(QString::fromUtf8("labelUSBDeviceName"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelUSBDeviceName);

        lineEditUSBDeviceName = new QLineEdit(groupBoxUSB);
        lineEditUSBDeviceName->setObjectName(QString::fromUtf8("lineEditUSBDeviceName"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, lineEditUSBDeviceName);

        labelUSBBusPort = new QLabel(groupBoxUSB);
        labelUSBBusPort->setObjectName(QString::fromUtf8("labelUSBBusPort"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelUSBBusPort);

        spinBoxBusPort = new QSpinBox(groupBoxUSB);
        spinBoxBusPort->setObjectName(QString::fromUtf8("spinBoxBusPort"));
        spinBoxBusPort->setMinimum(1);
        spinBoxBusPort->setMaximum(255);
        spinBoxBusPort->setValue(5);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, spinBoxBusPort);


        verticalLayout_2->addLayout(formLayout_2);

        tabWidget->addTab(tabConnection, QString());
        tabPickup = new QWidget();
        tabPickup->setObjectName(QString::fromUtf8("tabPickup"));
        groupBoxPickupNetwork = new QGroupBox(tabPickup);
        groupBoxPickupNetwork->setObjectName(QString::fromUtf8("groupBoxPickupNetwork"));
        groupBoxPickupNetwork->setGeometry(QRect(10, 10, 266, 151));
        labelPickupAddress = new QLabel(groupBoxPickupNetwork);
        labelPickupAddress->setObjectName(QString::fromUtf8("labelPickupAddress"));
        labelPickupAddress->setGeometry(QRect(15, 30, 116, 19));
        spinBoxPickupAddress = new QSpinBox(groupBoxPickupNetwork);
        spinBoxPickupAddress->setObjectName(QString::fromUtf8("spinBoxPickupAddress"));
        spinBoxPickupAddress->setGeometry(QRect(15, 50, 63, 29));
        tabWidget->addTab(tabPickup, QString());
        tabMisc = new QWidget();
        tabMisc->setObjectName(QString::fromUtf8("tabMisc"));
        groupBoxDisplay = new QGroupBox(tabMisc);
        groupBoxDisplay->setObjectName(QString::fromUtf8("groupBoxDisplay"));
        groupBoxDisplay->setGeometry(QRect(10, 10, 133, 157));
        verticalLayout_4 = new QVBoxLayout(groupBoxDisplay);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        label = new QLabel(groupBoxDisplay);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(groupBoxDisplay);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_3->setWidget(2, QFormLayout::SpanningRole, label_2);

        spinBoxDisplayTextUpdateRate = new QSpinBox(groupBoxDisplay);
        spinBoxDisplayTextUpdateRate->setObjectName(QString::fromUtf8("spinBoxDisplayTextUpdateRate"));
        spinBoxDisplayTextUpdateRate->setMaximum(1000);
        spinBoxDisplayTextUpdateRate->setValue(250);

        formLayout_3->setWidget(1, QFormLayout::SpanningRole, spinBoxDisplayTextUpdateRate);

        spinBoxDisplayProgressBarUpdateRate = new QSpinBox(groupBoxDisplay);
        spinBoxDisplayProgressBarUpdateRate->setObjectName(QString::fromUtf8("spinBoxDisplayProgressBarUpdateRate"));
        spinBoxDisplayProgressBarUpdateRate->setMaximum(1000);
        spinBoxDisplayProgressBarUpdateRate->setValue(50);

        formLayout_3->setWidget(3, QFormLayout::SpanningRole, spinBoxDisplayProgressBarUpdateRate);


        verticalLayout_4->addLayout(formLayout_3);

        groupBox = new QGroupBox(tabMisc);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(155, 10, 143, 217));
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_3);

        doubleSpinBoxAlarmSWR = new QDoubleSpinBox(groupBox);
        doubleSpinBoxAlarmSWR->setObjectName(QString::fromUtf8("doubleSpinBoxAlarmSWR"));
        doubleSpinBoxAlarmSWR->setDecimals(1);
        doubleSpinBoxAlarmSWR->setMinimum(1);
        doubleSpinBoxAlarmSWR->setMaximum(10);
        doubleSpinBoxAlarmSWR->setSingleStep(0.1);
        doubleSpinBoxAlarmSWR->setValue(2);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, doubleSpinBoxAlarmSWR);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_4);

        spinBoxAlarmFwdPower = new QSpinBox(groupBox);
        spinBoxAlarmFwdPower->setObjectName(QString::fromUtf8("spinBoxAlarmFwdPower"));
        spinBoxAlarmFwdPower->setMaximum(15000);
        spinBoxAlarmFwdPower->setSingleStep(100);
        spinBoxAlarmFwdPower->setValue(1500);

        formLayout_4->setWidget(3, QFormLayout::LabelRole, spinBoxAlarmFwdPower);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_5);

        spinBoxAlarmRefPower = new QSpinBox(groupBox);
        spinBoxAlarmRefPower->setObjectName(QString::fromUtf8("spinBoxAlarmRefPower"));
        spinBoxAlarmRefPower->setMaximum(1500);
        spinBoxAlarmRefPower->setSingleStep(10);
        spinBoxAlarmRefPower->setValue(150);

        formLayout_4->setWidget(5, QFormLayout::LabelRole, spinBoxAlarmRefPower);


        verticalLayout_5->addLayout(formLayout_4);

        tabWidget->addTab(tabMisc, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(SetupDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SetupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetupDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SetupDialog);
    } // setupUi

    void retranslateUi(QDialog *SetupDialog)
    {
        SetupDialog->setWindowTitle(QApplication::translate("SetupDialog", "Setup", 0, QApplication::UnicodeUTF8));
        SetupDialog->setStyleSheet(QString());
        groupBoxNetwork->setTitle(QApplication::translate("SetupDialog", "Network", 0, QApplication::UnicodeUTF8));
        labelNetworkAddress->setText(QApplication::translate("SetupDialog", "Address", 0, QApplication::UnicodeUTF8));
        labelNetworkPort->setText(QApplication::translate("SetupDialog", "Port", 0, QApplication::UnicodeUTF8));
        groupBoxUSB->setTitle(QApplication::translate("SetupDialog", "USB port", 0, QApplication::UnicodeUTF8));
        labelUSBDeviceName->setText(QApplication::translate("SetupDialog", "Device name", 0, QApplication::UnicodeUTF8));
        lineEditUSBDeviceName->setText(QApplication::translate("SetupDialog", "/dev/ttyUSB0", 0, QApplication::UnicodeUTF8));
        labelUSBBusPort->setText(QApplication::translate("SetupDialog", "Client address", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabConnection), QApplication::translate("SetupDialog", "Connection", 0, QApplication::UnicodeUTF8));
        groupBoxPickupNetwork->setTitle(QApplication::translate("SetupDialog", "Network", 0, QApplication::UnicodeUTF8));
        labelPickupAddress->setText(QApplication::translate("SetupDialog", "Pickup address", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabPickup), QApplication::translate("SetupDialog", "Pickup", 0, QApplication::UnicodeUTF8));
        groupBoxDisplay->setTitle(QApplication::translate("SetupDialog", "Display update", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SetupDialog", "Text", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SetupDialog", "Progressbar", 0, QApplication::UnicodeUTF8));
        spinBoxDisplayTextUpdateRate->setSuffix(QApplication::translate("SetupDialog", "ms", 0, QApplication::UnicodeUTF8));
        spinBoxDisplayProgressBarUpdateRate->setSuffix(QApplication::translate("SetupDialog", "ms", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SetupDialog", "Alarm", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SetupDialog", "SWR", 0, QApplication::UnicodeUTF8));
        doubleSpinBoxAlarmSWR->setSuffix(QApplication::translate("SetupDialog", ":1", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SetupDialog", "Forward power", 0, QApplication::UnicodeUTF8));
        spinBoxAlarmFwdPower->setSuffix(QApplication::translate("SetupDialog", "W", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SetupDialog", "Relected power", 0, QApplication::UnicodeUTF8));
        spinBoxAlarmRefPower->setSuffix(QApplication::translate("SetupDialog", "W", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabMisc), QApplication::translate("SetupDialog", "Misc", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SetupDialog);
    } // retranslateUi

};

namespace Ui {
    class SetupDialog: public Ui_SetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPDIALOG_H

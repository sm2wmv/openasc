/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created: Fri 30. Dec 02:55:48 2011
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QPushButton *pushButtonClose;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *labelPowerMeter;
    QLabel *labelBy;
    QLabel *labelVersion;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->setWindowModality(Qt::ApplicationModal);
        AboutDialog->resize(350, 227);
        pushButtonClose = new QPushButton(AboutDialog);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(140, 175, 80, 29));
        widget = new QWidget(AboutDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(60, 60, 237, 92));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        labelPowerMeter = new QLabel(widget);
        labelPowerMeter->setObjectName(QString::fromUtf8("labelPowerMeter"));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        labelPowerMeter->setFont(font);
        labelPowerMeter->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelPowerMeter);

        labelBy = new QLabel(widget);
        labelBy->setObjectName(QString::fromUtf8("labelBy"));
        labelBy->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelBy);

        labelVersion = new QLabel(widget);
        labelVersion->setObjectName(QString::fromUtf8("labelVersion"));
        labelVersion->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelVersion);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About...", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("AboutDialog", "Close", 0, QApplication::UnicodeUTF8));
        labelPowerMeter->setText(QApplication::translate("AboutDialog", "PowerMeter ", 0, QApplication::UnicodeUTF8));
        labelBy->setText(QApplication::translate("AboutDialog", "By SM2WMV (SJ2W)", 0, QApplication::UnicodeUTF8));
        labelVersion->setText(QApplication::translate("AboutDialog", "Version 0.1b", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H

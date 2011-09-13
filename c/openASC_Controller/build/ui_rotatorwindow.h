/********************************************************************************
** Form generated from reading UI file 'rotatorwindow.ui'
**
** Created: Mon Sep 12 14:08:20 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROTATORWINDOW_H
#define UI_ROTATORWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *imageLabel;
    QGroupBox *groupBoxAnt1;
    QLabel *labelAnt1Dir;
    QLabel *labelAnt1Title;
    QLabel *labelAnt1Status;
    QGroupBox *groupBoxAnt2;
    QLabel *labelAnt2Dir;
    QLabel *labelAnt2Title;
    QLabel *labelAnt2Status;
    QGroupBox *groupBoxAnt3;
    QLabel *labelAnt3Dir;
    QLabel *labelAnt3Title;
    QLabel *labelAnt3Status;
    QGroupBox *groupBoxAnt4;
    QLabel *labelAnt4Dir;
    QLabel *labelAnt4Title;
    QLabel *labelAnt4Status;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setWindowModality(Qt::NonModal);
        Dialog->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        imageLabel = new QLabel(Dialog);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(0, 0, 600, 600));
        sizePolicy.setHeightForWidth(imageLabel->sizePolicy().hasHeightForWidth());
        imageLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(8);
        font.setStyleStrategy(QFont::PreferDefault);
        imageLabel->setFont(font);
        imageLabel->setStyleSheet(QString::fromUtf8(""));
        imageLabel->setFrameShape(QFrame::Box);
        imageLabel->setLineWidth(1);
        imageLabel->setScaledContents(false);
        imageLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        imageLabel->setIndent(0);
        groupBoxAnt1 = new QGroupBox(Dialog);
        groupBoxAnt1->setObjectName(QString::fromUtf8("groupBoxAnt1"));
        groupBoxAnt1->setGeometry(QRect(610, 10, 181, 96));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setWeight(50);
        groupBoxAnt1->setFont(font1);
        groupBoxAnt1->setAutoFillBackground(true);
        labelAnt1Dir = new QLabel(groupBoxAnt1);
        labelAnt1Dir->setObjectName(QString::fromUtf8("labelAnt1Dir"));
        labelAnt1Dir->setGeometry(QRect(10, 40, 86, 16));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        labelAnt1Dir->setFont(font2);
        labelAnt1Title = new QLabel(groupBoxAnt1);
        labelAnt1Title->setObjectName(QString::fromUtf8("labelAnt1Title"));
        labelAnt1Title->setGeometry(QRect(10, 10, 161, 16));
        labelAnt1Title->setFont(font2);
        labelAnt1Status = new QLabel(groupBoxAnt1);
        labelAnt1Status->setObjectName(QString::fromUtf8("labelAnt1Status"));
        labelAnt1Status->setGeometry(QRect(10, 65, 156, 21));
        QFont font3;
        font3.setPointSize(12);
        labelAnt1Status->setFont(font3);
        groupBoxAnt2 = new QGroupBox(Dialog);
        groupBoxAnt2->setObjectName(QString::fromUtf8("groupBoxAnt2"));
        groupBoxAnt2->setGeometry(QRect(610, 110, 181, 96));
        groupBoxAnt2->setFont(font1);
        groupBoxAnt2->setAutoFillBackground(true);
        labelAnt2Dir = new QLabel(groupBoxAnt2);
        labelAnt2Dir->setObjectName(QString::fromUtf8("labelAnt2Dir"));
        labelAnt2Dir->setGeometry(QRect(10, 40, 86, 16));
        labelAnt2Dir->setFont(font2);
        labelAnt2Title = new QLabel(groupBoxAnt2);
        labelAnt2Title->setObjectName(QString::fromUtf8("labelAnt2Title"));
        labelAnt2Title->setGeometry(QRect(10, 10, 161, 16));
        labelAnt2Title->setFont(font2);
        labelAnt2Status = new QLabel(groupBoxAnt2);
        labelAnt2Status->setObjectName(QString::fromUtf8("labelAnt2Status"));
        labelAnt2Status->setGeometry(QRect(10, 65, 156, 21));
        labelAnt2Status->setFont(font3);
        groupBoxAnt3 = new QGroupBox(Dialog);
        groupBoxAnt3->setObjectName(QString::fromUtf8("groupBoxAnt3"));
        groupBoxAnt3->setGeometry(QRect(610, 210, 181, 96));
        groupBoxAnt3->setFont(font1);
        groupBoxAnt3->setAutoFillBackground(true);
        labelAnt3Dir = new QLabel(groupBoxAnt3);
        labelAnt3Dir->setObjectName(QString::fromUtf8("labelAnt3Dir"));
        labelAnt3Dir->setGeometry(QRect(10, 40, 86, 16));
        labelAnt3Dir->setFont(font2);
        labelAnt3Title = new QLabel(groupBoxAnt3);
        labelAnt3Title->setObjectName(QString::fromUtf8("labelAnt3Title"));
        labelAnt3Title->setGeometry(QRect(10, 10, 156, 16));
        labelAnt3Title->setFont(font2);
        labelAnt3Status = new QLabel(groupBoxAnt3);
        labelAnt3Status->setObjectName(QString::fromUtf8("labelAnt3Status"));
        labelAnt3Status->setGeometry(QRect(10, 65, 156, 21));
        labelAnt3Status->setFont(font3);
        groupBoxAnt4 = new QGroupBox(Dialog);
        groupBoxAnt4->setObjectName(QString::fromUtf8("groupBoxAnt4"));
        groupBoxAnt4->setGeometry(QRect(610, 310, 181, 96));
        groupBoxAnt4->setFont(font1);
        groupBoxAnt4->setAutoFillBackground(true);
        labelAnt4Dir = new QLabel(groupBoxAnt4);
        labelAnt4Dir->setObjectName(QString::fromUtf8("labelAnt4Dir"));
        labelAnt4Dir->setGeometry(QRect(10, 40, 86, 16));
        labelAnt4Dir->setFont(font2);
        labelAnt4Title = new QLabel(groupBoxAnt4);
        labelAnt4Title->setObjectName(QString::fromUtf8("labelAnt4Title"));
        labelAnt4Title->setGeometry(QRect(10, 10, 160, 16));
        labelAnt4Title->setFont(font2);
        labelAnt4Status = new QLabel(groupBoxAnt4);
        labelAnt4Status->setObjectName(QString::fromUtf8("labelAnt4Status"));
        labelAnt4Status->setGeometry(QRect(10, 65, 156, 21));
        labelAnt4Status->setFont(font3);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Rotators", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
        groupBoxAnt1->setTitle(QString());
        labelAnt1Dir->setText(QApplication::translate("Dialog", "305\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt1Title->setText(QApplication::translate("Dialog", "6/6/6el", 0, QApplication::UnicodeUTF8));
        labelAnt1Status->setText(QApplication::translate("Dialog", "Rotating CW to 250\302\260", 0, QApplication::UnicodeUTF8));
        groupBoxAnt2->setTitle(QString());
        labelAnt2Dir->setText(QApplication::translate("Dialog", "230\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt2Title->setText(QApplication::translate("Dialog", "5/5el", 0, QApplication::UnicodeUTF8));
        labelAnt2Status->setText(QApplication::translate("Dialog", "Stopped", 0, QApplication::UnicodeUTF8));
        groupBoxAnt3->setTitle(QString());
        labelAnt3Dir->setText(QApplication::translate("Dialog", "180\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt3Title->setText(QApplication::translate("Dialog", "4el mono", 0, QApplication::UnicodeUTF8));
        labelAnt3Status->setText(QApplication::translate("Dialog", "Stopped", 0, QApplication::UnicodeUTF8));
        groupBoxAnt4->setTitle(QString());
        labelAnt4Dir->setText(QString());
        labelAnt4Title->setText(QString());
        labelAnt4Status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROTATORWINDOW_H

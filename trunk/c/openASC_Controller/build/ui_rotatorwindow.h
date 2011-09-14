/********************************************************************************
** Form generated from reading UI file 'rotatorwindow.ui'
**
** Created: Wed Sep 14 21:24:00 2011
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
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *imageLabel;
    QLabel *labelDebug;
    QPushButton *pushButton;
    QLabel *labelAnt1Status;
    QLabel *labelAnt1Dir;
    QLabel *labelAnt1Title;
    QLabel *labelX;
    QLabel *labelY;
    QLabel *labelAnt2Dir;
    QLabel *labelAnt2Title;
    QLabel *labelAnt2Status;
    QLabel *labelAnt3Title;
    QLabel *labelAnt3Status;
    QLabel *labelAnt3Dir;
    QLabel *labelAnt4Status;
    QLabel *labelAnt4Dir;
    QLabel *labelAnt4Title;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;

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
        labelDebug = new QLabel(Dialog);
        labelDebug->setObjectName(QString::fromUtf8("labelDebug"));
        labelDebug->setGeometry(QRect(615, 420, 52, 14));
        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(650, 485, 80, 25));
        labelAnt1Status = new QLabel(Dialog);
        labelAnt1Status->setObjectName(QString::fromUtf8("labelAnt1Status"));
        labelAnt1Status->setGeometry(QRect(620, 70, 156, 21));
        QFont font1;
        font1.setPointSize(12);
        labelAnt1Status->setFont(font1);
        labelAnt1Dir = new QLabel(Dialog);
        labelAnt1Dir->setObjectName(QString::fromUtf8("labelAnt1Dir"));
        labelAnt1Dir->setGeometry(QRect(620, 45, 86, 16));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        labelAnt1Dir->setFont(font2);
        labelAnt1Title = new QLabel(Dialog);
        labelAnt1Title->setObjectName(QString::fromUtf8("labelAnt1Title"));
        labelAnt1Title->setGeometry(QRect(620, 15, 161, 16));
        labelAnt1Title->setFont(font2);
        labelX = new QLabel(Dialog);
        labelX->setObjectName(QString::fromUtf8("labelX"));
        labelX->setGeometry(QRect(615, 435, 52, 14));
        labelY = new QLabel(Dialog);
        labelY->setObjectName(QString::fromUtf8("labelY"));
        labelY->setGeometry(QRect(615, 450, 52, 14));
        labelAnt2Dir = new QLabel(Dialog);
        labelAnt2Dir->setObjectName(QString::fromUtf8("labelAnt2Dir"));
        labelAnt2Dir->setGeometry(QRect(620, 145, 86, 16));
        labelAnt2Dir->setFont(font2);
        labelAnt2Title = new QLabel(Dialog);
        labelAnt2Title->setObjectName(QString::fromUtf8("labelAnt2Title"));
        labelAnt2Title->setGeometry(QRect(620, 115, 161, 16));
        labelAnt2Title->setFont(font2);
        labelAnt2Status = new QLabel(Dialog);
        labelAnt2Status->setObjectName(QString::fromUtf8("labelAnt2Status"));
        labelAnt2Status->setGeometry(QRect(620, 170, 156, 21));
        labelAnt2Status->setFont(font1);
        labelAnt3Title = new QLabel(Dialog);
        labelAnt3Title->setObjectName(QString::fromUtf8("labelAnt3Title"));
        labelAnt3Title->setGeometry(QRect(620, 215, 156, 16));
        labelAnt3Title->setFont(font2);
        labelAnt3Status = new QLabel(Dialog);
        labelAnt3Status->setObjectName(QString::fromUtf8("labelAnt3Status"));
        labelAnt3Status->setGeometry(QRect(620, 270, 156, 21));
        labelAnt3Status->setFont(font1);
        labelAnt3Dir = new QLabel(Dialog);
        labelAnt3Dir->setObjectName(QString::fromUtf8("labelAnt3Dir"));
        labelAnt3Dir->setGeometry(QRect(620, 245, 86, 16));
        labelAnt3Dir->setFont(font2);
        labelAnt4Status = new QLabel(Dialog);
        labelAnt4Status->setObjectName(QString::fromUtf8("labelAnt4Status"));
        labelAnt4Status->setGeometry(QRect(620, 370, 156, 21));
        labelAnt4Status->setFont(font1);
        labelAnt4Dir = new QLabel(Dialog);
        labelAnt4Dir->setObjectName(QString::fromUtf8("labelAnt4Dir"));
        labelAnt4Dir->setGeometry(QRect(620, 345, 86, 16));
        labelAnt4Dir->setFont(font2);
        labelAnt4Title = new QLabel(Dialog);
        labelAnt4Title->setObjectName(QString::fromUtf8("labelAnt4Title"));
        labelAnt4Title->setGeometry(QRect(620, 315, 160, 16));
        labelAnt4Title->setFont(font2);
        line = new QFrame(Dialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(620, 90, 151, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(Dialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(620, 190, 151, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(Dialog);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(620, 290, 151, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Rotators", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
        labelDebug->setText(QApplication::translate("Dialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Dialog", "PushButton", 0, QApplication::UnicodeUTF8));
        labelAnt1Status->setText(QApplication::translate("Dialog", "Rotating CW to 250\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt1Dir->setText(QApplication::translate("Dialog", "305\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt1Title->setText(QApplication::translate("Dialog", "6/6/6el", 0, QApplication::UnicodeUTF8));
        labelX->setText(QApplication::translate("Dialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelY->setText(QApplication::translate("Dialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelAnt2Dir->setText(QApplication::translate("Dialog", "230\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt2Title->setText(QApplication::translate("Dialog", "5/5el", 0, QApplication::UnicodeUTF8));
        labelAnt2Status->setText(QApplication::translate("Dialog", "Stopped", 0, QApplication::UnicodeUTF8));
        labelAnt3Title->setText(QApplication::translate("Dialog", "4el mono", 0, QApplication::UnicodeUTF8));
        labelAnt3Status->setText(QApplication::translate("Dialog", "Stopped", 0, QApplication::UnicodeUTF8));
        labelAnt3Dir->setText(QApplication::translate("Dialog", "180\302\260", 0, QApplication::UnicodeUTF8));
        labelAnt4Status->setText(QApplication::translate("Dialog", "Stopped", 0, QApplication::UnicodeUTF8));
        labelAnt4Dir->setText(QApplication::translate("Dialog", "250", 0, QApplication::UnicodeUTF8));
        labelAnt4Title->setText(QApplication::translate("Dialog", "5el @SA", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROTATORWINDOW_H

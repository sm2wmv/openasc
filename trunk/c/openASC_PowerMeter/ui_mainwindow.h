/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Thu Mar 18 23:14:13 2010
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionNetworkConnect;
    QAction *actionNetworkDisconnect;
    QAction *actionSettingsSetup;
    QAction *actionSettingsAbout;
    QAction *actionSettingsCalibrate;
    QAction *actionPEP;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frameForward;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *labelFwd;
    QProgressBar *progressBarFwd;
    QLabel *labelFwdValue;
    QFrame *frameReflected;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_2;
    QLabel *labelRef;
    QProgressBar *progressBarRef;
    QLabel *labelRefValue;
    QSpacerItem *horizontalSpacer;
    QLabel *labelSWRValue;
    QMenuBar *menuBar;
    QMenu *menuSettings;
    QMenu *menuConnection;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(351, 179);
        actionNetworkConnect = new QAction(MainWindowClass);
        actionNetworkConnect->setObjectName(QString::fromUtf8("actionNetworkConnect"));
        actionNetworkDisconnect = new QAction(MainWindowClass);
        actionNetworkDisconnect->setObjectName(QString::fromUtf8("actionNetworkDisconnect"));
        actionSettingsSetup = new QAction(MainWindowClass);
        actionSettingsSetup->setObjectName(QString::fromUtf8("actionSettingsSetup"));
        actionSettingsAbout = new QAction(MainWindowClass);
        actionSettingsAbout->setObjectName(QString::fromUtf8("actionSettingsAbout"));
        actionSettingsCalibrate = new QAction(MainWindowClass);
        actionSettingsCalibrate->setObjectName(QString::fromUtf8("actionSettingsCalibrate"));
        actionPEP = new QAction(MainWindowClass);
        actionPEP->setObjectName(QString::fromUtf8("actionPEP"));
        actionPEP->setCheckable(true);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frameForward = new QFrame(centralWidget);
        frameForward->setObjectName(QString::fromUtf8("frameForward"));
        frameForward->setFrameShape(QFrame::StyledPanel);
        frameForward->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameForward);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelFwd = new QLabel(frameForward);
        labelFwd->setObjectName(QString::fromUtf8("labelFwd"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labelFwd->setFont(font);

        gridLayout->addWidget(labelFwd, 0, 0, 1, 1);

        progressBarFwd = new QProgressBar(frameForward);
        progressBarFwd->setObjectName(QString::fromUtf8("progressBarFwd"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBarFwd->sizePolicy().hasHeightForWidth());
        progressBarFwd->setSizePolicy(sizePolicy);
        progressBarFwd->setMinimumSize(QSize(250, 0));
        progressBarFwd->setMaximum(1500);
        progressBarFwd->setValue(1200);
        progressBarFwd->setTextVisible(false);

        gridLayout->addWidget(progressBarFwd, 1, 0, 1, 1);

        labelFwdValue = new QLabel(frameForward);
        labelFwdValue->setObjectName(QString::fromUtf8("labelFwdValue"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        labelFwdValue->setFont(font1);

        gridLayout->addWidget(labelFwdValue, 1, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout->addWidget(frameForward);

        frameReflected = new QFrame(centralWidget);
        frameReflected->setObjectName(QString::fromUtf8("frameReflected"));
        frameReflected->setFrameShape(QFrame::StyledPanel);
        frameReflected->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frameReflected);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelRef = new QLabel(frameReflected);
        labelRef->setObjectName(QString::fromUtf8("labelRef"));
        labelRef->setFont(font);

        gridLayout_2->addWidget(labelRef, 0, 0, 1, 1);

        progressBarRef = new QProgressBar(frameReflected);
        progressBarRef->setObjectName(QString::fromUtf8("progressBarRef"));
        sizePolicy.setHeightForWidth(progressBarRef->sizePolicy().hasHeightForWidth());
        progressBarRef->setSizePolicy(sizePolicy);
        progressBarRef->setMinimumSize(QSize(125, 0));
        QFont font2;
        font2.setStrikeOut(false);
        progressBarRef->setFont(font2);
        progressBarRef->setAutoFillBackground(false);
        progressBarRef->setMaximum(150);
        progressBarRef->setValue(15);
        progressBarRef->setTextVisible(false);
        progressBarRef->setInvertedAppearance(false);
        progressBarRef->setTextDirection(QProgressBar::TopToBottom);

        gridLayout_2->addWidget(progressBarRef, 1, 0, 1, 1);

        labelRefValue = new QLabel(frameReflected);
        labelRefValue->setObjectName(QString::fromUtf8("labelRefValue"));
        labelRefValue->setFont(font1);
        labelRefValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(labelRefValue, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 2, 1, 1);

        labelSWRValue = new QLabel(frameReflected);
        labelSWRValue->setObjectName(QString::fromUtf8("labelSWRValue"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        labelSWRValue->setFont(font3);
        labelSWRValue->setLayoutDirection(Qt::LeftToRight);
        labelSWRValue->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);

        gridLayout_2->addWidget(labelSWRValue, 1, 3, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_2);


        verticalLayout->addWidget(frameReflected);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 351, 19));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menuConnection = new QMenu(menuBar);
        menuConnection->setObjectName(QString::fromUtf8("menuConnection"));
        MainWindowClass->setMenuBar(menuBar);

        menuBar->addAction(menuConnection->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionSettingsSetup);
        menuSettings->addSeparator();
        menuSettings->addAction(actionSettingsAbout);
        menuConnection->addAction(actionNetworkConnect);
        menuConnection->addAction(actionNetworkDisconnect);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Power Meter - Radio 1", 0, QApplication::UnicodeUTF8));
        MainWindowClass->setStyleSheet(QApplication::translate("MainWindowClass", "QWidget {\n"
" background-color: rgb(225, 225, 225);\n"
" color: black;\n"
"}\n"
" QProgressBar {\n"
"     border: 1px solid black;\n"
"     border-radius: 3px;\n"
"     background-color: rgb(170, 170, 170);\n"
" }\n"
" QProgressBar::chunk {\n"
"     background-color: lime;\n"
"     width: 3px;\n"
"     margin: 1px;\n"
" }\n"
" QMenu {\n"
"     background-color: white;\n"
"     margin: 2px; /* some spacing around the menu */\n"
" }\n"
"\n"
" QMenu::item {\n"
"     padding: 2px 25px 2px 20px;\n"
"     border: 1px solid transparent; /* reserve space for selection border */\n"
" }\n"
"\n"
" QMenu::item:selected {\n"
"     border-color: darkblue;\n"
"     background: rgba(100, 100, 100, 150);\n"
" }\n"
"\n"
" QMenu::icon:checked { /* appearance of a 'checked' icon */\n"
"     background: gray;\n"
"     border: 1px inset gray;\n"
"     position: absolute;\n"
"     top: 1px;\n"
"     right: 1px;\n"
"     bottom: 1px;\n"
"     left: 1px;\n"
" }\n"
"\n"
" QMenu::separator {\n"
"     height: 1px;\n"
"     background: s"
                        "ilver;\n"
"     margin-left: 5px;\n"
"     margin-right: 5px;\n"
" }\n"
"\n"
" QMenu::indicator {\n"
"     width: 13px;\n"
"     height: 13px;\n"
" }\n"
"\n"
" QMenuBar {\n"
"     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                       stop:0 lightgray, stop:1 darkgray);\n"
" }\n"
"\n"
" QMenuBar::item {\n"
"     spacing: 3px; /* spacing between menu bar items */\n"
"     padding: 1px 4px;\n"
"     background: transparent;\n"
"     border-radius: 4px;\n"
" }\n"
"\n"
" QMenuBar::item:selected { /* when selected using mouse or keyboard */\n"
"     background: #a8a8a8;\n"
" }\n"
"\n"
" QMenuBar::item:pressed {\n"
"     background: #888888;\n"
" }\n"
"\n"
"QFrame#frame{\n"
"      border-width: 1px;\n"
"      border-style: solid;\n"
"      border-color: black;\n"
"      border-radius: 5px;\n"
"}\n"
"\n"
"QFrame#frameForward{\n"
"      border-width: 1px;\n"
"      border-style: solid;\n"
"      border-color: black;\n"
"      border-radius: 5px;\n"
"}\n"
"\n"
"QFrame#frame"
                        "Reflected{\n"
"      border-width: 1px;\n"
"      border-style: solid;\n"
"      border-color: black;\n"
"      border-radius: 5px;\n"
"}", 0, QApplication::UnicodeUTF8));
        actionNetworkConnect->setText(QApplication::translate("MainWindowClass", "Connect", 0, QApplication::UnicodeUTF8));
        actionNetworkDisconnect->setText(QApplication::translate("MainWindowClass", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionSettingsSetup->setText(QApplication::translate("MainWindowClass", "Setup", 0, QApplication::UnicodeUTF8));
        actionSettingsAbout->setText(QApplication::translate("MainWindowClass", "About", 0, QApplication::UnicodeUTF8));
        actionSettingsCalibrate->setText(QApplication::translate("MainWindowClass", "Calibrate", 0, QApplication::UnicodeUTF8));
        actionPEP->setText(QApplication::translate("MainWindowClass", "PEP", 0, QApplication::UnicodeUTF8));
        labelFwd->setText(QApplication::translate("MainWindowClass", "Forward", 0, QApplication::UnicodeUTF8));
        progressBarFwd->setStyleSheet(QString());
        progressBarFwd->setFormat(QApplication::translate("MainWindowClass", "%vW", 0, QApplication::UnicodeUTF8));
        labelFwdValue->setText(QApplication::translate("MainWindowClass", "1235W", 0, QApplication::UnicodeUTF8));
        labelRef->setText(QApplication::translate("MainWindowClass", "Reflected", 0, QApplication::UnicodeUTF8));
        progressBarRef->setFormat(QApplication::translate("MainWindowClass", "%vW", 0, QApplication::UnicodeUTF8));
        labelRefValue->setText(QApplication::translate("MainWindowClass", "25W", 0, QApplication::UnicodeUTF8));
        labelSWRValue->setStyleSheet(QString());
        labelSWRValue->setText(QApplication::translate("MainWindowClass", "SWR: 1.23", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("MainWindowClass", "Settings", 0, QApplication::UnicodeUTF8));
        menuConnection->setTitle(QApplication::translate("MainWindowClass", "Connection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

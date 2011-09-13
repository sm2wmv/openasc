/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Sep 13 20:10:27 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowImpl
{
public:
    QAction *actionWindowsRotators;
    QAction *actionSettings;
    QAction *actionSettingsEdit;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionClearErrors;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frameMain;
    QFrame *frameRXAntennas;
    QLabel *labelRXAntennas;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonRXAnt1;
    QPushButton *pushButtonRXAnt2;
    QPushButton *pushButtonRXAnt3;
    QPushButton *pushButtonRXAnt4;
    QPushButton *pushButtonRXAnt5;
    QPushButton *pushButtonRXAnt6;
    QPushButton *pushButtonRXAnt7;
    QPushButton *pushButtonRXAnt9;
    QPushButton *pushButtonRXAnt10;
    QPushButton *pushButtonRXAnt8;
    QFrame *frameBandInfo;
    QLabel *labelRXAntennas_2;
    QComboBox *comboBoxBand;
    QFrame *frame;
    QLabel *labelTXAntennas;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonTX1;
    QPushButton *pushButtonRX1;
    QLabel *labelAnt1;
    QSpacerItem *horizontalSpacer;
    QLabel *labelAnt1Dir;
    QPushButton *pushButtonTX2;
    QPushButton *pushButtonRX2;
    QLabel *labelAnt2;
    QLabel *labelAnt2Dir;
    QPushButton *pushButtonTX3;
    QPushButton *pushButtonRX3;
    QLabel *labelAnt3;
    QLabel *labelAnt3Dir;
    QPushButton *pushButtonTX4;
    QPushButton *pushButtonRX4;
    QLabel *labelAnt4;
    QLabel *labelAnt4Dir;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonSubMenu4;
    QPushButton *pushButtonSubMenu3;
    QPushButton *pushButtonSubMenu2;
    QPushButton *pushButtonSubMenu1;
    QLabel *labelRXAntennas3;
    QLabel *labelCurrentRXAntenna;
    QLabel *labelCurrentBand;
    QLabel *labelCurrentBandInfo;
    QFrame *frameMisc;
    QPushButton *pushButtonTXRXMode;
    QPushButton *pushButtonRXAnt;
    QTextEdit *textEdit;
    QFrame *frame_2;
    QLineEdit *lineEditCommand;
    QMenuBar *menubar;
    QMenu *menuNetwork;
    QMenu *menuSettings;
    QMenu *menuWindows;
    QMenu *menuCommands;

    void setupUi(QMainWindow *MainWindowImpl)
    {
        if (MainWindowImpl->objectName().isEmpty())
            MainWindowImpl->setObjectName(QString::fromUtf8("MainWindowImpl"));
        MainWindowImpl->resize(590, 547);
        MainWindowImpl->setMouseTracking(false);
        MainWindowImpl->setStyleSheet(QString::fromUtf8(""));
        MainWindowImpl->setTabShape(QTabWidget::Rounded);
        actionWindowsRotators = new QAction(MainWindowImpl);
        actionWindowsRotators->setObjectName(QString::fromUtf8("actionWindowsRotators"));
        actionWindowsRotators->setCheckable(false);
        actionSettings = new QAction(MainWindowImpl);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionSettingsEdit = new QAction(MainWindowImpl);
        actionSettingsEdit->setObjectName(QString::fromUtf8("actionSettingsEdit"));
        actionConnect = new QAction(MainWindowImpl);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionDisconnect = new QAction(MainWindowImpl);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionDisconnect->setEnabled(false);
        actionDisconnect->setVisible(true);
        actionClearErrors = new QAction(MainWindowImpl);
        actionClearErrors->setObjectName(QString::fromUtf8("actionClearErrors"));
        centralwidget = new QWidget(MainWindowImpl);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(4, 4, 4, 4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frameMain = new QFrame(centralwidget);
        frameMain->setObjectName(QString::fromUtf8("frameMain"));
        frameMain->setStyleSheet(QString::fromUtf8(""));
        frameMain->setFrameShape(QFrame::StyledPanel);
        frameMain->setFrameShadow(QFrame::Raised);
        frameRXAntennas = new QFrame(frameMain);
        frameRXAntennas->setObjectName(QString::fromUtf8("frameRXAntennas"));
        frameRXAntennas->setGeometry(QRect(10, 10, 145, 381));
        frameRXAntennas->setFrameShape(QFrame::StyledPanel);
        frameRXAntennas->setFrameShadow(QFrame::Raised);
        labelRXAntennas = new QLabel(frameRXAntennas);
        labelRXAntennas->setObjectName(QString::fromUtf8("labelRXAntennas"));
        labelRXAntennas->setGeometry(QRect(10, 5, 121, 20));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labelRXAntennas->setFont(font);
        labelRXAntennas->setLayoutDirection(Qt::LeftToRight);
        labelRXAntennas->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(frameRXAntennas);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 127, 346));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setSpacing(3);
        gridLayout_2->setContentsMargins(4, 4, 4, 4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButtonRXAnt1 = new QPushButton(layoutWidget);
        pushButtonRXAnt1->setObjectName(QString::fromUtf8("pushButtonRXAnt1"));
        QFont font1;
        font1.setUnderline(false);
        pushButtonRXAnt1->setFont(font1);
        pushButtonRXAnt1->setAutoFillBackground(false);
        pushButtonRXAnt1->setCheckable(false);
        pushButtonRXAnt1->setFlat(false);

        gridLayout_2->addWidget(pushButtonRXAnt1, 0, 0, 1, 1);

        pushButtonRXAnt2 = new QPushButton(layoutWidget);
        pushButtonRXAnt2->setObjectName(QString::fromUtf8("pushButtonRXAnt2"));
        pushButtonRXAnt2->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt2, 1, 0, 1, 1);

        pushButtonRXAnt3 = new QPushButton(layoutWidget);
        pushButtonRXAnt3->setObjectName(QString::fromUtf8("pushButtonRXAnt3"));
        pushButtonRXAnt3->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt3, 2, 0, 1, 1);

        pushButtonRXAnt4 = new QPushButton(layoutWidget);
        pushButtonRXAnt4->setObjectName(QString::fromUtf8("pushButtonRXAnt4"));
        pushButtonRXAnt4->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt4, 3, 0, 1, 1);

        pushButtonRXAnt5 = new QPushButton(layoutWidget);
        pushButtonRXAnt5->setObjectName(QString::fromUtf8("pushButtonRXAnt5"));
        pushButtonRXAnt5->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt5, 4, 0, 1, 1);

        pushButtonRXAnt6 = new QPushButton(layoutWidget);
        pushButtonRXAnt6->setObjectName(QString::fromUtf8("pushButtonRXAnt6"));
        pushButtonRXAnt6->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt6, 5, 0, 1, 1);

        pushButtonRXAnt7 = new QPushButton(layoutWidget);
        pushButtonRXAnt7->setObjectName(QString::fromUtf8("pushButtonRXAnt7"));
        pushButtonRXAnt7->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt7, 6, 0, 1, 1);

        pushButtonRXAnt9 = new QPushButton(layoutWidget);
        pushButtonRXAnt9->setObjectName(QString::fromUtf8("pushButtonRXAnt9"));
        pushButtonRXAnt9->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt9, 8, 0, 1, 1);

        pushButtonRXAnt10 = new QPushButton(layoutWidget);
        pushButtonRXAnt10->setObjectName(QString::fromUtf8("pushButtonRXAnt10"));
        pushButtonRXAnt10->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt10, 9, 0, 1, 1);

        pushButtonRXAnt8 = new QPushButton(layoutWidget);
        pushButtonRXAnt8->setObjectName(QString::fromUtf8("pushButtonRXAnt8"));
        pushButtonRXAnt8->setCheckable(false);

        gridLayout_2->addWidget(pushButtonRXAnt8, 7, 0, 1, 1);

        frameBandInfo = new QFrame(frameMain);
        frameBandInfo->setObjectName(QString::fromUtf8("frameBandInfo"));
        frameBandInfo->setGeometry(QRect(160, 10, 176, 70));
        frameBandInfo->setFrameShape(QFrame::StyledPanel);
        frameBandInfo->setFrameShadow(QFrame::Raised);
        labelRXAntennas_2 = new QLabel(frameBandInfo);
        labelRXAntennas_2->setObjectName(QString::fromUtf8("labelRXAntennas_2"));
        labelRXAntennas_2->setGeometry(QRect(25, 5, 121, 20));
        labelRXAntennas_2->setFont(font);
        labelRXAntennas_2->setLayoutDirection(Qt::LeftToRight);
        labelRXAntennas_2->setAlignment(Qt::AlignCenter);
        comboBoxBand = new QComboBox(frameBandInfo);
        comboBoxBand->setObjectName(QString::fromUtf8("comboBoxBand"));
        comboBoxBand->setGeometry(QRect(35, 25, 104, 26));
        frame = new QFrame(frameMain);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(160, 85, 405, 216));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        labelTXAntennas = new QLabel(frame);
        labelTXAntennas->setObjectName(QString::fromUtf8("labelTXAntennas"));
        labelTXAntennas->setGeometry(QRect(110, 5, 121, 20));
        labelTXAntennas->setFont(font);
        labelTXAntennas->setLayoutDirection(Qt::LeftToRight);
        labelTXAntennas->setAlignment(Qt::AlignCenter);
        layoutWidget1 = new QWidget(frame);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 30, 530, 136));
        gridLayout_3 = new QGridLayout(layoutWidget1);
        gridLayout_3->setSpacing(3);
        gridLayout_3->setContentsMargins(4, 4, 4, 4);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        pushButtonTX1 = new QPushButton(layoutWidget1);
        pushButtonTX1->setObjectName(QString::fromUtf8("pushButtonTX1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonTX1->sizePolicy().hasHeightForWidth());
        pushButtonTX1->setSizePolicy(sizePolicy);
        pushButtonTX1->setMaximumSize(QSize(45, 16777215));
        pushButtonTX1->setCheckable(true);

        gridLayout_3->addWidget(pushButtonTX1, 0, 0, 1, 1);

        pushButtonRX1 = new QPushButton(layoutWidget1);
        pushButtonRX1->setObjectName(QString::fromUtf8("pushButtonRX1"));
        sizePolicy.setHeightForWidth(pushButtonRX1->sizePolicy().hasHeightForWidth());
        pushButtonRX1->setSizePolicy(sizePolicy);
        pushButtonRX1->setMaximumSize(QSize(45, 16777215));
        pushButtonRX1->setCheckable(true);

        gridLayout_3->addWidget(pushButtonRX1, 0, 1, 1, 1);

        labelAnt1 = new QLabel(layoutWidget1);
        labelAnt1->setObjectName(QString::fromUtf8("labelAnt1"));
        labelAnt1->setMinimumSize(QSize(110, 0));
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        font2.setWeight(75);
        labelAnt1->setFont(font2);

        gridLayout_3->addWidget(labelAnt1, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 4, 1, 1);

        labelAnt1Dir = new QLabel(layoutWidget1);
        labelAnt1Dir->setObjectName(QString::fromUtf8("labelAnt1Dir"));
        labelAnt1Dir->setMinimumSize(QSize(70, 0));
        QFont font3;
        font3.setPointSize(8);
        font3.setBold(true);
        font3.setWeight(75);
        labelAnt1Dir->setFont(font3);

        gridLayout_3->addWidget(labelAnt1Dir, 0, 5, 1, 1);

        pushButtonTX2 = new QPushButton(layoutWidget1);
        pushButtonTX2->setObjectName(QString::fromUtf8("pushButtonTX2"));
        sizePolicy.setHeightForWidth(pushButtonTX2->sizePolicy().hasHeightForWidth());
        pushButtonTX2->setSizePolicy(sizePolicy);
        pushButtonTX2->setMaximumSize(QSize(45, 16777215));
        pushButtonTX2->setCheckable(true);

        gridLayout_3->addWidget(pushButtonTX2, 1, 0, 1, 1);

        pushButtonRX2 = new QPushButton(layoutWidget1);
        pushButtonRX2->setObjectName(QString::fromUtf8("pushButtonRX2"));
        sizePolicy.setHeightForWidth(pushButtonRX2->sizePolicy().hasHeightForWidth());
        pushButtonRX2->setSizePolicy(sizePolicy);
        pushButtonRX2->setMaximumSize(QSize(45, 16777215));
        pushButtonRX2->setCheckable(true);

        gridLayout_3->addWidget(pushButtonRX2, 1, 1, 1, 1);

        labelAnt2 = new QLabel(layoutWidget1);
        labelAnt2->setObjectName(QString::fromUtf8("labelAnt2"));
        labelAnt2->setMinimumSize(QSize(110, 0));
        labelAnt2->setFont(font2);

        gridLayout_3->addWidget(labelAnt2, 1, 3, 1, 1);

        labelAnt2Dir = new QLabel(layoutWidget1);
        labelAnt2Dir->setObjectName(QString::fromUtf8("labelAnt2Dir"));
        labelAnt2Dir->setMinimumSize(QSize(70, 0));
        labelAnt2Dir->setFont(font3);

        gridLayout_3->addWidget(labelAnt2Dir, 1, 5, 1, 1);

        pushButtonTX3 = new QPushButton(layoutWidget1);
        pushButtonTX3->setObjectName(QString::fromUtf8("pushButtonTX3"));
        sizePolicy.setHeightForWidth(pushButtonTX3->sizePolicy().hasHeightForWidth());
        pushButtonTX3->setSizePolicy(sizePolicy);
        pushButtonTX3->setMaximumSize(QSize(45, 16777215));
        pushButtonTX3->setCheckable(true);

        gridLayout_3->addWidget(pushButtonTX3, 2, 0, 1, 1);

        pushButtonRX3 = new QPushButton(layoutWidget1);
        pushButtonRX3->setObjectName(QString::fromUtf8("pushButtonRX3"));
        sizePolicy.setHeightForWidth(pushButtonRX3->sizePolicy().hasHeightForWidth());
        pushButtonRX3->setSizePolicy(sizePolicy);
        pushButtonRX3->setMaximumSize(QSize(45, 16777215));
        pushButtonRX3->setCheckable(true);

        gridLayout_3->addWidget(pushButtonRX3, 2, 1, 1, 1);

        labelAnt3 = new QLabel(layoutWidget1);
        labelAnt3->setObjectName(QString::fromUtf8("labelAnt3"));
        labelAnt3->setMinimumSize(QSize(110, 0));
        labelAnt3->setFont(font2);

        gridLayout_3->addWidget(labelAnt3, 2, 3, 1, 1);

        labelAnt3Dir = new QLabel(layoutWidget1);
        labelAnt3Dir->setObjectName(QString::fromUtf8("labelAnt3Dir"));
        labelAnt3Dir->setMinimumSize(QSize(70, 0));
        labelAnt3Dir->setFont(font3);

        gridLayout_3->addWidget(labelAnt3Dir, 2, 5, 1, 1);

        pushButtonTX4 = new QPushButton(layoutWidget1);
        pushButtonTX4->setObjectName(QString::fromUtf8("pushButtonTX4"));
        sizePolicy.setHeightForWidth(pushButtonTX4->sizePolicy().hasHeightForWidth());
        pushButtonTX4->setSizePolicy(sizePolicy);
        pushButtonTX4->setMaximumSize(QSize(45, 16777215));
        pushButtonTX4->setCheckable(true);

        gridLayout_3->addWidget(pushButtonTX4, 3, 0, 1, 1);

        pushButtonRX4 = new QPushButton(layoutWidget1);
        pushButtonRX4->setObjectName(QString::fromUtf8("pushButtonRX4"));
        sizePolicy.setHeightForWidth(pushButtonRX4->sizePolicy().hasHeightForWidth());
        pushButtonRX4->setSizePolicy(sizePolicy);
        pushButtonRX4->setMaximumSize(QSize(45, 16777215));
        pushButtonRX4->setCheckable(true);

        gridLayout_3->addWidget(pushButtonRX4, 3, 1, 1, 1);

        labelAnt4 = new QLabel(layoutWidget1);
        labelAnt4->setObjectName(QString::fromUtf8("labelAnt4"));
        labelAnt4->setMinimumSize(QSize(110, 0));
        labelAnt4->setFont(font2);

        gridLayout_3->addWidget(labelAnt4, 3, 3, 1, 1);

        labelAnt4Dir = new QLabel(layoutWidget1);
        labelAnt4Dir->setObjectName(QString::fromUtf8("labelAnt4Dir"));
        labelAnt4Dir->setMinimumSize(QSize(70, 0));
        labelAnt4Dir->setFont(font3);

        gridLayout_3->addWidget(labelAnt4Dir, 3, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        pushButtonSubMenu4 = new QPushButton(layoutWidget1);
        pushButtonSubMenu4->setObjectName(QString::fromUtf8("pushButtonSubMenu4"));
        sizePolicy.setHeightForWidth(pushButtonSubMenu4->sizePolicy().hasHeightForWidth());
        pushButtonSubMenu4->setSizePolicy(sizePolicy);
        pushButtonSubMenu4->setMaximumSize(QSize(45, 16777215));
        pushButtonSubMenu4->setCheckable(false);

        gridLayout_3->addWidget(pushButtonSubMenu4, 3, 6, 1, 1);

        pushButtonSubMenu3 = new QPushButton(layoutWidget1);
        pushButtonSubMenu3->setObjectName(QString::fromUtf8("pushButtonSubMenu3"));
        sizePolicy.setHeightForWidth(pushButtonSubMenu3->sizePolicy().hasHeightForWidth());
        pushButtonSubMenu3->setSizePolicy(sizePolicy);
        pushButtonSubMenu3->setMaximumSize(QSize(45, 16777215));
        pushButtonSubMenu3->setCheckable(false);

        gridLayout_3->addWidget(pushButtonSubMenu3, 2, 6, 1, 1);

        pushButtonSubMenu2 = new QPushButton(layoutWidget1);
        pushButtonSubMenu2->setObjectName(QString::fromUtf8("pushButtonSubMenu2"));
        sizePolicy.setHeightForWidth(pushButtonSubMenu2->sizePolicy().hasHeightForWidth());
        pushButtonSubMenu2->setSizePolicy(sizePolicy);
        pushButtonSubMenu2->setMaximumSize(QSize(45, 16777215));
        pushButtonSubMenu2->setCheckable(false);

        gridLayout_3->addWidget(pushButtonSubMenu2, 1, 6, 1, 1);

        pushButtonSubMenu1 = new QPushButton(layoutWidget1);
        pushButtonSubMenu1->setObjectName(QString::fromUtf8("pushButtonSubMenu1"));
        sizePolicy.setHeightForWidth(pushButtonSubMenu1->sizePolicy().hasHeightForWidth());
        pushButtonSubMenu1->setSizePolicy(sizePolicy);
        pushButtonSubMenu1->setMaximumSize(QSize(45, 16777215));
        pushButtonSubMenu1->setCheckable(false);

        gridLayout_3->addWidget(pushButtonSubMenu1, 0, 6, 1, 1);

        labelRXAntennas3 = new QLabel(frame);
        labelRXAntennas3->setObjectName(QString::fromUtf8("labelRXAntennas3"));
        labelRXAntennas3->setGeometry(QRect(15, 170, 86, 20));
        labelRXAntennas3->setFont(font);
        labelRXAntennas3->setLayoutDirection(Qt::LeftToRight);
        labelRXAntennas3->setAlignment(Qt::AlignCenter);
        labelCurrentRXAntenna = new QLabel(frame);
        labelCurrentRXAntenna->setObjectName(QString::fromUtf8("labelCurrentRXAntenna"));
        labelCurrentRXAntenna->setGeometry(QRect(105, 170, 131, 20));
        labelCurrentRXAntenna->setFont(font);
        labelCurrentRXAntenna->setLayoutDirection(Qt::LeftToRight);
        labelCurrentRXAntenna->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelCurrentBand = new QLabel(frame);
        labelCurrentBand->setObjectName(QString::fromUtf8("labelCurrentBand"));
        labelCurrentBand->setGeometry(QRect(10, 190, 86, 20));
        labelCurrentBand->setFont(font);
        labelCurrentBand->setLayoutDirection(Qt::LeftToRight);
        labelCurrentBand->setAlignment(Qt::AlignCenter);
        labelCurrentBandInfo = new QLabel(frame);
        labelCurrentBandInfo->setObjectName(QString::fromUtf8("labelCurrentBandInfo"));
        labelCurrentBandInfo->setGeometry(QRect(105, 190, 131, 20));
        labelCurrentBandInfo->setFont(font);
        labelCurrentBandInfo->setLayoutDirection(Qt::LeftToRight);
        labelCurrentBandInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        frameMisc = new QFrame(frameMain);
        frameMisc->setObjectName(QString::fromUtf8("frameMisc"));
        frameMisc->setGeometry(QRect(340, 10, 225, 70));
        frameMisc->setFrameShape(QFrame::StyledPanel);
        frameMisc->setFrameShadow(QFrame::Raised);
        pushButtonTXRXMode = new QPushButton(frameMisc);
        pushButtonTXRXMode->setObjectName(QString::fromUtf8("pushButtonTXRXMode"));
        pushButtonTXRXMode->setGeometry(QRect(10, 20, 70, 29));
        sizePolicy.setHeightForWidth(pushButtonTXRXMode->sizePolicy().hasHeightForWidth());
        pushButtonTXRXMode->setSizePolicy(sizePolicy);
        pushButtonTXRXMode->setMaximumSize(QSize(70, 16777215));
        pushButtonTXRXMode->setCheckable(true);
        pushButtonRXAnt = new QPushButton(frameMisc);
        pushButtonRXAnt->setObjectName(QString::fromUtf8("pushButtonRXAnt"));
        pushButtonRXAnt->setGeometry(QRect(85, 20, 70, 29));
        sizePolicy.setHeightForWidth(pushButtonRXAnt->sizePolicy().hasHeightForWidth());
        pushButtonRXAnt->setSizePolicy(sizePolicy);
        pushButtonRXAnt->setMaximumSize(QSize(70, 16777215));
        pushButtonRXAnt->setCheckable(false);
        textEdit = new QTextEdit(frameMain);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(160, 305, 405, 141));
        frame_2 = new QFrame(frameMain);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 395, 145, 80));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        lineEditCommand = new QLineEdit(frameMain);
        lineEditCommand->setObjectName(QString::fromUtf8("lineEditCommand"));
        lineEditCommand->setGeometry(QRect(160, 450, 406, 24));

        gridLayout->addWidget(frameMain, 0, 0, 1, 1);

        MainWindowImpl->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowImpl);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 590, 22));
        menuNetwork = new QMenu(menubar);
        menuNetwork->setObjectName(QString::fromUtf8("menuNetwork"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menuWindows = new QMenu(menubar);
        menuWindows->setObjectName(QString::fromUtf8("menuWindows"));
        menuCommands = new QMenu(menubar);
        menuCommands->setObjectName(QString::fromUtf8("menuCommands"));
        MainWindowImpl->setMenuBar(menubar);

        menubar->addAction(menuNetwork->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuCommands->menuAction());
        menubar->addAction(menuWindows->menuAction());
        menuNetwork->addAction(actionConnect);
        menuNetwork->addAction(actionDisconnect);
        menuSettings->addAction(actionSettingsEdit);
        menuWindows->addAction(actionWindowsRotators);
        menuCommands->addAction(actionClearErrors);

        retranslateUi(MainWindowImpl);

        comboBoxBand->setCurrentIndex(10);


        QMetaObject::connectSlotsByName(MainWindowImpl);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowImpl)
    {
        MainWindowImpl->setWindowTitle(QApplication::translate("MainWindowImpl", "openASC remote controller v0.1", 0, QApplication::UnicodeUTF8));
        actionWindowsRotators->setText(QApplication::translate("MainWindowImpl", "Rotators", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("MainWindowImpl", "Settings", 0, QApplication::UnicodeUTF8));
        actionSettingsEdit->setText(QApplication::translate("MainWindowImpl", "Edit", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("MainWindowImpl", "Connect", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("MainWindowImpl", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionClearErrors->setText(QApplication::translate("MainWindowImpl", "Clear Errors", 0, QApplication::UnicodeUTF8));
        labelRXAntennas->setText(QApplication::translate("MainWindowImpl", "RX Antennas", 0, QApplication::UnicodeUTF8));
        pushButtonRXAnt1->setText(QString());
        pushButtonRXAnt2->setText(QString());
        pushButtonRXAnt3->setText(QString());
        pushButtonRXAnt4->setText(QString());
        pushButtonRXAnt5->setText(QString());
        pushButtonRXAnt6->setText(QString());
        pushButtonRXAnt7->setText(QString());
        pushButtonRXAnt9->setText(QString());
        pushButtonRXAnt10->setText(QString());
        pushButtonRXAnt8->setText(QString());
        labelRXAntennas_2->setText(QApplication::translate("MainWindowImpl", "Band", 0, QApplication::UnicodeUTF8));
        comboBoxBand->clear();
        comboBoxBand->insertItems(0, QStringList()
         << QApplication::translate("MainWindowImpl", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "160m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "80m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "40m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "30m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "20m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "17m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "15m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "12m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "10m", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindowImpl", "Auto", 0, QApplication::UnicodeUTF8)
        );
        labelTXAntennas->setText(QApplication::translate("MainWindowImpl", "Antennas", 0, QApplication::UnicodeUTF8));
        pushButtonTX1->setText(QApplication::translate("MainWindowImpl", "TX", 0, QApplication::UnicodeUTF8));
        pushButtonRX1->setText(QApplication::translate("MainWindowImpl", "RX", 0, QApplication::UnicodeUTF8));
        labelAnt1->setText(QString());
        labelAnt1Dir->setText(QString());
        pushButtonTX2->setText(QApplication::translate("MainWindowImpl", "TX", 0, QApplication::UnicodeUTF8));
        pushButtonRX2->setText(QApplication::translate("MainWindowImpl", "RX", 0, QApplication::UnicodeUTF8));
        labelAnt2->setText(QString());
        labelAnt2Dir->setText(QString());
        pushButtonTX3->setText(QApplication::translate("MainWindowImpl", "TX", 0, QApplication::UnicodeUTF8));
        pushButtonRX3->setText(QApplication::translate("MainWindowImpl", "RX", 0, QApplication::UnicodeUTF8));
        labelAnt3->setText(QString());
        labelAnt3Dir->setText(QString());
        pushButtonTX4->setText(QApplication::translate("MainWindowImpl", "TX", 0, QApplication::UnicodeUTF8));
        pushButtonRX4->setText(QApplication::translate("MainWindowImpl", "RX", 0, QApplication::UnicodeUTF8));
        labelAnt4->setText(QString());
        labelAnt4Dir->setText(QString());
        pushButtonSubMenu4->setText(QApplication::translate("MainWindowImpl", "Sub", 0, QApplication::UnicodeUTF8));
        pushButtonSubMenu3->setText(QApplication::translate("MainWindowImpl", "Sub", 0, QApplication::UnicodeUTF8));
        pushButtonSubMenu2->setText(QApplication::translate("MainWindowImpl", "Sub", 0, QApplication::UnicodeUTF8));
        pushButtonSubMenu1->setText(QApplication::translate("MainWindowImpl", "Sub", 0, QApplication::UnicodeUTF8));
        labelRXAntennas3->setText(QApplication::translate("MainWindowImpl", "RX Antenna:", 0, QApplication::UnicodeUTF8));
        labelCurrentRXAntenna->setText(QString());
        labelCurrentBand->setText(QApplication::translate("MainWindowImpl", "Current band:", 0, QApplication::UnicodeUTF8));
        labelCurrentBandInfo->setText(QString());
        pushButtonTXRXMode->setText(QApplication::translate("MainWindowImpl", "TX/RX", 0, QApplication::UnicodeUTF8));
        pushButtonRXAnt->setText(QApplication::translate("MainWindowImpl", "RX Ant", 0, QApplication::UnicodeUTF8));
        menuNetwork->setTitle(QApplication::translate("MainWindowImpl", "Network", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("MainWindowImpl", "Settings", 0, QApplication::UnicodeUTF8));
        menuWindows->setTitle(QApplication::translate("MainWindowImpl", "Windows", 0, QApplication::UnicodeUTF8));
        menuCommands->setTitle(QApplication::translate("MainWindowImpl", "Commands", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowImpl: public Ui_MainWindowImpl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

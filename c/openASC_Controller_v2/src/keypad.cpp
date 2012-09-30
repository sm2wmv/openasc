#include "keypad.h"
#include "ui_keypad.h"

#include "../../../uC/remote_commands.h"
#include "../../../uC/ext_events.h"

void Keypad::pushButtonExt0Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,0);
  }
}

void Keypad::pushButtonExt1Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,1);
  }
}

void Keypad::pushButtonExt2Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,2);
  }
}

void Keypad::pushButtonExt3Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,3);
  }
}

void Keypad::pushButtonExt4Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,4);
  }
}

void Keypad::pushButtonExt5Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,5);
  }
}

void Keypad::pushButtonExt6Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,6);
  }
}

void Keypad::pushButtonExt7Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,7);
  }
}

void Keypad::pushButtonExt8Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,8);
  }
}

void Keypad::pushButtonExt9Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,9);
  }
}

void Keypad::pushButtonExtAPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,10);
  }
}

void Keypad::pushButtonExtBPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,11);
  }
}

void Keypad::pushButtonExtCPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,12);
  }
}

void Keypad::pushButtonExtDPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,13);
  }
}

void Keypad::pushButtonExtEPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,14);
  }
}

void Keypad::pushButtonExtFPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,15);
  }
}

void Keypad::pushButtonExtGPressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,16);
  }
}

void Keypad::pushButtonExtE1Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,17);
  }
}

void Keypad::pushButtonExtE2Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,18);
  }
}

void Keypad::pushButtonExtE3Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,19);
  }
}

void Keypad::pushButtonExtE4Pressed(void) {
  if (TCPComm->isConnected()) {
    TCPComm->addTXMessage(REMOTE_COMMAND_KEYPAD_BUTTON_EVENT,20);
  }
}


void Keypad::setCOMMPtr(TCPClass *comm) {
  TCPComm = comm;
}

Keypad::Keypad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Keypad)
{
    ui->setupUi(this);

    connect(ui->pushButtonExt0, SIGNAL(pressed()), this, SLOT(pushButtonExt0Pressed()));
    connect(ui->pushButtonExt1, SIGNAL(pressed()), this, SLOT(pushButtonExt1Pressed()));
    connect(ui->pushButtonExt2, SIGNAL(pressed()), this, SLOT(pushButtonExt2Pressed()));
    connect(ui->pushButtonExt3, SIGNAL(pressed()), this, SLOT(pushButtonExt3Pressed()));
    connect(ui->pushButtonExt4, SIGNAL(pressed()), this, SLOT(pushButtonExt4Pressed()));
    connect(ui->pushButtonExt5, SIGNAL(pressed()), this, SLOT(pushButtonExt5Pressed()));
    connect(ui->pushButtonExt6, SIGNAL(pressed()), this, SLOT(pushButtonExt6Pressed()));
    connect(ui->pushButtonExt7, SIGNAL(pressed()), this, SLOT(pushButtonExt7Pressed()));
    connect(ui->pushButtonExt8, SIGNAL(pressed()), this, SLOT(pushButtonExt8Pressed()));
    connect(ui->pushButtonExt9, SIGNAL(pressed()), this, SLOT(pushButtonExt9Pressed()));

    connect(ui->pushButtonExtA, SIGNAL(pressed()), this, SLOT(pushButtonExtAPressed()));
    connect(ui->pushButtonExtB, SIGNAL(pressed()), this, SLOT(pushButtonExtBPressed()));
    connect(ui->pushButtonExtC, SIGNAL(pressed()), this, SLOT(pushButtonExtCPressed()));
    connect(ui->pushButtonExtD, SIGNAL(pressed()), this, SLOT(pushButtonExtDPressed()));
    connect(ui->pushButtonExtE, SIGNAL(pressed()), this, SLOT(pushButtonExtEPressed()));
    connect(ui->pushButtonExtF, SIGNAL(pressed()), this, SLOT(pushButtonExtFPressed()));
    connect(ui->pushButtonExtG, SIGNAL(pressed()), this, SLOT(pushButtonExtGPressed()));

    connect(ui->pushButtonExtE1, SIGNAL(pressed()), this, SLOT(pushButtonExtE1Pressed()));
    connect(ui->pushButtonExtE2, SIGNAL(pressed()), this, SLOT(pushButtonExtE2Pressed()));
    connect(ui->pushButtonExtE3, SIGNAL(pressed()), this, SLOT(pushButtonExtE3Pressed()));
    connect(ui->pushButtonExtE4, SIGNAL(pressed()), this, SLOT(pushButtonExtE4Pressed()));


    QSettings settings("settings.ini",QSettings::IniFormat,0);

    settings.beginGroup("KeypadLabels");

    //Check if we seem to have the labels in the settings file
    if (settings.contains("pushButtonExtE1")) {
      ui->pushButtonExtE1->setText(settings.value("pushButtonExtE1").toString());
      ui->pushButtonExtE2->setText(settings.value("pushButtonExtE2").toString());
      ui->pushButtonExtE3->setText(settings.value("pushButtonExtE3").toString());
      ui->pushButtonExtE4->setText(settings.value("pushButtonExtE4").toString());
      ui->pushButtonExtA->setText(settings.value("pushButtonExtA").toString());
      ui->pushButtonExtB->setText(settings.value("pushButtonExtB").toString());
      ui->pushButtonExtC->setText(settings.value("pushButtonExtC").toString());
      ui->pushButtonExtD->setText(settings.value("pushButtonExtD").toString());
      ui->pushButtonExtE->setText(settings.value("pushButtonExtE").toString());
      ui->pushButtonExtF->setText(settings.value("pushButtonExtF").toString());
      ui->pushButtonExtG->setText(settings.value("pushButtonExtG").toString());
      ui->pushButtonExt0->setText(settings.value("pushButtonExt0").toString());
      ui->pushButtonExt1->setText(settings.value("pushButtonExt1").toString());
      ui->pushButtonExt2->setText(settings.value("pushButtonExt2").toString());
      ui->pushButtonExt3->setText(settings.value("pushButtonExt3").toString());
      ui->pushButtonExt4->setText(settings.value("pushButtonExt4").toString());
      ui->pushButtonExt5->setText(settings.value("pushButtonExt5").toString());
      ui->pushButtonExt6->setText(settings.value("pushButtonExt6").toString());
      ui->pushButtonExt7->setText(settings.value("pushButtonExt7").toString());
      ui->pushButtonExt8->setText(settings.value("pushButtonExt8").toString());
      ui->pushButtonExt9->setText(settings.value("pushButtonExt9").toString());
    }
    else {
      settings.setValue("pushButtonExtE1","E1");
      settings.setValue("pushButtonExtE2","E2");
      settings.setValue("pushButtonExtE3","E3");
      settings.setValue("pushButtonExtE4","E4");
      settings.setValue("pushButtonExtA","A");
      settings.setValue("pushButtonExtB","B");
      settings.setValue("pushButtonExtC","C");
      settings.setValue("pushButtonExtD","D");
      settings.setValue("pushButtonExtE","E");
      settings.setValue("pushButtonExtF","F");
      settings.setValue("pushButtonExtG","G");
      settings.setValue("pushButtonExt0","0");
      settings.setValue("pushButtonExt1","1");
      settings.setValue("pushButtonExt2","2");
      settings.setValue("pushButtonExt3","3");
      settings.setValue("pushButtonExt4","4");
      settings.setValue("pushButtonExt5","5");
      settings.setValue("pushButtonExt6","6");
      settings.setValue("pushButtonExt7","7");
      settings.setValue("pushButtonExt8","8");
      settings.setValue("pushButtonExt9","9");
    }

    settings.endGroup();

}

Keypad::~Keypad()
{
    delete ui;
}

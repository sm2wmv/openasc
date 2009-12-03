#include "submenuimpl.h"
#include "bandclass.h"
//

BandClass *currBandClass;

void SubMenuImpl::setAntenna(BandClass &bandClass, int antIndex) {
	currBandClass = &bandClass;
	currAntIndex = antIndex;
	
	labelBandAntenna->setText(bandClass.getBandName() + " - " + bandClass.getAntennaName(antIndex));
	
	if (currBandClass->getSubMenuType(antIndex) == SUBMENU_4SQ) {
		radioButton4SQ->setChecked(true);
		tabWidgetType->setCurrentIndex(0);
		
		//Load the settings
		spinBoxDirCount->setValue(currBandClass->getSubMenu4SQdirectionCount());
		
		lineEditDirection1->setText("");
		lineEditDirection2->setText("");
		lineEditDirection3->setText("");
		lineEditDirection4->setText("");
		lineEditDirection5->setText("");
		lineEditDirection6->setText("");
		lineEditDirection7->setText("");
		lineEditDirection8->setText("");
		
		lineEditOutputStr1->setText("");
		lineEditOutputStr2->setText("");
		lineEditOutputStr3->setText("");
		lineEditOutputStr4->setText("");
		lineEditOutputStr5->setText("");
		lineEditOutputStr6->setText("");
		lineEditOutputStr7->setText("");
		lineEditOutputStr8->setText("");
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 0) {
			lineEditDirection1->setText(currBandClass->getSubMenu4SQdirectionName(0));
			lineEditOutputStr1->setText(currBandClass->getSubMenu4SQdirectionOutputStr(0));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 1) {
			lineEditDirection2->setText(currBandClass->getSubMenu4SQdirectionName(1));
			lineEditOutputStr2->setText(currBandClass->getSubMenu4SQdirectionOutputStr(1));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 2) {
			lineEditDirection3->setText(currBandClass->getSubMenu4SQdirectionName(2));
			lineEditOutputStr3->setText(currBandClass->getSubMenu4SQdirectionOutputStr(2));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 3) {
			lineEditDirection4->setText(currBandClass->getSubMenu4SQdirectionName(3));
			lineEditOutputStr4->setText(currBandClass->getSubMenu4SQdirectionOutputStr(3));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 4) {
			lineEditDirection5->setText(currBandClass->getSubMenu4SQdirectionName(4));
			lineEditOutputStr5->setText(currBandClass->getSubMenu4SQdirectionOutputStr(4));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 5) {
			lineEditDirection6->setText(currBandClass->getSubMenu4SQdirectionName(5));
			lineEditOutputStr6->setText(currBandClass->getSubMenu4SQdirectionOutputStr(5));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 6) {
			lineEditDirection7->setText(currBandClass->getSubMenu4SQdirectionName(6));
			lineEditOutputStr7->setText(currBandClass->getSubMenu4SQdirectionOutputStr(6));
		}
		
		if (currBandClass->getSubMenu4SQdirectionCount() > 7) {
			lineEditDirection8->setText(currBandClass->getSubMenu4SQdirectionName(7));
			lineEditOutputStr8->setText(currBandClass->getSubMenu4SQdirectionOutputStr(7));
		}
		
	}
	else if (currBandClass->getSubMenuType(antIndex) == SUBMENU_STACK) {
		radioButtonStack->setChecked(true);
		tabWidgetType->setCurrentIndex(1);
		
		spinBoxStackPositions->setValue(currBandClass->getSubMenuStackCombinationCount());
		lineEditStackName->setText(currBandClass->getSubMenuStackName());

		lineEditStackCombo1->setText("");
		lineEditStackCombo2->setText("");
		lineEditStackCombo3->setText("");
		lineEditStackCombo4->setText("");
		lineEditStackCombo5->setText("");
		lineEditStackCombo6->setText("");
		
		lineEditStackOutputStr1->setText("");
		lineEditStackOutputStr2->setText("");
		lineEditStackOutputStr3->setText("");
		lineEditStackOutputStr4->setText("");
		lineEditStackOutputStr5->setText("");
		lineEditStackOutputStr6->setText("");
		
		lineEditStackCombo1->setText(currBandClass->getSubMenuStackCombinationName(0));
		lineEditStackCombo2->setText(currBandClass->getSubMenuStackCombinationName(1));
		lineEditStackCombo3->setText(currBandClass->getSubMenuStackCombinationName(2));
		lineEditStackCombo4->setText(currBandClass->getSubMenuStackCombinationName(3));
		lineEditStackCombo5->setText(currBandClass->getSubMenuStackCombinationName(4));
		lineEditStackCombo6->setText(currBandClass->getSubMenuStackCombinationName(5));
		
		lineEditStackOutputStr1->setText(currBandClass->getSubMenuStackCombinationOutputStr(0));
		lineEditStackOutputStr2->setText(currBandClass->getSubMenuStackCombinationOutputStr(1));
		lineEditStackOutputStr3->setText(currBandClass->getSubMenuStackCombinationOutputStr(2));
		lineEditStackOutputStr4->setText(currBandClass->getSubMenuStackCombinationOutputStr(3));
		lineEditStackOutputStr5->setText(currBandClass->getSubMenuStackCombinationOutputStr(4));
		lineEditStackOutputStr6->setText(currBandClass->getSubMenuStackCombinationOutputStr(5));
	}
	else {
		radioButton4SQ->setChecked(true);
		tabWidgetType->setCurrentIndex(0);
	}
}

void SubMenuImpl::radioButton4SQClicked(bool state) {
	if (state == true) {
		tabWidgetType->setCurrentIndex(0);
	}
}

void SubMenuImpl::radioButtonStackClicked(bool state) {
	if (state == true) {
		tabWidgetType->setCurrentIndex(1);
		currBandClass->setSubMenuType(currAntIndex,SUBMENU_STACK);
	}
}

void SubMenuImpl::pushButtonSavePressed() {
	if (radioButton4SQ->isChecked()) {
		currBandClass->setSubMenuType(currAntIndex,SUBMENU_4SQ);
		
		currBandClass->setSubMenu4SQdirectionCount(spinBoxDirCount->value());
		
		currBandClass->setSubMenu4SQdirectionName(0,lineEditDirection1->text());
		currBandClass->setSubMenu4SQdirectionName(1,lineEditDirection2->text());
		currBandClass->setSubMenu4SQdirectionName(2,lineEditDirection3->text());
		currBandClass->setSubMenu4SQdirectionName(3,lineEditDirection4->text());
		currBandClass->setSubMenu4SQdirectionName(4,lineEditDirection5->text());
		currBandClass->setSubMenu4SQdirectionName(5,lineEditDirection6->text());
		currBandClass->setSubMenu4SQdirectionName(6,lineEditDirection7->text());
		currBandClass->setSubMenu4SQdirectionName(7,lineEditDirection8->text());

		currBandClass->setSubMenu4SQdirectionOutputStr(0,lineEditOutputStr1->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(1,lineEditOutputStr2->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(2,lineEditOutputStr3->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(3,lineEditOutputStr4->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(4,lineEditOutputStr5->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(5,lineEditOutputStr6->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(6,lineEditOutputStr7->text());
		currBandClass->setSubMenu4SQdirectionOutputStr(7,lineEditOutputStr8->text());		
	}
	else if (radioButtonStack->isChecked()) {
		currBandClass->setSubMenuType(currAntIndex,SUBMENU_STACK);
		
		currBandClass->setSubMenuStackName(lineEditStackName->text());
		currBandClass->setSubMenuStackCombinationCount(spinBoxStackPositions->value());
		
		currBandClass->setSubMenuStackCombinationName(0,lineEditStackCombo1->text());
		currBandClass->setSubMenuStackCombinationName(1,lineEditStackCombo2->text());
		currBandClass->setSubMenuStackCombinationName(2,lineEditStackCombo3->text());
		currBandClass->setSubMenuStackCombinationName(3,lineEditStackCombo4->text());
		currBandClass->setSubMenuStackCombinationName(4,lineEditStackCombo5->text());
		currBandClass->setSubMenuStackCombinationName(5,lineEditStackCombo6->text());
		
		currBandClass->setSubMenuStackCombinationOutputStr(0,lineEditStackOutputStr1->text());
		currBandClass->setSubMenuStackCombinationOutputStr(1,lineEditStackOutputStr2->text());
		currBandClass->setSubMenuStackCombinationOutputStr(2,lineEditStackOutputStr3->text());
		currBandClass->setSubMenuStackCombinationOutputStr(3,lineEditStackOutputStr4->text());
		currBandClass->setSubMenuStackCombinationOutputStr(4,lineEditStackOutputStr5->text());
		currBandClass->setSubMenuStackCombinationOutputStr(5,lineEditStackOutputStr6->text());
	}
	
	this->accept();
}

void SubMenuImpl::spinBoxDirCountChanged(int currValue) {
	lineEditDirection3->setEnabled(false);
	lineEditDirection4->setEnabled(false);
	lineEditDirection5->setEnabled(false);
	lineEditDirection6->setEnabled(false);
	lineEditDirection7->setEnabled(false);
	lineEditDirection8->setEnabled(false);
		
	lineEditOutputStr3->setEnabled(false);
	lineEditOutputStr4->setEnabled(false);
	lineEditOutputStr5->setEnabled(false);
	lineEditOutputStr6->setEnabled(false);
	lineEditOutputStr7->setEnabled(false);
	lineEditOutputStr8->setEnabled(false);
	
	if (currValue > 2) {
		lineEditDirection3->setEnabled(true);
		lineEditOutputStr3->setEnabled(true);
	}
	
	if (currValue > 3) {
		lineEditDirection4->setEnabled(true);
		lineEditOutputStr4->setEnabled(true);
	}	
	
	if (currValue > 4) {
		lineEditDirection5->setEnabled(true);
		lineEditOutputStr5->setEnabled(true);
	}	
	
	if (currValue > 5) {
		lineEditDirection6->setEnabled(true);
		lineEditOutputStr6->setEnabled(true);
	}
	
	if (currValue > 6) {
		lineEditDirection7->setEnabled(true);
		lineEditOutputStr7->setEnabled(true);
	}
	
	if (currValue > 7) {
		lineEditDirection8->setEnabled(true);
		lineEditOutputStr8->setEnabled(true);
	}
}

void SubMenuImpl::spinBoxStackPositionsValueChanged(int currValue) {
	lineEditStackCombo3->setEnabled(false);
	lineEditStackCombo4->setEnabled(false);
	lineEditStackCombo5->setEnabled(false);
	lineEditStackCombo6->setEnabled(false);
		
	lineEditStackOutputStr3->setEnabled(false);
	lineEditStackOutputStr4->setEnabled(false);
	lineEditStackOutputStr5->setEnabled(false);
	lineEditStackOutputStr6->setEnabled(false);
	
	if (currValue > 2) {
		lineEditStackCombo3->setEnabled(true);
		lineEditStackOutputStr3->setEnabled(true);
	}
	
	if (currValue > 3) {
		lineEditStackCombo4->setEnabled(true);
		lineEditStackOutputStr4->setEnabled(true);
	}	
	
	if (currValue > 4) {
		lineEditStackCombo5->setEnabled(true);
		lineEditStackOutputStr5->setEnabled(true);
	}	
	
	if (currValue > 5) {
		lineEditStackCombo6->setEnabled(true);
		lineEditStackOutputStr6->setEnabled(true);
	}
}

void SubMenuImpl::setupConnections() {
	connect(radioButton4SQ, SIGNAL(clicked(bool)), this, SLOT(radioButton4SQClicked(bool)));
	connect(radioButtonStack, SIGNAL(clicked(bool)), this, SLOT(radioButtonStackClicked(bool)));
	
	connect(spinBoxDirCount, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDirCountChanged(int)));
	connect(spinBoxStackPositions, SIGNAL(valueChanged(int)), this, SLOT(spinBoxStackPositionsValueChanged(int)));
	
	connect(pushButtonSave, SIGNAL(pressed()), this, SLOT(pushButtonSavePressed()));
}

SubMenuImpl::SubMenuImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	
	setupConnections();
}
//


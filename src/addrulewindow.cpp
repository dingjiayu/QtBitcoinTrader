// Copyright (C) 2013 July IGHOR.
// I want to create trading application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form https://sourceforge.net/projects/bitcointrader/
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "addrulewindow.h"
#include "main.h"
#include "julyspinboxfix.h"
#include <QMessageBox>
#include <QTimer>

AddRuleWindow::AddRuleWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.buttonSaveRule->setVisible(false);
	ui.thanValue->setValue(mainWindow.ui.marketLast->value());
	ui.exactPriceValue->setValue(mainWindow.ui.marketLast->value());
	ui.btcValue->setValue(mainWindow.getAvailableBTC());

	setWindowFlags(Qt::WindowCloseButtonHint);
	amountChanged();

	mainWindow.fillAllUsdLabels(this,currencyBStr);

	mainWindow.fillAllBtcLabels(this,currencyAStr);

	new JulySpinBoxFix(ui.thanValue);
	new JulySpinBoxFix(ui.btcValue);
	new JulySpinBoxFix(ui.exactPriceValue);

	foreach(QPushButton* pushButtons, findChildren<QPushButton*>())pushButtons->setMinimumWidth(textFontWidth(pushButtons->text())+10);

	foreach(QCheckBox* checkBoxes, findChildren<QCheckBox*>())checkBoxes->setMinimumWidth(textFontWidth(checkBoxes->text())+10);

	ui.exactPriceValue->setVisible(false);
	ui.label_53->setVisible(false);//sorry for that label name
	ui.priceBtcIcon->setVisible(false);
	languageChanged();
	setMinimumSize(size());
	setMaximumSize(width()+100,height());

	connect(julyTranslator,SIGNAL(languageChanged()),this,SLOT(languageChanged()));
	QTimer::singleShot(100,this,SLOT(checkToEnableButtons()));
}

AddRuleWindow::~AddRuleWindow()
{

}

void AddRuleWindow::checkToEnableButtons()
{
	int currentThanType=thanType();
	double thanValue=ui.thanValue->value();

	bool canBeEnabled=(ui.btcValue->isVisible()&&ui.btcValue->value()>=minTradeVolume||!ui.btcValue->isVisible())&&(currentThanType!=0||currentThanType==0&&thanValue>=minTradePrice)&&(!ui.exactPriceValue->isVisible()||ui.exactPriceValue->isVisible()&&ui.exactPriceValue->value()>minTradePrice);
	ui.buttonAddRule->setEnabled(canBeEnabled);
	ui.buttonSaveRule->setEnabled(canBeEnabled);
}

void AddRuleWindow::languageChanged()
{
	julyTranslator->translateUi(this);
	ui.checkBtcBalance->setText(julyTr("IF_BALANCE","%1 Balance").arg(QString(currencyAStr)));
	ui.checkUsdBalance->setText(julyTr("IF_BALANCE","%1 Balance").arg(QString(currencyBStr)));

	mainWindow.fixAllChildButtonsAndLabels(this);
}

void AddRuleWindow::buttonAddRule()
{
	if(!ui.ruleIsEnabled->isChecked()||mainWindow.ui.ruleSequencialMode->isChecked()&&mainWindow.rulesModel->rowCount()>0||checkIsValidRule())accept();
	else QMessageBox::warning(this,windowTitle(),julyTr("INVALID_RULE_CHECK","This rule will be executed instantly.<br>This means that you make a mistake.<br>Please check values you entered."));
}

bool AddRuleWindow::checkIsValidRule()
{
	checkToEnableButtons();
	if(ui.checkLastPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketLast->value()))return false;
	if(ui.checkMarketBuy->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketBuy->value()))return false;
	if(ui.checkMarketSell->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketSell->value()))return false;
	if(ui.checkMarketHigh->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketHigh->value()))return false;
	if(ui.checkMarketLow->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketLow->value()))return false;
	if(ui.checkOrdersLastBuyPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ordersLastBuyPrice->value()))return false;
	if(ui.checkOrdersLastSellPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ordersLastSellPrice->value()))return false;
	if(ui.checkBtcBalance->isChecked()&&getRuleHolder().isAchieved(mainWindow.getAvailableBTC()))return false;
	if(ui.checkUsdBalance->isChecked()&&getRuleHolder().isAchieved(mainWindow.getAvailableUSD()))return false;
	if(ui.checkTotalToBuy->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ruleTotalToBuyValue->value()))return false;
	if(ui.checkAmountToReceive->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ruleAmountToReceiveValue->value()))return false;
	if(ui.checkTotalToBuyBS->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ruleTotalToBuyBSValue->value()))return false;
	if(ui.checkAmountToReceiveBS->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ruleAmountToReceiveBSValue->value()))return false;
	return true;
}

void AddRuleWindow::amountChanged()
{
	bool btcVisible=ui.checkSellAmount->isChecked()||ui.checkBuyAmount->isChecked();
	ui.btcValue->setVisible(btcVisible);
	ui.btcLabel->setVisible(btcVisible);
	ui.labelSellAll->setVisible(ui.checkSellAllIn->isChecked());
	ui.labelSellHalf->setVisible(ui.checkSellHalfIn->isChecked());
	ui.labelSpendAll->setVisible(ui.checkBuyAllIn->isChecked());
	ui.labelSpendHalf->setVisible(ui.checkBuyHalfIn->isChecked());
	ui.labelCancelAllOrders->setVisible(ui.checkCancelAllOrders->isChecked());
	ui.labelEnableAllRules->setVisible(ui.checkEnableAllRules->isChecked());
	ui.labelDisableAllRules->setVisible(ui.checkDisableAllRules->isChecked());
	checkToEnableButtons();
}

void AddRuleWindow::fillByRuleHolder(RuleHolder *holder)
{
	ui.addNewRuleGroupbox->setTitle(julyTr("EDIT_RULE","Edit rule"));

	ui.buttonAddRule->setVisible(false);
	ui.buttonSaveRule->setVisible(true);

	switch(holder->getRulePriceType())
	{
	case 1: ui.checkLastPrice->setChecked(true);break;
	case 2: ui.checkMarketBuy->setChecked(true);break;
	case 3: ui.checkMarketSell->setChecked(true);break;
	case 4: ui.checkMarketHigh->setChecked(true);break;
	case 5: ui.checkMarketLow->setChecked(true);break;
	case 6: ui.checkOrdersLastBuyPrice->setChecked(true);break;
	case 7: ui.checkOrdersLastSellPrice->setChecked(true);break;
	case 8: ui.checkBtcBalance->setChecked(true);break;
	case 9: ui.checkUsdBalance->setChecked(true);break;
	case 10: ui.checkTotalToBuy->setChecked(true);break;
	case 11: ui.checkAmountToReceive->setChecked(true);break;
	case 12: ui.checkTotalToBuyBS->setChecked(true);break;
	case 13: ui.checkAmountToReceiveBS->setChecked(true);break;
	default: break;
	}

	switch(holder->getRuleMoreLessEqual())
	{
	case 1: ui.checkGoesAbove->setChecked(true); break;
	case 0: ui.checkEqual->setChecked(true); break;
	case -1: ui.checkGoesBelow->setChecked(true); break;
	default: break;
	}

	ui.thanValue->setValue(holder->getRuleCheckPrice());

	double fillRuleBtc=holder->getRuleBtc();
	if(fillRuleBtc>-1.0)
	{
		ui.btcValue->setValue(fillRuleBtc);
		if(holder->isBuying())ui.checkBuyAmount->setChecked(true);
		else ui.checkSellAmount->setChecked(true);
	}
	else
	{
	if(fillRuleBtc==-1.0)ui.checkSellAllIn->setChecked(true);
	if(fillRuleBtc==-2.0)ui.checkSellHalfIn->setChecked(true);
	if(fillRuleBtc==-3.0)ui.checkBuyAllIn->setChecked(true);
	if(fillRuleBtc==-4.0)ui.checkBuyHalfIn->setChecked(true);
	if(fillRuleBtc==-5.0)ui.checkCancelAllOrders->setChecked(true);
	if(fillRuleBtc==-6.0)ui.checkEnableAllRules->setChecked(true);
	if(fillRuleBtc==-7.0)ui.checkDisableAllRules->setChecked(true);
	}

	double fillRulePrice=holder->getRulePrice();
	if(fillRulePrice>-1.0)
	{
		ui.exactPrice->setChecked(true);
		ui.exactPriceValue->setValue(fillRulePrice);
	}
	else
	{
	if(fillRulePrice==-1.0)ui.checkLastPrice_2->setChecked(true);
	if(fillRulePrice==-2.0)ui.checkMarketBuy_2->setChecked(true);
	if(fillRulePrice==-3.0)ui.checkMarketSell_2->setChecked(true);
	if(fillRulePrice==-4.0)ui.checkMarketHigh_2->setChecked(true);
	if(fillRulePrice==-5.0)ui.checkMarketLow_2->setChecked(true);
	if(fillRulePrice==-6.0)ui.checkOrdersLastBuyPrice_2->setChecked(true);
	if(fillRulePrice==-7.0)ui.checkOrdersLastSellPrice_2->setChecked(true);
	}

	ui.ruleIsEnabled->setChecked(holder->getRuleState()==1);
	checkToEnableButtons();
}

RuleHolder AddRuleWindow::getRuleHolder()
{
	double btcValue=ui.btcValue->value();
	bool isBuying=ui.checkBuyAmount->isChecked();
	if(ui.checkSellAllIn->isChecked())btcValue=-1.0;
	if(ui.checkSellHalfIn->isChecked())btcValue=-2.0;
	if(ui.checkBuyAllIn->isChecked()){isBuying=true;btcValue=-3.0;}
	if(ui.checkBuyHalfIn->isChecked()){isBuying=true;btcValue=-4.0;}
	if(ui.checkCancelAllOrders->isChecked())btcValue=-5.0;
	if(ui.checkEnableAllRules->isChecked())btcValue=-6.0;
	if(ui.checkDisableAllRules->isChecked())btcValue=-7.0;

	double ruleSellPrice=ui.exactPriceValue->value();

	if(ui.checkLastPrice_2->isChecked())ruleSellPrice=-1.0;
	if(ui.checkMarketBuy_2->isChecked())ruleSellPrice=-2.0;
	if(ui.checkMarketSell_2->isChecked())ruleSellPrice=-3.0;
	if(ui.checkMarketHigh_2->isChecked())ruleSellPrice=-4.0;
	if(ui.checkMarketLow_2->isChecked())ruleSellPrice=-5.0;
	if(ui.checkOrdersLastBuyPrice_2->isChecked())ruleSellPrice=-6.0;
	if(ui.checkOrdersLastSellPrice_2->isChecked())ruleSellPrice=-7.0;

	int moreLessEqual=0;
	if(ui.checkGoesAbove->isChecked())moreLessEqual=1;
	if(ui.checkGoesBelow->isChecked())moreLessEqual=-1;

	int ruleSellType=0;
	if(ui.checkLastPrice->isChecked())ruleSellType=1;
	if(ui.checkMarketBuy->isChecked())ruleSellType=2;
	if(ui.checkMarketSell->isChecked())ruleSellType=3;
	if(ui.checkMarketHigh->isChecked())ruleSellType=4;
	if(ui.checkMarketLow->isChecked())ruleSellType=5;
	if(ui.checkOrdersLastBuyPrice->isChecked())ruleSellType=6;
	if(ui.checkOrdersLastSellPrice->isChecked())ruleSellType=7;
	if(ui.checkBtcBalance->isChecked())ruleSellType=8;
	if(ui.checkUsdBalance->isChecked())ruleSellType=9;
	if(ui.checkTotalToBuy->isChecked())ruleSellType=10;
	if(ui.checkAmountToReceive->isChecked())ruleSellType=11;
	if(ui.checkTotalToBuyBS->isChecked())ruleSellType=12;
	if(ui.checkAmountToReceiveBS->isChecked())ruleSellType=13;

	return RuleHolder(moreLessEqual, ui.thanValue->value(), btcValue, isBuying, ruleSellPrice, ruleSellType, ui.ruleIsEnabled->isChecked());
}

void AddRuleWindow::setOrdersBackInvisible(bool on)
{
	ui.sellBack->setEnabled(!on);
}

int AddRuleWindow::thanType()
{
	if(ui.checkBtcBalance->isChecked()||ui.checkTotalToBuy->isChecked()||ui.checkTotalToBuyBS->isChecked())return 1;//BTC Volume
	if(ui.checkUsdBalance->isChecked()||ui.checkAmountToReceive->isChecked()||ui.checkAmountToReceiveBS->isChecked())return 2;//USD Volume
	return 0; //Price
}

void AddRuleWindow::ifChanged(bool on)
{
	if(!on)return;

	bool equalEnabled=!ui.checkTotalToBuy->isChecked()&&!ui.checkTotalToBuyBS->isChecked()&&!ui.checkAmountToReceive->isChecked()&&!ui.checkAmountToReceiveBS->isChecked();
	if(!equalEnabled&&ui.checkEqual->isChecked())ui.checkGoesAbove->setChecked(true);
	ui.checkEqual->setEnabled(equalEnabled);

	switch(thanType())
	{
	case 1:
		{
		ui.thanValue->setDecimals(btcDecimals);
		if(ui.checkBtcBalance->isChecked())ui.thanValue->setValue(mainWindow.getAvailableBTC());
		if(ui.checkTotalToBuy->isChecked())ui.thanValue->setValue(mainWindow.ui.ruleTotalToBuyValue->value());
		if(ui.checkTotalToBuyBS->isChecked())ui.thanValue->setValue(mainWindow.ui.ruleTotalToBuyBSValue->value());

		ui.priceBtcIcon->setVisible(true);
		ui.priceUsdIcon->setVisible(false);
		}
		break;
		case 2:
		{
		ui.thanValue->setDecimals(usdDecimals);
		if(ui.checkUsdBalance->isChecked())ui.thanValue->setValue(mainWindow.getAvailableUSD());
		if(ui.checkAmountToReceive->isChecked())ui.thanValue->setValue(mainWindow.ui.ruleAmountToReceiveValue->value());
		if(ui.checkAmountToReceiveBS->isChecked())ui.thanValue->setValue(mainWindow.ui.ruleAmountToReceiveBSValue->value());

		ui.priceBtcIcon->setVisible(false);
		ui.priceUsdIcon->setVisible(true);
		}
		break;
		default:
		{
			ui.thanValue->setDecimals(priceDecimals);
			ui.thanValue->setValue(mainWindow.ui.marketLast->value());

			ui.priceBtcIcon->setVisible(false);
			ui.priceUsdIcon->setVisible(true);
		}
		break;
	}
	double oldValue=ui.thanValue->value();
	ui.thanValue->setValue(0.123456789);//Layout Fix
	ui.thanValue->setValue(oldValue);
	setMinimumSize(minimumSizeHint());
	checkToEnableButtons();
}
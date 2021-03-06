// Copyright (C) 2013 July IGHOR.
// I want to create trading application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form https://sourceforge.net/projects/bitcointrader/
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#ifndef TRADESMODEL_H
#define TRADESMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class TradesModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	TradesModel();
	~TradesModel();
	void updateTotalBTC();
	double getRowPrice(int);
	double getRowVolume(int);
	bool getRowType(int);
	void clear();
	void removeDataOlderThen(quint32);
	void addNewTrade(quint32 dateT, double volumeT, double priceT, QByteArray curRency, int isSell);

	void setHorizontalHeaderLabels(QStringList list);

	QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	double lastPrecentBids;
	quint32 lastRemoveDate;
	void removeFirst();
	QString textBid;
	QString textAsk;

	QString upArrow;
	QString downArrow;

	double lastPrice;
	int columnsCount;
	int dateWidth;
	int typeWidth;

	QStringList headerLabels;

	QHash<quint32,int> colorMap;

	QList<quint32> dateList;
	QList<double> volumeList;
	QList<double> priceList;
	QList<int> typesList;
	QList<int> directionList;
signals:
	void precentBidsChanged(double);
	void trades10MinVolumeChanged(double);
};

#endif // TRADESMODEL_H

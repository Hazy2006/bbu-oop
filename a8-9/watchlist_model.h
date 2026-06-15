#pragma once
#include <QAbstractTableModel>
#include "watchlist_service.h"

class WatchListModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit WatchListModel(WatchListService& wls, QObject* parent = nullptr);

    int      rowCount   (const QModelIndex& parent = QModelIndex()) const override;
    int      columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data       (const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    WatchListService& wls;
};

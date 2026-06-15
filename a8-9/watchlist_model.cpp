#include "watchlist_model.h"

WatchListModel::WatchListModel(WatchListService& wls, QObject* parent)
    : QAbstractTableModel(parent), wls(wls) {}

int WatchListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(wls.getAll().size());
}

int WatchListModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return 5;
}

QVariant WatchListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return {};
    const auto& all = wls.getAll();
    if (index.row() >= static_cast<int>(all.size())) return {};
    const Tutorial& t = all[index.row()];
    switch (index.column()) {
        case 0: return QString::fromStdString(t.getTitle());
        case 1: return QString::fromStdString(t.getPresenter());
        case 2: return QString("%1m %2s")
                    .arg(t.getDuration().getMinutes())
                    .arg(t.getDuration().getSeconds());
        case 3: return t.getLikes();
        case 4: return QString::fromStdString(t.getLink());
        default: return {};
    }
}

QVariant WatchListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return {};
    static const char* headers[] = {"Title", "Presenter", "Duration", "Likes", "Link"};
    if (section < 0 || section >= 5) return {};
    return headers[section];
}

#include "moc_watchlist_model.cpp"

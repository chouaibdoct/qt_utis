#include "custumsortfilterproxymodel.h"

#include <QDebug>
W_OBJECT_IMPL(CustumSortFilterProxyModel);

CustumSortFilterProxyModel::CustumSortFilterProxyModel(
    int column, std::function<bool(const QVariant &)> pred, QObject *parent)
    : QSortFilterProxyModel(parent), _column{column}, _predicate{pred} {}

bool CustumSortFilterProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex &source_parent) const {
  if (_enabled)
    return _predicate(
        sourceModel()->index(source_row, _column, source_parent).data());
  else
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

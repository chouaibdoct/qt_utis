#include "modifyflagsproxymodel.h"
W_OBJECT_IMPL(ModifyFlagsProxyModel);

ModifyFlagsProxyModel::ModifyFlagsProxyModel(int column, QObject *parent)
    : QIdentityProxyModel(parent), _column{column} {}

ModifyFlagsProxyModel::ModifyFlagsProxyModel(
    int column, std::function<bool(QVariant)> predicate, QObject *parent)
    : QIdentityProxyModel(parent), _column{column}, _predicate{predicate} {}

Qt::ItemFlags ModifyFlagsProxyModel::flags(const QModelIndex &index) const {
  bool toModify = _predicate(index.siblingAtColumn(_column).data());

  if (toModify)
    return (QIdentityProxyModel::flags(index) | _toAddFlags) &
           (~_toRemoveFlags);
  else
    return QIdentityProxyModel::flags(index);
}

Qt::ItemFlags ModifyFlagsProxyModel::flagsToAdd() const { return _toAddFlags; }

void ModifyFlagsProxyModel::addFlags(const Qt::ItemFlags &toAddFlags) {
  beginResetModel();
  _toAddFlags = toAddFlags;
  endResetModel();
}

Qt::ItemFlags ModifyFlagsProxyModel::flagsToRemove() const {
  return _toRemoveFlags;
}

void ModifyFlagsProxyModel::removeFlags(const Qt::ItemFlags &toRemoveFlags) {
  beginResetModel();
  _toRemoveFlags = toRemoveFlags;
  endResetModel();
}

#include "colorporxymodel.h"

#include <QBrush>
#include <QColor>
W_OBJECT_IMPL(ColorPorxyModel);

ColorPorxyModel::ColorPorxyModel(int column, QObject *parent)
    : QIdentityProxyModel(parent), _column{column} {}

ColorPorxyModel::ColorPorxyModel(int column, std::function<bool(QVariant)> pred,
                                 QObject *parent)
    : QIdentityProxyModel(parent), _column{column}, _predicate{pred} {}

QVariant ColorPorxyModel::data(const QModelIndex &index, int role) const {
  if (!byBackground) {
    if (role == Qt::ForegroundRole &&
        !_predicate(index.siblingAtColumn(_column).data()))
      return QVariant(QBrush(color));
  } else {
    if (role == Qt::BackgroundRole &&
        !_predicate(index.siblingAtColumn(_column).data()))
      return QVariant(QBrush(color));
  }
  return QIdentityProxyModel::data(index, role);
}

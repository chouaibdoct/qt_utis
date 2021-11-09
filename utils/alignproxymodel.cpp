#include "alignproxymodel.h"

W_OBJECT_IMPL(AlignProxyModel);

AlignProxyModel::AlignProxyModel(int column, int align, QObject *parent)
    : QIdentityProxyModel(parent), _col{column}, _alignment{align} {}

QVariant AlignProxyModel::data(const QModelIndex &index, int role) const {
  if (_col < 0) {
    if (role == Qt::TextAlignmentRole) return _alignment;
  } else {
    if (index.column() == _col && role == Qt::TextAlignmentRole)
      return _alignment;
  }
  return QIdentityProxyModel::data(index, role);
}

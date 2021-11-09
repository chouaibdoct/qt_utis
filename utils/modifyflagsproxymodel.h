#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QIdentityProxyModel>

#include "utils_global.h"

class ModifyFlagsProxyModel : public QIdentityProxyModel {
  W_OBJECT(ModifyFlagsProxyModel);

 public:
  explicit ModifyFlagsProxyModel(int column, QObject *parent = nullptr);
  explicit ModifyFlagsProxyModel(int column, std::function<bool(QVariant)>,
                                 QObject *parent = nullptr);
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  inline std::function<bool(QVariant)> predicate() const { return _predicate; };
  inline void setPredicate(const std::function<bool(QVariant)> &predicate) {
    beginResetModel();
    _predicate = predicate;
    endResetModel();
  };
  inline int column() const { return _column; };
  inline void setColumn(int column) {
    beginResetModel();
    _column = column;
    endResetModel();
  }

  Qt::ItemFlags flagsToAdd() const;
  void addFlags(const Qt::ItemFlags &toAddFlags);

  Qt::ItemFlags flagsToRemove() const;
  void removeFlags(const Qt::ItemFlags &toRemoveFlags);

 private:
  Qt::ItemFlags _toAddFlags = Qt::NoItemFlags;
  Qt::ItemFlags _toRemoveFlags = Qt::NoItemFlags;
  int _column = -1;
  std::function<bool(QVariant)> _predicate = [](QVariant par) {
    return par.toBool();
  };
};


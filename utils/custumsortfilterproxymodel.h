#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QSortFilterProxyModel>
#include <functional>

#include "utils_global.h"
class CustumSortFilterProxyModel : public QSortFilterProxyModel {
    W_OBJECT(CustumSortFilterProxyModel);

 public:
  CustumSortFilterProxyModel(int column,
                             std::function<bool(const QVariant &)> pred,
                             QObject *parent = nullptr);
  int column() const { return _column; }
  void setColumn(int column) { _column = column; }
  std::function<bool(QVariant)> predicate() const { return _predicate; }
  void setPredicate(const std::function<bool(QVariant)> &predicate) {
    _predicate = predicate;
  }
  bool isEnabled() const { return _enabled; }

  // public slots:
  void setEnabled(bool enabled) {
    if (_enabled != enabled) {
      _enabled = enabled;
      emit enabledChange(_enabled);
    }
  };
  W_SLOT(setEnabled);


 signals:
  void enabledChange(bool newValue) W_SIGNAL(enabledChange, newValue);

 protected:
  bool filterAcceptsRow(int source_row,
                        const QModelIndex &source_parent) const override;

 private:
  int _column;
  std::function<bool(const QVariant &)> _predicate;
  bool _enabled = true;
};


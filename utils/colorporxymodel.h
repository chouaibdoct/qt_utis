#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>
#include <QColor>
#include <QIdentityProxyModel>
#include <QtDebug>

#include "utils_global.h"

class ColorPorxyModel : public QIdentityProxyModel {
  W_OBJECT(ColorPorxyModel);

 public:
  explicit ColorPorxyModel(int column, QObject *parent = nullptr);
  explicit ColorPorxyModel(int column, std::function<bool(QVariant)>,
                           QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role) const override;
  inline void setColor(QColor c) { color = std::move(c); };
  inline QColor Color(void) const { return color; };
  inline void setBybackground(bool st) { byBackground = st; };
  inline bool isBybackground(void) const { return byBackground; };
  inline std::function<bool(QVariant)> predicate() const { return _predicate; };
  inline void setPredicate(const std::function<bool(QVariant)> &predicate) {
    _predicate = predicate;
  };

 private:
  int _column;
  std::function<bool(QVariant)> _predicate = [](QVariant par) {
    return par.toDouble() > 0 ? true : false;
  };
  QColor color = Qt::darkRed;
  bool byBackground = false;
};

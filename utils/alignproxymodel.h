#pragma once

#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QIdentityProxyModel>

#include "utils_global.h"

class AlignProxyModel : public QIdentityProxyModel {
  W_OBJECT(AlignProxyModel);

 public:
  explicit AlignProxyModel(int column, int align, QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role) const override;

  inline int alignment() const { return _alignment; };
  inline void setAlignment(int alignment) { _alignment = alignment; };

  inline int col() const { return _col; };
  inline void setCol(int column_to_align) { _col = column_to_align; };

 private:
  int _col;
  int _alignment;
};

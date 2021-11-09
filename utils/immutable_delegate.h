#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QStyledItemDelegate>

#include "utils_global.h"

class ImmutableDelegate : public QStyledItemDelegate {
  W_OBJECT(ImmutableDelegate);

public:
  explicit ImmutableDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}

protected:
  bool editorEvent(QEvent *event, QAbstractItemModel *model,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) override {
    Q_UNUSED(event)
    Q_UNUSED(model)
    Q_UNUSED(option)
    Q_UNUSED(index)
    return false;
  }
  QWidget *createEditor(QWidget *, const QStyleOptionViewItem &,
                        const QModelIndex &) const override
  //    { return Q_NULLPTR; }
  {
    return nullptr;
  }
};
W_OBJECT_IMPL_INLINE(ImmutableDelegate);

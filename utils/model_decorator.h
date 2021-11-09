
#pragma once
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <qabstractproxymodel.h>
#include <qnamespace.h>
#include <qsqltablemodel.h>
#include <type_traits>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

template <class T> class Mimed : public T {
public:
  template <typename... Args>
  Mimed(Args &&...args) : T(std::forward<Args>(args)...) {}
  inline QStringList mimeTypes() const override {
    if constexpr (std::is_base_of_v<QSqlTableModel, T>) {
      QStringList types;
      types << T::tableName();
      return types;
    } else if (std::is_base_of_v<QAbstractProxyModel, T>) {
      return T::sourceModel()->mimeTypes();
    }
  }
};

template <class T, Qt::DropAction action> class DragedAction : public T {
public:
  template <typename... Args>
  DragedAction(Args &&...args) : T(std::forward<Args>(args)...) {}
  inline Qt::DropActions supportedDragActions() const override {
    return T::supportedDragActions() | action;
  }
};

template <class T, Qt::DropAction action> class DropedAction : public T {
public:
  template <typename... Args>
  DropedAction(Args &&...args) : T(std::forward<Args>(args)...) {}
  inline Qt::DropActions supportedDropActions() const override {
    return T::supportedDropActions() | action;
  }
};

template <class T>
using DragedCopyMove =
    DragedAction<DragedAction<T, Qt::CopyAction>, Qt::MoveAction>;
template <class T>
using DropedCopyMove =
    DropedAction<DropedAction<T, Qt::CopyAction>, Qt::MoveAction>;

template <class T>
using DecoratedModel = Mimed<DropedCopyMove<DragedCopyMove<T>>>;

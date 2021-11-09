
#pragma once
#include <qabstractitemmodel.h>
#include <qglobal.h>
#include <qsqlrelationaltablemodel.h>
#include <qsqltablemodel.h>
#include <qstyleditemdelegate.h>
#include <tuple>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QDebug>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QStyledItemDelegate>

#include <algorithm>
#include <vector>

class TreeSqlRelationalDelegate : public QStyledItemDelegate {
  W_OBJECT(TreeSqlRelationalDelegate)

public:
  inline explicit TreeSqlRelationalDelegate(QObject *parent = nullptr)
      : QStyledItemDelegate(parent) {}

  QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override {

    // const QSqlRelationalTableModel *sqlModel =
    //     qobject_cast<const QSqlRelationalTableModel *>(index.model());
    // QSqlTableModel *childModel =
    //     sqlModel ? sqlModel->relationModel(index.column()) : nullptr;
    // if (!childModel) {
    //   const QAbstractProxyModel *proxyModel =
    //       qobject_cast<const QAbstractProxyModel *>(index.model());
    //   if (proxyModel) {
    //     sqlModel = qobject_cast<const QSqlRelationalTableModel *>(
    //         proxyModel->sourceModel());
    //     childModel =
    //         sqlModel ? sqlModel->relationModel(index.column()) : nullptr;
    //   }
    // }

    // if (!childModel) {
    //   return QStyledItemDelegate::createEditor(aParent, option, index);
    // }

    // QComboBox *combo = new QComboBox(aParent);
    // combo->setModel(childModel);
    // combo->setModelColumn(childModel->fieldIndex(
    //     sqlModel->relation(index.column()).displayColumn()));
    // combo->installEventFilter(const_cast<TreeSqlRelationalDelegate *>(this));
    // return combo;

    if (auto [sqlModel, childModel] = realModel(index);
        sqlModel && childModel) {
      QComboBox *combo = new QComboBox(aParent);
      combo->setModel(childModel);
      combo->setModelColumn(childModel->fieldIndex(
          sqlModel->relation(index.column()).displayColumn()));
      combo->installEventFilter(const_cast<TreeSqlRelationalDelegate *>(this));
      return combo;
    } else {
      return QStyledItemDelegate::createEditor(aParent, option, index);
    }
  };

  void setEditorData(QWidget *editor, const QModelIndex &index) const override {
    // QString strVal = "";
    // const QSqlRelationalTableModel *sqlModel =
    //     qobject_cast<const QSqlRelationalTableModel *>(index.model());
    // if (!sqlModel) {
    //   const QAbstractProxyModel *proxyModel =
    //       qobject_cast<const QAbstractProxyModel *>(index.model());
    //   if (proxyModel) {
    //     strVal = proxyModel->data(index).toString();
    //   }
    // } else {
    //   strVal = sqlModel->data(index).toString();
    // }

    // QComboBox *combo = qobject_cast<QComboBox *>(editor);
    // if (strVal.isEmpty() || !combo) {
    //   QStyledItemDelegate::setEditorData(editor, index);
    //   return;
    // }
    // combo->setCurrentIndex(combo->findText(strVal));
    auto [sqlModel, childModel] = realModel(index);
    if (QComboBox *combo = qobject_cast<QComboBox *>(editor);
        index.isValid() && sqlModel && childModel && combo) {
      combo->setCurrentIndex(combo->findText(index.data().toString()));
    } else {
      QStyledItemDelegate::setEditorData(editor, index);
    }
  }

  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override {
    if (!index.isValid())
      return;

    QSqlRelationalTableModel *sqlModel =
        qobject_cast<QSqlRelationalTableModel *>(model);
    QAbstractProxyModel *proxyModel = nullptr;
    if (!sqlModel) {
      proxyModel = qobject_cast<QAbstractProxyModel *>(model);
      if (proxyModel)
        sqlModel =
            qobject_cast<QSqlRelationalTableModel *>(proxyModel->sourceModel());
    }

    QSqlTableModel *childModel =
        sqlModel ? sqlModel->relationModel(index.column()) : nullptr;
    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (!sqlModel || !childModel || !combo) {
      QStyledItemDelegate::setModelData(editor, model, index);
      return;
    }

    int currentItem = combo->currentIndex();
    int childColIndex = childModel->fieldIndex(
        sqlModel->relation(index.column()).displayColumn());
    int childEditIndex = childModel->fieldIndex(
        sqlModel->relation(index.column()).indexColumn());

    if (proxyModel) {
      proxyModel->setData(
          index,
          childModel->data(childModel->index(currentItem, childColIndex),
                           Qt::DisplayRole),
          Qt::DisplayRole);
      proxyModel->setData(
          index,
          childModel->data(childModel->index(currentItem, childEditIndex),
                           Qt::EditRole),
          Qt::EditRole);
    } else {
      sqlModel->setData(
          index,
          childModel->data(childModel->index(currentItem, childColIndex),
                           Qt::DisplayRole),
          Qt::DisplayRole);
      sqlModel->setData(
          index,
          childModel->data(childModel->index(currentItem, childEditIndex),
                           Qt::EditRole),
          Qt::EditRole);
    }
  }

private:
  std::tuple<const QSqlRelationalTableModel *, QSqlTableModel *>
  realModel(const QModelIndex &index) const {
    for (auto *model = index.model();;) {
      if (auto sqlModel = qobject_cast<const QSqlRelationalTableModel *>(model);
          sqlModel) {
        QSqlTableModel *childModel = sqlModel->relationModel(index.column());
        return std::make_tuple(sqlModel, childModel);

      } else if (auto proxyModel =
                     qobject_cast<const QAbstractProxyModel *>(model);
                 proxyModel) {
        model = proxyModel->sourceModel();
        continue;
      } else {
        return std::make_tuple(nullptr, nullptr);
      }
    }
  }
};

W_OBJECT_IMPL_INLINE(TreeSqlRelationalDelegate);

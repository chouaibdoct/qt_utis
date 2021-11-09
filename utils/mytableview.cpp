#include "mytableview.h"

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMimeData>
#include <QMainWindow>
#include <QPushButton>
#include <QTabBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <qboxlayout.h>

W_OBJECT_IMPL(MyTableView);
/*
MyTableView::MyTableView(QVector<int> hidenColumns, QWidget *parent)
    : Copyable<QTableView>(parent) {
  setHidenColumns(hidenColumns);
  QObject::connect(this->model(), &QAbstractItemModel::modelReset,
                   [&]() { this->setModel(this->model()); });
}
*/
MyTableView::MyTableView(QWidget *parent) : Copyable<QTableView>(parent) {
  config_table();
  
  QVBoxLayout *l = new QVBoxLayout(this);
  QToolBar *t = new QToolBar;
  l->addWidget(t);
  l->addWidget(this);
}

void MyTableView::setHidenColumns(const QVector<int> &value) {
  if (value != _hidenColumns) {
    _hidenColumns = value;
    for (auto el : _hidenColumns) {
      QTableView::hideColumn(el);
    }
    config_table();
  }
}

void MyTableView::setShownColumns(const QVector<int> &value) {
  if (value != _shownColumns) {
    _shownColumns = value;
    for (int i = 0; i != this->model()->columnCount(); ++i)
      hideColumn(i);
    for (auto el : _shownColumns) {
      QTableView::showColumn(el);
    }
    config_table();
  }
}

void MyTableView::setModel(QAbstractItemModel *model) {
  QTableView::setModel(model);
  for (auto el : _hidenColumns) {
    QTableView::hideColumn(el);
  }
  config_table();
}

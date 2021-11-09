#pragma once
#include <QAbstractTableModel>

#include "utils_global.h"

class SaveHeaders {
 public:
  SaveHeaders(QAbstractTableModel& table) : _table{table} {
    for (int i = 0; i != _table.columnCount(); ++i) {
      list.push_back(_table.headerData(i, Qt::Horizontal));
    }
  };
  virtual ~SaveHeaders() {
    for (int i = 0; i != _table.columnCount(); ++i) {
      _table.setHeaderData(i, Qt::Horizontal, list[i]);
    }
  }
  QVariantList list;
  QAbstractTableModel& _table;
};

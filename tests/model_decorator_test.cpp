#include "utils/table_to_tree_proxy_model.h"
#include "utils/test.h"

#include "utils/model_decorator.h"

#include <QApplication>
#include <QSqlTableModel>
#include <QTableView>
#include <QTreeView>
#include <qidentityproxymodel.h>


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qDebug() << "sql connection =" << connect_db();
  DecoratedModel<QSqlTableModel> tb;
  tb.setEditStrategy(QSqlTableModel::OnManualSubmit);
  tb.setTable("admin.composant_type");
  tb.setSort(2, Qt::AscendingOrder);
  tb.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
  tb.select();
  // DecoratedModel<QIdentityProxyModel> p;
  // p.setSourceModel(&tb);
  
  qDebug() <<tb.mimeTypes();
}


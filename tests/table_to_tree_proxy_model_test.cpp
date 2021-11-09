#include "utils/model_decorator.h"
#include "utils/table_to_tree_proxy_model.h"
#include "utils/tree_sql_relatioanl_delegate.h"
#include "utils/test.h"

#include <qabstractitemmodel.h>
#include <qabstractitemview.h>
#include <qdebug.h>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QAbstractItemModelTester>
#include <QApplication>
#include <QPushButton>
#include <QSqlTableModel>
#include <QStyleFactory>
#include <QTableView>
#include <QTreeView>
#include <QUuid>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  //#if (defined (_WIN32) || defined (_WIN64))
  //  a.setStyle(QStyleFactory::create("Fusion"));
  //#endif
  qDebug() << "sql connection =" << connect_db();
  Mimed<QSqlTableModel> tb;
  //    tb.setEditStrategy(QSqlTableModel::OnFieldChange);
  tb.setEditStrategy(QSqlTableModel::OnManualSubmit);
  tb.setTable("admin.composant_type");
  tb.setSort(2, Qt::AscendingOrder);
  tb.setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
  tb.select();

  GetNewInt increment(tb.tableName().append("_id_seq"));
  TableToTreeProxyModel<int> proxyy(2, 3, increment);
  proxyy.setIdCol(2);
  proxyy.setParentCol(3);
  proxyy.setSourceModel(&tb);
  // QObject::connect(&tb, &QSqlTableModel::modelReset, &tb,
  //                  [&]() { qDebug() << "Table model reset"; });
  // QObject::connect(&tb, &QSqlTableModel::rowsInserted,
  //                  [&](const QModelIndex &, int f, int l) {
  //                    qDebug() << "Table rows inserted" << f << l;
  //                    // qDebug() << tb.index(l, 0).data();
  //                    // qDebug() << tb.index(l, 1).data();
  //                    // qDebug() << tb.index(l, 2).data();
  //                    // qDebug() << tb.index(l, 3).data();
  //                  });
  // QObject::connect(&tb, &QSqlTableModel::dataChanged,
  //                  [&](const QModelIndex &f, const QModelIndex &s) {
  //                    qDebug() << "Table data changed\n" << f << s;
  //                    qDebug() << f.data();
  //                  });

  // QObject::connect(&proxyy, &decltype(proxyy)::modelReset, &proxyy,
  //                  [&]() { qDebug() << "proxyy model reset "; });
  // QObject::connect(&proxyy, &decltype(proxyy)::rowsInserted,
  //                  [&](const QModelIndex &parent, int f, int l) {
  //                    qDebug() << "proxyy rows inserted" << f << l;
  //                  });
  // QObject::connect(&proxyy, &decltype(proxyy)::dataChanged,
  //                  [&](const QModelIndex &f, const QModelIndex &s) {
  //                    qDebug() << "proxyy data changed\n" << f << s;
  // 		     // qDebug() << f.data();
  //                  });

  QPushButton addBtn("add Item");
  QObject::connect(&addBtn, &QPushButton::clicked, [&]() {
    auto r = tb.record();
    r.setGenerated("id", false);
    r.setValue("nom", QString("chouaibyyassine"));
    r.setValue("description", "descript");
    tb.insertRecord(-1, r);
    qDebug() << tb.index(0, 2).data().isValid();
    qDebug() << tb.index(0, 2).data().isNull();
    qDebug() << tb.index(0, 2).data().canConvert<int>();
  });
  QPushButton saveBtn("save Changes");
  QObject::connect(&saveBtn, &QPushButton::clicked, [&]() { tb.submitAll(); });

  QTableView tbView;
  tbView.setModel(&tb);
  tbView.setSelectionBehavior(QAbstractItemView::SelectRows);
  tbView.setSelectionMode(QAbstractItemView::SingleSelection);

  // QTreeView other;
  // other.setModel(&tb);
  // other.setStyleSheet("QTreeView::item { padding: 10px }");
  // other.show();

  QTreeView treeView;
  treeView.setModel(&proxyy);
  treeView.setItemDelegate(new TreeSqlRelationalDelegate(&treeView));
  treeView.setSelectionMode(QAbstractItemView::ExtendedSelection);
  treeView.setDragEnabled(true);
  treeView.setAcceptDrops(true);
  treeView.setDropIndicatorShown(true);
  treeView.setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
  treeView.setSelectionMode(QAbstractItemView::ExtendedSelection);
  treeView.setStyleSheet("QTreeView::item { padding: 7px }");

  //  treeView.setDefaultDropAction(Qt::MoveAction);
  QObject::connect(&proxyy, &TableToTreeProxyModel<int>::dropedIn,
                   [&](QModelIndex index) {
                     for (auto idx = index; idx.isValid(); idx = idx.parent()) {
                       treeView.expand(idx);
                     }
                   });
  QWidget *widgetTable(new QWidget);
  QHBoxLayout horizontalLayout;
  horizontalLayout.addWidget(&addBtn);
  horizontalLayout.addWidget(&saveBtn);
  QVBoxLayout verticalLayout(widgetTable);
  verticalLayout.addWidget(&tbView);
  verticalLayout.addLayout(&horizontalLayout);
  //  widgetTable->show();
  treeView.show();
  return a.exec();
};

//  qDebug()<<uid.isNull();

// TabletoToTreeProxyModel proxy(2, 3, nullptr);
// proxy.setSourceModel(&tb);

// QWidget window;

// QTreeView treeview;
// QTableView tblview;
// treeview.setModel(&proxy);
// treeview.setItemDelegate(new QSqlRelationalDelegate(&treeview));
// treeview.setRootIsDecorated(true);

// tblview.setModel(&tb);
// tblview.setItemDelegate(new QSqlRelationalDelegate(&tblview));

// QPushButton insertchildBtn("insert child ", &window);
// ////////////////////////////////
// ///QObject::connect(&insertBtn,&QPushButton::clicked,&proxy,[&proxy,&infra](){proxy.insertRows(0,1,infra);});
// QObject::connect(&insertchildBtn, &QPushButton::clicked, &proxy,
//                  [&proxy, &treeview]() {
//                    proxy.insertRows(0, 1, treeview.currentIndex());
//                  });
// QPushButton insertSibling("insert Sibling", nullptr);
// QObject::connect(&insertSibling, &QPushButton::clicked, &proxy,
// 		   [&proxy, &treeview]() {
// 		     proxy.insertRows(treeview.currentIndex().row(), 1,
// 				      treeview.currentIndex().parent());
// 		   });
// QPushButton deletBtn("delete", nullptr);
// QObject::connect(&deletBtn, &QPushButton::clicked, &proxy,
//                  [&proxy, &treeview]() {
//                    proxy.removeRows(treeview.currentIndex().row(), 0,
//                                     treeview.currentIndex().parent());
//                  });
// QPushButton addtoDb("insert directly intodatabase", nullptr);
// QObject::connect(&addtoDb, &QPushButton::clicked, &tb,
//                  [&proxy, &treeview, &tb] {
//                    auto rec = tb.record();
//                    rec.setValue("data", "essalam");
//                    proxy.insertRows(treeview.currentIndex().row(), 1,
//                                     treeview.currentIndex().parent(), rec);
//                  }
// );
// QVBoxLayout layout;
// layout.addWidget(&treeview);
// layout.addWidget(&insertchildBtn);
// layout.addWidget(&insertSibling);
// layout.addWidget(&deletBtn);
// layout.addWidget(&tblview);
// layout.addWidget(&addtoDb);

// window.setLayout(&layout);
// window.show();

//   QCoreApplication app(argc, argv);
//    tb.setTable("componenttypes");
//    tb.setRelation(tb.fieldIndex("geometrie"),QSqlRelation("geometrie","id","nom"));
//    tb.setJoinMode(QSqlRelationalTableModel::LeftJoin);
//    qDebug()<<proxy.rowCount(QModelIndex());

//   auto infra = proxy.index(3,0,QModelIndex());
//   auto cule  = infra.child(1,0);
//   qDebug()<<proxy.mapToSource(infra);
//      QPushButton insertBtn("insert ",nullptr);
//       QObject::connect(&insertBtn,&QPushButton::clicked,&proxy,[&proxy,&infra](){proxy.insertRows(0,1,infra);});
//   insertBtn.show();
//   QPushButton deletBtn("delete",nullptr);
//   QObject::connect(&deletBtn,&QPushButton::clicked,&proxy,[&proxy,&infra](){proxy.removeRows(1,0,infra);});
//   deletBtn.show();

//    qDebug()<<proxy.hasChildren(infra);
//    bool has_index=proxy.hasIndex(1,0,infra);
//    qDebug()<<proxy.rowCount(infra)<<"\t"<<proxy.columnCount(infra);
//    qDebug()<<has_index;

//    qDebug()<<infra.data()<<infra.sibling(infra.row(),2).data();
//    qDebug() <<proxy.index(0,0,infra).parent().parent().isValid();

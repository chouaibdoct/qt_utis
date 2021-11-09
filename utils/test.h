#pragma once 
#include <QDebug>
#include <QSqlDatabase>




#if (defined (_WIN32) || defined (_WIN64))
inline bool connect_db(QString host = "localhost", QString userName = "sgoa_user") {
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName(host);
  db.setPort(5433);
  db.setDatabaseName("sgoa");
  //  db.setConnectOptions("sslmode=require");
  db.setUserName(userName);
  db.setPassword(" ");
  return db.open();
};
#endif

#if (defined (LINUX) || defined (__linux__))
inline bool connect_db(QString host = "winhost", QString userName = "sgoa_user") {
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName(host);
  db.setPort(5433);
  db.setDatabaseName("sgoa");
  //  db.setConnectOptions("sslmode=require");
  db.setUserName(userName);
  db.setPassword(" ");
  return db.open();
};
#endif

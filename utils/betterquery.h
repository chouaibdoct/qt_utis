#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QObject>
#include <QSqlError>
#include <QSqlQueryModel>

#include "property.h"
#include "utils_global.h"

/*
template <class T>
class BetterQueryModel : public QSqlQueryModel {
  W_OBJECT(BetterQueryModel)
 public:
  explicit BetterQueryModel(QString queryCore, const Property<T>* varibaleQuery,
                            QObject* parent = nullptr)
      : QSqlQueryModel(parent),
        queryString(queryCore),
        variable{*varibaleQuery} {
    configure();
    QObject::connect(&variable,
&Property<T>::valueChanged,this,&BetterQueryModel<T>::configure);
  }
  virtual ~BetterQueryModel() = default;

 private:
  void configure() {
    QString s = queryString.arg(variable.value());
    this->setQuery(s);
  };
  QString queryString;
  const Property<T>& variable;
};
W_OBJECT_IMPL(BetterQueryModel<T>, template <typename T>);
*/
template <typename... Args>
class BetterQueryModel : public QSqlQueryModel {
  W_OBJECT(BetterQueryModel)
 public:
  explicit BetterQueryModel(const QString &coreQuery,
                            const Property<Args> &... args,
                            QObject *parent = nullptr)
      : QSqlQueryModel{parent},
        coreQuery{coreQuery},
        t{std::forward_as_tuple(args...)} {
    init();
    config_connections();
  };
  void refresh() {init();};

 private:
  using QSqlQueryModel::setQuery;
  void init() {
    QString s = coreQuery;
    std::apply([&s](auto &&... args) { ((s = s.arg(args.value())), ...); }, t);
    //    qDebug() << s;
    this->setQuery(s);
    Q_ASSERT_X(this->lastError().type() != QSqlError::NoError, Q_FUNC_INFO,
               this->lastError().text());    
  };
  W_SLOT(init);
  void config_connections() {
    std::apply(
        [this](auto &&... args) {
          ((QObject::connect(&args, SIGNAL(valueChangedVoid()), this,
                             SLOT(init()))),
           ...);
        },
        this->t);
  }
  QString coreQuery;
  const std::tuple<const Property<Args> &...> t;
};
W_OBJECT_IMPL(BetterQueryModel<Args...>, template <typename... Args>);

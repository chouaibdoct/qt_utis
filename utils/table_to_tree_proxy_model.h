#pragma once

#include <qglobal.h>
#include <qmimedata.h>
#include <qnamespace.h>
#include <qtimer.h>
#include <qvariant.h>
#include <tuple>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QDebug>
#include <QMimeData>
#include <QMimeType>
#include <QModelIndex>
#include <QSqlRecord>
#include <QTimer>
#include <QUuid>
#include <QVariant>
#include <QtAlgorithms>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include <utils/model_decorator.h>
#include <utils/scope_logger.h>

template <class Begin, class End> class Raii {
public:
  Raii(Begin begin, End &&end) : _end{end} { begin(); };
  Raii(Begin begin, const End &end) : _end{end} { begin(); };
  ~Raii() { _end(); }
private:
  End _end;
};

class GetNewInt {
public:
  GetNewInt(QString sequanceName) : _sequanceName{sequanceName} {
    QSqlQuery q(QString("select last_value from %1 ;").arg(_sequanceName));
    if (q.next())
      i = q.value(0).toInt();
    else
      Q_ASSERT(false);
  };
  int operator()(void) { return ++i; };

private:
  QString _sequanceName;
  int i;
};

template <typename T> class TableToTreeProxyModel : public QAbstractProxyModel {
  W_OBJECT(TableToTreeProxyModel)

public:
  inline TableToTreeProxyModel(int idCol, int parentCol,
                               const std::function<T(void)> f,
                               QObject *parent = nullptr)
      : QAbstractProxyModel(parent), _idCol(idCol), _parentCol(parentCol),
        _f(f) {}

  inline virtual ~TableToTreeProxyModel() = default;

  inline void setSourceModel(QAbstractItemModel *sourceModel) override {
    emit beginResetModel();
    QAbstractTableModel *test =
        dynamic_cast<QAbstractTableModel *>(sourceModel);
    Q_ASSERT_X(test, __FUNCTION__, "source model should be table model");
    if (this->sourceModel()) {
      disconnect(this->sourceModel(),
                 &QAbstractItemModel::columnsAboutToBeInserted, this,
                 &TableToTreeProxyModel<T>::sourceColumnsAboutToBeInserted);
      disconnect(this->sourceModel(), &QAbstractItemModel::columnsInserted,
                 this, &TableToTreeProxyModel<T>::sourceColumnsInserted);

      disconnect(this->sourceModel(),
                 &QAbstractItemModel::columnsAboutToBeRemoved, this,
                 &TableToTreeProxyModel<T>::sourceColumnsAboutToBeRemoved);
      disconnect(this->sourceModel(), &QAbstractItemModel::columnsRemoved, this,
                 &TableToTreeProxyModel<T>::sourceColumnsRemoved);

      disconnect(this->sourceModel(), &QAbstractItemModel::dataChanged, this,
                 &TableToTreeProxyModel<T>::sourceDataChanged);

      disconnect(this->sourceModel(), &QAbstractItemModel::headerDataChanged,
                 this, &TableToTreeProxyModel<T>::sourceHeaderDataChanged);

      disconnect(this->sourceModel(), &QAbstractItemModel::modelAboutToBeReset,
                 this, &TableToTreeProxyModel<T>::sourceModelAboutToBeReset);
      disconnect(this->sourceModel(), &QAbstractItemModel::modelReset, this,
                 &TableToTreeProxyModel<T>::sourceModelReset);

      disconnect(this->sourceModel(),
                 &QAbstractItemModel::rowsAboutToBeInserted, this,
                 &TableToTreeProxyModel<T>::sourceRowsAboutToBeInserted);
      disconnect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this,
                 &TableToTreeProxyModel<T>::sourceRowsInserted);

      disconnect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved,
                 this, &TableToTreeProxyModel<T>::sourceRowsAboutToBeRemoved);
      disconnect(this->sourceModel(), &QAbstractItemModel::rowsRemoved, this,
                 &TableToTreeProxyModel<T>::sourceRowsRemoved);
    }
    QAbstractProxyModel::setSourceModel(sourceModel);
    //    init();

    connect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeInserted,
            this, &TableToTreeProxyModel<T>::sourceColumnsAboutToBeInserted);
    connect(this->sourceModel(), &QAbstractItemModel::columnsInserted, this,
            &TableToTreeProxyModel<T>::sourceColumnsInserted);

    connect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeRemoved,
            this, &TableToTreeProxyModel<T>::sourceColumnsAboutToBeRemoved);
    connect(this->sourceModel(), &QAbstractItemModel::columnsRemoved, this,
            &TableToTreeProxyModel<T>::sourceColumnsRemoved);

    connect(this->sourceModel(), &QAbstractItemModel::dataChanged, this,
            &TableToTreeProxyModel<T>::sourceDataChanged);

    connect(this->sourceModel(), &QAbstractItemModel::headerDataChanged, this,
            &TableToTreeProxyModel<T>::sourceHeaderDataChanged);

    connect(this->sourceModel(), &QAbstractItemModel::modelAboutToBeReset, this,
            &TableToTreeProxyModel<T>::sourceModelAboutToBeReset);
    connect(this->sourceModel(), &QAbstractItemModel::modelReset, this,
            &TableToTreeProxyModel<T>::sourceModelReset);

    connect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeInserted,
            this, &TableToTreeProxyModel<T>::sourceRowsAboutToBeInserted);
    connect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this,
            &TableToTreeProxyModel<T>::sourceRowsInserted);

    connect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &TableToTreeProxyModel<T>::sourceRowsAboutToBeRemoved);
    connect(this->sourceModel(), &QAbstractItemModel::rowsRemoved, this,
            &TableToTreeProxyModel<T>::sourceRowsRemoved);

    emit endResetModel();
  }

  inline QModelIndex
  mapFromSource(const QModelIndex &sourceIndex) const override {
    if (!sourceIndex.isValid()) {
      return QModelIndex();
    }

    // auto res = this->createIndex(table[sourceIndex.row()].j,
    // sourceIndex.column(),
    //                              table[sourceIndex.row()].id);
    //  return res;

    auto it = ith_el(sourceIndex.row());
    return this->createIndex(it->j, sourceIndex.column(), it->id);
  }
  inline QModelIndex mapToSource(const QModelIndex &proxyIndex) const override {
    if (!proxyIndex.isValid()) {
      return QModelIndex();
    }
    return sourceModel()->index(itOfId(proxyIndex.internalId())->i,
                                proxyIndex.column());
  }

  inline QVariant data(const QModelIndex &index,
                       int role = Qt::DisplayRole) const override {
    return sourceModel()->data(mapToSource(index), role);
  }

  inline bool setData(const QModelIndex &index, const QVariant &value,
                      int role = Qt::EditRole) override {
    return sourceModel()->setData(mapToSource(index), value, role);
  }
  inline Qt::ItemFlags flags(const QModelIndex &index) const override {
    if (!index.isValid())
      return Qt::NoItemFlags | Qt::ItemIsDropEnabled;
    return sourceModel()
        ->flags(mapToSource(index))
        .setFlag(Qt::ItemNeverHasChildren, false)
        .setFlag(Qt::ItemIsDragEnabled, true)
        .setFlag(Qt::ItemIsDropEnabled, true);
    // if (!index.isValid())
    //   return Qt::NoItemFlags;
    // return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
  }

  inline virtual QModelIndex index(int row, int column,
                                   const QModelIndex &parent) const override {

    it_type it;
    if (!parent.isValid())
      it = nthChildOf(table.end(), row);
    else
      it = nthChildOf(itOfId(parent.internalId()), row);

    QString msg_j = QString("it->j=%2 && row=%3").arg(it->j).arg(row);
    QString msg_parent = QString("it->parent=%2 &&parent.internalId()=%3")
                             .arg(it->parent)
                             .arg(parent.internalId());
    Q_ASSERT_X(it->j == row,
               ScopeLogger(__FUNCTION__)
                   .indentation()
                   .append(__FUNCTION__)
                   .toLatin1()
                   .data(),
               msg_j.toLatin1().data());
    Q_ASSERT_X(it->parent == parent.internalId(),
               ScopeLogger(__FUNCTION__)
                   .indentation()
                   .append(__FUNCTION__)
                   .toLatin1()
                   .data(),
               msg_parent.toLatin1().data());

    auto res = createIndex(row, column, it->id);
    //    qDebug()<<"res"<<res<<" id"<<res.internalId();
    return res;
  }

  inline virtual QModelIndex parent(const QModelIndex &child) const override {
    auto it = parentOf(itOfId(child.internalId()));
    if (isNull(itOfId(child.internalId())->parent)) {
      //    if (itOfId(child.internalId())->parent == null()) {
      return QModelIndex();
    } else if (it == table.end()) {
      ScopeLogger(__FUNCTION__).qcritical()
          << "problem when finding parent of" << child;
      return QModelIndex();
    }
    auto res = mapFromSource(sourceModel()->index(it->i, 0));
    return res;
  }

  inline virtual bool hasChildren(const QModelIndex &parent) const override {
    //   bool has=rowCount(parent)>0 ;
    //   QString msg= has?"has Childrens":"dont have childrens";
    //   if(parent.isValid())
    //       qDebug()<< "QMI valid row"<<
    //       parent.row()<<"column"<<parent.column()<<"id"<<parent.internalId()<<msg;
    //   else
    //       qDebug()<< "invalid QMI"<<msg;

    return (rowCount(parent) > 0);
  }

  inline virtual int rowCount(const QModelIndex &parent) const override {
    // ScopeLogger sl(__FUNCTION__);
    if (!parent.isValid()) {
      return countChildsOf(table.end());
    } else {
      int n = countChildsOf(itOfId(parent.internalId()));
      return n;
    }
  }

  inline virtual int columnCount(const QModelIndex &) const override {
    return sourceModel()->columnCount();
  }

  inline virtual QVariant headerData(int section, Qt::Orientation orientation,
                                     int role) const override {
    return sourceModel()->headerData(section, orientation, role);
  }
  inline virtual bool setHeaderData(int section, Qt::Orientation orientation,
                                    const QVariant &value, int role) override {
    return sourceModel()->setHeaderData(section, orientation, value, role);
  };

  inline Qt::DropActions supportedDragActions() const override {
    return Qt::MoveAction | Qt::CopyAction;
  }

  inline Qt::DropActions supportedDropActions() const override {
    return Qt::MoveAction | Qt::CopyAction;
  }

  inline QStringList mimeTypes() const override {
    return this->sourceModel()->mimeTypes();
  }

  QMimeData *mimeData(const QModelIndexList &indexes) const override {
    QMimeData *data = new QMimeData();
    QString format = mimeTypes().at(0);
    QStringList encodedData;

    for (const QModelIndex &index : indexes) {
      if (index.isValid() && index.column() == _idCol) {
        encodedData << this->data(index, Qt::DisplayRole).toString();
      }
    }
    QByteArray encoded = encodedData.join(";").toLatin1();

    data->setData(format, encoded);
    return data;
  }

  inline std::tuple<bool, int, QVariantList>
  getDataFromMime(const QMimeData *data) const {
    bool hasformat = false;
    QString dat;
    int j;
    const QStringList modelTypes = mimeTypes();
    for (int i = 0; i < modelTypes.count(); ++i) {
      if (data->hasFormat(modelTypes.at(i))) {
        hasformat |= true;
        dat = data->data(modelTypes[i]);
        j = i;
        break;
      }
    }
    if (!hasformat)
      return std::make_tuple(false, -1, QVariantList());

    QStringList list = dat.split(';', QString::SkipEmptyParts);
    QVariantList indexes;
    for (const QString &idx : list) {
      indexes.push_back(idx);
    }
    return std::tie(hasformat, j, indexes);
  };

  inline virtual bool canDropDataDecorator(bool hasFormat, int mimeIndex,
                                           QVariantList indexes,
                                           const QModelIndex &parent) const {
    if ((!hasFormat) || mimeIndex != 0)
      return false;
    bool childParentConditionAccepted = true;
    for (QVariant v : indexes) {
      it_type it_child = itOfId(v.value<T>());
      it_type it_parent =
          parent.isValid()
              ? itOfId(parent.siblingAtColumn(_idCol).data().value<T>())
              : table.end();
      childParentConditionAccepted =
          childParentConditionAccepted &&
          !(isSameOrParentRecursive(it_parent, it_child));
    }
    return childParentConditionAccepted;
  };
  inline bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column,
                              const QModelIndex &parent) const override {
    ScopeLogger sl(__FUNCTION__);
    sl.qdebug() << action;
    if (!(action & supportedDropActions())) {
      sl.qdebug() << "return: DropAction not supported";
      return false;
    }
    if (row >= 0 || column >= 0) {
      sl.qdebug() << "return: row|column >=0";
      return false;
    }
    auto [hasFormat, mimeIndex, indexes] = getDataFromMime(data);

    return canDropDataDecorator(hasFormat, mimeIndex, indexes, parent);
  }

  inline virtual bool dropDataDecorator(int mimeIndex,
                                        const QVariantList &indexes,
                                        const QModelIndex &parent) {
    if (mimeIndex == 0) {
      for (const auto &idx : indexes)
        for (int i = 0; i < this->sourceModel()->rowCount(); ++i) {
          if (sourceModel()->index(i, _idCol, QModelIndex()).data() == idx) {
            sourceModel()->setData(
                sourceModel()->index(i, _parentCol, QModelIndex()),
                parent.siblingAtColumn(_idCol).data());
          }
        }
      return true;
    }
    return false;
  };
  
  inline bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                           int row, int column,
                           const QModelIndex &parent) override {
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(action);

    ScopeLogger sl(__FUNCTION__);

    QModelIndex sourceParent = mapToSource(parent);
    Raii r{[]() {}, [&]() { emit dropedIn(mapFromSource(sourceParent)); }};

    auto [hasFormat, j, indexesV] = getDataFromMime(data);
    return dropDataDecorator(j, indexesV, parent);
  }

private:
  int _idCol = -1;
  int _parentCol = -1;

public:
  void setIdCol(int newIdCol) { _idCol = newIdCol; }
  int idCol(void) const { return _idCol; };
  void setParentCol(int newParentCol) { _parentCol = newParentCol; }
  int parentCol(void) const { return _parentCol; };

protected:
  void resetInternalData() {
    ScopeLogger sl(__FUNCTION__);
    init();
  };
  W_SLOT(resetInternalData)

public:
  struct entry {
    entry() = delete;
    entry(int _i, T _id, T _parent, int _j)
        : i{_i}, id{_id}, parent{_parent}, j{_j} {}
    int i;
    T id;
    T parent;
    int j;
  };
  using it_type = typename std::vector<entry>::iterator;

  mutable std::vector<entry> table;
  std::function<T(void)> _f;
  inline void init() {
    ScopeLogger sl(__FUNCTION__);
    table.clear();
    QVariant v;
    for (int i = 0; i < sourceModel()->rowCount(); ++i) {
      if (sourceModel()->index(i, _idCol).data().isNull()) {
        sl.qdebug() << "id null for " << sourceModel()->index(i, _idCol);
        Raii r(
            [&, this]() {
              disconnect(this->sourceModel(), &QAbstractItemModel::dataChanged,
                         this, &TableToTreeProxyModel<T>::sourceDataChanged);
            },
            [&, this]() {
              connect(this->sourceModel(), &QAbstractItemModel::dataChanged,
                      this, &TableToTreeProxyModel<T>::sourceDataChanged);
            });
        const_cast<QAbstractItemModel *>(sourceModel())
            ->setData(sourceModel()->index(i, _idCol), _f());
        table.emplace_back(
            i, sourceModel()->index(i, _idCol).data().template value<T>(),
            sourceModel()->index(i, _parentCol).data().template value<T>(), -1);
        // table.emplace_back(
        //     i, _f(),
        //     sourceModel()->index(i, _parentCol).data().template value<T>(),
        //     -1);
      } else {
        table.emplace_back(
            i, sourceModel()->index(i, _idCol).data().template value<T>(),
            sourceModel()->index(i, _parentCol).data().template value<T>(), -1);
      }
    }
    updateJ();
  }

  inline void updateJ(void) {
    ScopeLogger sl(__FUNCTION__);

    std::stable_sort(
        begin(table), end(table),
        [](const entry &e1, const entry &e2) { return e1.id == e2.parent; });
    // for (auto e : table)
    //   sl.qdebug() << e.i << "\t" << e.id << "\t" << e.parent << "\t" <<
    //   e.j;
    for (auto e = table.begin(); e != table.end(); ++e)
      e->j = classementOf(e);
    for (auto e : table)
      sl.qdebug() << e.i << "\t" << e.id << "\t" << e.parent << "\t" << e.j;
  }
  inline int countChildsOf(it_type it) const {
    //    ScopeLogger sl(__FUNCTION__);
    if (it == table.end())
      return std::count_if(std::begin(table), std::end(table),
                           [this](entry e) { return (isNull(e.parent)); });
    return std::count_if(std::begin(table), std::end(table),
                         [&it](entry e) { return (e.parent == it->id); });
  }

  inline it_type nthChildOf(it_type parentIt, int n) const {
    //    ScopeLogger sl(__FUNCTION__);
    int count = -1;
    it_type returnVal;
    if (parentIt == table.end())
      returnVal =
          std::find_if(table.begin(), table.end(), [this, &count, &n](entry e) {
            return ((isNull(e.parent)) && ++count == n);
          });
    else
      returnVal = std::find_if(
          table.begin(), table.end(), [&count, &parentIt, &n](entry e) {
            return ((e.parent == parentIt->id) && ++count == n);
          });

    auto message =
        QString("can't find the %2th child of element with id=%3 ,i=%4")
            //            .arg(sl.indentation())
            .arg(n)
            .arg(parentIt->id)
            .arg(parentIt->i);
    Q_ASSERT_X(returnVal != table.end(),
               ScopeLogger(__FUNCTION__).msg.toLatin1().data(),
               message.toLatin1().data());

    return returnVal;
  }
  inline it_type itOfId(T Id) const {
    auto returnVal = std::find_if(table.begin(), table.end(),
                                  [Id](entry e) { return e.id == Id; });

    //    Q_ASSERT(returnVal != table.end());
    return returnVal;
  }

  inline it_type ith_el(int n) const {
    // return std::next(table.begin(), n);
    return std::find_if(table.begin(), table.end(),
                        [n](entry e) { return e.i == n; });
  }
  inline it_type parentOf(it_type it) const {
    //    return itOfId(it->parent);
    return std::find_if(table.begin(), table.end(),
                        [&](entry e) { return e.id == it->parent; });
  }
  inline int classementOf(it_type itChild) const {
    T par = itChild->parent;
    return std::count_if(table.begin(), itChild,
                         [par](entry e) { return e.parent == par; });
  }
  inline void print(const QModelIndex &index) const {
    qDebug() << "r " << index.row() << "c " << index.column() << "id "
             << index.model()
                    ->data(index.model()->sibling(index.row(), _idCol, index))
                    .toInt()
             << "name"
             << index.model()
                    ->data(index.model()->sibling(index.row(), 2, index))
                    .toString();
  }
  inline static T null() { return T(); }
  inline static bool isNull(const T &v) {
    if constexpr (std::is_integral_v<T>) {
      return v == null();
    } else
      return v.isNull();
  };
  inline bool isSameOrParentRecursive(const it_type &child,
                                      const it_type &parent) const {
    if (child == parent)
      return true;

    for (auto itt = child;;) {
      itt = std::find_if(std::begin(table), std::end(table),
                         [&](const entry &e) { return e.id == itt->parent; });
      if (itt == parent)
        return true;

      if (itt == table.end())
        return false;
    }
    Q_ASSERT(false);
  }

public:
  inline void sourceDataChanged(const QModelIndex &source_top_left,
                                const QModelIndex &source_bottom_right) {
    ScopeLogger sl(__FUNCTION__);
    Q_ASSERT(sourceModel());
    Q_ASSERT(source_top_left.isValid());
    Q_ASSERT(source_bottom_right.isValid());

    if ((source_top_left.column() <= _idCol &&
         source_bottom_right.column() >= _idCol) ||
        (source_top_left.column() <= _parentCol &&
         source_bottom_right.column() >= _parentCol)) {
      beginResetModel();
      endResetModel();
    } else
      for (int r(source_top_left.row()); r <= source_bottom_right.row(); ++r)
        for (int c(source_top_left.column()); c <= source_bottom_right.column();
             ++c) {
          QModelIndex proxyIndex = mapFromSource(sourceModel()->index(r, c));
          if (!proxyIndex.isValid())
            return; // incomplete record with missing id value; safely ignore
                    // as not used in QXTreeModel
          else
            emit dataChanged(proxyIndex, proxyIndex);
        }
  };
  W_SLOT(sourceDataChanged);

  inline void sourceRowsAboutToBeInserted(const QModelIndex, int, int) {
    ScopeLogger sl(__FUNCTION__);
    emit beginResetModel();
  }
  W_SLOT(sourceRowsAboutToBeInserted);

  inline void sourceRowsInserted(const QModelIndex &parent, int start,
                                 int end) {
    ScopeLogger sl(__FUNCTION__);
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    // QAbstractTableModel *model = const_cast<QAbstractTableModel *>(
    //     dynamic_cast<const QAbstractTableModel *>(parent.model()));
    // for (int i = start; i != end; i++)
    //   model->setData(model->index(i, idCol()), _f());
    emit endResetModel();
  }
  W_SLOT(sourceRowsInserted);

  inline void sourceRowsAboutToBeRemoved(const QModelIndex &, int, int) {
    ScopeLogger sl(__FUNCTION__);
    emit beginResetModel();
  }
  W_SLOT(sourceRowsAboutToBeRemoved);

  inline void sourceRowsRemoved(const QModelIndex &, int, int) {
    ScopeLogger sl(__FUNCTION__);
    emit endResetModel();
  }
  W_SLOT(sourceRowsRemoved);

  inline void sourceColumnsAboutToBeInserted(const QModelIndex &, int start,
                                             int end) {
    ScopeLogger sl(__FUNCTION__);
    emit beginInsertColumns(QModelIndex(), start, end);
  }
  W_SLOT(sourceColumnsAboutToBeInserted)

  inline void sourceColumnsInserted(const QModelIndex &, int, int) {
    ScopeLogger sl(__FUNCTION__);
    emit endInsertColumns();
  }
  W_SLOT(sourceColumnsInserted)

  inline void sourceColumnsAboutToBeRemoved(const QModelIndex &, int start,
                                            int end) {
    ScopeLogger sl(__FUNCTION__);
    emit beginRemoveColumns(QModelIndex(), start, end);
  }
  W_SLOT(sourceColumnsAboutToBeRemoved)

  inline void sourceColumnsRemoved(const QModelIndex &, int, int) {
    ScopeLogger sl(__FUNCTION__);
    emit endRemoveColumns();
  }
  W_SLOT(sourceColumnsRemoved)

  inline void sourceHeaderDataChanged(Qt::Orientation orientation, int start,
                                      int end) {
    ScopeLogger sl(__FUNCTION__);
    headerDataChanged(orientation, start, end);
  }
  W_SLOT(sourceHeaderDataChanged, (Qt::Orientation, int, int));

  void sourceModelAboutToBeReset() {
    ScopeLogger sl(__FUNCTION__);
    beginResetModel();
  }
  W_SLOT(sourceModelAboutToBeReset)
  void sourceModelReset() {
    ScopeLogger sl(__FUNCTION__);
    endResetModel();
  };
  W_SLOT(sourceModelReset)

  void dropedIn(QModelIndex x) W_SIGNAL(dropedIn, x)
};

W_OBJECT_IMPL_INLINE((TableToTreeProxyModel<T>), template <typename T>);

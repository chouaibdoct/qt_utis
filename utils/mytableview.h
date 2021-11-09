#pragma once
#include <qheaderview.h>
#include <qline.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qwidget.h>
#include <qwindowdefs.h>
#include <vector>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMimeData>
#include <QTableView>
#include <QWidget>
#include <QLineEdit>

#include <QHeaderView>

#include "utils_global.h"

template <class T> class Copyable : public T {
  W_OBJECT(Copyable);

public:
  using QTableView::QTableView;

protected:
  virtual void keyPressEvent(QKeyEvent *event) override {
    if (event->matches(QKeySequence::Copy)) {
      QModelIndexList cells = this->selectionModel()->selectedIndexes();
      std::sort(
          std::begin(cells),
          std::end(cells)); // Necessary, otherwise they are in column order
      QString text;
      int currentRow = 0;
      for (const auto &cell : cells) {
        if (this->isColumnHidden(cell.column()))
          continue;
        if (cell.row() != currentRow)
          text += "</td></tr><tr><td>";
        else
          text += "</td><td>";
        currentRow = cell.row();
        if (cell.data().isNull())
          text.append(QString());
        else
          text.append(cell.data().toString());
      }
      auto md = new QMimeData();
      md->setHtml(text);
      QApplication::clipboard()->setMimeData(md);
    } else {
      T::keyPressEvent(event);
    }
  };
};
W_OBJECT_IMPL((Copyable<T>), template <class T>);

class MyTableView : public Copyable<QTableView> {
  W_OBJECT(MyTableView);

public:
  //  explicit MyTableView(QVector<int> hidenColumns, QWidget *parent =
  //  nullptr);
  explicit MyTableView(QWidget *parent = nullptr);

  inline QVector<int> hidenColumns() const { return _hidenColumns; };
  void setHidenColumns(const QVector<int> &value);
  void setShownColumns(const QVector<int> &value);
  void setModel(QAbstractItemModel *model) override;

private:
  inline void config_table(void) {
    this->resizeColumnsToContents();
    //    this->resizeRowsToContents();
    this->horizontalHeader()->setSectionsMovable(true);
    this->setSortingEnabled(true);
    /*
    for (int c = 0; c < this->horizontalHeader()->count(); ++c) {
      this->horizontalHeader()->setSectionResizeMode(
                                                     c,
    QHeaderView::ResizeToContents);
    }
    */
    this->horizontalHeader()->setStretchLastSection(false);
    this->horizontalHeader()->setStretchLastSection(true);
  };
  QVector<int> _hidenColumns;
  QVector<int> _shownColumns;
};




/*
class MyHeaderView : public QHeaderView {
  W_OBJECT(MyHeaderView);

public:
  inline explicit MyHeaderView(Qt::Orientation orientation,
                               QWidget *parent = nullptr)
      : QHeaderView(orientation, parent) {
    this->setStretchLastSection(true);
    this->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->setSortIndicatorShown(false);
    // self.sectionResized.connect(self.adjustPositions)
    // parent.horizontalScrollBar().valueChanged.connect(self.adjustPositions)
  }

  void setFilterBoxes(int count) {
    for(auto& e:_editors)
      e->deleteLater();

    _editors.clear();
    
    for(int i=0;i!=count;++i){
      _editors[i]= this->create_editor(this ,i);
    }
    this->adjustPositions();
  }
  
  inline QWidget *create_editor(QWidget* parent, int index){
   QLineEdit* editor = new QLineEdit(parent);
   editor->setPlaceholderText("Filter");
   //todo
   //connection
   return editor;
  }

  inline QSize sizeHint() const override {
    auto size = QHeaderView::sizeHint();
    if (!_editors.empty()) {
      auto height = _editors.at(0)->sizeHint().height();
      size.setHeight(size.height() + height + _padding);
    }
    return size;
  }

  inline void updateGeometries() override {
    if (!_editors.empty()) {
      int height = _editors.at(0)->sizeHint().height();
      this->setViewportMargins(0, 0, 0, height + _padding);
    } else {
      this->setViewportMargins(0, 0, 0, 0);
    }
    QHeaderView::updateGeometries();
    adjustPositions();
  }

  inline void adjustPositions() {
    for (int i = 0; i != _editors.size(); ++i) {
      int height = _editors[i]->sizeHint().height();
      int compensate_y = 0;
      int compensate_x = 0;
      _editors[i]->move(this->sectionPosition(i) - this->offset() + 1 +
                            compensate_x,
                        height + (_padding / 2) + 2 + compensate_y);
      _editors[i]->resize(this->sectionSize(i),height);
    }
  }
  virtual ~MyHeaderView();

public:
  QWidgetList _editors;
  int _padding = 4;
};
W_OBJECT_IMPL_INLINE(MyHeaderView)

*/

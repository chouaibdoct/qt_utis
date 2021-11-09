#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QWidget>

template <class T> class ReadOnly : public T {
  static_assert(std::is_same_v<T, QComboBox>, "this is only of combo now");
public:
  ReadOnly(QWidget *parent = nullptr) : QComboBox{parent} {
    idx = QComboBox::currentIndex();
    QObject::connect(this, QOverload<int>::of(&T::activated),
                     [&](int) { T::setCurrentIndex(idx); });
    QObject::connect(this, QOverload<int>::of(&T::currentIndexChanged),
                     [&](int i) {
                       if (!_readOnly)
                         idx = i;
                     });
  }
  void keyPressEvent(QKeyEvent *e) override {
    if (!_readOnly)
      T::keyPressEvent(e);
  }

  void wheelEvent(QWheelEvent *e) override {
    //    if (!_readOnly)
    //T::wheelEvent(e);
  }
  void setReadOnly(bool val) { _readOnly = val; }
  bool isReadOnly() { return _readOnly; }

private:
  int idx;
  bool _readOnly = false;
};


using MyCombo = ReadOnly<QComboBox> ;

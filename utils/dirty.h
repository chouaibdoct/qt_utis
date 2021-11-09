#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QTabWidget>
#include <QWidget>
#include <type_traits>

class Dirt {
public:
  Dirt() = default;
  bool is_dirty() { return _dirty; };
  void set_dirty(bool value) { _dirty = value; };

protected:
  bool _dirty = false;
};

template <class T> class Dirtible : public T, public Dirt {
  static_assert(std::is_base_of_v<QWidget, T>, "the class should be a QObject");
};

class MyTabWidget : public QTabWidget {
 public:
  MyTabWidget() {};
  void onTabChanged(int index) {
    
  }
};

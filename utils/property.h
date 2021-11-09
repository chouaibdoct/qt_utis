#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QObject>
#include <QDebug>
#include "utils_global.h"

template <class T> struct Property : public QObject {
  W_OBJECT(Property);

public:

  Property(const T &value, QObject *parent = nullptr)
      : QObject{parent}, _value(value){};
  T value() const { return _value; }

  void setValue(const T &newValue) {
    if (newValue != _value) {
      _value = newValue;
      emit valueChanged(newValue);
      emit valueChangedVoid();
      //      qDebug()<<newValue;
    }
  };
  void valueChanged(T newValue = T{}) W_SIGNAL(valueChanged, (T), newValue);
  void valueChangedVoid() W_SIGNAL(valueChangedVoid,());

  operator T() const { return _value; };
  W_PROPERTY(T, value WRITE setValue READ value NOTIFY valueChanged);

  inline virtual ~Property() = default;
  
  Property(const Property &) = delete;
  Property &operator=(const Property) = delete;

private:
  T _value;
};
W_OBJECT_IMPL(Property<T>, template <class T>);

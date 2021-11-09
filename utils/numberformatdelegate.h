#pragma once
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QStyledItemDelegate>

#include "utils_global.h"

class NumberFormatDelegate : public QStyledItemDelegate {
  W_OBJECT(NumberFormatDelegate);
 public:
  explicit NumberFormatDelegate(QObject *parent = nullptr);
  QString displayText(const QVariant &value,
                      const QLocale &locale) const override;
};

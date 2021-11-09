#include "numberformatdelegate.h"
W_OBJECT_IMPL(NumberFormatDelegate);

NumberFormatDelegate::NumberFormatDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QString NumberFormatDelegate::displayText(const QVariant &value,
                                          const QLocale &locale) const {
  QString formattedNum =
      locale.toString(value.toDouble(), 'f', 2).append(locale.currencySymbol());

  return formattedNum;
}

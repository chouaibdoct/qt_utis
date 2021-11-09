#pragma once

#include <QDebug>
#include <atomic>
#include <cstdarg>
#include <iostream>

class ScopeLogger {
public:
  ScopeLogger(const char *msg) : msg(msg) {
    ++indent;
    qDebug().noquote().nospace() << indentation() << "{" << msg;
  }
  ~ScopeLogger() {
    qDebug().noquote().nospace() << indentation() << "}" << msg;
    --indent;
  }
  static std::atomic<int> indent;
  QDebug qdebug() {
    return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE,
                          QT_MESSAGELOG_FUNC)
               .debug()
               .noquote()
           << indentation();
  }
  QDebug qinfo() {
    return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE,
                          QT_MESSAGELOG_FUNC)
               .info()
               .noquote()
           << indentation();
  }
  QDebug qwarning() {
    return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE,
                          QT_MESSAGELOG_FUNC)
               .warning()
               .noquote()
           << indentation();
  };
  QDebug qcritical() {
    return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE,
                          QT_MESSAGELOG_FUNC)
               .critical()
               .noquote()
           << indentation();
  };
  void qfatal(const char *message, ...) {
    va_list args;
    va_start(args, message);
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC)
        .fatal(indentation().toLatin1().data(), message, args);
    va_end(args);
  }
  QString msg;
  QString indentation() { return QString(indent, '\t'); }
};
inline std::atomic<int> ScopeLogger::indent = -1;
//#define FUNCTION(x) ScopeLogger l_##x##_scope(x);

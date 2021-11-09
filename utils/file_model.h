#pragma once
#include <verdigris/wobjectcpp.h>
#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>

#include <QEventLoop>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSqlTableModel>
#include <QUrl>

class FileModel : public QSqlTableModel {
  W_OBJECT(FileModel);

public:
  explicit FileModel(QUrl serverUrl, QObject *parent = nullptr);
  virtual ~FileModel() = default;
  bool upload(QStringList filename);
  //  void uploadFinished();       
  void uploadProgress(qint64 bytesSent, qint64 bytesTotal)
      W_SIGNAL(uploadProgress, bytesSent, bytesTotal);
  void onError(QNetworkReply::NetworkError err)
      W_SIGNAL(onError, (QNetworkReply::NetworkError), err);

protected:
  QUrl url;
  QHttpMultiPart *multiPart;
  QNetworkAccessManager *pManager;
  QNetworkReply *pReply;
  //  QEventLoop *pELoop;
  bool uploadInProgress = true;
};

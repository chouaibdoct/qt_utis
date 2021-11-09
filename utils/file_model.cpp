#include "utils/file_model.h"
#include <QFileInfo>
#include <QMimeDatabase>

W_OBJECT_IMPL(FileModel);

FileModel::FileModel(QUrl serverUrl, QObject *parent)
    : QSqlTableModel(parent), url{serverUrl},
      multiPart{new QHttpMultiPart(QHttpMultiPart::FormDataType)} {
  QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
  sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
  QSslConfiguration::setDefaultConfiguration(sslConf);
}

bool FileModel::upload(QStringList absoluteFilePaths) {
  qDebug() << "Upload Starting"; //
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QVector<QHttpPart> fileParts(absoluteFilePaths.size());
  for (int i = 0; i != absoluteFilePaths.size(); ++i) {
    QFileInfo fileInfo(absoluteFilePaths[i]);
    //    QHttpPart filePart;
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(absoluteFilePaths[i]);
    fileParts[i].setHeader(QNetworkRequest::ContentTypeHeader,
                           QVariant(mime.name()));
    fileParts[i].setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QVariant("form-data; name=\"sampleFile\"; filename=\"" +
                 fileInfo.fileName() + "\""));
    QFile *file = new QFile(absoluteFilePaths[i]);
    if (!file->exists()) {
      qDebug() << "File Does not exist";
      return false;
    }

    file->open(QIODevice::ReadOnly);
    fileParts[i].setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it
    multiPart->append(fileParts[i]);
  }
  QNetworkRequest request(url);
  pManager = new QNetworkAccessManager();
  pReply = pManager->post(request, multiPart);
  multiPart->setParent(pReply);

  connect(pReply, &QNetworkReply::uploadProgress, this, [](qint64 a, qint64 b) {
    Q_UNUSED(a);
    Q_UNUSED(b);
    //    qDebug() << " SOME PROGRESS!";
    //    qDebug() << a << "/" << b;
  });

  connect(pReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error),
          this, [](QNetworkReply::NetworkError err) {
            qDebug() << " SOME ERROR!";
            qDebug() << err;
          });

  connect(pReply, &QNetworkReply::finished, this, [this]() {
    uploadInProgress = false;
    if (pReply->error() > 0) {
      qDebug() << "Error occured: " << pReply->error() << " : "
               << pReply->errorString();
    } else {
      qDebug() << "Upload success";
    }
    pReply->deleteLater();
  });

  QEventLoop eventLoop;
  // connect(pManager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
  //         SLOT(quit()));
  QObject::connect(pManager, &QNetworkAccessManager::finished, &eventLoop,
                   &QEventLoop::quit);
  eventLoop.exec();
}

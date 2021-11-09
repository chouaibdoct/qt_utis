

#include <QApplication>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include <utils/file_model.h>

int main (int argc, char *argv[]) {
  QApplication a(argc, argv);
  FileModel *f = new FileModel(QUrl("https://localhost:5000/uploads"));

  f->upload({QString("/home/anaya/Desktop/GCAlgerie.com(169).pdf"),QString("/home/anaya/Desktop/adatzifaf.pdf")});
  return a.exec();
};

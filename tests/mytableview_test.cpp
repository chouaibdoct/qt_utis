
#include <QBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QWidget>
#include <qnamespace.h>
#include <utils/mytableview.h>

#include <QApplication>

class Window : public QWidget {
public:
  Window(QWidget *parent) : QWidget{parent}, panel{new QWidget(this)} {
    panel->setFixedHeight(42);
    auto btn{new QPushButton(this)};
    btn->setText("Set table headers");
    QObject::connect(btn, &QPushButton::clicked, this, &Window::setThemUp);

    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(btn);

    panel->setLayout(lay);
    view = new QTableView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(panel);
    layout->addWidget(view);

    header = new MyHeaderView(Qt::Horizontal, this);
    qDebug() << "header instance (init) = " << header->objectName();
    view->setHorizontalHeader(header);
  }

  inline void handleFilterActivated() const {
    qDebug() << "handleFilterActivated!";
  }
  inline void setThemUp() {
    qDebug() << "setThemUp........";

    // header = self.view
    //              .horizontalHeader()
    //                  print("header instance (SetThemUp) = " + str(header))
    //                      header.setFilterBoxes(7)
  };

private:
  QWidget *panel;
  QTableView *view;
  MyHeaderView *header;
};







int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  return a.exec();
};
